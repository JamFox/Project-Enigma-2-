/**
  This program is free software: you can redistribute it and / or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see < https : //www.gnu.org/licenses/>.
**/

/* Includes */
#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>
#include "IRremote.h"

/* Defines */
RF24 radio(9, 10); // Defines radio pins

/* Global variables */
byte addresses[][6] = {"1Node", "2Node"};
long data;

/* Private functions */
IRsend irsend;

void sendSamsung() {
  Serial.println("Sent Samsung:");
  switch (data) {
    case 0xa90:
      Serial.println("Power button");
      irsend.sendSAMSUNG(0xE0E040BF, 32);
      break;
    case 0x490:
      Serial.println("Volume Up");
      irsend.sendSAMSUNG(0xE0E0E01F, 32);
      break;
    case 0xc90:
      Serial.println("Volume Down");
      irsend.sendSAMSUNG(0xE0E0D02F, 32);
      break;
    case 0x90:
      Serial.println("Channel Up");
      irsend.sendSAMSUNG(0xE0E048B7, 32);
      break;
    case 0x890:
      Serial.println("Channel Down");
      irsend.sendSAMSUNG(0xE0E008F7, 32);
      break;
    case 0x290:
      Serial.println("Mute Button");
      irsend.sendSAMSUNG(0xE0E0F00F, 32);
      break;
    case 0xd10:
      Serial.println("Enter");
      irsend.sendSAMSUNG(0xE0E0807F, 32);
      break;
    case 0x10:
      Serial.println("1");
      irsend.sendSAMSUNG(0xE0E020DF, 32);
      break;
    case 0x810:
      Serial.println("2");
      irsend.sendSAMSUNG(0xE0E0A05F, 32);
      break;
    case 0x410:
      Serial.println("3");
      irsend.sendSAMSUNG(0xE0E0609F, 32);
      break;
    case 0xC10:
      Serial.println("4");
      irsend.sendSAMSUNG(0xE0E010EF, 32);
      break;
    case 0x210:
      Serial.println("5");
      irsend.sendSAMSUNG(0xE0E0906F, 32);
      break;
    case 0xA10:
      Serial.println("6");
      irsend.sendSAMSUNG(0xE0E050AF, 32);
      break;
    case 0x610:
      Serial.println("7");
      irsend.sendSAMSUNG(0xE0E030CF, 32);
      break;
    case 0xE10:
      Serial.println("8");
      irsend.sendSAMSUNG(0xE0E0B04F, 32);
      break;
    case 0x110:
      Serial.println("9");
      irsend.sendSAMSUNG(0xE0E0708F, 32);
      break;
    case 0x910:
      Serial.println("0");
      irsend.sendSAMSUNG(0xE0E08877, 32);
      break;
    case 0xDD0:
      Serial.println("PREV CHANNEL");
      irsend.sendSAMSUNG(0xE0E0C837, 32);
      break;
    default:
      Serial.println("Other button");
  }
  delay(250);
}

/* Arduino setup functions */
void setup() {
  pinMode (3, OUTPUT); // IR LED output pin
  Serial.begin(9600);
  radio.begin(); // Initiate the radio object
  radio.setAutoAck(true); // Enable automatic ack payloads
  radio.enableAckPayload(); // Enable ack payloads for replies for every packet sent
  radio.enableDynamicPayloads(); // Ack payloads are dynamic payloads, enable dynamic payloads
  radio.setPALevel(RF24_PA_MIN); // Set the transmit power to lowest
  radio.setDataRate(RF24_2MBPS); // Set the speed of the transmission to the quickest
  radio.setChannel(124); // // Set the channel
  radio.openWritingPipe(addresses[0]); // Open a writing pipe
  radio.openReadingPipe(1, addresses[1]); // Open reading pipe with opposite address to writing pipe
  radio.startListening(); // Start the radio listening for data
}

void loop() {
  static uint32_t ack = 1;
  if ( radio.available()) { // Is there any data for us to get?
    radio.writeAckPayload(1, &ack, sizeof(ack)); // Sends ack packet
    ++ack; // Counts ack packets, just for fun
    while (radio.available()) { // Go and read the data and put it into that variable
      radio.read( &data, sizeof(data));
    }
    radio.startListening();
    Serial.print("Button press received: ");
    Serial.println(data);
    sendSamsung();
  }
}
