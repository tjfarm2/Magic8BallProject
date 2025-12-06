//Arduino references can be found at: https://www.arduino.cc/reference/en/

//Necessary library files for the OLED and Accelerometer. Do not edit or delete!

#include <Arduino.h>
#include <U8x8lib.h>
#include "LIS3DHTR.h"

#ifdef SOFTWAREWIRE
  #include <SoftwareWire.h>
  SoftwareWire myWire(3, 2);
  LIS3DHTR<SoftwareWire> LIS;  
  #define WIRE myWire
#else
  #include <Wire.h>
  LIS3DHTR<TwoWire> LIS;       
  #define WIRE Wire
#endif


struct MagicResult {
  const char *text;     // outcome 
  const char *mood;     // mood 
  int id;               // result 
};

// Same messages, but in a slightly different format/order in the struct
MagicResult results[10] = {
  { "For Sure!",   "Positive", 1 },
  { "Yes",         "Positive", 2 },
  { "Definitely!", "Positive", 3 },
  { "Maybe",       "Neutral",  4 },
  { "Not Sure..",  "Neutral",  5 },
  { "Ask Later",   "Neutral",  6 },
  { "I'm Tired",   "Neutral",  7 },
  { "No Way!",     "Negative", 8 },
  { "No",          "Negative", 9 },
  { "Nope",        "Negative", 10 }
};

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);  //do not delete

// Cooldown so one shake doesn't trigger a bunch of times
unsigned long lastShake = 0;
const unsigned long shakeDelay = 1000;  // 1 second

//Setup loop to ensure the OLED and other processes actually start
void setup() {

  LIS.begin(WIRE, 0x19);  //I2C init -- do not delete

  Serial.begin(9600);
  u8x8.begin();
  u8x8.setFlipMode(1);
  u8x8.clearDisplay();

 


  
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 1);
  u8x8.print("Magic 8 Ball");
  u8x8.setCursor(0, 3);
  u8x8.print("Ask your questions & Shake for a response!");
}

//Loop to randomize the outcomes and show them for 2 seconds
void loop() {

  u8x8.setFont(u8x8_font_chroma48medium8_r);  //do not edit or delete
  float z = LIS.getAccelerationZ();
  unsigned long now = millis();

  // Check if shaken and for the delay/cooldown
  if (z > 2.0 && (now - lastShake) > shakeDelay) {

    lastShake = now;  // update timer

    int index = random(0, 10);

    u8x8.clearDisplay();

    // result number
    u8x8.setCursor(0, 0);
    u8x8.print("Result #");
    u8x8.print(results[index].id);

    // outcome text
    u8x8.setCursor(0, 2);
    u8x8.print(results[index].text);

    // mood
    u8x8.setCursor(0, 4);
    u8x8.print("Mood: ");
    u8x8.print(results[index].mood);

    // Debug 
    Serial.print("Outcome ");
    Serial.print(results[index].id);
    Serial.print(": ");
    Serial.print(results[index].text);
    Serial.print(" (");
    Serial.print(results[index].mood);
    Serial.println(")");

    // Show result 
    delay(2000);

    // Show a prompt again
    u8x8.clearDisplay();
    u8x8.setCursor(0, 2);
    u8x8.print("Shake again!");
  }
  else {
    // Idle state — leave the last screen on instead of clearing every time
    // This avoids flicker and still behaves correctly
  }

  
}
