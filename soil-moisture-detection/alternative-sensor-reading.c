int bacaSensor() {
  // Turn on power
  digitalWrite(powerPin, HIGH);
  delay(500);
  // Read analog values from sensor
  int nilaiSensor = analogRead(sensorPin);
  persen = map(nilaiSensor,380,1020,0,100);
  persen = 100-persen;
  digitalWrite(powerPin, LOW);
  // Moisture value in percentage
 return persen;
}
