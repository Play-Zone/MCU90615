/***********************************************************************

  Sample-Sketch for the MCU90615 Breakout

  Connections:
  Arduino -> Breakout
  VCC        VCC
  GND        GND
  IO 10      TX
  IO 11      RX

  Default Baudrate is 9600

  Copyright (c) 2017 GY & PLAY-ZONE GmbH http://www.play-zone.ch
  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associate documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

*************************************************************************/

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

unsigned char Re_buf[11], counter = 0;
unsigned char sign = 0;
float TO = 0, TA = 0;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  mySerial.listen();
  delay(10);
  
  mySerial.write(0XA5);
  mySerial.write(0X45);    // Enable Output
  mySerial.write(0XEA);
}

void loop() {
  unsigned char i = 0, sum = 0;
  while (mySerial.available()) {
    Re_buf[counter++] = (unsigned char)mySerial.read();
    if (counter == 1 && Re_buf[0] != 0x5A) {
      counter = 0;                // Waiting for Start of Data
      return;
    }
    if (counter == 2 && Re_buf[1] != 0x5A) {
      counter = 0;
      return;
    }
    if (counter == 9)             // Data Received
    {
      counter = 0;               // Get Ready for next Stream
      sign = 1;
    }
  }

  // Data received, so let's parse it
  if (sign)
  {
    sign = 0;
    for (i = 0; i < 8; i++)
      sum += Re_buf[i];
    if (sum == Re_buf[i] )     // Check header
    {
      TO = (float)(Re_buf[4] << 8 | Re_buf[5]) / 100;
      Serial.print("TO:");
      Serial.println(TO);
      TA = (float)(Re_buf[6] << 8 | Re_buf[7]) / 100;
      Serial.print("TA:");
      Serial.println(TA);
      Serial.println("");
    }
  }

}
