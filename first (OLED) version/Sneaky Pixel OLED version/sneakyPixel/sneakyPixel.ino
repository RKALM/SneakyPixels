
#include <SPI.h> //Library for the monitor. it has to do with the type of the OLED
#include <Wire.h> //Library for the monitor.
#include <Adafruit_GFX.h> //Library for the display on the monitor. Everything about Display
#include <Adafruit_SSD1306.h>//Library for the display on the monitor. Everything about Display

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Dangerous area. Configurations for our OLED display. WE DONT'T CHANGE ANYTHING HERE
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 //LED_BUILTIN //  4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int buttonPin = 2; //digital pin 2. For the attachinterrupt
int keyPin = 6; //For the key of the joystic. Should be connected and should be Digital
int lives = 0; //The lives of the player in the game
int allowOLED = 1; //if it is 1 the OLED is allowed to function
int theScreen = 1; //if it is 1 it means that the OLED works
int allowSerial = 1; //allow serials to be printed
int positionX = 0; //the x position of the vehicle (Horizontal)
int positionY = 5; //the y position of the vehicle (Vertical)
int moveX = 0; //The horizontal (X) reading of the joystic
int moveY = 0; //The vertical (Y) reading of the joystic
int road[15] = {5, 6, 7, 8, 9, 8, 7, 5, 4, 3, 2, 1, 2, 3, 4};
int stage[15]; //Array with the data for the displayed road every round
int stepInStage = 0; //it shows in which position of the stage the player is 
int valueInStep; //it shows what how is the road designed in that step
int upperColision = 0; //the upper limit of the road to the specific step
int underCollision = 0; //the under limit of the road to the specific step
int delayer = 10; //it will go until 10. the higher the delayer, the slower the car is.
int ticker = 0; //it gives the tempo

// variables will change:
int buttonState = 0; // int variable for button status

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT); //The button that causes the interrupt
  pinMode(13, OUTPUT); //just for testing
  pinMode(A0, INPUT); //Joystick X
  pinMode(A1, INPUT); //Joystick y
  pinMode(keyPin, INPUT); //Joystick key
  pinMode(A3, INPUT); //potentiometer. We don't use it
  pinMode(A4, OUTPUT); //OLED SDA. always on A4
  pinMode(A5, OUTPUT); //OLED SCL. Always on A5
  attachInterrupt(digitalPinToInterrupt(buttonPin), insertCoin, FALLING); //The attachInterrupt
  Serial.begin(9600); //for the serial monitor.

  Serial.println("Starting setup"); //Caveman debugging
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed")); //Caveman debugging
    theScreen = 0;
    //for(;;); // Don't proceed, loop forever
  }
  //Serial.println("allocation passed");
  if(allowOLED == 0){
    theScreen = 0;
    Serial.println("OLED is turned off by the admin"); //Caveman debugging
  }
  

//====================== TESTING THE SCREEN STARTS =====================
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //Serial.println("initialization");
  if(theScreen == 1){
    // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //Serial.println("initialization");
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  //Serial.println("Clear the buffer"); //Caveman debugging
  display.clearDisplay();

  // Draw a single pixel in white
  //Serial.println("Draw a single pixel in white");
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  //Serial.println("drawing commands to make them visible on screen!");
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  //Serial.println("Draw characters of the default font");
  testdrawchar();      // Draw characters of the default font


    // Invert and restore display, pausing in-between
  //Serial.println("Invert: True");
  display.invertDisplay(true);
  delay(1000);
  Serial.println("Invert false"); //Caveman debugging
  display.invertDisplay(false);
  delay(1000);
  //Serial.println("Test logo");
  //testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
  }
  //================ TESTING THE SCREEN ENDS==============================
Serial.println("Finishing the setup()");
}

//=============== Start of loop() =========================================
void loop() {
 if(lives > 0){ //checks if the game is on
  
  //generates the stage for the round
  int stepForLoop = stepInStage;
  int count = 0;
for(int x=0; x<15; x++ ){
  if((stepForLoop + count) < 15){
    stage[x] = road[stepForLoop + count];
  } else {
    stage[x] = road[0];
    count = 0;
  }
  count++;
}


//defines the free space of the road to our step to move without collision
valueInStep = stage[0];
//Serial.print("Value in step:");
//Serial.println(valueInStep);
upperColision = valueInStep + 2;
//Serial.print("upperColision:");
//Serial.println(upperColision);
underCollision = valueInStep - 2;
//Serial.print("underCollision:");
//Serial.println(underCollision);

//checks for collision with the limits of the road. if it is collide, then the player loses
if((positionY < underCollision)|| (positionY > upperColision)){
  lives = lives - 1;
  Serial.println("Boom! You lost a life!");
}

  
 //Serial.println("Game is on!");
 Serial.print("Lives:");
 Serial.println(lives);
 //Serial.print("Horizontal position (X):");
 //Serial.println(positionX);
 //Serial.print("Vertical position (Y):");
 //Serial.println(positionY);
 moveX = analogRead(A0);
 //Serial.print("X:");
 //Serial.println(moveX);
 moveY = analogRead(A1);
 //Serial.print("Y:");
 //Serial.println(moveY);
 //Serial.print("Key:");
 //Serial.println(digitalRead(6));
 delay(250); //speed means difficulty. For now. We should leave it as it is
 digitalWrite(13, HIGH);  // turn the test led (13) on
 //delay(1000); //speed means difficulty. For now.

displayAdvanceStageSerial(); //displays the displayed road on the serial monitor
displayAdvanceStageOLED();  //displays the displayed road on the OLED screen

//updates the positon of the "pixel" horizontally 
if(moveX < 250){
  //Serial.println("the pixel moves backwards");
  if(delayer < 10){
  delayer++;
  }
}
if (moveX > 750){
  //Serial.println("the pixel moves forwards");
  if(delayer>0){
    delayer--;
  }
}

//updates the positon of the "pixel" vertically 
//Serial.println("the pixel moves downwards");
if(moveY > 750){
  if(positionY > 0){
    positionY = positionY - 1;
  }
}
if (moveY < 250){
  //Serial.println("the pixel moves upwards");
  if(positionY < 10){
    positionY = positionY +1;
  } 
}

//updates the frame and gives the tempo
if(ticker < delayer){
  ticker++;
} else {
  if(stepInStage < 14){
  stepInStage++;
 } else {
  stepInStage = 0;
 }
 ticker = 0;
}

 } else {
  Serial.println("Game Out! Insert Coin!");
  displayTheText("Game Out! Insert Coin!");
  //display.drawPixel(10, 10, SSD1306_WHITE);
  //delay(100);
 }

 
}

// ========================= end of loop() ---------------------------------------

//when on interrupt, one life is added to the game. Like inserting a coin
void insertCoin(){
  lives = lives + 1;
  Serial.print("A life is added. Lives: ");
 Serial.println(lives);
}

//Displays text messages to the OLED Screen
void displayTheText(String text){
  if(true){
    display.clearDisplay(); // Clear display buffer
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setCursor(0, 0);     // Start at top-left corner
    display.print(text);
    //display.drawPixel(10, 10, SSD1306_WHITE);
    display.display();
    delay(1000);
  }
  
}

//Displays the actual game to the OLED Screen
void displayAdvanceStageOLED(){
  if(allowOLED == 1){
    display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
     //Generates and advance stage
for (int x = 10; x>0; x--){
  if(x == positionY){
    display.print("]]D|| |");
  } else {
    display.print("      |");
  }
  for (int k = 1; k<15; k++){
    if((stage[k] <= x + 2) && (stage[k] >= x-2)){
      display.print("[ ]");
    } else {
      display.print("[X]");
    }
  }
  display.println(">>>");
}
display.display();
  }
}

//For testing purposes, and it creates a nice intro
void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

//displays the actual game in the serial monitor
void displayAdvanceStageSerial(){
  if(allowSerial == 1){
     //Generates and advance stage
for (int x = 10; x>0; x--){
  if(x == positionY){
    Serial.print("]]D|| |");
  } else {
    Serial.print("      |");
  }
  for (int k = 1; k<15; k++){
    if((stage[k] <= x + 2) && (stage[k] >= x-2)){
      Serial.print("[ ]");
    } else {
      Serial.print("[X]");
    }
  }
  Serial.println(">>>");
}
  }
}

//displays the road. The road is the array that has the data of the road
void displayTheRoadSerial(){
  if(allowSerial == 1){
    //display of the stage
Serial.print("Road: ");
for(int x=0; x<15; x++ ){
  Serial.print("[");
  Serial.print(road[x]);
  Serial.print("]");
}
Serial.println("");
  }
}

//displays the data that builds the actual game stage.
void displayTheStageSerial(){
  if(allowSerial == 1){
    //display of the stage
Serial.print("Stage: ");
for(int x=0; x<15; x++ ){
  Serial.print("[");
  Serial.print(stage[x]);
  Serial.print("]");
}
Serial.println("");
  }
}
