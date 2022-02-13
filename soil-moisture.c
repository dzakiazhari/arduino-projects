#include <LiquidCrystal_I2C.h>
// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,16,2);  

int sensorPin = A0;  // pin for sensor
int powerPin = 6;    // input for VCC
int persen;
 
void setup() { 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight(); 
  // jadikan pin power sebagai output
  pinMode(powerPin, OUTPUT);
  // default bernilai LOW
  digitalWrite(powerPin, LOW);
  // mulai komunikasi serial
  Serial.begin(9600);
}
 
void loop() {
  lcd.clear();
  Serial.print("Nilai kelembaban: ");
  Serial.println(bacaSensor());
  lcd.setCursor(0,0);
  lcd.print("Nilai kelembapan: ");
  lcd.setCursor(0,1);
  lcd.print(bacaSensor());
  // baca setiap 2 detik
  delay(2000);
}
 
int bacaSensor() {
  // Turn on power
  digitalWrite(powerPin, HIGH);
  delay(500);
  // Read analog values from sensor
  int nilaiSensor = analogRead(sensorPin);
  digitalWrite(powerPin, LOW);
  // Higher value means higher moisture
 return 1023 - nilaiSensor;
}
