#include <LiquidCrystal.h>

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int numOfButtons = 4;
const int button[numOfButtons] = {9, 10, 11, 12};
int buttonState[numOfButtons];
int lastButtonState[numOfButtons] = {LOW, LOW, LOW, LOW};
bool inputFlags[numOfButtons] = {LOW, LOW, LOW, LOW};
long lastDebounceTime[numOfButtons] = {0, 0, 0, 0};
long debounceDelay = 5;
const int numStairPin = 14;
const int stairPin[numStairPin] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43};
const int numStairButton = 2;
const int stairButton[numStairButton] = {28, 29};
int stairButtonState[numStairButton] = {LOW, LOW};


const int numOfScreens = 6;
int currentScreen = 0;
String screens[numOfScreens][2] = {{"First Button", "Delay 1:"}, {"First Button", "Delay 2:"}, {"First Button", "Delay 3:"},
{"Second Button", "Delay 1:"}, {"Second Button", "Delay 2:"}, {"Second Button", "Delay 3:"}};
int parameters[numOfScreens] = {500, 1000, 500, 500, 1000, 500};

void setup() {
  for (int i = 0; i < numOfButtons; i++){
    pinMode(button[i], INPUT);
    }

  Serial.begin(9600);
  Serial.println(currentScreen);
  lcd.begin(16, 2);
  
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(0, 1);
  lcd.print(screens[currentScreen][1]);
  lcd.print(" ");
  lcd.print(parameters[currentScreen]);

  for (int i = 0; i < numStairPin; i++){
    pinMode(stairPin[i], OUTPUT);
    digitalWrite(stairPin[i], HIGH);
  }
  for (int j = 0; j < numStairButton; j++){
    pinMode(stairButton[j], INPUT);
  }
}

void loop(){

for (int k = 0; k < numStairButton; k++){
    stairButtonState[k] = digitalRead(stairButton[k]);
  }

firstButton (50, parameters[0], parameters[1], parameters[2]);
secondButton (50, parameters[3], parameters[4], parameters[5]);

setInputFlags();
resolveInputFlags();

}
 

void setInputFlags(){
  for(int i = 0; i < numOfButtons;i++){
    int reading = digitalRead(button[i]);
    if(reading != lastButtonState[i]){
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay){
      if (reading != buttonState[i]){
        buttonState[i] = reading;
        if (buttonState[i] == HIGH){
          inputFlags[i] = HIGH;
        }
      }
    }
    lastButtonState[i] = reading;
  }
}


void resolveInputFlags(){
  for (int i = 0; i < numOfButtons; i++){
    if (inputFlags[i] == HIGH){
      buttonAction(i);
      inputFlags[i] = LOW;
      printScreen();
    }
  }
}


void buttonAction(int input){
  if(input == 0){
    if(currentScreen == 0){
      currentScreen = numOfScreens - 1;
    }
    else{
    lcd.clear();
    currentScreen --;
    printScreen();
    Serial.println(currentScreen);
    }
   }
   else if(input == 1){  
    if (currentScreen == numOfScreens -1){
      currentScreen = 0;
    }
    else{
    lcd.clear();
    currentScreen ++;
    Serial.println(currentScreen);
     }
   }
   else if(input == 2){
    parameterChange(0);
    }
   else if(input == 3){
    parameterChange(1);
    }
}

void parameterChange(int change){
  if (change == 0){
    if(parameters[currentScreen] == 0){
      parameters[currentScreen] = 0;
    }
    else {
      (parameters[currentScreen]) = (parameters[currentScreen] - 500);
    }
  }
  else if (change == 1){
    (parameters[currentScreen]) = (parameters[currentScreen] + 500);
  } 
}

void printScreen(){
  lcd.clear();
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(0, 1);
  lcd.print(screens[currentScreen][1]);
  lcd.print(" ");
  lcd.print(parameters[currentScreen]);
}

void firstButton (int t1, int t2, int t3, int t4){
  if (stairButtonState[0] == HIGH){

    for(int i = 13; i >= 0; i--){
      digitalWrite(stairPin[i], LOW);
      if ((stairPin[i] <= 43) && (stairPin[i] >= 42)){
        delay(t1);
      }
      else if ((stairPin[i] <= 41) && (stairPin[i] >= 31)){
        delay(t2);
      }
    }
    delay(t3);

    for(int j = 13; j >=0; j--){
      digitalWrite(stairPin[j], HIGH);
      delay(t4);
    }
    stairButtonState[0] = 0;
  }
}

void secondButton (int t1, int t2, int t3, int t4){
  if (stairButtonState[1] == HIGH){
    
    for(int i = 0; i < numStairPin; i++){
      digitalWrite(stairPin[i], LOW);
      if ((stairPin[i] <= 31) && (stairPin[i] >= 30)){
        delay(t1);
      }
      else if ((stairPin[i] <= 42) && (stairPin[i] >= 32)){
        delay(t2);
      }
    }
    delay(t3);

    for(int j = 0; j < numStairPin; j++){
      digitalWrite(stairPin[j], HIGH);
      delay(t4);
    }
    stairButtonState[1] = 0;
  }
 }

  
