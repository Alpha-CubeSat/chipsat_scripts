/*
   RadioLib SX1278 Transmit Example

   This example transmits packets using SX1278 LoRa radio module.
   Each packet contains up to 256 bytes of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)

   Other modules from SX127x/RFM9x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/


#include <RadioLib.h>
#define ERR_NONE 0
#define ERR_CRC_MISMATCH -7
#define ERR_PACKET_TOO_LONG -4
#define ERR_TX_TIMEOUT -5

float freq = 433.0;
float bw = 125.0;
int sf = 12;
int cr = 5;
int sw = 18;
int pwr = 20;
int pl = 8;
int gn = 0;

#define RADIO_CS_PIN 10
#define RADIO_DI0_PIN 2
#define RADIO_RST_PIN 9
#define RADIO_BUSY_PIN A2

RFM96 radio = new Module(RADIO_CS_PIN, RADIO_DI0_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);

void setup()
{
    Serial.begin(115200);
    // initialize SX1278 with default settings
    Serial.print(F("[SX1278] Initializing ... "));

    // begin(freq, bw, sf, cr, syncWord, power, preambleLength, gain)
    int state = radio.begin(freq,bw,sf,cr,sw,pwr,pl,gn);

    if (state == 0) { //ERR_NONE
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
      while (true);
    }

    // saftey check to make sure frequency is at 436.7
    Serial.print(F("[RF96] Setting frequency ... "));
    state = radio.setFrequency(freq);
    if (state == 0) { //ERR_NONE
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
      while (true);
    }

    // adjust output power, avialable ranges: -3 to 15 dBm
    // increasing power increases range of transmission
    Serial.print(F("[RF96] Setting Output Power parameter ... "));
    state = radio.setOutputPower(pwr); 
    if (state == 0) { //ERR_NONE
      Serial.println(F("success!"));
    } else {
      Serial.print(F("Output Power initialization error"));
      Serial.println(state);
      while (true);
    }
  
    // adjust spreading factor, avialable ranges: SF7 to SF12 (7 to 12)
    // increasing spreading factor increases range of transmission, but increases the time to transmit the message
    Serial.print(F("[RF96] Setting Spreading Factor parameter ... "));
    state = radio.setSpreadingFactor(sf); 
    if (state == 0) { //ERR_NONE
      Serial.println(F("success!"));
    } else {
      Serial.print(F("Spreading Factor initialization error"));
      Serial.println(state);
      while (true);
    }
  
    // set CRC parameter to true so it matches the CRC parameter on the TinyGS side
    Serial.print(F("[RF96] Setting CRC parameter ... "));
    state = radio.setCRC(true); 
    if (state == 0) { //ERR_NONE
      Serial.println(F("success!"));
    } else {
      Serial.print(F("CRC initialization error"));
      Serial.println(state);
      while (true);
    }
  
    // set forceLDRO parameter to true so it matches the forceLDRO parameter on the TinyGS side
    Serial.print(F("[RF96] Setting forceLDRO parameter ... "));
    state = radio.forceLDRO(true); // FLDRO = 1 on TinyGS side
    if (state == 0) { //ERR_NONE
      Serial.println(F("success!"));
    } else {
      Serial.print(F("forceLDRO initialization error"));
      Serial.println(state);
      while (true);
    }
    if (state == ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true);
    }
}

uint8_t byte_counter = 0x00;

void loop()
{
    Serial.print(F("[SX1278] Transmitting packet ... "));

    // you can transmit C-string or Arduino string up to
    // 256 characters long
    // NOTE: transmit() is a blocking method!
    //       See example SX127x_Transmit_Interrupt for details
    //       on non-blocking transmission method.
    //int state = radio.transmit("Hello World!");

    // you can also transmit byte array up to 256 bytes long
    /*
      byte byteArr[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
      int state = radio.transmit(byteArr, 8);
    */

  if (byte_counter == 10){
    byte_counter = 0x00;
  }
  byte byteArr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, byte_counter};
//  byte byteArr[] = {byte_counter, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  int state = radio.transmit(byteArr, 8);
  byte_counter ++;
  
    if (state == ERR_NONE) {
        // the packet was successfully transmitted
        Serial.println(F(" success!"));

        // print measured data rate
        Serial.print(F("[SX1278] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));
    }
    else if (state == ERR_PACKET_TOO_LONG) {
        // the supplied packet was longer than 256 bytes
        Serial.println(F("too long!"));

    } else if (state == ERR_TX_TIMEOUT) {
        // timeout occurred while transmitting packet
        Serial.println(F("timeout!"));

    } else {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);

    }

    // wait for a second before transmitting again
    delay(100);
}
