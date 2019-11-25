/*
 * LCD RS pin to digital pin 8
 * LCD Enable to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 *** This is the pinout used by legacy LCD shields. ***
 * Pushbutton digital pin 2 [momentary-contact switch]
 */

#include <LiquidCrystal.h>
const int rs = 10, e = 9, d4 = 8, d5 = 7, d6 = A3, d7 = A4;
//LiquidCrystal lcd(rs, e, d4, d5, d6, d7);
LiquidCrystal lcd(10, 9, 8, 7, A3 , A4);

//button related variables
int B1Pin = A7;
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;
boolean pushed = false; 
boolean gameEnd = false;
boolean result  = false;

//time related variables
unsigned long tempTime = millis();
unsigned long currentTime;
int s = 1000;

//Other variables
const int totalNum = 36;
int originalArr[totalNum];
int arr[totalNum];
int welcomeOnceThrough = 0;
//boolean repeat = true;

void setup() 
{
  Serial.begin(115200);
  
  //Fill the aray 
  for(int i = 0; i <= totalNum-1; i++)
  {
    arr[i] = i+1;
    originalArr[i] = i+1;
  }
  welcome(1);
  randomize (arr, totalNum); 
}

void loop() 
{
  result = analogButtonPush(B1Pin);
  nextNum(1);
}

void pause(int period)
{
  currentTime = millis();
  if(currentTime - tempTime > period) 
  {
   // execute action
   tempTime = currentTime;
  }
}

void scramble()
{
  Serial.println("This is the original array: ");
  
  for (int i = 0; i < totalNum; i++) 
  {
    Serial.print(originalArr[i]);
    Serial.print("   ");
  }
  
  Serial.println("");

  //randomize (arr, totalNum);

  Serial.println("This is the 'randomized' array: ");
  for (int i = 0; i < totalNum; i++) 
  {
    Serial.print(arr[i]);
    Serial.print("   ");
  }
}

void welcome(boolean outputType)
{
  if(welcomeOnceThrough <= 0)
  { 
    if(outputType == 1)
    {
      //lcd.clear(); 
      lcd.begin(16, 2);  // Standard 2-line, 16-column display.
      lcd.setCursor(0,0); 
      lcd.print(" Welcome to the ");
      delay(1800);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Karuba Number ");
      lcd.setCursor(0,1);
      lcd.print("    Generator   ");
      delay(1800);

      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("Press button to");
      lcd.setCursor(0,1);
      lcd.print("start new game");
      delay(2000);
       
      welcomeOnceThrough = welcomeOnceThrough + 1;
    }
  
    else
    {
      pause(s);
      Serial.println("");
      pause(100);
      Serial.println(" Welcome to the ");
      Serial.println("  Karuba Number ");
      Serial.println("    Generator   ");
      pause(s*3);
    
      for(int i = 0; i < 2; i++)
      { 
        Serial.println("");
      }
    
      Serial.print("Press button to");
      Serial.print("start new game");
    
      for(int i = 0; i < 2; i++)
      { 
        Serial.println("");
      }
      pause(s*6);
      
      welcomeOnceThrough = welcomeOnceThrough + 1;
    }
  }
}

void swap (int *sequentialEliment, int *randomEliment)
{
    int temp = *sequentialEliment;
    *sequentialEliment = *randomEliment;
    *randomEliment = temp;
}

void randomize (int arr[], int n )
{
    // Use a different seed value so that we don't get same
    // result each time we run this program
    randomSeed(analogRead(A0));
 
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    
    for(int i = n-1; i > 0; i--)
    {
      //randomly select an array eliment.
      int j = random(0, n);
        
      swap(&arr[i], &arr[j]);
      
      //This works because we start at one end of the
      //array and work our way to the oppisite end of the array.
      //As we travel sequentialy through the array we swap the 
      //current element number of the newly randomized number.

      //eg( our sample array size is 1 to 10                                                                           )
      //eg(                                                                                                            )
      //eg(                                                                      before                  after         )
      //eg( sequential number [1]     swaped with random number [7]   =   1,2,3,4,5,6,7,8,9,10    7,2,3,4,5,6,1,8,9,10 )
      //eg( sequential number [2]     swaped with random number [9]   =   7,2,3,4,5,6,1,8,9,10    7,9,3,4,5,6,1,8,2,10 )
      //eg( sequential number [3]     swaped with random number [5]   =   7,9,3,4,5,6,1,8,2,10    7,9,5,4,3,6,1,8,2,10 )
      //eg( sequential number [4]     swaped with random number [1]   =   7,9,5,4,3,6,1,8,2,10    4,9,5,7,3,6,1,8,2,10 )
      //eg( sequential number [5]     swaped with random number [10]  =   4,9,5,7,3,6,1,8,2,10    4,9,5,7,10,6,1,8,2,3 )
      //eg( sequential number [6]     swaped with random number [8]   =   4,9,5,7,10,6,1,8,2,3    4,9,5,7,10,8,1,6,2,3 )
      //eg( sequential number [7]     swaped with random number [3]   =   4,9,5,7,10,8,1,6,2,3    4,9,1,7,10,8,5,6,2,3 )
      //eg( sequential number [8]     swaped with random number [2]   =   4,9,1,7,10,8,5,6,2,3    4,6,1,7,10,8,5,9,2,3 )
      //eg( sequential number [9]     swaped with random number [4]   =   4,6,1,7,10,8,5,9,2,3    4,6,1,2,10,8,5,9,7,3 )
      //eg( sequential number [10]    swaped with random number [6]   =   4,6,1,2,10,8,5,9,7,3    4,6,1,2,10,3,5,9,7,8 )
      //eg(                                                                                                            )
      //eg(                                                                     original               end result      )
      //eg(                                                               1,2,3,4,5,6,7,8,9,10    4,6,1,2,10,3,5,9,7,8 )
    }
}

void nextNum(boolean outputType)
{
  if(outputType == 1)
  {
    //LCD output enabled
    if(result == true)
    {
      //button pushed
      
      if(gameEnd == false)
      {
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print(" Tile Number ");
        lcd.setCursor(0,1);
        Serial.print("Button Push Counter: "); 
        Serial.println(buttonPushCounter);
        lcd.print(arr[buttonPushCounter]);
        //buttonCounter += 1;
        //Bresult = 0;
        delay(100);
        
        if(buttonPushCounter == totalNum)
        {
          gameEnd=true;
        }
      }
    }
    if(gameEnd == true)
    {
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print(" Game Over ");
      delay(1800);
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("Time to tally");
      lcd.setCursor(0,1); 
      lcd.print(" the scores ");
      delay(1800);
    }
  }
}

boolean analogButtonPush(int buttonPin)
{
  buttonState = analogRead(buttonPin);
  if (buttonState != lastButtonState) 
  {
    // if the state has changed, increment the counter
    if (buttonState == 0) 
    {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      //Serial.print("number of button pushes: ");
      //Serial.println(buttonPushCounter);
      pushed = true;
    } 
    
    else 
    {
      // if the current state is LOW then the button went from on to off:
      pushed = false;
    }
    
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

  return(pushed);
}
