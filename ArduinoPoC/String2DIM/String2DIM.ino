// https://github.com/sandeepmistry/arduino-CAN
// Chinese MCP2515 comes with a 8 Mhz oscillator, you have to change it in the linked CAN library:
// MCP2515.h -> MCP2515_DEFAULT_CLOCK_FREQUENCY to 8e6.
// The setClockFrequency() method doesn`t worked for me.
#include <CAN.h>

// max length 32
const String TO_PRINT = "Hello           World           ";
const String TO_PRINT_FOTT = "Fottogesgatlo   szint alacsony  ";

//byte 6 diag1 commands
const int WINDOW_REAR_L_CLOSE = 0b00100000;
const int WINDOW_REAR_L_OPEN =  0b01000000;
const int WINDOW_REAR_R_CLOSE = 0b10000000;
//byte 7 diag1 commands
const int WINDOW_REAR_R_OPEN = 0b00000001;
const int LOW_BEAM_ON =  0b00001000;
const int HIGH_BEAM_ON=  0b00000100;
const int TAIL_LIGHTS_ON=0b00100000;
const int FOG_LIGHTS_ON = 0b01000000;
const int WIPER_ON = 0b10000000;
//diag2 commands
const int WINDOW_L_CLOSE = 0b00000001;
const int WINDOW_L_OPEN = 0b00000010;
const int WINDOW_R_CLOSE = 0b00001000;
const int WINDOW_R_OPEN = 0b00010000;


void txCAN(long id, uint8_t data[8]) {
  CAN.beginExtendedPacket(id);
  CAN.write(data[0]);
  CAN.write(data[1]);
  CAN.write(data[2]);
  CAN.write(data[3]);
  CAN.write(data[4]);
  CAN.write(data[5]);
  CAN.write(data[6]);
  CAN.write(data[7]);
  CAN.endPacket();
  delay(50);
}

void enableLCD() {
  // Enables LCD but also disabled media buttons needs to be done once
  uint8_t d[] = {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0b00000101};
  txCAN(0x0220200E, d);
  // LCD stays on but media buttons work again magic
  uint8_t d2[] = {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0b00000000};
  txCAN(0x0220200E, d2);
}

void disableLCD(){
  uint8_t d[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0b000100};
  txCAN(0x0220200E, d);
}

void clearLCD() {
  uint8_t d[] = {0xE1, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  txCAN(0x00C00008, d);
}

void printToLCD(String value) {
  char chars[31];
  value.toCharArray(chars, 31);
  char str[31] = {0};
  for (int i = 0; i < sizeof(str); i+=1) {
    str[i] = 0x20;
  }
  memcpy(str, chars, 31);
  
  uint8_t d[] = {0xA7, 0x00, str[0], str[1], str[2], str[3], str[4], str[5]};
  txCAN(0x00C00008, d);
  
  uint8_t d2[] = {0x21, str[6], str[7], str[8], str[9], str[10], str[11], str[12]};
  txCAN(0x00C00008, d2);

  uint8_t d3[] = {0x22, str[13], str[14], str[15], str[16], str[17], str[18], str[19]};
  txCAN(0x00C00008, d3);

  uint8_t d4[] = {0x23, str[20], str[22], str[23], str[24], str[25], str[26], str[27]};
  txCAN(0x00C00008, d4);

  uint8_t d5[] = {0x65, str[27], str[28], str[29], str[30], str[31], 0x00, 0x00};
  txCAN(0x00C00008, d5);
}

// Diag activations for CEM
void diag1_tx(int byte6, int byte7){
  uint8_t d[] = {0x8F, 0x40, 0xB1, 0x1A, 0x21, 0x01, byte6, byte7};
  txCAN(0x0FFFFE, d);
  uint8_t d1[] = {0x4E, 0x00, 0x00, byte6, byte7, 0x00, 0x00, 0x00};
  txCAN(0x0FFFFE, d1);

  if(byte6 == 0 && byte7 == 0){
    uint8_t d[] = {0xCD, 0x40, 0xB1, 0x1A, 0x21, 0x00, 0x00, 0x00};
    txCAN(0x0FFFFE, d);
  }
}

// Diag activations for Driver Door Module
void diag2_tx(int byte7){
  // Enable power command
  uint8_t d[] = {0xCE,0x43,0xB0,0x09,0x01,0x01,0x01,0x00};
  txCAN(0x0FFFFE, d);
  uint8_t d1[] = {0xCD,0x43,0xB0,0x10,0x01,0x03,0x00,byte7};
  txCAN(0x0FFFFE, d1);
}

void setup() {
  Serial.begin(250000);
  while (!Serial);
  Serial.println("String2DIM");

  // set your car's CAN bus speed, 125 Kbps for low speed CAN on 2002 Volvo S60
  if (!CAN.begin(125E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  
  enableLCD();
  printToLCD(TO_PRINT);
}

void loop() {
  Serial.println("Enter data:");
  while (Serial.available() == 0) {}
  String value = Serial.readString();
  value.trim();

  if (value == "WINDOWS_OPEN") {
    diag1_tx(WINDOW_REAR_L_OPEN,WINDOW_REAR_R_OPEN);
    diag2_tx(WINDOW_L_OPEN | WINDOW_R_OPEN);
    return;
  }
  if (value == "WINDOWS_CLOSE") {
    diag1_tx(WINDOW_REAR_L_CLOSE,0x00);
    diag1_tx(WINDOW_REAR_R_CLOSE,0x00);
    diag2_tx(WINDOW_L_CLOSE | WINDOW_R_CLOSE);
    return;
  }

  if (value.length() > 32) {
    Serial.println("Too long, max 32 char");
  } else {
    printToLCD(value);
    Serial.println("Print to LCD completed");
  }
}
