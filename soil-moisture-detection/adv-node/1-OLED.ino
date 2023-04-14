//clearing text on OLED Screen
void init_lcd(){
  lcd.begin(16,2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GMLEWS Start..");
}
