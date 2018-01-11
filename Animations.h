/* function to clear the screen */
void clean(){
  fill_solid(leds, NUMBER_OF_LEDS, CRGB::Black);
}

void hearts(){
  // Static variables only get initialised once, are only visible by the current function, and are remembered for the next run of the function
  static int fadeToggle = 0;
  static int brightness = 5;
  static int lux[] = {30,60,100,150,200,255};
  static int LEDs[22];

  // Set global variables to good values for this animation after a change of animation
  if (effectLoaded == false) {
    effectLoaded = true;
    effectSpeed = 50;

    // Only need to generate hearts when first called
    const byte shape[] = {
      B00101000, 
      B01111100, 
      B00111000, 
      B00010000, 
      B00000000
    };
    int counter = 0;
    for(int heart = 0; heart < 2; heart++) {
      for(int row = 0; row < 5; row++) {
        for(int col = 0; col < 8; col++) {
          if(bitRead(shape[row], col) == 1){
            LEDs[counter++] = XY2I((heart == 0 ? col-1 : col + 8), row);
          }
        }
      }
    }

    // Animation only needs 1 clean at the beginning. The used leds will always overwrite the other ones, so no need to clean on every frame
    clean();
  }
  
  if(fadeToggle == 0) {
    brightness++;
  } else {
    brightness--;
  }
  
  if(brightness >= 5){
    fadeToggle = 1;
  } else if(brightness <= 0) {
    fadeToggle = 0;
  }
  
  for(int i=0 ; i<22 ; i++){
    int led = LEDs[i];
    leds[led] = CHSV(0,255,lux[brightness]);
  }
}


/* FastLED Example rewritten for colorshades, original: https://pastebin.com/yAgKs0Ay */

void antialias(){

  if (effectLoaded == false) {
    effectLoaded = true;
    effectSpeed = 10;
  }
  
  static int     F16pos = 0; // position of the "fraction-based bar"
  static int     F16delta = 1; // how many 16ths of a pixel to move the Fractional Bar
  
  effectHue++;
  
  int Width  = 4;

  F16pos += F16delta;

  if( F16pos >= (NUMBER_OF_LEDS * 16)) {
    F16pos -= (NUMBER_OF_LEDS * 16);
  }

  memset8( leds, 0, NUMBER_OF_LEDS * sizeof(CRGB));
  int i = F16pos / 16;
  uint8_t frac = F16pos & 0x0F;
  uint8_t firstpixelbrightness = 255 - (frac * 16);
  uint8_t lastpixelbrightness  = 255 - firstpixelbrightness;
  uint8_t bright;
  for( int n = 0; n <= Width; n++) {
    if( n == 0) {
      bright = firstpixelbrightness;
    } else if( n == Width ) {
      bright = lastpixelbrightness;
    } else {
      bright = 255;
    }
    leds[i] += CHSV( effectHue, 255, bright);
    i++;
    if( i == NUMBER_OF_LEDS) i = 0;
  }
}


void rainbow() {
  if (effectLoaded == false) {
    effectLoaded = true;
    effectSpeed = 25;
    effectHue = 0;
  }
  effectHue++;

  for(int col = 0; col < MATRIX_WIDTH; col++) {
    int hue = effectHue + col*5;
    for(int row = 0; row <MATRIX_HEIGHT; row++) {
      leds[XY2I(col, row)] = CHSV(hue, 255, 155);
    }
  }
}

/* Noise animation based on FastLED Example */
void noise() {
  static uint16_t x;
  static uint16_t y;
  static uint16_t z;
  static uint8_t speed = 10;
  static uint16_t scale = 311;
  static uint8_t noise[MATRIX_WIDTH][MATRIX_WIDTH];

  if(effectLoaded == false) {
    effectLoaded = true;
    effectSpeed = 1;

    x = random16();
    y = random16();
    z = random16();
  }

  for(int i = 0; i < MATRIX_WIDTH; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < MATRIX_WIDTH; j++) {
      int joffset = scale * j;
      noise[i][j] = inoise8(x + ioffset,y + joffset,z);
    }
  }
  z += speed;

  for(int i = 0; i < MATRIX_WIDTH; i++) {
    for(int j = 0; j < MATRIX_HEIGHT; j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's hue.
      leds[XY2I(i,j)] = CHSV(noise[j][i],255,noise[i][j]);

      // You can also explore other ways to constrain the hue used, like below
      // leds[XY(i,j)] = CHSV(ihue + (noise[j][i]>>2),255,noise[i][j]);
    }
  }
}

/* Rain animation with random falling raindrops */
void rain() {
  if(effectLoaded == false) {
    effectLoaded = true;
    effectSpeed = 60;
    effectHue = 180; // 180 = blue
    clean();
  }

  // First of all, shift all existing pixels 1 pixel down. And set row 0 to Black
  for(int row = MATRIX_HEIGHT; row >= 0; row--){
    for(int col = 0; col < MATRIX_WIDTH; col++){
      if(row != 0) {
        leds[XY2I(col, row)] = leds[XY2I(col, row-1)];
      } else {
        if(leds[XY2I(col, row)] != CRGB(0,0,0)){
          leds[XY2I(col, row)].fadeToBlackBy(120);
        } else {
          leds[XY2I(col, row)] = CRGB::Black;
        }
      }
    }
  }
  
  byte randomRaindrop = random8(MATRIX_WIDTH);
  leds[XY2I(randomRaindrop, 0)] = CHSV(effectHue, 255, 255);
}

void matrix() {
  if(effectLoaded == false) {
    effectLoaded = true;
    effectSpeed = 60;
    effectHue = 96; // 180 = blue
    clean();
  }
  
  rain();
}


void textscroll() {
  static uint16_t pos;
  static int textLength;
  if(effectLoaded == false) {
    effectLoaded = true;
    effectSpeed = 200;
    effectHue = 96; // 180 = blue 
    pos = 0;
    
    textToSay = "Rune Laenen ";
    textLength = strlen(textToSay);
  }
  clean();

  if(pos >= textLength) pos = 0;

  byte* letter = charToFont(textToSay[pos]);

  for(int i = 0; i < 5; i++) {
    for(int j = 0; j < 5; j++) {
      if(bitRead(letter[i], j+3) == 1) {
        leds[XY2I(5-j, i)] = CRGB::Red;
      }
    }
  }
  
  pos ++;
}

/* Type to list animation-functions */
typedef void (*Animation)();

int numberOfAnimations = 7;

Animation animations[] = {
  hearts,
  antialias,
  rainbow,
  noise,
  rain,
  matrix,
  textscroll
};

