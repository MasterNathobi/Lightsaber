 #include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>
// WAV files converted to code by wav2sketch
#include "AudioSampleLightsaber_03.h" 
#include "AudioSamplePoweroff1.h" 
AudioPlayMemory    sound0;
AudioOutputAnalog  dac;     // play to both I2S audio board and on-chip DAC
AudioConnection    patchCord1(sound0, dac);
AudioControlSGTL5000 audioShield;
Bounce button0 = Bounce(8, 5);
#include <FastLED.h>
#define NUM_LEDS 236
#define LED_Enable 7
#define LED_Data 11
#define LED_Indicate 13 //indicator LED (on teesnsy) used for debugging
int bladeState = 0;
const int buttonPin = 8;
int buttonState = LOW;
CRGB leds[NUM_LEDS];
int shake = LOW;


void setup() {
  // put your setup code here, to run once:
  //LED code
  FastLED.addLeds<WS2812B,LED_Data, GRB>(leds, NUM_LEDS);
  pinMode(LED_Enable, OUTPUT);
  digitalWrite(LED_Enable, HIGH);//enable LED
  //LED indicator code
  pinMode(LED_Indicate, OUTPUT);
  pinMode(buttonPin, INPUT);           // and turn LED off
  //audio code
  pinMode(8, INPUT_PULLUP);
  AudioMemory(20);
  dac.analogReference(EXTERNAL); // much louder!
  delay(50);             // time for DAC voltage stable
  pinMode(5, OUTPUT);
  delay(10);             // allow time to wake up
  audioShield.enable();
  audioShield.volume(100);
  for(int n=0 ; (n < NUM_LEDS) ; n++){
    leds[n] = CRGB::Black;
  }
  FastLED.show();
//  pinMode(17,INPUT);
}

void loop() {
  digitalWrite(5,LOW);

//  shake = digitalRead(17);
//  
//  if(shake == HIGH and bladeState == 1){
//    digitalWrite(LED_Indicate, HIGH); 
//    hum();
//    digitalWrite(LED_Indicate, LOW);
//  }
  
   
  buttonState = digitalRead(buttonPin);
  //read state of button
  if (buttonState == HIGH and bladeState == 0) {
    // button pressed and blade off
    bladeState = 1;
    // change blade variable to 1
    digitalWrite(LED_Indicate, HIGH);
    //turn on indicater led used for debugging 
     bladeOn();
     //run blade on subroutine
     digitalWrite(LED_Indicate, LOW);
     //turn off indicater led used for debugging 
  }

  
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH and bladeState == 1) {
    bladeState = 0;
    digitalWrite(LED_Indicate, HIGH); 
      bladeOff();
      digitalWrite(LED_Indicate, LOW);  
  }
}

void bladeOn(){
  dac.analogReference(EXTERNAL); 
  // sets voltage for analog input to external voltage(5V)
  delay(50);             
  // time for DAC voltage stable
  pinMode(5, OUTPUT);
  // set amp to an output
  digitalWrite(5, HIGH); 
  // turn on the amplifier
  delay(10);             
  // allow time to wake up
  sound0.play(AudioSampleLightsaber_03);
  // play 'on' sound
  for(int n=0 ; (n < 118) ; n++){
    leds[n] = CRGB::DarkRed;
    leds[235-n] = CRGB::DarkRed;
    FastLED.show();
    //update leds
  }
  delay(200);
}


void bladeOff(){
  dac.analogReference(EXTERNAL); // much louder!
  delay(50);             // time for DAC voltage stable
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH); // turn on the amplifier
  delay(10);             // allow time to wake up
  sound0.play(AudioSamplePoweroff1);
  for(int n=0 ; (n < 118) ; n++){
    leds[117-n] = CRGB::Black;
    leds[118+n] = CRGB::Black;
    FastLED.show();
  }
  leds[0]= CRGB::Black;
  FastLED.show();
  delay(200);
}

void hum(){
  dac.analogReference(EXTERNAL);
    delay(50);            
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH); 
    delay(10);
    sound0.play(AudioSampleLightsaber_03);
    delay(2000);
  
}
