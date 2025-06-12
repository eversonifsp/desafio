#include <LiquidCrystal.h>

// LCD display initialization with Arduino pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Pin definitions for actuators and sensors
const int GREEN_LED = 4;
const int YELLOW_LED = 5;
const int RED_LED = 6;
const int BUZZER = 3;
const int TEMP_SENSOR = A0;    // TMP36 temperature sensor
const int HUMIDITY_POT = A1;   // Potentiometer simulating humidity
const int LIGHT_SENSOR = A2;   // LDR light sensor

// Sensor reading averages
float temperature;
int humidity;
int lightLevel;

// Moving average calculation settings
const int READINGS_COUNT = 5;
float tempReadings[READINGS_COUNT];
int humidityReadings[READINGS_COUNT];
int lightReadings[READINGS_COUNT];
int currentReading = 0;

// Display message control
unsigned long previousTime = 0;
const long DISPLAY_INTERVAL = 3000;  // Message change interval (3 seconds)
int currentMessage = 0;  // 0 = Light, 1 = Humidity, 2 = Temperature

// Light level thresholds
const int DARK_THRESHOLD = 500;
const int MODERATE_LIGHT_THRESHOLD = 800;

// Alert state variables
bool lightAlert = false;
bool temperatureAlert = false;
bool humidityAlert = false;

void setup() {
  // Initialize LCD display
  lcd.begin(16, 2);

  // Configure output pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Initialize reading arrays
  for (int i = 0; i < READINGS_COUNT; i++) {
    tempReadings[i] = 0;
    humidityReadings[i] = 0;
    lightReadings[i] = 0;
  }

  // Initial display message
  lcd.print("Wine Cellar");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read raw sensor values
  float rawTemp = analogRead(TEMP_SENSOR);
  int rawHumidity = analogRead(HUMIDITY_POT);
  int rawLight = analogRead(LIGHT_SENSOR);

  // Convert TMP36 reading to Celsius
  float currentTemp = (rawTemp * 5.0 / 1024.0 - 0.5) * 100;

  // Convert potentiometer reading to humidity percentage (0-100%)
  int currentHumidity = map(rawHumidity, 0, 1023, 0, 100);

  // Store current readings in circular arrays
  tempReadings[currentReading] = currentTemp;
  humidityReadings[currentReading] = currentHumidity;
  lightReadings[currentReading] = rawLight;

  // Update circular array index
  currentReading = (currentReading + 1) % READINGS_COUNT;

  // Calculate moving averages
  temperature = calculateTempAverage();
  humidity = calculateHumidityAverage();
  lightLevel = calculateLightAverage();

  // Check for alert conditions
  checkAlerts();

  // Update LEDs and buzzer based on alerts
  updateIndicators();

  // Update display message every 3 seconds
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= DISPLAY_INTERVAL) {
    previousTime = currentTime;
    currentMessage = (currentMessage + 1) % 3;
    updateDisplay();
  }
}

float calculateTempAverage() {
  float sum = 0;
  for (int i = 0; i < READINGS_COUNT; i++) {
    sum += tempReadings[i];
  }
  return sum / READINGS_COUNT;
}

int calculateHumidityAverage() {
  int sum = 0;
  for (int i = 0; i < READINGS_COUNT; i++) {
    sum += humidityReadings[i];
  }
  return sum / READINGS_COUNT;
}

int calculateLightAverage() {
  int sum = 0;
  for (int i = 0; i < READINGS_COUNT; i++) {
    sum += lightReadings[i];
  }
  return sum / READINGS_COUNT;
}

void checkAlerts() {
  // Light alert: too bright
  lightAlert = (lightLevel >= MODERATE_LIGHT_THRESHOLD);

  // Temperature alert: outside 10°C to 15°C range
  temperatureAlert = (temperature < 10 || temperature > 15);

  // Humidity alert: outside 50% to 70% range
  humidityAlert = (humidity < 50 || humidity > 70);
}

void updateIndicators() {
  // Turn off all LEDs before applying new logic
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  // LED and buzzer based on light level
  if (lightAlert) {
    digitalWrite(RED_LED, HIGH);
  } else if (lightLevel >= DARK_THRESHOLD && lightLevel < MODERATE_LIGHT_THRESHOLD) {
    digitalWrite(YELLOW_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, HIGH);
  }

  // LED and buzzer based on temperature (Yellow LED)
  if (temperatureAlert) {
    digitalWrite(YELLOW_LED, HIGH);
  }

  // LED and buzzer based on humidity (Red LED)
  if (humidityAlert) {
    digitalWrite(RED_LED, HIGH);
  }

  // Activate buzzer if any alert is active
  if (lightAlert || temperatureAlert || humidityAlert) {
    tone(BUZZER, 1000);  // 1kHz tone
  } else {
    noTone(BUZZER);
  }
}

void updateDisplay() {
  lcd.clear();

  switch (currentMessage) {
    case 0: // Light
      lcd.setCursor(0, 0);
      lcd.print("Light Level:");
      lcd.setCursor(0, 1);
      if (lightLevel < DARK_THRESHOLD) {
        lcd.print("Dark");
      } else if (lightLevel < MODERATE_LIGHT_THRESHOLD) {
        lcd.print("Moderate");
      } else {
        lcd.print("Bright");
      }
      break;

    case 1: // Humidity
      lcd.setCursor(0, 0);
      lcd.print("Humidity:");
      lcd.setCursor(0, 1);
      if (humidity < 50) {
        lcd.print("Low = ");
      } else if (humidity <= 70) {
        lcd.print("OK = ");
      } else {
        lcd.print("High = ");
      }
      lcd.print(humidity);
      lcd.print("%");
      break;

    case 2: // Temperature
      lcd.setCursor(0, 0);
      lcd.print("Temperature:");
      lcd.setCursor(0, 1);
      if (temperature < 10) {
        lcd.print("Low = ");
      } else if (temperature <= 15) {
        lcd.print("OK = ");
      } else {
        lcd.print("High = ");
      }
      lcd.print(temperature, 1);
      lcd.print("C");
      break;
  }
}
