void init_radio() {
  serial_radio.begin(9600);
  serial_radio.listen();
}

void radio_listening() {
  DEBUG_PRINTLN("radio listening...");
  uint8_t payload_received[13];
  // Waiting instruction from gateway
  
  digitalWrite(pin_enable, LOW);
  serial_radio.listen();
  while (serial_radio.available() < 10);
  Serial.print("Data Received = ");
  for (int i = 0; i < 13; i++) {
    payload_received[i] = (serial_radio.read() & 0xFF) ;
    Serial.print(payload_received[i] & 0xFF, HEX);Serial.print("-");
    serial_radio.flush();
    Serial.flush();
  }
  serial_radio.end();
  Serial.println("");
//  printNumber("received data: ", payload_received, 13);
  parsing_header(payload_received);
}
