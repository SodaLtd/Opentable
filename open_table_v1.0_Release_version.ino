/* OPENTABLE LED control v1.0. RELEASE VERSION
adding animation 'confetti' for rest state - random sparkling then fading LEDs
adding breathe for the 4 active place settings, like Apple power light fades up and dowm organically
 */
#include "FastLED.h"    //FastLED library

#define NUM_LEDS 20     // number of leds in the strip
#define LED_TYPE WS2801 // Type of LEDs used (bewarstupid wire colours (blue-clock, green-data, yellow ground, red +5v)
#define COLOR_ORDER RGB
#define DATA_PIN 3      // Data pin to send led data over SPI
#define CLOCK_PIN 2     // Clock pin SPI

boolean place1 = false;
boolean place2 = false;
boolean place3 = false;
boolean place4 = false;


CRGB leds[NUM_LEDS]; // The array of leds. One item for each led in the strip.

const int placeLEDs = 5; // 5 LEDs per LED strip
int incomingByte;      // a variable to read incoming serial data into
uint8_t gHue = 0; // rotating "base color" 

void setup() {
    
  delay(500); // sanity check delay - allows reprogramming if accidently blowing power w/leds
  Serial.begin(9600); // initialize serial communication

  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS); //set up LEDs and tell Arduino about them
  fill_solid( leds, 20, CRGB( 5, 0, 5) ); //pale purple
  FastLED.show(); // send the 'leds' array out to the actual LED strip
  FastLED.delay(1000);        // delay for stability and show startup is ok
  fill_solid( leds, 20, CRGB( 0, 5, 0) ); //pale green
  FastLED.show(); // send the 'leds' array out to the actual LED strip
  FastLED.delay(1000);        // delay for stability  and show startup is ok

}

void loop() {

    if (Serial.available() > 0) {
      
      int command = Serial.read();
      
        switch (command) {

        case 'i' :
          Serial.println("[id,led]");
          place1 = false;   // set all to false so that they are turned off after a restart of the PC app.
          place2 = false;
          place3 = false;
          place4 = false;
          break;    
        
        case 'a':         // place setting 1 LEDs 0-4
          place1 = true;
          break;
        case 'b':    
          place1 = false;
          break;
          
        case 'c':         // place setting 2 LEDs 5-9
          place2 = true;
          break;
        case 'd':    
          place2 = false;
          break;
          
        case 'e':         // place setting 3 LEDs 15-19
          place3 = true;
          break;
        case 'f':    
          place3 = false;
          break;
          
         case 'g':        // place setting 4 LEDs 10-14
          place4 = true;
          break;
        case 'h':    
          place4 = false;
          break;
    
      }
    }

    drawConfetti(); //draw confetti all along strip but let Breathe function below overwrite in places where active
    
    if (place1) { placeBreath(0, 0); }    // 1st top left = a|b 'place setting' red, LEDs 0-4  if active draw breathe over confetti
    if (place2) { placeBreath(5, 160); }  // 2nd bottom left = c|d 'place setting' blue, LEDs 5-9 if active draw breathe over confetti
    if (place3) { placeBreath(15, 96); }  // 3rd top right e|f 'place setting' green LEDs 14-19, if active draw breathe over confetti
    if (place4) { placeBreath(10, 64); }  // 4th bottom right g|h'place setting' yellow LEDs 10-14, if active draw breathe over confetti

    FastLED.show(); // send the 'leds' array out to the actual LED strip
    FastLED.delay(10); // delay in between reads for stability supposedly

    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}

void drawConfetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 8);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void placeBreath(int startPixel, int placeHue) {

      float breath = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
      fill_solid( &leds [startPixel], placeLEDs, CHSV( placeHue, 255, breath) ); 
}
