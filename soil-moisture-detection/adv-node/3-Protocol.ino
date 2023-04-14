/*
# standard data format
# header        : 6 bytes ("GMLEWS")
# data length   : 2 bytes
# data          : 3 bytes
# crc           : 1 bytes

# standard data format
# source node       : 1 bytes
# destination node  : 1 bytes
# command           : 1 bytes
*/

/*format LoRa Dorji command first
sync word , ID code, Header, Command, Length, Data, CRC, End Code

*/

#define RET_NG              0
#define RET_OK              1

#define MSG_CMD_LEN         13
#define MSG_STATUS_LEN      12  // jumlah data yang dikirim dari node untuk PING
#define PAYLOAD_LEN         34  //jumlah data yang dikirim
#define MSG_HEADER          "GMLEWS"
#define MSG_HEADER_LEN      6

#define CMD_PING            0x35
#define CMD_GET_DATA        0x36

#define NODE_STATUS_OK      1
#define NODE_STATUS_NG      0

typedef union
{
  float number;
  uint8_t bytes[4];
} floatunion_t;

uint8_t payload[PAYLOAD_LEN];
uint8_t checksum_t = 0; //checksum data transmit

//=========================Parsing header
void parsing_header(char *pMessage) {
  uint8_t header_t[MSG_HEADER_LEN];
  uint8_t dataMessage[13];
  uint8_t dest[1];
  copyChars(header_t, 1, pMessage, 6);
  copyChars(dest, 0, pMessage, 1);
//  printNumber("header_t in parsing_header: ", header_t, MSG_HEADER_LEN);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Data Received");
  lcd.setCursor(0,1);
  lcd.print("Processing..");
  if(!strncmp(header_t, MSG_HEADER, 6)){    //compare string header
    DEBUG_PRINTLN("GMLEWS header OK");
    copyChars(dataMessage,0,pMessage,12);
    parsing_data(dataMessage);    //parsing data
    } else {
    DEBUG_PRINTLN("unknown header");
    }
}

void parsing_data(char *pData) {
  char command = CMD_GET_DATA; // replace to get data or ping data
  char msg[24]; // store array untuk pesan text sprintf
  char retval;
  switch(command){
      case  CMD_PING: sprintf(msg,"RX: PING NODE-%02d",lora_node_id);
                      DEBUG_PRINTLN(msg);
                      delay(3000);
                      retval = response_status();  //=================== Respon status PING
                      if(retval == RET_OK) {
                        sprintf(msg,"TX: NODE-%02d STATUS OK",lora_node_id);
                      } else {
                        sprintf(msg,"TX: NODE-%02d STATUS NG",lora_node_id);
                      }
                      DEBUG_PRINTLN(msg);
                      break;
      case  CMD_GET_DATA: sprintf(msg,"RX: GET DATA NODE-%02d",lora_node_id);
                          DEBUG_PRINTLN(msg);
                          delay(3000);
                          retval = send_payload();   //=================== Respon status DATA
                          if(retval == RET_OK) {
                            Serial.println("");
                            sprintf(msg,"TX: NODE-%02d DATA SENT",lora_node_id);
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("Data Sent");
                          } else {
                            Serial.println("");
                            sprintf(msg,"TX: NODE-%02d DATA ERROR",lora_node_id);
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("Data Failed to sent");
                          }
                          DEBUG_PRINTLN(msg);
                          break;
  }  
}
bool send_payload(){
  serial_radio.listen();
  clearVariable(payload, PAYLOAD_LEN);
  prepare_payload(payload);
  printNumber("Payload To Send: ", payload, 34);
  for (int i = 0; i < PAYLOAD_LEN; i++) {
    serial_radio.write(payload[i]);
  }
  delay(1000);
  Serial.println("Payload Sent.");
  serial_radio.end();
  return RET_OK; 
}

void prepare_payload(uint8_t* payload_t){
  read_sensor();

  floatunion_t flat;
  floatunion_t flng;
  flat.number = latitude_data;
  flng.number = longitude_data;

  copyChars(payload_t, 0, MSG_HEADER, 6);
  
  payload_t[6] = (uint8_t)lora_node_id;
  payload_t[7] = (uint8_t)gateway_id;

  // Lat
  payload_t[8] = flat.bytes[0];
  payload_t[9] = flat.bytes[1];
  payload_t[10] = flat.bytes[2];
  payload_t[11] = flat.bytes[3];

  // Lng
  payload_t[12] = flng.bytes[0];
  payload_t[13] = flng.bytes[1];
  payload_t[14] = flng.bytes[2];
  payload_t[15] = flng.bytes[3];

  // Ang
  payload_t[16] = (uint8_t)(Ang & 0xFF);
  payload_t[17] = (uint8_t)((Ang >> 8) & 0xFF);

  //Acc-X
  payload_t[18] = (uint8_t)(AccX & 0xFF);
  payload_t[19] = (uint8_t)((AccX >> 8) & 0xFF);
  //Acc-Y
  payload_t[20] = (uint8_t)(AccY & 0xFF);
  payload_t[21] = (uint8_t)((AccY >> 8) & 0xFF);
  //Acc-Z
  payload_t[22] = (uint8_t)(AccZ & 0xFF);
  payload_t[23] = (uint8_t)((AccZ >> 8) & 0xFF);

  // Gy-X
  payload_t[24] = (uint8_t)(GyyX & 0xFF);
  payload_t[25] = (uint8_t)((GyyX >> 8) & 0xFF);
  // Gy-Y
  payload_t[26] = (uint8_t)(GyyY & 0xFF);
  payload_t[27] = (uint8_t)((GyyY >> 8) & 0xFF);
  // Gy-Z
  payload_t[28] = (uint8_t)(GyyZ & 0xFF);
  payload_t[29] = (uint8_t)((GyyZ >> 8) & 0xFF);

  // Vibration
  payload_t[30] = (uint8_t)(vibration_data & 0xFF);

  // Moisture
  payload_t[31] = (uint8_t)(moisture_data & 0xFF);

  // Moisture
  payload_t[32] = (uint8_t)(hitung_data & 0xFF);

  // Checksum
  checksum_t = get_checksum(payload, 33);
  payload_t[33] = checksum_t;

}

void printNumber(const char *name, const uint8_t *x, uint8_t len){
    static const char hexchars[] = "0123456789ABCDEF";
    Serial.print(name);
    for (uint8_t posn = 0; posn < len; ++posn) {
        if(posn % 32 == 0){ 
          Serial.println();
        }
        Serial.print(hexchars[(x[posn] >> 4) & 0x0F]);
        Serial.print(hexchars[x[posn] & 0x0F]);
        Serial.print(" ");
    }
    Serial.println();
}

void clearVariable( uint8_t* input, uint8_t isize){
  for(int i=0; i < isize; ++i){
    input[i] = 0;
  }
}

void copyChars(uint8_t* output, uint8_t startpos, uint8_t* input, uint8_t len){
  byte j = 0;
  for(int i=startpos; i < (startpos + len); ++i){
    output[j++] = input[i];
  }
}

char get_checksum(char *pS, int len) {
  char checksum = 0;
  for (int i=0;i< len; i++) {
    checksum += pS[i];
  }
  checksum = -(checksum % 256) & 0xFF;
  return checksum;
}

char response_status() {   //=============== RESPONSE STATUS PING
  char Message[MSG_STATUS_LEN];
  char *pMsg = Message;
  short *pS;
  short data_len = 3;
  digitalWrite(pin_enable, LOW);
  memcpy(pMsg,MSG_HEADER,MSG_HEADER_LEN);
  pMsg += MSG_HEADER_LEN;
  pS = (short *) pMsg;
  *pS++ = data_len;
  pMsg = (char *) pS;
  *pMsg++ = lora_node_id;
  *pMsg++ = gateway_id;
  *pMsg++ = NODE_STATUS_OK;
  char crc_data = get_checksum(Message,MSG_STATUS_LEN-1);
  *pMsg = crc_data;
  //serial_radio.listen();
  pMsg = Message;
  for (int i = 0; i < MSG_STATUS_LEN; i++) {
    serial_radio.write(*(pMsg+i));
    Serial.print(*(pMsg+i)&0xFF,HEX); Serial.print("-");
  }
  Serial.println("");
  delay(1000);
  serial_radio.end();
  return RET_OK;             
}
