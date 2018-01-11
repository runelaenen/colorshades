#define CONFIG_SSID "telenet-73638"
#define CONFIG_PASS "U24ArtJjn8T8"
#define CONFIG_MQTT_HOST "broker.shiftr.io"
#define CONFIG_MQTT_USER "3cab5139"
#define CONFIG_MQTT_PASS "2cbe270f7dbfd3f9"
#define CONFIG_MQTT_CLIENTID "ColorShades"
#define CONFIG_SOFTAP_SSID "ColorShades"
#define CONFIG_DEBUG true

#define LED_BLINKER 0

#define PIN_LEDS 2
#define PIN_POT A0

#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 

WiFiClient net;
MQTTClient client;

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#include "Global.h"
#include "Font.h"
#include "Matrix.h"
#include "Animations.h"
#include "Services.h"
#include "ROM.h"


void configModeCallback (WiFiManager *myWiFiManager) {
  #if CONFIG_DEBUG 
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  #endif
  
  leds[LED_BLINKER] = CRGB::Orange;
  FastLED.show();
}

void setup() {
  FastLED.addLeds<WS2812B, PIN_LEDS, GRB>(leds, NUMBER_OF_LEDS);
  #if CONFIG_DEBUG 
  Serial.println("LEDs added...");
  #endif
  FastLED.setBrightness(50);
  FastLED.setDither( 0 );

  
  #ifdef CONFIG_DEBUG 
  Serial.begin(115200); 
  #endif

  EEPROM.begin(512);

  WiFiManager wifiManager;

  //wifiManager.resetSettings();
  #if !CONFIG_DEBUG 
  wifiManager.setDebugOutput(false); 
  #endif

  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect(CONFIG_SOFTAP_SSID)) {
    #if CONFIG_DEBUG 
    Serial.println("failed to connect and hit timeout"); 
    #endif
    leds[LED_BLINKER] = CRGB::Red;
    FastLED.show();
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  } 

   leds[LED_BLINKER] = CRGB::Green;
   FastLED.show();
   FastLED.delay(100);

  #if CONFIG_DEBUG 
  Serial.print("Setup MQTT Client..."); 
  #endif
  client.begin(CONFIG_MQTT_HOST, net);
  client.onMessage(messageReceived);

  loadVarsFromROM();

  connect();
  #if CONFIG_DEBUG 
  Serial.println("MQTT Client connected...");
  #endif
}

void loop() {
  // Check new MQTT messages
  client.loop();

  // Adds stability to wifi (voodoo & black magic, can't find any documentation on this but it works)
  delay(10); 
  
  // Fix lost connections
  if (!client.connected()) {
    connect();
  }

  // Initialise timers
  static uint32_t currentMS = 0;
  static uint32_t effectMS = 0;
  static uint32_t wifiMS = 0;
  static uint32_t potMS = 0;
  static uint32_t romMS = 0;

  // Update timer
  currentMS  = millis();

  // Check effect timer, and run animation-function if needed
  if(currentMS - effectMS > effectSpeed){
    effectMS = currentMS;
    animations[effectCurrent]();
  }

  // Send leds-array to glasses
  FastLED.show(); 

  // Handle the PotMeter
  if(currentMS - potMS > 107){
    potMS = currentMS;
    handlePotmeter();
  }
  
  if(currentMS - romMS > 250) {
    romMS = currentMS;
    updateROM();
  }
  
}


void messageReceived(String& topic, String& message) {
  #if CONFIG_DEBUG
    Serial.print("MQTT topic: ");
    Serial.println(topic);
  #endif
  if(topic.equals("/effect/current")){
    changeCurrentEffect(message.toInt());
  }else if(topic.equals("/effect/hue")){
    effectHue = message.toInt();
  }else if(topic.equals("/effect/speed")){
    effectSpeed = message.toInt();
  }else if(topic.equals("/getInfo/effectCurrent")){
    sendCurrentEffect();
  } else {
    Serial.print("MQTT on unknown topic: ");
    Serial.println(topic);
  }
}
