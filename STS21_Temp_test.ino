#include <Wire.h>


//Define the STS21 address for proper I2C selection
#define addr 0x4A

void setup() {
  //Init I2C communication as master
  Wire.begin();

  Serial.begin(9600);
  delay(300);

}

void loop() {
  unsigned int data[2];

  //Start I2C transmit
  Wire.beginTransmission(addr);
  //Select no hold master
  Wire.write(0xF3);
  Wire.endTransmission();
  delay(300);

  //Request 2 bytes of data
  Wire.requestFrom(addr, 2);

  if(Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data
  int rawtmp = data[0] * 256 + data[1];
  int value = rawtmp & 0xFFFC;
  double cTemp = -46.85 + (175.72 * (value / 65536.0));
  double fTemp = cTemp * 1.8 + 32;

  // Output data to serial monitor
  Serial.print("Temperature in Celsius:  ");
  Serial.print(cTemp);
  Serial.println(" C");
  Serial.print("Temperature in Fahrenheit:  ");
  Serial.print(fTemp);
  Serial.println(" F");
  delay(300);

}
