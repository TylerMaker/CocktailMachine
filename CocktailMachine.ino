// -- -----------------------------------------
// CocktailMachine
//
// v1.0 - Nicolas CHRISTOPHE - Initial version
// -- ------------------------------------------

// -- -----------
// Declaratives
// -- -----------
// Declaratives, buttons
int POT = 15; // Assign POT pin D15
int WATER = 21; // Assign WATER pin D2 
int RED = 19; // Assign RED pin D4
int R_PUMP = 22; // Assign R_PUMP pin D22
int W_PUMP = 23; // Assign W_PUMP pin D23

int TEMPO_POT_MAX = 4095; // max tempo for the pot
int TEMPO_WATER_MAX = 12000; // max tempo for the water
int TEMPO_RED_MIN = 1000; // max tempo for the red
int TEMPO_RED_MAX = 3000; // max tempo for the red

// Declaratives, others
boolean blocked = false; // block flag
int waterState = 0; // GO button state
int redState = 0; // BLUE button state
int potValue = 0;  // value read from the pot
int tempo = 0; // tempo when a pump is on

// Declaratives, graphics and font library
#include <TFT_eSPI.h>
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();  // Invoke library

// -- -----------
// Setup
// -- -----------
void setup() {
  Serial.begin(9600);
  
  tft.init(); // init TFT
  tft.setRotation(1); // init TFT
  initTFT(); // init TFT

  pinMode(WATER, INPUT); // déclarer la broche comme entrée
  pinMode(RED, INPUT); // déclarer la broche comme entrée
  pinMode(R_PUMP, OUTPUT); // déclarer la broche comme sortie
  pinMode(W_PUMP, OUTPUT); // déclarer la broche comme sortie

  digitalWrite(R_PUMP, HIGH); // initial value
  digitalWrite(W_PUMP, HIGH); // initial value

}

// -- -----------
// Loop
// -- -----------
void loop() {  
  waterState = digitalRead(WATER); // init GO
  redState = digitalRead(RED); // init RED
  potValue = analogRead(POT); // read POT value

  delay(1000); // tempo
  if ( redState == LOW and waterState == LOW ) { // no buttton pushed
    blocked = false;

  }

  // * -- -----
  // * 1. RED
  // * -- -----
  if ( redState == HIGH and not blocked) { // RED pressed
    tempo = map( potValue, 0, TEMPO_POT_MAX, TEMPO_RED_MIN, TEMPO_RED_MAX); // pump tempo
    tft.fillScreen(TFT_RED); // does not work

    // pump RED
    Serial.println("");
    Serial.println("Pump ON for RED");
    digitalWrite(R_PUMP, LOW); 
    delay( tempo ); 
    digitalWrite(R_PUMP, HIGH); 

    // pump WATER
    tempo = TEMPO_WATER_MAX - tempo;
    Serial.println("Pump ON for WATER"); 
    digitalWrite(W_PUMP, LOW); 
    delay( tempo ); // Pump WATER 
    digitalWrite(W_PUMP, HIGH); 
    blocked = true;

  } else {  // RED not pressed
  // * -- -----
  // * 2. WATER
  // * -- -----
    digitalWrite(R_PUMP, HIGH);
    tft.fillScreen(TFT_BLUE); // does not work
    if ( waterState == HIGH and not blocked ) { // WATER pressed
      // pump WATER
      tempo = TEMPO_WATER_MAX;
      Serial.println("");
      Serial.println("Pump ON for WATER"); 
      digitalWrite(W_PUMP, LOW); 
      delay( tempo ); // 
      digitalWrite(W_PUMP, HIGH); 
      blocked = true;

    } else {  // RED and WATER not pressed
  // * -- -----
  // * 3. WAIT
  // * -- -----
     Serial.print(".");

    }
  }
}

// -- -----------
// Init TFT
// -- -----------
void initTFT() {
  // background
  tft.fillScreen(TFT_BLACK);

  // test header
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Syrup Machine");
  tft.println("Welcome !!");
  tft.println("");
  tft.println("Make a choice");
  
  // text body
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.println(" BLUE button");
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.println("  RED button");

}

