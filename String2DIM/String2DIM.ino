#include <CAN.h>

// max length 32
const String TO_PRINT = "Hello           World           ";
const String TO_PRINT_FOTT = "Fottogesgatlo   szint alacsony  ";

void printToLCD(String value) {
  char copy[31];
  value.toCharArray(copy, 31);
  char str[31] = {0};
  for (int i = 0; i < sizeof(str); i+=1) {
    str[i] = 0x20;
  }
  memcpy(str, copy, 31);
  
  CAN.beginExtendedPacket(0x00C00008);
  CAN.write(0xA7);
  CAN.write(0x00);
  CAN.write(str[0]);
  CAN.write(str[1]);
  CAN.write(str[2]);
  CAN.write(str[3]);
  CAN.write(str[4]);
  CAN.write(str[5]);
  CAN.endPacket();
  delay(50);
  
  CAN.beginExtendedPacket(0x00C00008);
  CAN.write(0x21);
  CAN.write(str[6]);
  CAN.write(str[7]);
  CAN.write(str[8]);
  CAN.write(str[9]);
  CAN.write(str[10]);
  CAN.write(str[11]);
  CAN.write(str[12]);
  CAN.endPacket();
  delay(50);
  
  CAN.beginExtendedPacket(0x00C00008);
  CAN.write(0x22);
  CAN.write(str[13]);
  CAN.write(str[14]);
  CAN.write(str[15]);
  CAN.write(str[16]);
  CAN.write(str[17]);
  CAN.write(str[18]);
  CAN.write(str[19]);
  CAN.endPacket();
  delay(50);
  
  CAN.beginExtendedPacket(0x00C00008);
  CAN.write(0x23);
  CAN.write(str[20]);
  CAN.write(str[21]);
  CAN.write(str[22]);
  CAN.write(str[23]);
  CAN.write(str[24]);
  CAN.write(str[25]);
  CAN.write(str[26]);
  CAN.endPacket();
  delay(50);
  
  CAN.beginExtendedPacket(0x00C00008);
  CAN.write(0x65);
  CAN.write(str[27]);
  CAN.write(str[28]);
  CAN.write(str[29]);
  CAN.write(str[30]);
  CAN.write(str[31]);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.endPacket();
}

void setup() {
  Serial.begin(250000);
  while (!Serial);
  Serial.println("CAN Sender");

  // adjust to your MCP2515 oscillator, most chiniese unite comes with 8 Mhz, default is 16 Mhz
  CAN.setClockFrequency(8E16);
  // set your car's CAN bus speed, 125 Kbps for low speed CAN on pre-facelift P2 Volvos (-2004)
  if (!CAN.begin(125E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  // enable lcd
  CAN.beginExtendedPacket(0x0220200E);
  CAN.write(0xC0);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0b00000101);
  CAN.endPacket();
  delay(50);
  
  // media button work again magic
  CAN.beginExtendedPacket(0x0220200E);
  CAN.write(0xC0);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0b00000000);
  CAN.endPacket();
  delay(50);
  
  // clear lcd (not mandatory before print)
  CAN.beginExtendedPacket(0x00C00008);
  CAN.write(0xE1);
  CAN.write(0xFE);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.write(0x00);
  CAN.endPacket();
  delay(50);
  
  printToLCD(TO_PRINT);
  Serial.println("Print to LCD completed");
}

void loop() {

}
