#define BLYNK_TEMPLATE_ID "TMPL6i5VyHDKE"
#define BLYNK_TEMPLATE_NAME "Project Skripsweet"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Set your Blynk token
char auth[] = "Sc0MrXzktOlQK_u08zqmbmRNnphP9qOp";

// Set your WiFi credentials
char ssid[] = "EZERDAM";
char pass[] = "ezerdam4389";

// DHT22
#define DHTPIN 25
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// HX711
#define LOADCELL_DOUT_PIN 32
#define LOADCELL_SCK_PIN 33
HX711 scale;

// I2C LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize the serial monitor
  Serial.begin(115200);

  // Initialize the DHT sensor
  dht.begin();

  // Initialize the HX711
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Display WiFi connection status on LCD
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("WiFi...");

  // Initialize Blynk and WiFi
  Blynk.begin(auth, ssid, pass);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Clear LCD and display WiFi connected message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");

  delay(2000); // Display connection status for 2 seconds

  // Clear LCD for further sensor readings
  lcd.clear();
}

void loop() {
  // Read DHT22 sensor values
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Read Load Cell value
  float weight = scale.get_units(5);

  // Display values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("%:");
  lcd.print(h);
  lcd.setCursor(9, 0);
  lcd.print("C:");
  lcd.print(t);
  lcd.setCursor(2, 1);
  lcd.print("Kg:");
  lcd.print(weight);

  // Send values to Blynk
  Blynk.virtualWrite(V0, h);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, weight);

  // Print values to Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C\t");
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" kg");

  // Add a delay
  delay(2000);

  // Run Blynk
  Blynk.run();
}
