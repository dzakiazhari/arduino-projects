void init_gps(){
  serial_gps.begin(9600);
  serial_gps.listen();
}

void read_gps(){
  serial_gps.listen();
  delay(100);
  while(serial_gps.available()){
    gps.encode(serial_gps.read());
  }
  Serial.print("satelite = ");Serial.println(gps.satellites.value());
  if(gps.location.isUpdated()){
    latitude_data = (gps.location.lat());
    longitude_data = (gps.location.lng());
  }
  else{
    if((latitude_data==0) && (longitude_data==0)){
      latitude_data = -7.758855;
      longitude_data = 110.376333;
    }
    else{
      latitude_data = latitude_data;
      longitude_data = longitude_data;
    }
  }
}
