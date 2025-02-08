#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //library from Confluence to install

SFE_UBLOX_GNSS myGNSS;

void setup() 
{
    Serial.begin(115200);
    Wire.begin();

    if (!myGNSS.begin()) // Connect to the u-blox module using Wire (I2C)
    {
        Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring."));
        while (1);
    }

    myGNSS.setI2COutput(COM_TYPE_UBX); // Set the I2C port to output UBX only (optional)
    myGNSS.setAutoPVT(true); // Enable automatic PVT updates
}
 
//Getting values. Can return whatever is needed by caller function in array?
void loop()
{

        float latitude = myGNSS.getLatitude() / 10000000.0;  // Convert to decimal degrees
        float longitude = myGNSS.getLongitude() / 10000000.0; 
        float altitude = myGNSS.getAltitude() / 1000.0; // Convert from mm to meters
        
        int year = myGNSS.getYear();
        int month = myGNSS.getMonth();
        int day = myGNSS.getDay();
        int hour = myGNSS.getHour();
        int minute = myGNSS.getMinute();
        int second = myGNSS.getSecond();

        // Print data to serial monitor for debugging
        Serial.print("Lat: "); Serial.print(latitude, 6);
        Serial.print(", Lon: "); Serial.print(longitude, 6);
        Serial.print(", Alt: "); Serial.print(altitude);
        Serial.print("m, Time: ");
        Serial.printf("%02d-%02d-%04d %02d:%02d:%02d\n", day, month, year, hour, minute, second);
}

