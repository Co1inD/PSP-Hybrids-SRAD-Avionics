/**
 * EnhancedDualCoreRocket.ino
 *
 * Demonstrates:
 *   - Two tasks pinned to different ESP32 cores
 *   - Real-time flight control (Core 0, high priority)
 *   - Logging/telemetry (Core 1, lower priority)
 *   - Multi-chute logic (drogue/main)
 *   - Simple safety checks for pyro channels
 *   - Kalman or complementary filter stub (expand as needed)
 *   - Thread-safe queue-based logging system
 *
 * Adapt this to your sensors, pins, and flight software needs.
 */

#include <Arduino.h>
#include <math.h>  // For math ops like sqrt (if needed)

//--------------------------------------------------------------
// STRUCTS & GLOBALS
//--------------------------------------------------------------

/**
 * Example structure for rocket flight data from the real-time task.
 * You might include orientation, battery voltage, etc. in real code.
 */
typedef struct {
  float altitude;
  float velocity;
  bool  armed;
  bool  launched;
  bool  drogueDeployed;
  bool  mainDeployed;
} FlightData;

/**
 * Example structure for a Kalman (or complementary) filter state.
 * In real usage, you might add more fields (biases, covariance, etc.).
 */
typedef struct {
  float altitude;
  float velocity;
} KalmanState;

/**
 * Global "kalmanState" updated by the real-time task on Core 0.
 */
volatile KalmanState kalmanState = {0.0f, 0.0f};

/**
 * Basic flight state tracking
 */
volatile bool rocketArmed       = false;  // Rocket is armed for launch
volatile bool rocketLaunched    = false;  // Has the rocket launched?
volatile bool drogueDeployed    = false;  // Drogue chute out
volatile bool mainDeployed      = false;  // Main chute out

//--------------------------------------------------------------
// FREE RTOS TASKS: DECLARATIONS
//--------------------------------------------------------------
void TaskRocketControl(void *pvParameters);
void TaskLoggingComms(void *pvParameters);

TaskHandle_t rocketControlHandle = NULL;
TaskHandle_t loggingCommsHandle  = NULL;

//--------------------------------------------------------------
// LOGGING QUEUE
//--------------------------------------------------------------
/**
 * We'll create a queue to store log messages (strings) safely.
 * The logging task will dequeue them and output to Serial (or radio).
 */
#define LOG_QUEUE_SIZE  10     // Up to 10 messages buffered
#define LOG_MESSAGE_LEN 128    // Max length of a single log message

static QueueHandle_t logQueue = NULL;

// Helper function to enqueue log messages
void enqueueLog(const char* message) {
  if (logQueue == NULL) return;

  // Allocate a buffer for the message
  char msgBuffer[LOG_MESSAGE_LEN];
  // Copy the message safely (truncate if needed)
  strncpy(msgBuffer, message, LOG_MESSAGE_LEN - 1);
  msgBuffer[LOG_MESSAGE_LEN - 1] = '\0'; // Ensure null-terminator

  // Send to queue (non-blocking). If queue is full, message is lost.
  xQueueSendToBack(logQueue, msgBuffer, 0);
}

//--------------------------------------------------------------
// PIN DEFINITIONS (EXAMPLE)
//--------------------------------------------------------------

// Example pyro pins for drogue and main. Active LOW = firing.
#define PYRO_DROGUE_PIN  4
#define PYRO_MAIN_PIN    5

// Optional safety sense pin (jumper or continuity sense)
#define PYRO_SENSE_DROGUE  6
#define PYRO_SENSE_MAIN    7

//--------------------------------------------------------------
// SETUP & LOOP
//--------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("==== Enhanced Dual-Core Rocket Example ====");
  Serial.print("Setup running on core: ");
  Serial.println(xPortGetCoreID());

  // Initialize pyro pins as OUTPUT (inactive = HIGH)
  pinMode(PYRO_DROGUE_PIN, OUTPUT);
  digitalWrite(PYRO_DROGUE_PIN, HIGH);
  pinMode(PYRO_MAIN_PIN, OUTPUT);
  digitalWrite(PYRO_MAIN_PIN, HIGH);

  // If you have continuity sense pins, set them as INPUT
  pinMode(PYRO_SENSE_DROGUE, INPUT_PULLUP);
  pinMode(PYRO_SENSE_MAIN, INPUT_PULLUP);

  // Create the logging queue
  logQueue = xQueueCreate(LOG_QUEUE_SIZE, LOG_MESSAGE_LEN);
  if (logQueue == NULL) {
    Serial.println("ERROR: Failed to create log queue!");
  }

  // Create the real-time rocket control task on Core 0 (high priority)
  xTaskCreatePinnedToCore(
    TaskRocketControl,   // Function
    "RocketControl",     // Name
    4096,                // Stack size
    NULL,                // Parameter
    3,                   // Priority (relatively high)
    &rocketControlHandle,// Task handle
    0                    // Pin to core 0
  );

  // Create the logging/communications task on Core 1 (lower priority)
  xTaskCreatePinnedToCore(
    TaskLoggingComms,
    "LoggingComms",
    4096,
    NULL,
    1,                   // Lower priority
    &loggingCommsHandle,
    1                    // Pin to core 1
  );
}

void loop() {
  // The Arduino default loop, running on Core 1 at priority 1 (same as Logging).
  // We can do minimal tasks here or just yield.
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 3000) {
    lastPrint = millis();
    Serial.printf("[loop()] Minimal tasks on core %d...\n", xPortGetCoreID());
  }
  delay(100);
}

//--------------------------------------------------------------
// REAL-TIME TASK: TaskRocketControl (Core 0, Priority 3)
//--------------------------------------------------------------
void TaskRocketControl(void *pvParameters)
{
  Serial.printf("[RocketControl] Running on core %d. Priority=%d\n",
                xPortGetCoreID(), uxTaskPriorityGet(NULL));

  // Initialize flight states
  rocketArmed    = true;    // Example: start armed
  rocketLaunched = false;
  drogueDeployed = false;
  mainDeployed   = false;

  // Desired loop rate: 20 Hz => 50 ms
  const TickType_t loopDelayTicks = pdMS_TO_TICKS(50);

  // For measuring loop timing (optional, for analysis)
  unsigned long lastMicros = micros();

  while(true) {
    // ============== TIMING MEASUREMENT (optional) ==============
    unsigned long now = micros();
    unsigned long loopDuration = now - lastMicros;
    lastMicros = now;
    // If you want to see how long the loop took:
    // char tempMsg[64];
    // snprintf(tempMsg, 64, "[RocketControl] Loop took %lu us", loopDuration);
    // enqueueLog(tempMsg);

    // ============== 1) READ SENSORS ==============
    float rawAlt  = readRawAltitude();
    float rawAccel = readRawAcceleration();
    // If using IMU, you’d also read orientation, etc.

    // ============== 2) UPDATE FILTER ==============
    updateKalmanFilter(rawAlt, rawAccel, &kalmanState);

    // ============== 3) FLIGHT LOGIC ==============
    float alt = kalmanState.altitude;
    float vel = kalmanState.velocity;

    if (rocketArmed && !rocketLaunched) {
      // If velocity above some threshold => launch
      if (vel > 10.0f) {
        rocketLaunched = true;
        enqueueLog("[RocketControl] Launch detected!");
      }
    }

    // Deploy drogue if rocket is descending (velocity < 0) and not yet deployed
    if (rocketLaunched && !drogueDeployed) {
      if (vel < 0.0f) {
        drogueDeployed = true;
        enqueueLog("[RocketControl] Apogee -> Deploying drogue!");
        deployChute(1);  // Fire drogue pyro
      }
    }

    // Deploy main if altitude below a certain threshold (e.g., 500m) (just an example)
    if (drogueDeployed && !mainDeployed) {
      if (alt < 500.0f) {
        mainDeployed = true;
        enqueueLog("[RocketControl] Main chute altitude threshold -> Deploy main!");
        deployChute(2);  // Fire main pyro
      }
    }

    // ============== 4) DELAY FOR NEXT CYCLE ==============
    vTaskDelay(loopDelayTicks);
  }
}

//--------------------------------------------------------------
// TASK: TaskLoggingComms (Core 1, Priority 1)
//--------------------------------------------------------------
void TaskLoggingComms(void *pvParameters)
{
  Serial.printf("[LoggingComms] Running on core %d. Priority=%d\n",
                xPortGetCoreID(), uxTaskPriorityGet(NULL));

  const TickType_t loopDelay = pdMS_TO_TICKS(500); // 2 Hz
  char msgBuffer[LOG_MESSAGE_LEN];

  while(true) {
    // ========== 1) Dequeue & Print Log Messages ==========
    // We try receiving from the queue with zero wait. If there's no message, we skip.
    while (xQueueReceive(logQueue, msgBuffer, 0) == pdTRUE) {
      // Output to Serial (or radio, or both)
      Serial.println(msgBuffer);
      // Potentially send over radio/Wi-Fi here
    }

    // ========== 2) Periodic Telemetry ==========
    // We also might want to log the current flight data every iteration
    float alt = kalmanState.altitude;
    float vel = kalmanState.velocity;
    bool armed = rocketArmed;
    bool launched = rocketLaunched;
    bool drogue = drogueDeployed;
    bool main   = mainDeployed;

    char telemMsg[128];
    snprintf(telemMsg, 128,
      "[Telemetry] ALT=%.2f, VEL=%.2f, ARM=%d, LCH=%d, DRG=%d, M=%d",
      alt, vel, armed, launched, drogue, main);
    Serial.println(telemMsg);

    // Send to ground station or flash if needed
    // e.g. sendTelemetryPacket(alt, vel, armed, launched, drogue, main);

    vTaskDelay(loopDelay);
  }
}

//--------------------------------------------------------------
// HELPER FUNCTIONS
//--------------------------------------------------------------

/**
 * readRawAltitude()
 * A placeholder to simulate reading altitude from a barometer (e.g. BMP280).
 */
float readRawAltitude() {
  static float testAltitude = 0.0;
  // Increase altitude until ~1000m, then descend (simple simulation).
  if (testAltitude < 1000.0) {
    testAltitude += 5.0;  // ascending
  } else {
    testAltitude -= 6.0;  // descending
    if (testAltitude < 0.0) {
      testAltitude = 0.0;
    }
  }
  return testAltitude;
}

/**
 * readRawAcceleration()
 * Placeholder for reading an accelerometer. 
 * In real usage, you'd call e.g. an IMU driver.
 */
float readRawAcceleration() {
  // Just return zero in this simulation
  return 0.0f;
}

/**
 * updateKalmanFilter()
 * Very basic demonstration of a filter step. Replace with real logic.
 */
void updateKalmanFilter(float rawAlt, float rawAccel, volatile KalmanState* state) {
  static float lastAlt = 0.0f;
  float dt = 0.05f; // 50 ms cycle

  // Rough velocity estimate
  float estimatedVel = (rawAlt - lastAlt) / dt;

  // Simple smoothing (not a real Kalman)
  state->altitude = 0.8f * state->altitude + 0.2f * rawAlt;
  state->velocity = 0.8f * state->velocity + 0.2f * estimatedVel;

  lastAlt = rawAlt;
}

/**
 * deployChute()
 * Fire the corresponding pyro output (active LOW) with safety checks.
 */
void deployChute(int chuteNumber) {
  // Check if pyro is armed or continuity sense is valid, etc.
  // Example: pyro continuity sense pins
  int sensePin = (chuteNumber == 1) ? PYRO_SENSE_DROGUE : PYRO_SENSE_MAIN;
  int pyroPin  = (chuteNumber == 1) ? PYRO_DROGUE_PIN   : PYRO_MAIN_PIN;

  // Check continuity (if the sense pin is low, maybe that means good continuity?)
  int continuity = digitalRead(sensePin);
  if (continuity == HIGH) {
    // If we have no continuity, we might log a warning:
    enqueueLog("[deployChute] WARNING: No continuity detected!");
  }

  // Actually fire the pyro. In real usage, you’d do a short pulse.
  enqueueLog("[deployChute] Firing pyro output!");
  digitalWrite(pyroPin, LOW);   // Active LOW
  delay(1000);                  // 1 second pulse
  digitalWrite(pyroPin, HIGH);  // Deactivate
}

