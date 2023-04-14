void read_dht(){
  delay(500);

  float hum_1 = dht.readHumidity();
  float tmp_1 = dht.readTemperature();

  humidity = hum_1*1;
  temp = tmp_1*1;
}
