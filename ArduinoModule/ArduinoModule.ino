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
#include "IRremote.h"
#include "ir_Lego_PF_BitStreamEncoder.h"
#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>

/* Defines */
#define RECV_PIN  4 // IR receiver pin 
RF24 radio(9, 10); // Defines radio pins

/* Globals */
byte addresses[][6] = {"1Node", "2Node"};

/* Private functions */
IRrecv irrecv(RECV_PIN); // Declares the IR receiver function
decode_results results; // Declares function to save received signal into "results"

/* Setup */
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  radio.begin(); // Initiate the radio
  radio.setAutoAck(true); // Enable automatic ack payloads
  radio.enableAckPayload(); // Enable ack payloads for replies for every packet sent
  radio.enableDynamicPayloads(); // Ack payloads are dynamic payloads, enable dynamic payloads
  radio.setPALevel(RF24_PA_MIN); // Set the transmit power to lowest
  radio.setDataRate(RF24_2MBPS); // Set the speed of the transmission to the quickest
  radio.setChannel(124); // Set the channel
  radio.openWritingPipe(addresses[1]); // Open a writing pipe
  radio.openReadingPipe(1, addresses[0]); // Open reading pipe with opposite address to writing pipe
}

void myPR() { // If an IR signal is received, compares IR standards, prints the one currently received
  if (irrecv.decode(&results)) {
    if (results.decode_type == SONY) {
      Serial.print("Protocol:SONY   |");
    } else if (results.decode_type == SAMSUNG) {
      Serial.print("Protocol: SAMSUNG  | ");
    } else if (results.decode_type == UNKNOWN) {
      Serial.print("Protocol: UNKNOWN  | ");
    }
  }
}
void bitResults() { // Decodes SONY signal and prints the HEX and the sum of all bits
  if (irrecv.decode(&results)) {
    Serial.print("'HEX' decode: ");
    Serial.print(results.value, HEX);
    Serial.print("|   Number of bits: ");
    Serial.println(results.bits);

  }
}
void buttonPress() { //Forwards to serial monitor, which button was pressed
  switch (results.value) {
    case 0xa90:
      Serial.println("Power button");
      break;
    case 0x490:
      Serial.println("Volume Up");
      break;
    case 0xc90:
      Serial.println("Volume Down");
      break;
    case 0x90:
      Serial.println("Channel Up");
      break;
    case 0x890:
      Serial.println("Channel Down");
      break;
    case 0x290:
      Serial.println("Mute Button");
      break;
    case 0xd10:
      Serial.println("Enter");
      break;
    case 0x10:
      Serial.println("1");
      break;
    case 0x810:
      Serial.println("2");
      break;
    case 0x410:
      Serial.println("3");
      break;
    case 0xC10:
      Serial.println("4");
      break;
    case 0x210:
      Serial.println("5");
      break;
    case 0xA10:
      Serial.println("6");
      break;
    case 0x610:
      Serial.println("7");
      break;
    case 0xE10:
      Serial.println("8");
      break;
    case 0x110:
      Serial.println("9");
      break;
    case 0x910:
      Serial.println("0");
      break;
    case 0xDD0:
      Serial.println("PREV CHANNEL");
      break;
    default:
      Serial.println("Other button");
  }
  delay(250);
}

void loop() {
  long data;
  static uint32_t ack;
  radio.stopListening(); // Ensure we have stopped listening, can't transmit otherwise
  if (irrecv.decode(&results)) { // Checks if an IR signal is received
    /* Prints protocol, hex result and button */
    myPR(); //
    bitResults();
    buttonPress();
    /* nRF transmission */
    data = results.value;
    radio.write(&data, sizeof(data));
    Serial.print("Sent: ");
    Serial.println(data);
    if (radio.isAckPayloadAvailable()) { // Checks for ack packets
      radio.read(&ack, sizeof(ack));
      Serial.print("Ack: ");
      Serial.println(ack);
    } else {
      Serial.print("Transmission failed!"); // Lets know if ack failed
    }
    irrecv.resume(); // Resumes receiving, resets results
  }
  delay(1000);
}
