#include <FastLED.h>
#include <LEDeffects.h>

// Constants
#define LED_PIN     2
#define BUTTON_PIN  6
#define NUM_LEDS    61
#define START_LED   2
#define END_LED     61
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100
CRGB leds[NUM_LEDS];

//Segment{start, end, width, delay, r, g, b}
//SegmentRGB bkg = {START_LED, END_LED, 0, 50};
//FastLED.LEDSetup(START_LED, END_LED);
Segment bkg = {START_LED, END_LED-START_LED};
Segment left_light = {START_LED, 10};
Segment right_light = {END_LED-10, 10};
Segment shuttleFive = {START_LED, 15};
Segment pixelTwo = {START_LED, 25};
Segment strobing = {START_LED, END_LED-START_LED};

// Dev flags
bool lights = true;
bool troubleshooting = false;
long master_clock = millis();

void setup() {
  delay(1500); // pwrLevel-up safety delay
  Serial.begin(9600);
  Serial.println("Program started!");
  
  //Configure LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(220);
  clearLEDs(); //Start from a fresh palette
  //pwrLevel indicator
  leds[1] = CRGB::Red;
  //bkg.SetRGB(100,10,100);
  bkg.SetHSV(180,255,200);
  bkg.GetHSV();
  shuttleFive.SetRGB(120,0,0);
  pixelTwo.SetRGB(20,100,10);
}

void loop() {
  if(troubleshooting) {
    Serial.print("Current Val:");
    Serial.println(bkg.cur_val);
    Serial.print("Target Val:");
    Serial.println(bkg.tgt_val);
  }
  if(lights) {
    Serial.println("Test");
    //// Motion Effects
    //shuttleFive.Bounce(2,50);
    pixelTwo.Scan(1);

    //// Color Effects
    bkg.DimTo(3, 20);
    
    //// Layers
    // Background
    hsvZone(bkg.loc0, bkg.loc1, bkg.cur_hue, bkg.cur_sat, bkg.cur_val);
    
    //// Filters
    // Dual pixel
    overlayZone(pixelTwo.loc0, pixelTwo.loc1, pixelTwo.cur_red, pixelTwo.cur_green, pixelTwo.cur_blue);

  } else {
    clearLEDs();
    delay(1000);
  }
  FastLED.show();
}

void rgbZone(uint16_t led_start, uint16_t led_end, uint8_t r, uint8_t g, uint8_t b) {
  //Light each pixel in zone
  for(uint16_t dot=led_start; dot<led_end; dot++) {
    setPixelRGB(dot, r, g, b);
  }
}

void hsvZone(uint16_t led_start, uint16_t led_end, uint8_t h, uint8_t s, uint8_t v) {
  //Light each pixel in zone
  for(uint16_t dot=led_start; dot<led_end; dot++) {
    leds[dot] = CHSV(h,s,v);
  }
}

void overlayZone(uint16_t led_start, uint16_t led_end, int r_offset, int g_offset, int b_offset) {
  //Light each pixel in zone
  uint8_t r, g, b;
  for(uint16_t dot=led_start; dot<led_end; dot++) {
    r=leds[dot][0];
    g=leds[dot][1];
    b=leds[dot][2];

    if(r+r_offset<0) {
      r = 0;
    } else if (r+r_offset>255) {
      r = 255;
    } else {
      r += r_offset;
    }
    
    if(g+g_offset<0) {
      g = 0;
    } else if (g+g_offset>255) {
      g = 255;
    } else {
      g += g_offset;
    }
    
    if(b+b_offset<0) {
      b = 0;
    } else if (b+b_offset>255) {
      b = 255;
    } else {
      b += b_offset;
    }
    
    setPixelRGB(dot, r, g, b);
  }
}

void setPixelRGB(uint16_t dot, uint8_t r, uint8_t g, uint8_t b) {
  leds[dot][0] = r;
  leds[dot][1] = g;
  leds[dot][2] = b;
}

void clearLEDs() {
  for(int dot = 0; dot < NUM_LEDS; dot++) {
    setPixelRGB(dot, 0, 0, 0); 
  }
}
