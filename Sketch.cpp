/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
/*End of auto generated code by Atmel studio */

#include <Wire.h>	//Arduino wraper around TWI
#include <LiquidCrystal.h>	//Library for 16x2 LCD with HD44780
#include "MPU6050.h"	//header for MPU6050 registers and functions
#include "KalmanFilter.h"	//header for Kalman Filter

MPU6050 mpu;	//mpu is object of MPU6050 class

KalmanFilter kalmanX(0.001, 0.03, 0.03);	//Kalman Filter in X plane. 
KalmanFilter kalmanY(0.001, 0.03, 0.03);	//
 
// Obliczone wartosci Pitch i Roll tylko z akcelerometru
float accPitch = 0;
float accRoll = 0;
 
// Obliczone wartosci Pitch i Roll z uwzglednieniem filtru Kalmana i zyroskopu
float kalPitch = 0;
float kalRoll = 0;

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 3, d7 = 2;	//declaration of pins check those!!!
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
void setup()
{
	pinMode(6, OUTPUT);
	digitalWrite(6, LOW);
	
 	
  // Inicjalizacja MPU6050
	Serial.println("Inicjalizacja MPU6050");
	while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
	{
		lcd.print("MPU6050 not found");
	}
 
	lcd.begin(16, 2); // Print a message to the LCD.
	lcd.print("Calibration");
	mpu.calibrateGyro(); //calibrate gyroscope
}
 
void loop()
{
	//loop for iterating Kalman Filter
	for ( int i = 0 ; i < 100 ; i++){
		Vector acc = mpu.readNormalizeAccel();
		Vector gyr = mpu.readNormalizeGyro();
     
		// Kalukacja Pitch &amp; Roll z akcelerometru
		accPitch = -(atan2(acc.XAxis, sqrt(acc.YAxis*acc.YAxis + acc.ZAxis*acc.ZAxis))*180.0)/M_PI;
		accRoll  = (atan2(acc.YAxis, acc.ZAxis)*180.0)/M_PI;
     
		// Kalman - dane z akcelerometru i zyroskopu
		kalPitch = kalmanY.update(accPitch, gyr.YAxis);
		kalRoll = kalmanX.update(accRoll, gyr.XAxis);	  
	}
	uint8_t thisLetter[] = "asds";
	//lcd.print(*thisLetter);
	lcd.setCursor(0, 0);	//top left
	lcd.print("Pitch  ");
	lcd.print(kalPitch);
	lcd.setCursor(0, 1); // bottom left
	lcd.print("Roll  ");
	lcd.print(kalRoll);
}
