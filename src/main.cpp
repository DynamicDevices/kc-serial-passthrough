/*

  SerialPassthrough sketch

  Some boards, like the Arduino 101, the MKR1000, Zero, or the Micro, have one

  hardware serial port attached to Digital pins 0-1, and a separate USB serial

  port attached to the IDE Serial Monitor. This means that the "serial

  passthrough" which is possible with the Arduino UNO (commonly used to interact

  with devices/shields that require configuration via serial AT commands) will

  not work by default.

  This sketch allows you to emulate the serial passthrough behaviour. Any text

  you type in the IDE Serial monitor will be written out to the serial port on

  Digital pins 0 and 1, and vice-versa.

  On the 101, MKR1000, Zero, and Micro, "Serial" refers to the USB Serial port

  attached to the Serial Monitor, and "Serial1" refers to the hardware serial

  port attached to pins 0 and 1. This sketch will emulate Serial passthrough

  using those two Serial ports on the boards mentioned above, but you can change

  these names to connect any two serial ports on a board that has multiple ports.

  created 23 May 2016

  by Erik Nyquist

*/

#include <Arduino.h>


#define MODEM_POWER_ON 3
#define MODEM_PWRKEY 21

#define MODEM_RX 18 
#define MODEM_TX 17

#define SerialMon Serial0
#define SerialAT Serial1

#define BAUD_RATE 115200

void setup() {

  SerialMon.begin(BAUD_RATE);
  SerialAT.begin(BAUD_RATE, SERIAL_8N1, MODEM_RX, MODEM_TX);

  SerialMon.println("Starting - console\n");

  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);

  // Turn off modem
  digitalWrite(MODEM_POWER_ON, LOW);
  delay(1000);
  // Turn on the Modem power first
  digitalWrite(MODEM_POWER_ON, HIGH);
  delay(1000);

  // Pull down PWRKEY for more than 1 second according to manual requirements
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(100);
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, LOW);
}

void loop() {

  if (SerialMon.available()) {      // If anything comes in Serial (USB),
    SerialAT.write(SerialMon.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (SerialAT.available()) {     // If anything comes in Serial1 (pins 0 & 1)

    SerialMon.write(SerialAT.read());   // read it and send it out Serial (USB)

  }
}