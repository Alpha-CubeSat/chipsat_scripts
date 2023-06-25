#include <Arduino_LSM9DS1.h>

float gyroX, gyroY, gyroZ;

float accelX, accelY, accelZ;

void setup() {
  Serial.begin(9600);
  Serial.println("initializing IMU");
  if (!IMU.begin()) { //initialize the IMU - returns 0 if failure and 1 if success
    Serial.println("Failed to initialize LSMD9DS1 IMU");
    //while (1);
  }
}

void loop() {


  if(IMU.gyroscopeAvailable()) { //check if the gyroscope has new data available
    IMU.readGyroscope(gyroX, gyroY, gyroZ); //library function to read from the gyroscope

    //Print Gyroscope Values
    Serial.println("Reading Gyroscopic Values");
    Serial.print("X: ");
    Serial.println(gyroX);
    Serial.print("Y: ");
    Serial.println(gyroY);
    Serial.print("Z ");
    Serial.println(gyroZ);
  }

  if(IMU.accelerationAvailable()) { //check if accelerometer is available
    IMU.readAcceleration(accelX, accelY, accelZ);

    //Print Accelerometer Values
    Serial.println("Reading Accelerometer Values");
    Serial.print("X: ");
    Serial.println(accelX);
    Serial.print("Y ");
    Serial.println(accelY);
    Serial.print("Z ");
    Serial.println(accelZ);
  }

  //IMU.end(); //de-initialize the IMU

  delay(1000); //wait 1 second in between measurements

}
