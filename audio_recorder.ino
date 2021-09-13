
/*
This sketch demonstrates recording of standard WAV files that can be played on any device that supports WAVs. The recording
uses a single ended input from any of the analog input pins. Uses AVCC (5V) reference currently.

Requirements:
Class 4 or 6 SD Card
Audio Input Device (Microphone, etc)
Arduino Uno,Nano, Mega, etc.

Steps:
1. Edit pcmConfig.h
    a: On Uno or non-mega boards, #define buffSize 128. May need to increase.
    b: Uncomment #define ENABLE_RECORDING and #define BLOCK_COUNT 10000UL

2. Usage is as below. See https://github.com/TMRh20/TMRpcm/wiki/Advanced-Features#wiki-recording-audio for
   additional informaiton.

Notes: Recording will not work in Multi Mode.
Performance is very dependant on SD write speed, and memory used.
Better performance may be seen using the SdFat library. See included example for usage.
Running the Arduino from a battery or filtered power supply will reduce noise.
*/

#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

//#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#define SD_ChipSelectPin 8  //using digital pin 4 on arduino nano 328, can use other pins

TMRpcm audio;   // create an object for use in this sketch 

int count = 0;
int ss1 = 1;
int ss2 = 1;
int ss3 = 1;
bool started = false;
bool stopped = false;
bool playy = false;
bool paused = false;

int yellow = A3;
int red = A4;

void setup() {
  
  //audio.quality(1);

  audio.speakerPin = 10;  //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  pinMode(12,OUTPUT);  //Pin pairs: 9,10 Mega: 5-2,6-7,11-12,46-45
  pinMode(13, OUTPUT);
  pinMode(red,OUTPUT);  //Pin pairs: 9,10 Mega: 5-2,6-7,11-12,46-45
  pinMode(yellow, OUTPUT);
  
  Serial.begin(9600);
  
  if (!SD.begin(SD_ChipSelectPin)) {  
    Serial.println("SD Not Found"); 
    digitalWrite(red, HIGH);
    delay(2000);
    digitalWrite(red, LOW);
    return;
  }else{
    Serial.println("SD OK"); 
    digitalWrite(yellow, HIGH);
    delay(2000);
    digitalWrite(yellow, LOW);
  }
  // The audio library needs to know which CS pin to use for recording
  audio.CSPin = SD_ChipSelectPin;
  audio.setVolume(5);

}
unsigned long pv1 = 0;
unsigned long pv2 = 0;
unsigned long pv3 = 0;


void loop() {
  
  if(digitalRead(3) == 1){
    if(millis()-pv1 > 200){
      pv1 = millis();
      ss1++;
      started = true;
      }
    }
    else if (digitalRead(4) == 1){
    if(millis()-pv2 > 200){
      pv2 = millis();
      ss2++;
      playy = true;
      }
    }

    if(ss1%2 == 0 && started){
      Serial.println("Recording on Progress"); 
      started = false; 
      stopped = true;
      digitalWrite(red, HIGH);
      audio.startRecording("test.wav",16000,A0);
      
      }
    else if(ss1%2 != 0 && stopped){
      Serial.println("Recording Stopped"); 
      digitalWrite(red, 0);
      audio.stopRecording("test.wav");
      started = true; 
      stopped = false;
      
      }
    else if(ss2%2 == 0 && playy){
      Serial.println("playy file"); 
      digitalWrite(yellow, HIGH);
      audio.play("test.wav");
      paused = true; 
      playy = false;
      
      }
    else if(ss2%2 != 0 && paused){
      Serial.println("Paused File"); 
      digitalWrite(yellow, 0);
      audio.pause();
      playy = true; 
      paused = false;
      
      }
 
}
