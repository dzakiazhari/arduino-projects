//================================Library
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

//================================LCD global variable
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

//===============================LoRa Pin definition and global variable
#define pin_enable 13
#define pin_radio_TX   12
#define pin_radio_RX   11
SoftwareSerial serial_radio (pin_radio_RX, pin_radio_TX);

//================================DHT sensor pin and global variable
#define DHTPIN 2               
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);   
short humidity, temp;

//===============================GPS global variable
TinyGPSPlus gps;
float latitude_data, longitude_data;
#define pin_gps_TX 8
#define pin_gps_RX 7
SoftwareSerial serial_gps (pin_gps_RX, pin_gps_TX);

//===============================Rotary Encoder pin and variable
#define outputA 6 //clk
#define outputB 5 //dt
short counter = 0;
uint8_t hitung_data;
int aState = LOW;
int bState = LOW;
int aLastState = LOW;

//==============================Vibration pin analog and variable
#define pin_vibration     A2
short vibration_data;

//==============================Soil pin analog and variable
#define pin_soil_moisture A3
short moisture_data;

//==============================MPU6050
const int MPU_addr=0x68;  // I2C address of the MPU-6050
//int16_t AcX,AcY,AcZ,GyX,GyY,GyZ;
short AccX,AccY,AccZ,GyyX,GyyY,GyyZ,Ang;

#define   DEBUG_SERIAL
//#define   DEBUG_OLED
#ifdef DEBUG_SERIAL
  #define DEBUG_PRINTLN(x)  Serial.println(x)
#endif

//=============================DEFINE NODE ID AND GATEWAY ID
char lora_node_id = 2; //node id
char gateway_id = 0; // gateway id
//=============================konversi analog read moisture dan vibration
int highValue = 1023; //for adc converter to 100%
int lowValue = 0;     //for adc converter to 0%

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  DEBUG_PRINTLN("OLED INIT..");
  init_lcd();
  delay(2000);

  DEBUG_PRINTLN("LoRa Init..");
  init_radio();
  delay(500);

  DEBUG_PRINTLN("GPS init...");
  init_gps();
  delay(500);

  DEBUG_PRINTLN("IMU init...");
  init_imu();
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_sensor();
  print_sensor();
  radio_listening();
}

void print_sensor(){
  Serial.print("suhu=");              Serial.println(temp);
  Serial.print("Kelembaban=");        Serial.println(humidity);
  Serial.print("Sensor 1 Lat= ");               Serial.println(latitude_data);
  Serial.print("Sensor 2 Long= ");              Serial.println(longitude_data);
  Serial.print("Sensor 3 Ang= ");               Serial.println(Ang);
  Serial.print("Sensor 4 Accx m/s= ");              Serial.println(AccX);
  Serial.print("Sensor 5 Accy m/s= ");              Serial.println(AccY);
  Serial.print("Sensor 6 Accz m/s= ");              Serial.println(AccZ);
  Serial.print("Sensor 7 GyX= ");               Serial.println(GyyX);
  Serial.print("Sensor 8 GyY= ");               Serial.println(GyyY);
  Serial.print("Sensor 9 GyZ= ");               Serial.println(GyyZ);
  Serial.print("Sensor 10 vibration= ");         Serial.println(vibration_data);
  Serial.print("Sensor 11 Soil Moisture= ");      Serial.println(moisture_data);
  Serial.print("Sensor 12 Displacement= ");      Serial.println(hitung_data);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp=");
  lcd.setCursor(5,0);
  lcd.print(temp);
  lcd.setCursor(8,0);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Hum=");
  lcd.setCursor(4,1);
  lcd.print(humidity);
  lcd.setCursor(7,1);
  lcd.print("%");
}

void read_sensor(){
  read_dht();
  read_gps();
  read_imu();
  read_soil_moisture();
  read_vibration();
  read_rotary();
}
