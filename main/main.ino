#include <FastLED.h> 

#define NUM_LEDS 36
#define DATA_PIN 11
#define CLOCK_PIN 13

int blend_rate = 1000;

CRGB leds[NUM_LEDS];
const uint8_t color_array[30][3] = {
  {247,	120,161},
  {255, 105,180},
  {255, 119,255},
  {244, 51, 255},
  {212,	98, 255},
  {176,	65,	255},
  {137, 59,	255},
  {108,	45,	199},
  {158,	123,255},
  {115,	106,255},
  {88,	101,242},
  {9,	  9,	255},
  {21,	137,255},
  {0,	  191,255},
  {67,	198,219},
  {0,	  206,209},
  {1,	  249,198},
  {0,	  250,154},
  {0,   255,127},
  {94,	251,110},
  {173,	255,47},
  {226,	245,22},
  {255,	215,0},
  {255,	206,68},
  {253,	189,1},
  {255,	140,0},
  {255,	103,0},
  {255,	99, 71},
  {250,	42, 85},
  {252,	108,133},
} ;

const int number_of_colors = sizeof(color_array)/sizeof(uint8_t)/3;
CRGB color_sequence[number_of_colors];
CRGB start_color;  // starting color
CRGB target_color ;  // target color
CRGB current_color;
int current_color_index = 0;

// Helper function that blends one uint8_t toward another by a given amount
void nblendU8TowardU8( uint8_t& cur, const uint8_t target, int N){
  if( cur == target) return;

  int timeToReachTarget = 10000;  // 10 seconds in milliseconds
  int amount = 255 * N / timeToReachTarget;
  
  if( cur < target ) {
    uint8_t delta = target - cur;
    delta = scale8_video( delta, amount);
    cur += delta;
  } else {
    uint8_t delta = cur - target;
    delta = scale8_video( delta, amount);
    cur -= delta;
  }
}

void generate_sequence(){
  for (int i = 0; i < number_of_colors; i++){
      color_sequence[i] = CRGB(color_array[i][0],color_array[i][1],color_array[i][2]);
      }
}

// Blend one CRGB color toward another CRGB color by a given amount.
// Blending is linear, and done in the RGB color space.
// This function modifies 'cur' in place.
CRGB fadeTowardColor( CRGB& cur, const CRGB& target, uint8_t amount){
  nblendU8TowardU8( cur.red,   target.red,   amount);
  nblendU8TowardU8( cur.green, target.green, amount);
  nblendU8TowardU8( cur.blue,  target.blue,  amount);
  return cur;
}

void setup() {
    Serial.begin(115200);
    LEDS.addLeds<APA102,DATA_PIN, CLOCK_PIN,BGR>(leds,NUM_LEDS);
    generate_sequence();
    start_color = color_sequence[0];  // starting color
    target_color = color_sequence[1];  // target color
    current_color = start_color;
    // for(int i = 0; i < number_of_colors; i++)
    // {
    //   Serial.print(color_sequence[i].r); Serial.print(" "); Serial.print(color_sequence[i].g); Serial.print(" "); Serial.print(color_sequence[i].b); Serial.print("\n");
    // }
    fill_solid(leds, NUM_LEDS, current_color);
    FastLED.show();
    Serial.print(current_color.r); Serial.print(" "); Serial.print(current_color.g); Serial.print(" "); Serial.print(current_color.b); Serial.print("\n");
    Serial.print("Size of color_sequence: "); Serial.print(number_of_colors); Serial.print("\n");
    Serial.println("Setup done. \n");
    
    // delay(10000);
}

unsigned long start = millis();
void loop(){
    EVERY_N_MILLISECONDS(blend_rate){
      if (current_color == target_color){
        unsigned long end = millis();
        unsigned long delta = end - start;
        Serial.println(delta/1000.0);
        start_color = current_color;
        target_color = color_sequence[current_color_index];
        current_color_index += 1;
        start = millis();


        if (current_color_index == number_of_colors) {
          current_color_index = 0;
        }

        // delay(10000);
      }
      current_color = fadeTowardColor(current_color, target_color, blend_rate);
      fill_solid(leds, NUM_LEDS, current_color);
      leds[0] = color_sequence[current_color_index];
    }

    // Serial.print(current_color.r); Serial.print(" "); Serial.print(current_color.g); Serial.print(" "); Serial.print(current_color.b); Serial.print("\n");
    FastLED.show();
}
