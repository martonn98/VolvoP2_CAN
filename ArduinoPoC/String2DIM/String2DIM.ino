// https://github.com/sandeepmistry/arduino-CAN
// Chinese MCP2515 comes with a 8 Mhz oscillator, you have to change it in the linked CAN library:
// MCP2515.h -> MCP2515_DEFAULT_CLOCK_FREQUENCY to 8e6.
// The setClockFrequency() method doesn`t worked for me.
#include <CAN.h>

// max length 32
const String TO_PRINT = "Hello           World           ";
const String TO_PRINT_FOTT = "Fottogesgatlo   szint alacsony  ";

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

void clearLCD() {
  uint8_t d[] = {0xE1, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  txCAN(0x00C00008, d);
}

void printToLCD(String value) {
  char chars[32];
  value.toCharArray(chars, 32);
  char str[32] = {0};
  for (int i = 0; i < sizeof(str); i+=1) {
    str[i] = 0x20;
  }
  memcpy(str, chars, 32);
  
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

  if (value.length() > 32) {
    Serial.println("Too long, max 32 char");
  } else {
    printToLCD(value);
    Serial.println("Print to LCD completed");
  }
}
