#include <Wire.h>
#include <BH1750.h>
#include <BlynkSimpleEsp32.h>
#include <RTClib.h>

// Blynk Auth Token
char auth[] = "YXRx_nZC3DiLLRPsjjL8256bVxWGYoEB";

// WiFi credentials
char ssid[] = "Cudy-8990";
char pass[] = "s.aj32021";

// Pin Definitions
#define RELAY_PIN 5

// Initialize BH1750 sensor
BH1750 lightMeter;

// Initialize RTC
RTC_DS3231 rtc;

void setup() {
  // Set up pin mode for relay module
  pinMode(RELAY_PIN, OUTPUT);

  // Initialize serial communication
  Serial.begin(115200);

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  // Initialize BH1750 sensor
  Wire.begin();
  lightMeter.begin();

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Check if RTC lost power and if so, set the date and time
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    // This is a simple way to set the date and time (you can change the values as needed)
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  Blynk.run();
  
  // Get current time
  DateTime now = rtc.now();
  
  // Check if it's midnight (12 AM)
  if (now.hour() == 0 && now.minute() == 0) {
    // Turn on relay
    digitalWrite(RELAY_PIN, HIGH);

    // Measure light level
    float lightLevel = lightMeter.readLightLevel();

    // Calculate relative change percentage
    float fixed_light_Value = 1000.0;
    float relativeChange = ((lightLevel - fixed_light_Value) / fixed_light_Value) * 100.0;

    // Post data to Blynk
    Blynk.virtualWrite(V1, lightLevel);
    Blynk.virtualWrite(V2, relativeChange);

    Serial.print("Light Level: ");
    Serial.print(lightLevel);
    Serial.print(" lx, Relative Change: ");
    Serial.print(relativeChange);
    Serial.println(" %");

      if (relativeChange > 15) {
    // Send push notification to the Blynk app
    Blynk.logEvent("High_Dust");
  }

    // Wait for 60 seconds to prevent multiple triggers within the same minute
    delay(60000);
  } else {
    // Turn off relay
    digitalWrite(RELAY_PIN, LOW);
  }
  
  // Delay to prevent rapid polling
  delay(1000);
}
