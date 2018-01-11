uint8_t XY2I(uint8_t x, uint8_t y){
  // Omdat er verschillende leds in de matrix niet bestaan (zoals waar de neus zit)
  // kan deze functie een x-y-waarde omzetten naar een geldige led-index
  // Hiervoor is gebruik gemaakt van een 'rainbow tabel' om de omzetting te maken

  uint8_t rainbow[] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 
   31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
   32, 33, 34, 35, 36, 37, 38, 70, 70, 51, 52, 53, 54, 55, 56, 57,
   45, 44, 43, 42, 41, 40, 39, 70, 70, 64, 63, 62, 61, 60, 59, 58,
   70, 46, 47, 48, 49, 50, 70, 70, 70, 70, 65, 66, 67, 68, 69, 70
  };

  uint8_t index = x + (y * MATRIX_WIDTH);

  if(index>80){
    return 70;
  }
  
  return rainbow[ index ];
}
