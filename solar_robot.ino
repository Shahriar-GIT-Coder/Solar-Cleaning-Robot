#include <Ultrasonic.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Blynk Auth Token
char auth[] = "fpBZuZprnJ2g-H58zzjd9g0IlOLjgd6S";

// WiFi credentials
char ssid[] = "Cudy-8990";
char pass[] = "s.aj32021";

// Pin Definitions for L298N Motor Driver
#define L298N1_IN1  2  // Motor driver 1 IN1
#define L298N1_IN2  4  // Motor driver 1 IN2
#define L298N1_IN3  16  // Motor driver 1 IN3
#define L298N1_IN4  7  // Motor driver 1 IN4

#define L298N2_IN1  14  // Motor driver 2 IN1
#define L298N2_IN2  27  // Motor driver 2 IN2
#define L298N2_IN3  26  // Motor driver 2 IN3
#define L298N2_IN4  25  // Motor driver 2 IN4

// Pin Definitions for Sensors
#define RAIN_SENSOR_PIN 33
#define BME_SDA 21
#define BME_SCL 15

// Pin Definitions for Ultrasonic Sensors
#define TRIG_PIN1 5
#define ECHO_PIN1 18
#define TRIG_PIN2 19
#define ECHO_PIN2 22

// Relay Module Pin
#define RELAY_PIN 13

// Initialize Sensors
Ultrasonic ultrasonic1(TRIG_PIN1, ECHO_PIN1);
Ultrasonic ultrasonic2(TRIG_PIN2, ECHO_PIN2);
Adafruit_BME280 bme; // I2C

int step = 0;
bool blynkSwitchOn = false;
int rainValue;

// Timer for Blynk
BlynkTimer timer;

void setup() {
  // Set up pin modes for L298N motor driver
  pinMode(L298N1_IN1, OUTPUT);
  pinMode(L298N1_IN2, OUTPUT);
  pinMode(L298N1_IN3, OUTPUT);
  pinMode(L298N1_IN4, OUTPUT);

  pinMode(L298N2_IN1, OUTPUT);
  pinMode(L298N2_IN2, OUTPUT);
  pinMode(L298N2_IN3, OUTPUT);
  pinMode(L298N2_IN4, OUTPUT);

  // Set up pin mode for relay module
  pinMode(RELAY_PIN, OUTPUT);

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  // Initialize BME280
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  // Set up a timer to post sensor data to Blynk every hour
  timer.setInterval(3600000L, sendSensorData);

  // Initialize serial communication
  Serial.begin(115200);
}

void loop() {
  Blynk.run();
  timer.run();

// Execute the steps only when Blynk switch is on
  if (blynkSwitchOn) {
    executeStep();
  }
}

// This function handles the Blynk switch (V0) state
BLYNK_WRITE(V0) {
  int switchValue = param.asInt();
  blynkSwitchOn = (switchValue == 1);
}

void executeStep() {
  switch (step) {
    case 0:
      // First step: ultrasonic1 > 5
      if (ultrasonic1.read() > 5) {
        // Motor driver 1: IN1 and IN3 clockwise, IN2 and IN4 anticlockwise
        digitalWrite(L298N1_IN1, HIGH);
        digitalWrite(L298N1_IN2, LOW);
        digitalWrite(L298N1_IN3, HIGH);
        digitalWrite(L298N1_IN4, LOW);

        // Motor driver 2: IN1 and IN3 clockwise, IN2 and IN4 anticlockwise
        digitalWrite(L298N2_IN1, HIGH);
        digitalWrite(L298N2_IN2, LOW);
        digitalWrite(L298N2_IN3, HIGH);
        digitalWrite(L298N2_IN4, LOW);

        rainValue = analogRead(RAIN_SENSOR_PIN);
          // Control relay based on rain sensor value
        if (rainValue == 4095) {
          digitalWrite(RELAY_PIN, HIGH); // Turn on relay
        } else if (rainValue < 4095) {
          digitalWrite(RELAY_PIN, LOW); // Turn off relay
        }

        step++; // Move to the next step
      }
      break;

    case 1:
      // Second step: ultrasonic2 > 5
      if (ultrasonic2.read() > 5) {
        // Motor driver 1: IN1 and IN3 anticlockwise, IN2 and IN4 clockwise
        digitalWrite(L298N1_IN1, LOW);
        digitalWrite(L298N1_IN2, HIGH);
        digitalWrite(L298N1_IN3, LOW);
        digitalWrite(L298N1_IN4, HIGH);

        // Motor driver 2: IN1 and IN3 anticlockwise, IN2 and IN4 clockwise
        digitalWrite(L298N2_IN1, LOW);
        digitalWrite(L298N2_IN2, HIGH);
        digitalWrite(L298N2_IN3, LOW);
        digitalWrite(L298N2_IN4, HIGH);

        rainValue = analogRead(RAIN_SENSOR_PIN);
          // Control relay based on rain sensor value
        if (rainValue == 4095) {
          digitalWrite(RELAY_PIN, HIGH); // Turn on relay
        } else if (rainValue < 4095) {
          digitalWrite(RELAY_PIN, LOW); // Turn off relay
        }

        step++; // Move to the next step
      }
      break;

    case 2:
      // Third step: ultrasonic1 > 5, stop everything
      if (ultrasonic1.read() > 5) {
        // Stop all motors
        digitalWrite(L298N1_IN1, LOW);
        digitalWrite(L298N1_IN2, LOW);
        digitalWrite(L298N1_IN3, LOW);
        digitalWrite(L298N1_IN4, LOW);

        digitalWrite(L298N2_IN1, LOW);
        digitalWrite(L298N2_IN2, LOW);
        digitalWrite(L298N2_IN3, LOW);
        digitalWrite(L298N2_IN4, LOW);

        digitalWrite(RELAY_PIN, LOW); // Turn off relay

        // Reset Blynk switch
        Blynk.virtualWrite(V0, 0);
        blynkSwitchOn = false;
        step = 0; // Reset step counter for the next cycle
      }
      break;
  }
}

void sendSensorData() {
  // Read data from sensors
  rainValue = analogRead(RAIN_SENSOR_PIN);
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  // Post data to Blynk
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, pressure);
  Blynk.virtualWrite(V4, rainValue);


}
