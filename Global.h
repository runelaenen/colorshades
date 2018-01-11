/* Matrix */
#define MATRIX_HEIGHT 5
#define MATRIX_WIDTH 16
#define NUMBER_OF_LEDS (MATRIX_HEIGHT * MATRIX_WIDTH)

CRGB leds[NUMBER_OF_LEDS];

/* ROM */
bool needToUpdateROM = false;

/* Services */
uint8_t curPotVal = 0; // Global variable with potentiometer value

/* Animations */
bool effectLoaded = false; // Keep track wether global vars have yet been set by animation
uint8_t effectCurrent = 2;
uint8_t effectSpeed = 250;
uint8_t effectHue = 0;
char* textToSay = "";
