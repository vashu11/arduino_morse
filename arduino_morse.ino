#include "MorseEncoder.h"
#include "MorseDecoder.h"


#define _ETAIMN_
#define _O_
#include "Vocabulary.h"

const int ENCODING_BUZZER_PIN = 10;//A1;
const int ENCODING_SPEED = 100;
const int DECODING_KEY_PIN = A3;
const int DECODING_SPEED = 100;


MorseEncoder *morseEncoder = new MorseEncoder(ENCODING_BUZZER_PIN, ENCODING_SPEED);
MorseEncoder *morseEncoderQuick = new MorseEncoder(ENCODING_BUZZER_PIN, ENCODING_SPEED / 3);
MorseDecoder *morseDecoder = new MorseDecoder(DECODING_KEY_PIN, DECODING_SPEED);

char *cur = 0;
void startTest() {
  randomSeed(millis());
  Serial.print("Start Test - ");
  cur = vocabulary[random(0, sizeof(vocabulary) / sizeof(vocabulary[0]))];
  Serial.println(cur);
  morseEncoder->playMorse(cur);
}
char msg[128];
void endTest(char *str) {
  if(morseEncoder->isPlayingMorse())
    return;
  if(String(cur) == "n") { // new word on 'n' letter
    startTest();
    return;
  }
  Serial.print("End Test - ");
  if(String(str) != String(cur)) {
    Serial.println(str);
    Serial.println(cur);
    morseEncoder->playMorse(cur);
  } else {
      morseEncoderQuick->playMorse("ok");
      Serial.println("OK");
  }
}

void setup() {
  Serial.begin(9600); 
  pinMode(ENCODING_BUZZER_PIN, OUTPUT);
  pinMode(DECODING_KEY_PIN, INPUT_PULLUP);

  morseDecoder->resetCallback = startTest;
  morseDecoder->decodedWordCallback = endTest;
 
  morseEncoderQuick->playMorse("ok");
}

void loop() {
  morseEncoder->loopPlayMorse();
  morseEncoderQuick->loopPlayMorse();
  
  morseDecoder->loopDecodeMorse();
  
  // key sound
  if(!morseEncoder->isPlayingMorse() && !morseEncoderQuick->isPlayingMorse()) {
    if(morseDecoder->testPin()) {
      MorseEncoder::buzzerOn(ENCODING_BUZZER_PIN);  
    } else {
      MorseEncoder::buzzerOff(ENCODING_BUZZER_PIN);
    } 
  } else {
    // to stop replay - simply press the button
    //TODO on unpress + jitter
    //morseEncoder->resetEncoder();
    //morseEncoderQuick->resetEncoder();
  }
}
