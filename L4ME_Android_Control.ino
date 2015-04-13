#include <FastLED.h>
#include <SoftwareSerial.h>// import the serial library

#define DATA_PIN  11
#define CLOCK_PIN  13
#define COLOR_ORDER RGB
#define CHIPSET     LPD8806

#define BRIGHTNESS 128

SoftwareSerial Genotronex(0, 1); // RX, TX
//int ledpin=13; // led on D13 will show blink on / off
int BluetoothData; // the data given from Computer

const uint8_t kMatrixWidth  = 8;
const uint8_t kMatrixHeight = 4;
const bool    kMatrixSerpentineLayout = true;

CHSV look4me_Amber(77, 255, 255);
CHSV look4me_Amber_dim(77, 255, 25);
CHSV look4me_Red(125, 255, 255);
CHSV look4me_Red_dim(125, 255, 25);
CHSV look4me_White(255, 0, 255);

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  
    // put your setup code here, to run once:
  Genotronex.begin(9600);
  Genotronex.println("Controls are as follows:");
  Genotronex.println("X - Brake (2)");
  Genotronex.println("Square/Circle - Left/Right Turn (1,0)");
  Genotronex.println("Triangle - Flag (3)\n");
  Genotronex.println("Noble Prize or Bust!!!");
    //pinMode(ledpin,OUTPUT);
  turn_off_all_LEDs();
  running_lights();
}

void loop(){

  
  
   // put your main code here, to run repeatedly:
   if (Genotronex.available()){
    BluetoothData=Genotronex.read();
    Genotronex.println(BluetoothData);
    
    
    if (BluetoothData=='0'){// if number 0 pressed ....
     //digitalWrite(ledpin,0);
     Genotronex.println("read a zero - RIGHT TURN!! ");
     right_turn_signal();
      //turn_off_all_LEDs();
      running_lights();
    }

  if(BluetoothData=='1'){   // if number 1 pressed ....
   //digitalWrite(ledpin,1);
   Genotronex.println("read a 1 - LEFT TURN!!! ");
     left_turn_signal();
     //turn_off_all_LEDs();
     running_lights();
   }

  if (BluetoothData=='2'){// if number 2 pressed ....
   //digitalWrite(ledpin,0);
    Genotronex.println("read a 2 - BRAKE LIGHTS!!!");
    brake_lights();
    delay(2000);
    //turn_off_all_LEDs();
    running_lights();
  }

  if (BluetoothData=='3'){// if number 3 pressed ....
   //digitalWrite(ledpin,0);
    Genotronex.println("read a 3 - American Flag!!!");
    american_flag();
    delay(3000);
    //turn_off_all_LEDs();
  }
  
  BluetoothData=Genotronex.read(); // added this cause Ardudroid is sending an extra 1 after
}
delay(100);// prepare for next data ...

/*
  american_flag();
    delay(3000);
    turn_off_all_LEDs();
  
  left_turn_signal();
    turn_off_all_LEDs();
    
  right_turn_signal();
    turn_off_all_LEDs();
    
  brake_lights();
    delay(2000);
    turn_off_all_LEDs();
 */   
  /*
  uint8_t row = 2;
  uint8_t column = 2;
  
  light_single_led(row, column, look4me_Red);
    delay(1000);
    turn_off_all_LEDs();
    
  light_a_column(column, look4me_Red);
    delay(1000);
    turn_off_all_LEDs();
    
  light_a_row(row, look4me_Red);
    delay(1000);
    turn_off_all_LEDs();
    
  light_all_LEDs(look4me_Red);
    delay(1000);
    turn_off_all_LEDs();
    
  */
    
  //delay(1000);
}


void right_turn_signal(){
  for(uint8_t loops = 1; loops <= 10; loops++){
    for(uint8_t column = 0; column <= kMatrixWidth; column++) {
      light_a_column(column, look4me_Amber);
      FastLED.show();
      delay(30);
      light_a_column(column, look4me_Amber_dim);
      
    }
  }
}



void left_turn_signal(){
  for(uint8_t loops = 1; loops <= 10; loops++){
    for(uint8_t column = kMatrixWidth; column > 0; column--) {
      light_a_column(column, look4me_Amber);
      FastLED.show();
      delay(30);
      light_a_column(column, look4me_Amber_dim);
      FastLED.show();
    }
  }
}

void brake_lights(){
  
  // flash 3 times
  for(uint8_t loops = 1; loops <= 3; loops++){
    //for(uint8_t column = kMatrixWidth; column > 0; column--) {
      light_all_LEDs(look4me_Red);
      FastLED.show();
      delay(50);
      light_all_LEDs(look4me_Red_dim);
      FastLED.show();
   // }
  }
  
  // stay on
  for(uint8_t column = kMatrixWidth; column > 0; column--) {
  //  light_a_column(column, look4me_Red);     
   light_all_LEDs(look4me_Red); 
  }
  FastLED.show();
  
} //end of brake light function


void running_lights(){
  
  // stay on
  for(uint8_t column = kMatrixWidth; column > 0; column--) {
  //  light_a_column(column, look4me_Red);     
   light_all_LEDs(look4me_Red_dim); 
  }
  FastLED.show();
  
} //end of brake light function



void light_single_led(uint8_t row, uint8_t column, CHSV color){
  leds[ XY(column-1, row-1)]  = color;
  FastLED.show();
}

void light_a_column(uint8_t column, CHSV color){
  for( uint8_t y = 0; y < kMatrixHeight; y++) {     
      leds[ XY(column-1, y)]  = color;
      FastLED.show();
  }
}

void light_a_row(uint8_t row, CHSV color){
  for( uint8_t x = 0; x < kMatrixWidth; x++) {     
      leds[ XY(x, row-1)]  = color;
      FastLED.show();
  }
}

void light_all_LEDs(CHSV color){
  for( uint8_t x = 1; x <= kMatrixWidth; x++) { 
    light_a_column(x, color);  
  }
}

void turn_off_all_LEDs(){
  for(uint8_t x = 0; x <= kMatrixWidth-1; x++){
    for(uint8_t y = 0; y <= kMatrixHeight-1; y++){
      leds[ XY(x, y)]  = CRGB::Black;
      FastLED.show();
    }
  }
}






void american_flag(){
 // 3x3 blue with a center white dot
// the rest red and white
// maybe animate a dimming across it for waving effect
leds[ XY(0, 0)]  = CRGB::White;
leds[ XY(1, 0)]  = CRGB::White;
leds[ XY(2, 0)]  = CRGB::White;
leds[ XY(3, 0)]  = CRGB::White;
leds[ XY(4, 0)]  = CRGB::White;
leds[ XY(5, 0)]  = CRGB::White;
leds[ XY(6, 0)]  = CRGB::White;
leds[ XY(7, 0)]  = CRGB::White;
leds[ XY(0, 1)]  = CRGB::Blue;
leds[ XY(1, 1)]  = CRGB::Blue;
leds[ XY(2, 1)]  = CRGB::Blue;
leds[ XY(3, 1)]  = look4me_Red;
leds[ XY(4, 1)]  = look4me_Red;
leds[ XY(5, 1)]  = look4me_Red;
leds[ XY(6, 1)]  = look4me_Red;
leds[ XY(7, 1)]  = look4me_Red;
leds[ XY(0, 2)]  = CRGB::Blue;
leds[ XY(1, 2)]  = CRGB::White;
leds[ XY(2, 2)]  = CRGB::Blue;
leds[ XY(3, 2)]  = CRGB::White;
leds[ XY(4, 2)]  = CRGB::White;
leds[ XY(5, 2)]  = CRGB::White;
leds[ XY(6, 2)]  = CRGB::White;
leds[ XY(7, 2)]  = CRGB::White;
leds[ XY(0, 3)]  = CRGB::Blue;
leds[ XY(1, 3)]  = CRGB::Blue;
leds[ XY(2, 3)]  = CRGB::Blue;
leds[ XY(3, 3)]  = look4me_Red;
leds[ XY(4, 3)]  = look4me_Red;
leds[ XY(5, 3)]  = look4me_Red;
leds[ XY(6, 3)]  = look4me_Red;
leds[ XY(7, 3)]  = look4me_Red;
FastLED.show();
}



// Helper function that translates from x, y into an index into the LED array
// Handles both 'row order' and 'serpentine' pixel layouts.
uint16_t XY( uint8_t x, uint8_t y){
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  } else {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
  
  return i;
}
