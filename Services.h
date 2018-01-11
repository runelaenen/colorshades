void sendCurrentEffect() {
  client.publish("/info/effectCurrent", String(effectCurrent));
}

void changeCurrentEffect(int to) {
  if(to >= numberOfAnimations || to < 0){
    effectCurrent = 0;
  } else {
    effectCurrent = to;
  }
  effectLoaded = false;
  needToUpdateROM = true;
  sendCurrentEffect();
}

void handlePotmeter() {
  uint8_t potVal = analogRead(PIN_POT)/16;
  uint8_t diff = abs(curPotVal - potVal);
  if(diff >= 2) { // Only update when the difference is big enough. MQTT calls are expensive!
    curPotVal = potVal;
    #if CONFIG_DEBUG 
    Serial.println(potVal);
    #endif
    client.publish("/potmeter", String(potVal));
    FastLED.setBrightness(potVal);
  
  }
}

/* delay(1000) but with a blinking LED on the matrix */
void delaySecond(CRGB color) {
  for(int i = 0; i<5; i++){
    if(leds[LED_BLINKER] == CRGB(0,0,0)) {
      leds[LED_BLINKER] = color;
    } else {
      leds[LED_BLINKER] = CRGB(0,0,0);
    }
    FastLED.show();
    FastLED.delay(200);
  }
}

/* Handles connection to Wifi & subscribing to MQTT */
void connect() {
  clean();
  
  #if CONFIG_DEBUG 
  Serial.print("\nconnecting...");
  #endif
  while (!client.connect(CONFIG_MQTT_CLIENTID, CONFIG_MQTT_USER, CONFIG_MQTT_PASS)) {
    #if CONFIG_DEBUG 
    Serial.print(".");
    #endif
    delaySecond(CRGB::Red);
  }

  #if CONFIG_DEBUG 
  Serial.println("\nconnected!");
  #endif

  // Docs: https://docs.shiftr.io/guides/topics/
  client.subscribe("/effect/#");
  client.subscribe("/getInfo/#");
  #if CONFIG_DEBUG 
  Serial.println("\nsubscribed!");
  #endif
}
