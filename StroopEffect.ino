/*
 * StroopEffect.ino
 * 
 * see https://en.wikipedia.org/wiki/Stroop_effect
 * 
 * A game based on the Stroop Effect and DFRobot's DF2301Q voice recognition module.
 * 
 * This is a game that takes advantage of our brains confusion
 * when reading the names of different colors when those names
 * are dispayed in a color different than the spelled word describes.
 * 
 * Using the DFRobot.com DF2301Q (SEN0539) Wake Phrase and Sound recognition
 * module we will train it to recognize the various spoken colors. Then we
 * will display the name of a color on a panel but in a different color
 * from the named color and randomly ask the user to say either the color
 * or the word spelled.
 * 
 * The user must speak either the color or the word before it times out.
 * Failure to say the correct color ends the game.
 * 
 * The game can optionally speed up as you continue to advance.
 * 
 * version 1.0 July 2023 ++trent m. wyatt
 * 
 */

#include <Wire.h>
#include <FastLED.h>
#include <DFRobot_DF2301Q.h>
#include "StroopEffect.h"

CRGB const colors[5] = { CRGB::Blue, CRGB::Red, CRGB::Yellow, CRGB::Purple, CRGB::Green };
DFRobot_DF2301Q_I2C sr;

CRGB leds[NUM_LEDS];
CRGB rnd_colors[5] = { CRGB::Blue, CRGB::Red, CRGB::Yellow, CRGB::Purple, CRGB::Green };

int which_led = 0;
int which_clr = 0;
volatile uint32_t ontime = DEF_ONTIME;
volatile uint32_t start_time = 0;
int fails_in_a_row = 0;
int max_fails = 1;
bool match_word = false;


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(COLOR_PIN, OUTPUT);
    pinMode(WORD_PIN, OUTPUT);

    delay( 3000 ); // power-up safety delay
    Serial.begin(115200);
    while (!Serial) {}
    
    setup_fastLED();
    clear_leds();

    setup_DF2301Q();

    // make random numbers different each time we run
    pinMode(A3, INPUT);
    randomSeed(analogRead(A3) + analogRead(A3) + analogRead(A3));

    ontime = DEF_ONTIME;
    fails_in_a_row = 0;

    digitalWrite(LED_BUILTIN, HIGH);
}


void loop() {
    // We only leave the LED to be guessed ON for a shorter and shorter
    // amount of time as the game goes on:
//    if ((start_time != 0) && (millis() - start_time) >= ontime / 2) {
//        set_led(which_led, CRGB::Black);
//    }

    // See if this guess has timed out:

    
    
    
    if ((start_time != 0) && (millis() - start_time) >= ontime) {
        fail();
        fails_in_a_row++;
        if (fails_in_a_row < max_fails) {
            start_round();
        }
        else {
            start_time = 0;
        }
    }

    uint8_t const cmd = sr.getCMDID();

    if (0 == cmd) return;

    Serial.print(F("Command = "));
    Serial.println(cmd, DEC);
    
    switch (cmd) {
        case 1: // Wake up phrase 1 "Hey Arduino"
            wake_up();
            start_round();
            break;

        case 2: // Wake up phrase 2 "hello robot"
            wake_up();
            start_round();
            break;
        
        case 5: // Command phrase "BLUE"
            check(0);
            break;
        
        case 6: // Command phrase "RED"
            check(1);
            break;
        
        case 7: // Command phrase "YELLOW"
            check(2);
            break;
        
        case 8: // Command phrase "PURPLE"
            check(3);
            break;
        
        case 9: // Command phrase "GREEN"
            check(4);
            break;
        
        default:
            break;
    }
}
