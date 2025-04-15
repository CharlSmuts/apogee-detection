#include <Wire.h>
#include <SFE_BMP180.h>
#include <Servo.h>

SFE_BMP180 bmp180;
Servo Release;

float p0 = 848.72;//848.72
double a1, a2, a3, avg;
double maxalt = 0;
int maxdisplay = 0;
int pos = 5;
bool calibrated = false;
bool recheck, apogee = false;


void setup() 
  {
    Serial.begin(9600);
    if (!bmp180.begin())
      {
        Serial.println("Could not find a valid BMP180 sensor, check wiring!");
        while (1);
      }

    pinMode(2, INPUT_PULLUP); //button
    Release.attach(9);
    Release.write(5);

    digitalWrite(11, HIGH); //white
    digitalWrite(12, HIGH); //green
    digitalWrite(13, HIGH); //red

    delay(800);

    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);

  }



void loop() 
  {
    if (calibrated == true)
      {
        a1 = altitude();
        a2 = altitude();
        a3 = altitude();
        avg = (a1 + a2 + a3) / 3;

        Serial.println(avg);

        if (avg > maxalt)
          {
            maxalt = avg;
          }

        if ((avg < maxalt - 2) && (recheck == 0))
          {
            digitalWrite(11, HIGH);
            recheck = true;
          } 
        else
        if ((avg > maxalt - 2) && (recheck == 1))
          {
            recheck = false;
            Serial.println("false apogee");
            digitalWrite(11, LOW);
          }
        else
        if ((avg < maxalt - 2) && (recheck == 1))
          {
            Serial.println("real apogee");
            for (pos = 0; pos < 90; pos += 1)
              {
                Release.write(pos);
              }
            recheck = false;
            calibrated = false;
            apogee = true;
          }  

      }
    else
      {
        if (apogee == true)
          {
            if (maxalt > 0)
              {
                maxdisplay = (maxalt / 10);  //155 / 10 = 15.5

                for (int i = 0; i < maxdisplay; i++) 
                  {
                    digitalWrite(11, HIGH);
                    digitalWrite(12, HIGH);
                    digitalWrite(13, HIGH);

                    delay(500);

                    digitalWrite(11, LOW);
                    digitalWrite(12, LOW);
                    digitalWrite(13, LOW);

                    delay(500);
                  }
              }

            digitalWrite(11, HIGH);
            digitalWrite(13, HIGH);

            delay(1000);

            }
            
        if (digitalRead(2) == LOW) 
              {
                p0 = Preassure();
                Serial.print("initial pressure updated to :");
                Serial.println(p0);
                digitalWrite(11, LOW);
                digitalWrite(12, HIGH);
                digitalWrite(13, LOW);
                maxalt = 0;
                avg = 0;
                Release.write(0);
                a1 = 0;
                a2 = 0;
                a3 = 0;
                maxdisplay = 0;
                recheck = false; 
                apogee = false;
                calibrated = true;
              }
      }

  }



// Functions
double altitude()
  {
    char Status;
    double T, P;    

    Status = bmp180.startTemperature();

    if (Status != 0)
      {
        delay(85);
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
      }
    
    if (Status != 0)
      {
        return bmp180.altitude(P, p0); 
      }

  }

double Preassure()
  {
    char Status;
    double T, P;    

    Status = bmp180.startTemperature();

    if (Status != 0)
      {
        delay(85);
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
      }

    return P;
  }
