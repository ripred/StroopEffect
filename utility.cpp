/*
 * utility.cpp
 * 
 * utility file for StroopEffect.ino
 * 
 */
#include <FastLED.h>
#include "StroopEffect.h"

// 
// set up FastLED
// 
void setup_fastLED() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
}

// 
// set the color of a given LED
// 
void set_led(int const index, CRGB const clr) {
    leds[index] = clr;
    FastLED.show();
}

// 
// clear all LEDs
// 
void clear_leds() {
    for (CRGB &led : leds) {
        led = CRGB::Black;
    }
    FastLED.show();
}


// 
// get the color index for the specified color
// 
int get_clr_index(CRGB clr) {
    for (int i=0; i < NUM_LEDS; i++) {
        if (colors[i] ==clr) {
            return i;
        }
    }
    return -1;
}

// 
// Shuffle the LED colors randomly, leaving one
// 'which_led' as the LED to guess with the color 'which_clr'
// 
void shuffle() {
    for (int i=0; i < NUM_LEDS; i++) {
        set_led(i, colors[i]);
    }

    int not_finished = NUM_LEDS;
    bool changed[NUM_LEDS] = { 0,0,0,0,0 };

    int passes = 60;
    for (int i=0; i < passes && not_finished; i++) {
        match_word = random(0, 2);
        analogWrite(COLOR_PIN, (!match_word) ? 0 : 255);
        analogWrite( WORD_PIN, ( match_word) ? 0 : 10);

        which_clr = random(0, 5);
        which_led = random(0, 5);
        if (!changed[which_led]) {
            changed[which_led] = true;
            not_finished--;
        }
        int which_led2 = random(0, 5);
        while (which_led == which_led2) {
            which_led2 = random(0, 5);
        }

        CRGB const tmp = leds[which_led];
        set_led(which_led, leds[which_led2]);
        set_led(which_led2, tmp);

        which_clr = get_clr_index(leds[which_led]);
        if (which_clr == which_led) {
            if (changed[which_led]) {
                changed[which_led] = 0;
                not_finished++;
            }
        }

        delay(100);
    }

    for (int i=0; i < NUM_LEDS; i++) {
        if (i != which_led) {
            set_led(i, CRGB::Black);
        }
    }
}

// 
// The user lost. Flash all of the LEDs to indicate the failure
// 
// NOTE: This function also resets the "ON time" for guessing the LEDs back to the default value
// 
void fail() {
    int passes = 15;
    for (int i=0; i < passes; i++) {
        for (int led=0; led < NUM_LEDS; led++) {
            set_led(led, colors[led]);
        }

        analogWrite(COLOR_PIN, 255);
        analogWrite(WORD_PIN, 10);

        delay(100);
        for (int led=0; led < NUM_LEDS; led++) {
            set_led(led, CRGB::Black);
        }

        analogWrite(COLOR_PIN, 0);
        analogWrite(WORD_PIN, 0);

        delay(60);
    }
    ontime = DEF_ONTIME;
}


// 
// show a colorful wakeup sequence
// 
void wake_up() {
    clear_leds();
    for (int i=0; i < NUM_LEDS; i++) {
        set_led(i, colors[i]);
        delay(50);
    }
    delay(250);
    for (int i=0; i < NUM_LEDS; i++) {
        set_led(i, CRGB::Black);
        delay(175);
    }
}


// 
// start a new round of the game using the current guess time
// 
void start_round() {
    clear_leds();
    shuffle();
    start_time = millis();
}


void check(int index) {
    if (0 == start_time) {
        return;
    }

    int const answer = match_word ? which_clr : which_led;

    if (answer != index) {
        fail();
        fails_in_a_row++;
    }
    else {
        fails_in_a_row = 0;


//        ontime -= REDUCE;
//        if (ontime < MIN_ONTIME) {
//            ontime = MIN_ONTIME;
//        }
    }

    if (fails_in_a_row < max_fails) {
        start_round();
    }
    else {
        start_time = 0;
    }
}


void setup_DF2301Q() {
    while(!sr.begin()) {
        Serial.println(F("Communication with the DFRobot DF2301Q failed, check the connections"));
        Serial.println(F("The I2C address should be 0x64"));
        delay(3000);
    }
    Serial.println(F("Communication with the DFRobot DF2301Q successfully started"));

    sr.setWakeTime(255);
    //sr.setMuteMode(0);
    //sr.setVolume(7);

    Serial.print("Wake-time: ");
    Serial.println(sr.getWakeTime());
}
