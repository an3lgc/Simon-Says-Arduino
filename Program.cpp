/*Simon Says game with LCD Display.
  Originally by Vineeth Balachnadran
  Modified to use a standard buzzer with tone()/noTone() instead of the Tone library.
*/

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Note frequencies (replaces what Tone.h used to provide)
#define NOTE_C3  131
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_C5  523

const int buzzerPin = 12; // buzzer is on pin 12 (same pin the speaker used)

int starttune[] = {NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4};
int duration2[] = {100, 200, 100, 200, 100, 400, 100, 100, 100, 100, 200, 100, 500};
int note[] = {NOTE_C4, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5};
int duration[] = {100, 100, 100, 300, 100, 300};
int button[] = {8, 9, 10, 11}; //The four button input pins (matches actual wiring)
int ledpin[] = {2, 3, 4, 5};  // LED pins (matches actual wiring)
int turn = 0;  // turn counter
int buttonstate;  // button state checker
int buttonstate1;
int randomArray[100]; //Intentionally long to store up to 100 inputs (doubtful anyone will get this far)
int inputArray[100];
int positionCounter;
int score = 0;

bool exitRequested = false; //set true when the No/Exit button is pressed mid-game

int tim = 500; //the value of delay time

char array1[] = "Simon Says!";
char array2[] = "Want to Play?";
char array3[] = "Lets Play";
char array4[] = "You Lose";

const int button1 = 13; // Yes button
const int button2 = 0;  // No / Exit button

LiquidCrystal_I2C lcd(0x27, 16, 2);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  pinMode(button1, INPUT_PULLUP); // internal pull-up keeps pin HIGH until pressed (pulled to GND)
  pinMode(button2, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  showWelcome();
}

//Scrolls "Simon Says!" then "Want to Play?" across the LCD.
//Called once at startup, and again any time the No/Exit button is pressed.
void showWelcome()
{
  lcd.clear();
  lcd.setCursor(15, 0); // set the cursor to column 15, line 0
  for (int positionCounter = 0; positionCounter < 11; positionCounter++)
  {
    lcd.scrollDisplayLeft(); //Scrolls the contents of the display one space to the left.
    lcd.print(array1[positionCounter]); // Print a message to the LCD.
    delay(tim); //wait for 250 microseconds
  }
  lcd.clear(); //Clears the LCD screen and positions the cursor in the upper-left  corner.
  lcd.setCursor(15, 0); // set the cursor to column 15, line 1

  for (int positionCounter = 0; positionCounter < 13; positionCounter++)
  {
    lcd.scrollDisplayLeft(); //Scrolls the contents of the display one space to the left.
    lcd.print(array2[positionCounter]); // Print a message to the LCD.
    delay(tim); //wait for 250 microseconds
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
  buttonstate = digitalRead(button1);
  buttonstate1 = digitalRead(button2);
  Serial.println(buttonstate);
  Serial.println(buttonstate1);

  //When the user clicks the YES button, the game will begin.
  if (buttonstate == LOW) {

    lcd.clear();
    lcd.setCursor(15, 0); // set the cursor to column 15, line 0
    for (int positionCounter = 0; positionCounter < 9; positionCounter++)
    {
      lcd.scrollDisplayLeft(); //Scrolls the contents of the display one space to the left.-
      lcd.print(array3[positionCounter]); // Print a message to the LCD
      delay(tim); //wait for 250 microseconds
    }

    lcd.clear(); //Clears the LCD screen and positions the cursor in the upper-left  corner.

    //This for loop will register all the LEDS as outputs
    for (int x = 0; x < 4; x++) // LED pins are outputs
    {
      pinMode(ledpin[x], OUTPUT);
    }

    //This for loop will register buttons that control LEDs as inputs and to make them do digitalWrite
    for (int x = 0; x < 4; x++)
    {
      pinMode(button[x], INPUT);  // button pins are inputs
      digitalWrite(button[x], HIGH);  // enable internal pullup; buttons start in high position; logic reversed
    }

    randomSeed(analogRead(0)); //Added to generate "more randomness" with the randomArray for the output function

    //This for loop will being to play the intro song of the program
    for (int thisNote = 0; thisNote < 13; thisNote ++) {
      // play the next note:
      tone(buzzerPin, starttune[thisNote]);
      // hold the note:
      if (thisNote == 0 || thisNote == 2 || thisNote == 4 || thisNote == 6)
      {
        digitalWrite(ledpin[0], HIGH);
      }
      if (thisNote == 1 || thisNote == 3 || thisNote == 5 || thisNote == 7 || thisNote == 9 || thisNote == 11)
      {
        digitalWrite(ledpin[1], HIGH);
      }
      if (thisNote == 8 || thisNote == 12)
      {
        digitalWrite(ledpin[2], HIGH);
      }
      if (thisNote == 10)
      {
        digitalWrite(ledpin[3], HIGH);
      }
      delay(duration2[thisNote]);
      // stop for the next note:
      noTone(buzzerPin);
      digitalWrite(ledpin[0], LOW);
      digitalWrite(ledpin[1], LOW);
      digitalWrite(ledpin[2], LOW);
      digitalWrite(ledpin[3], LOW);
      delay(25);
    }
    delay(1000);

    //The actual game will being. Random LEDS will begin to genetate and be outputted to the user.
    for (int y = 0; y <= 99; y++)
    {
      //Check if the player wants to quit before starting the next round
      if (digitalRead(button2) == LOW) {
        lcd.clear();
        lcd.print("Exiting Game");
        delay(1000);
        score = 0;
        turn = 0;
        showWelcome();
        break; //leave the round loop and fall back to the top-level loop()
      }

      //function for generating the array to be matched by the player
      digitalWrite(ledpin[0], HIGH);
      digitalWrite(ledpin[1], HIGH);
      digitalWrite(ledpin[2], HIGH);
      digitalWrite(ledpin[3], HIGH);

      for (int thisNote = 0; thisNote < 6; thisNote ++) {
        // play the next note:
        tone(buzzerPin, note[thisNote]);
        // hold the note:
        delay(duration[thisNote]);
        // stop for the next note:
        noTone(buzzerPin);
        delay(25);
      }

      digitalWrite(ledpin[0], LOW);
      digitalWrite(ledpin[1], LOW);
      digitalWrite(ledpin[2], LOW);
      digitalWrite(ledpin[3], LOW);
      delay(1000);

      for (int y = turn; y <= turn; y++)
      { //Limited by the turn variable
        Serial.println(""); //Some serial output to follow along
        lcd.print("Score: ");
        lcd.print(score);
        score++;
        Serial.print(y);
        Serial.println("");
        randomArray[y] = random(1, 5); //Assigning a random number (1-4) to the randomArray[y], y being the turn count
        for (int x = 0; x <= turn; x++)
        {
          Serial.print(randomArray[x]);

          //Light up and sound the ONE led/tone that matches this step in the sequence,
          //using the randomArray value (1-4) as a POSITION in ledpin[], not a hardcoded pin number.
          //This way it works no matter which physical pins button[]/ledpin[] actually point to.
          int idx = randomArray[x] - 1; // convert 1-4 to array index 0-3
          int stepTone;
          switch (idx) {
            case 0: stepTone = NOTE_G3; break;
            case 1: stepTone = NOTE_A3; break;
            case 2: stepTone = NOTE_B3; break;
            default: stepTone = NOTE_C4; break;
          }
          digitalWrite(ledpin[idx], HIGH);
          tone(buzzerPin, stepTone, 100);
          delay(400);
          digitalWrite(ledpin[idx], LOW);
          delay(100);
        }
      }
      //Will go into the input function and check and see if user input mathced program generated LED
      input();
      if (exitRequested) {
        exitRequested = false;
        break; //player exited mid-round; leave the round loop and fall back to the top-level loop()
      }
    }
  }
  //If the user clicks the NO button, treat it as a retry: reset the game and
  //loop back to the welcome prompt so they can press Yes again.
  else if (buttonstate1 == LOW){
    score = 0;
    turn = 0;
    lcd.clear();
    lcd.print("See you later!");
    delay(1000);
    showWelcome();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void input() { //Function for allowing user input and checking input against the generated array

  for (int x = 0; x <= turn;)
  { //Statement controlled by turn count

    for (int y = 0; y < 4; y++)
    {

      //Check if the player wants to quit while waiting for their next move
      if (digitalRead(button2) == LOW) {
        lcd.clear();
        lcd.print("Exiting Game");
        delay(1000);
        score = 0;
        turn = 0;
        showWelcome();
        exitRequested = true;
        return; //leave input() immediately
      }

      buttonstate = digitalRead(button[y]);

      //Checking for button push. Uses the button's POSITION in button[] (y) instead of a
      //hardcoded pin number, so this works no matter which physical pins button[]/ledpin[] point to.
      if (buttonstate == LOW)
      {
        int inputTone;
        switch (y) {
          case 0: inputTone = NOTE_G3; break;
          case 1: inputTone = NOTE_A3; break;
          case 2: inputTone = NOTE_B3; break;
          default: inputTone = NOTE_C4; break;
        }
        digitalWrite(ledpin[y], HIGH);
        tone(buzzerPin, inputTone, 100);
        delay(200);
        digitalWrite(ledpin[y], LOW);
        inputArray[x] = y + 1; //Checks value input by user and checks it against
        delay(250);
        Serial.print(" ");
        Serial.print(y + 1);
        if (inputArray[x] != randomArray[x]) { //the value in the same spot on the generated array
          digitalWrite(ledpin[0], LOW);
          digitalWrite(ledpin[1], LOW);
          digitalWrite(ledpin[2], LOW);
          digitalWrite(ledpin[3], LOW);
          fail();                              //The fail function is called if it does not match
        }
        x++;
      }
    }
  }
  lcd.clear();
  delay(500);
  turn++; //Increments the turn count, also the last action before starting the output function over again
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fail() { //Function used if the player fails to match the sequence

  for (int y = 0; y <= 2; y++)
  { //Flashes lights for failure, each flash paired with a buzzer tone
    digitalWrite(ledpin[0], HIGH);
    digitalWrite(ledpin[1], HIGH);
    digitalWrite(ledpin[2], HIGH);
    digitalWrite(ledpin[3], HIGH);
    tone(buzzerPin, NOTE_G3, 300);
    delay(200);
    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);
    tone(buzzerPin, NOTE_C3, 300);
    delay(200);
  }
  //Reset score to 0
  score = 0;
  lcd.clear();
  lcd.print("You Lost");
  delay(2000);
  turn = -1; //Resets turn value so the game starts over without need for a reset button
}
