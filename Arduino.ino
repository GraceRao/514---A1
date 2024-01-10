#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_VEML7700.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET_PIN -1

// Sensor and display objects
Adafruit_BME280 bme280;
Adafruit_VEML7700 veml7700;
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET_PIN);

// Pin and thresholds
const int lightControlPin = A1;
const float temperatureThreshold = 30.0;
const float lightLowThresholdValue = 50.0;
const float lightHighThresholdValue = 100.0;

void setup() {
  Serial.begin(9600);

  if (!bme280.begin(0x76) || !veml7700.begin() || !display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Failed to initialize sensors. Please check the wiring!");
    while (1);
  }

  pinMode(lightControlPin, OUTPUT);
  display.clearDisplay();
  display.display();
  delay(2000);
}

void loop() {
  float currentTemperature = bme280.readTemperature();
  float currentHumidity = bme280.readHumidity();
  float currentPressure = bme280.readPressure() / 100.0F;
  float currentLightIntensity = veml7700.readLux();

  displayDataOnOLED(currentTemperature, currentHumidity, currentPressure, currentLightIntensity);
  controlLightBasedOnIntensity(currentLightIntensity);

  delay(2000);
}

void displayDataOnOLED(float temp, float hum, float press, float lightIntensity) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temperature: "); display.print(temp); display.println(" C");
  display.print("Humidity: "); display.print(hum); display.println(" %");
  display.print("Pressure: "); display.print(press); display.println(" hPa");
  display.print("Light: "); display.print(lightIntensity); display.println(" Lux");
  display.display();
}

void controlLightBasedOnIntensity(float lightIntensity) {
  if (lightIntensity < lightLowThresholdValue) {
    digitalWrite(lightControlPin, HIGH);
  } else if (lightIntensity > lightHighThresholdValue) {
    digitalWrite(lightControlPin, LOW);
  }
}
