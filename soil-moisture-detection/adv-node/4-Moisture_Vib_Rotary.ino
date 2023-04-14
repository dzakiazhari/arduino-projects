int moisture, vibration;

void read_soil_moisture(){
  moisture = analogRead(pin_soil_moisture);
  moisture_data = map(moisture,highValue,lowValue,0,100);
}
void read_vibration(){
  vibration = analogRead(pin_vibration);
  vibration_data = map(vibration,highValue,lowValue,100,0);
}
void read_rotary(){
  aState = digitalRead(outputA);
  bState = digitalRead(outputB);
  if (aState != aLastState){
    bState = aState^bState;
    if(bState){//digitalRead(outputB != aState)
      counter--;
    } else {
      counter++;  
    }
  }
  aLastState = aState;
  hitung_data = counter * 2; // permisalan dalam cm untuk satu kali rotate
}
