// ======= LCD block starts======================
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// ======= LCD block ends ======================

int buttonPin = 2; //digital pin 2. For the attachinterrupt
int keyPin = 6; //For the key of the joystic. Should be connected and should be Digital
int lives = 0; //The lives of the player in the game
int allowOLED = 1; //if it is 1 the OLED is allowed to function
int theScreen = 1; //if it is 1 it means that the OLED works
int allowSerial = 1; //allow serials to be printed
int positionX = 0; //the x position of the vehicle (Horizontal)
int positionY = 1; //the y position of the vehicle (Vertical)
int moveX = 0; //The horizontal (X) reading of the joystic
int moveY = 0; //The vertical (Y) reading of the joystic
int road[180] = {0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0,0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0,0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0,0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0,0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0,0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0,0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0,0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0};
int stage[15]; //Array with the data for the displayed road every round
int stepInStage = 0; //it shows in which position of the stage the player is 
int valueInStep; //it shows what how is the road designed in that step
int upperColision = 0; //the upper limit of the road to the specific step
int underCollision = 0; //the under limit of the road to the specific step
int delayer = 1; //it will go until 10. the higher the delayer, the slower the car is.
int ticker = 0; //it gives the tempo

// variables will change:
int buttonState = 0; // int variable for button status

void setup() {
//===========LCD setup block starts =====================
lcd.begin(16, 2);
  lcd.print("First line");
  lcd.setCursor(0,1);
  lcd.print("Second line");
//===========LCD setup block ends =====================
  
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
  if(allowOLED == 0){
    theScreen = 0;
    Serial.println("OLED is turned off by the admin"); //Caveman debugging
  }
  
Serial.println("Finishing the setup()");
}

//=============== Start of loop() =========================================
void loop() {
 if(lives > 0){ //checks if the game is on
  
  //generates the stage for the round
  int stepForLoop = stepInStage;
  int count = 0;
for(int x=0; x<15; x++ ){
  if((stepForLoop + count) < 180){
    stage[x] = road[stepForLoop + count];
  } else {
    stage[x] = road[count + (15 - stepForLoop)];
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
if((positionY == valueInStep)){
  lives = lives - 1;
  Serial.println("Boom! You lost a life!");
  lcd.setCursor(15,1);
  lcd.print("B");
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
displayAdvanceStageLCD();
//lcd.print("Lives:");
//displayAdvanceStageOLED();  //displays the displayed road on the OLED screen

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
  if(positionY > 1){
    positionY = positionY - 1;
  }
}
if (moveY < 250){
  //Serial.println("the pixel moves upwards");
  if(positionY < 2){
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
delay(200);
 } else {
  Serial.println("Game Out! Insert Coin!");
  lcd.setCursor(0,0);
  lcd.print("Game Out!");
  lcd.setCursor(0,1);
  lcd.print("Insert Coin!");
  //displayTheText("Game Out! Insert Coin!");
  //display.drawPixel(10, 10, SSD1306_WHITE);
  delay(1000);
 }

 lcd.clear();
}

// ========================= end of loop() ---------------------------------------

//when on interrupt, one life is added to the game. Like inserting a coin
void insertCoin(){
  lives = 9;
  
}





//displays the actual game in the serial monitor
void displayAdvanceStageSerialOLD(){
  if(allowSerial == 1){
     //Generates and advance stage
for (int x = 2; x>0; x--){
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

//displays the actual game in the serial monitor
void displayAdvanceStageSerial(){
  if(allowSerial == 1){
     //Generates and advance stage
for (int x = 2; x>0; x--){
  if(x == positionY){
    Serial.print("@>>");
  } else {
    Serial.print("   ");
  }
  for (int k = 1; k<15; k++){
    if(stage[k] != x){
      Serial.print("[ ]");
    } else {
      Serial.print("[X]");
    }
  }
  Serial.println(">>>");
}
  }
}


//displays the actual game in the serial monitor
void displayAdvanceStageLCD(){
  if(allowSerial == 1){
     //Generates and advance stage
for (int x = 2; x>0; x--){
  if(x == positionY){
    lcd.setCursor(0, 1-(x -1));
    lcd.print("@");
  } else {
    lcd.setCursor(0, 1-(x -1));
    lcd.print(" ");
  }
  for (int k = 1; k<15; k++){
    if(stage[k] != x){
      lcd.setCursor(k,1-(x -1));
      lcd.print("");
    } else {
      lcd.setCursor(k,1-(x -1));
      lcd.print("X");
    }
  }
  lcd.setCursor(15,0);
  lcd.print(lives);
  //Serial.println(">");
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
