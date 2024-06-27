#include <Wire.h>
#include <SFE_BMP180.h>
#include <Servo.h>

SFE_BMP180 bmp180;
Servo Release;

float p0 = 848.72;//848.72
double T, P, alt, maxalt, avgalt, a1, a2, a3;
int i, iavgcount = 0;
int pos = 5;
bool calibrated = false;

void setup() 
  {     
        a1 = 0;
        a2 = 0;
        a3 = 0;
        Serial.begin(9600);
    if (!bmp180.begin())
      {
        Serial.println("Could not find a valid BMP180 sensor, check wiring!");
        while (1);
      }
        pinMode(2, INPUT_PULLUP); //button
        Release.attach(9);
        Release.write(5);

    digitalWrite(11, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);

    delay(800);

    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
        
  }

void loop() 
  {
    char Status;

    Status = bmp180.startTemperature();

    if (Status != 0)
      {
        delay(50);
        Status = bmp180.getTemperature(T); 
      }

      if (Status != 0)
      {
        Status = bmp180.startPressure(3);
      }

      if (Status != 0)
      {
        delay(Status);
        Status = bmp180.getPressure(P, T);

        //digitalWrite(13, HIGH);
        if (digitalRead(2) == LOW) 
          {
            p0 = P;
            Serial.print("initial pressure updated to :");
            Serial.println(p0);
            digitalWrite(11, LOW);
            digitalWrite(12, HIGH);
            digitalWrite(13, LOW);
            maxalt = 0;
            calibrated = true;
            Release.write(0);
            a1 = 0;
            a2 = 0;
            a3 = 0;
          }
        

      }

      if (Status != 0)
      {
        avgalt = bmp180.altitude(P, p0); 

      switch (iavgcount) 
  {
    case 0:
      a1 = avgalt;
      iavgcount++;
    break;
    case 1:
      a2 = avgalt;
      iavgcount++;
    break;
    case 2:
      a3 = avgalt;
      iavgcount = 0;
      alt = ((a1 + a2 + a3) / 3);
    break;
    default:
      iavgcount = 0;
    break;
  }
        //Serial.println(a1);
        //Serial.println(a2);
        //Serial.println(a3);
        Serial.println(alt);


        if (alt > maxalt)
          {
            maxalt = alt;
          }
          //Serial.println(calibrated);
          //if (alt < maxalt - 3)
          if (alt < maxalt - 2 && calibrated == 1)
          {
              digitalWrite(11, LOW);
              digitalWrite(12, LOW);
              digitalWrite(13, HIGH);
            calibrated = false;
            //Serial.println("servo");
            for (pos = 0; pos < 90; pos += 1)
              {
                Release.write(pos);

              }

          }
      }

        //Serial.print("Altitude :");
        //Serial.print(alt);
        //Serial.println(" Meters");
        //Serial.println(maxalt);

  }




  /////////


