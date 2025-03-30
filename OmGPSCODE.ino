
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
// Create a GNSS object using the updated class name:
SFE_UBLOX_GNSS myGNSS;
// Create a TwoWire instance for the GPS
TwoWire GPSWire = TwoWire(0);
// Define your I2C pins (WARNING: 35 & 36 are input-only on most ESP32)
const int GPS_SDA = 36;
const int GPS_SCL = 35;
void setup()
{
  Serial.begin(115200);
  Serial.println("Starting GPS Test...");
  // Initialize the custom I2C bus at 400kHz
  GPSWire.begin(GPS_SDA, GPS_SCL, 400000);
  // Initialize the u-blox GNSS module over I2C
  if (myGNSS.begin(GPSWire) == false)
  {
    Serial.println("u-blox GNSS not detected at default I2C address. Check wiring!");
    while (1)
    {
      delay(1000);
    }
  }
  else
  {
    Serial.println("GPS detected successfully!");
  }
}
void loop()
{
  // The getPVT() function checks if fresh PVT data is available
  
if (myGNSS.getPVT())
{
  long rawLat = myGNSS.getLatitude();      // e.g., 425759000
  long rawLon = myGNSS.getLongitude();     // e.g., -833723153
  long rawAlt = myGNSS.getAltitude();      // e.g., 270950 (in mm)
  double latDeg = rawLat / 10000000.0;     // -> 42.5759
  double lonDeg = rawLon / 10000000.0;     // -> -83.3723153
  double altM   = rawAlt / 1000.0;         // -> 270.95 meters
  Serial.print("Lat: ");
  Serial.print(latDeg, 7);
  Serial.print("  Lon: ");
  Serial.print(lonDeg, 7);
  Serial.print("  Alt: ");
  Serial.print(altM, 2);
  Serial.println(" m");
}
  delay(1000);
}
