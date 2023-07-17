/*
 * StroopEffect.h
 * 
 * header file for SroopEffect.ino project
 * 
 */
#ifndef  STROOPEFFECT_H
#define  STROOPEFFECT_H

#include <FastLED.h>
#include <DFRobot_DF2301Q.h>

#define COLOR_PIN   5
#define WORD_PIN    6

#define LED_PIN     3
#define NUM_LEDS    5
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

#define DEF_ONTIME  4000
#define MIN_ONTIME  1000
#define REDUCE      50

extern CRGB const colors[5];
extern DFRobot_DF2301Q_I2C sr;

extern CRGB leds[NUM_LEDS];
extern CRGB rnd_colors[5];

extern int which_led;
extern int which_clr;
extern volatile uint32_t ontime;
extern volatile uint32_t start_time;
extern int fails_in_a_row;
extern int max_fails;
extern bool match_word;

extern void setup_fastLED();
extern void setup_DF2301Q();
extern void set_led(int const index, CRGB const clr);
extern void clear_leds();
extern void make_rand(int index);
extern int make_one_rand();
extern void shuffle();
extern void fail();
extern void fade_up(int t=100);
extern void fade_down(int t=100);
extern void wake_up();
extern void start_round();
extern void check(int index);

#endif // STROOPEFFECT_H
