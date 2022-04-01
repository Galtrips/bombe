#include <Adafruit_SH110X.h>
#include <splash.h>

#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include "FeatherShieldPinouts.h"
#define B 32
#define C 14

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
hw_timer_t*timer=NULL;
hw_timer_t*timer2=NULL;

bool volatile Ap = false;
bool volatile led2 = false;

int s = 30;
int m = 1;

void IRAM_ATTR Tim1Int(){
  if (Ap == true) {
    s--;
    if (m == 0 && s == 0) {
      Ap = false;  
      digitalWrite(A0,HIGH);
      led2 = false;
    }
    else if (m <= 0) {
      led2 = true;
      if (led2 == true) {
        if (digitalRead(D4) == HIGH) {
          digitalWrite(D4,LOW);
        }
        else {
          digitalWrite(D4,HIGH);
        }
      }
    }
    if (m != 0 && s == 0) {
      s = 59;
      m--;  
    }
  }
}

void IRAM_ATTR But1Int(){
  if (Ap == false){
    Ap = true;
  }
  else {
    Ap = false;
  }
}

void setup() {
 display.begin(0x3C, true);
 display.clearDisplay();
 Serial.begin(115200);
 pinMode(B, INPUT);
 pinMode(A0, OUTPUT);
 pinMode(D4, OUTPUT);


 timer=timerBegin(0,10,true);
 timerAttachInterrupt(timer,&Tim1Int,true);
 timerAlarmWrite(timer,1000000,true);
 timerAlarmEnable(timer);
 
 attachInterrupt(B, But1Int, RISING);
 Ap = false;
}

void loop() {

  display.clearDisplay();
  
  display.setTextSize(4);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(5, 15);
  display.setRotation(3);

  if (s < 10) {
    if (m < 10) {
      display.print("0");  
      display.print(m);
      display.print(":0");
      display.print(s);
    }
    else {
      display.print(m);
      display.print(":0");
      display.print(s);
    }
  }
  else if (m < 10) {
    display.print("0");  
    display.print(m);
    display.print(":");
    display.print(s);
  }
  else {
    display.print(m);
    display.print(":");
    display.print(s);
  }
  display.display();
}
