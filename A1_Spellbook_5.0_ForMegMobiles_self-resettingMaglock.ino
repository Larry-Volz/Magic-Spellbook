  /*WORKING version where: 
 * reed switches light up One center RBG LED using a protoshield
 * debouncing working
 * Muliple wands fixed
 * maglock good
 
 * To do:  
 * - Set up a reset switch/system (reed switch so when box closed auto-reset)
 * - Design contingency for battery dead
 * - begin world domination plan XK-1
 * - Moo hah haaaaaaa!!!!
 */
 
/*REED SWITCHE WIRING:  
 * wire 5v to one end of reed switch
 * Other end wire to pin #X (4,12,11,7) AND THEN 10K Ohm resister and THEN resister to gnd 
  (has to be in THAT order to work)
  
 LED WIRING
 * USE Radio Shack common-anode RGB part number 2760028
 * If the longest wire is 2nd from the left that is the anode and the other 3 from left to right
 * are red, blue, green (not RGB -> RBG).  
 * Wire the anode (long wire) to 5v power
 * left pin (red) 150 Ohm resistor then to A3
 * Third pin from left (the one to right of long pin - blue) 75 Ohm resistor and then to A1
 * Right-most pin (green) 75 Ohm resistor and then to A2
 */

// ######## READING AND LIGHTING LED VARIABLES ####

 int reading[4];
 #define numberOfreadings 6

 int i; //loop integer
 int j; //loop integer

 int reedSwitchPin[] = {4,12,11,7};  
 // 4=red lower L, 12=dk blue upper L, 11=green lower Rt, 7=lt blue upper Rt
 #define numberOfReedSwitches 4 

//FOR MOBILE ROOMS ACCORDING TO XSCAPE PROTOTYPE
//4=RED   12=GREEN    11=DK BLUE   7=LT BLUE
 // ############# PUZZLE ORDER VARIABLES ############

#define numberOfTaps 6 //what Eli called MAX_ELEMENTS

//OLD
//int glyphOrder[] = {7,12,4,12,12,11}; //7,the code = order of reedSwitchPins

//GLYPH ORDER PER XSCAPE: LTBLUE, GREEN, RED, GREEN, GREEN, DKBLUE
int glyphOrder[] = {7,12,4,12,12,11}; //7,the code = order of reedSwitchPins


#define UNIQUE_PINS 4
int checkNextPuzzleEntry=0;
int correctTaps=0;
int attemptNumber=0;

int wandCheck = 0;
int reedSwitchToSkip;
int lastRuneTapped;
unsigned long runesTouched=0;
unsigned long wandCheckTimerStart;
unsigned long wandCheckTimerEnd;

 //###########this part unused - was for 4-LED's.  But don't comment it out without rewriting those parts
 
 int ledPin[] = {13,2,5,6}; //originally 13, 4, 5, 6 - no longer used with one RGB LED?
 #define numberOfLeds 4

//Debouncing variables
int ledState = LOW;         // the current state of the output pin
int buttonState[] = {0,0,0,0,0}; // add a 0 for each new pin
int lastbuttonState[] = {0,0,0,0,0};   // the previous reading from the input pin
unsigned long lastDebounceTime[] = {0,0,0,0,0};  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long timeElapsed[numberOfReedSwitches];

//RGB LED variables for that "win"

int redPin = A3; 
int greenPin = A1; 
int bluePin = A2;
int RGBcounter=0;

//RGB LED variables for colors
int redLED[] = {255,0,0,110}; // 1st of 3 digits for red, green, blue, yellow
int greenLED[] = {0,255,0,255}; // 2nd (green) digit for those colors
int blueLED[] = {0,0,255,255}; //3rd (blue) digit ""

#define COMMON_ANODE //uncomment this line if using a Common Anode LED //

// maglock set-up

int maglockPin = 10;
int maglockState = 0; //****changed from 0 to 1

void setup()
{
  //Define inputs and outputs
  for(i=1; i< numberOfReedSwitches; i++)
      {
        pinMode(ledPin[i], OUTPUT);      //define settings
        pinMode(reedSwitchPin[i], INPUT);
      }

   pinMode(maglockPin, OUTPUT);
    
    // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}


void loop()
{

static int correctTaps = 0; //reads to see if taps are in sequence

// TRIGGER maglock (or maglock)
digitalWrite(maglockPin,maglockState);   //****Somehow locks it?****
//Serial.println("maglock on");

i=0;
for(i = 0; i< numberOfReedSwitches; i++)
    {   
      
     //read reedswitchs/light led's 
     reading[i] = digitalRead(reedSwitchPin[i]);  //read reedswitch[] status

/*
Serial.print("switch #");
Serial.print(i);
Serial.print(" ");
Serial.print(" = ");
Serial.print(reading[i]);
Serial.print("   ");
*/


      if (reading[i] != lastbuttonState[i]) //check for reedswitch change     
          {            
             // reset the debouncing TIMER
             lastDebounceTime[i] = millis(); //reset the debounce counter
          }

        timeElapsed[i] = millis() - lastDebounceTime[i];
  
        if (timeElapsed[i] > debounceDelay) 
          {
             // whatever the reading is at, it's been there for longer
             // than the debounce delay, so take it as the actual current state:
             // if the button state has changed:
 
                  if (reading[i] != buttonState[i]) // if there is an unbounced change then change the button state
                      {
                      buttonState[i] = reading[i];

                      // Light LED if reedswitch[] is tapped

                        if (buttonState[i] == HIGH)
                          {
                            digitalWrite(ledPin[i], HIGH);
                            setColor(redLED[i],greenLED[i],blueLED[i]);
                           // delay(500);
                          }
                        else
                          {
                            digitalWrite(ledPin[i], LOW);
                            setColor(0,0,0);
                          // delay(100);
                          }

                      
                       if (reading[i]) // if change is another tap then check next puzzle entry
                          { 
                          checkNextPuzzleEntry = reading[i];
                          lastRuneTapped=reedSwitchPin[i];
                          attemptNumber +=1;
                          }
                        }
                      
           }        
                     
                  



/*

                   digitalWrite(ledPin[i], buttonState[i]);
                    setColor(redLED[i],greenLED[i],blueLED[i]);
                   // delay(500);
*/


  lastbuttonState[i] = reading[i];  //reset for debouncing
  


//BEGIN PUZZLE LOGIC LOOP
if(checkNextPuzzleEntry)
  {
    if(digitalRead(glyphOrder[correctTaps]))
      {

        //MAKE SURE NO MULTIPLE WANDS
          reedSwitchToSkip = lastRuneTapped;       
          wandCheck = wandCheckFunction (reedSwitchToSkip);
          
          if(wandCheck == 0)
            {
              correctTaps += 1;
            }
          
         /*  Serial.print("    correctTaps");
           Serial.print(correctTaps);   
           Serial.print("    attempts");
           Serial.print(attemptNumber);   
           Serial.println();     
            */
            
         if(correctTaps == numberOfTaps) // DID YOU COLLECT ALL CORRECT TAPS?
            {
             Serial.print("YOU WIN!!!");   
             Serial.println();     
        
              //We have unlocked
              //TURN maglock ON/OFF
Serial.print("maglockState Before = ");
Serial.print(maglockState);
Serial.print("      ");

            maglockState = 1;  //open or close the maglock possibly later  **** changed this from !maglockState to 1
            digitalWrite(maglockPin,maglockState);

Serial.print("maglockState After = ");
Serial.print(maglockState);
Serial.print("      ");
Serial.println();
              

/* RUN THE RGB LED 'WIN' SUBROUTINE TO LET YOU KNOW YOU GOT IT RIGHT */

              for(j = 0; j < 4; j++)
                {
                  setColor(255, 0, 0); // red 
                  delay(100);
                  setColor(0, 255, 0); // green 
                  delay(100);
                  setColor(0, 0, 255); // blue 
                  delay(100);
                  setColor(255, 255, 0); // yellow 
                  delay(100);
                  setColor(80, 0, 80); // purple 
                  delay(100);
                  setColor(0, 255, 255); // aqua 
                  delay(100);
             
                setColor(0,0,0); //off
                }

                delay(5000);  //Changed
                maglockState = 0;  //changed **** changed this to 0 to lock
                digitalWrite(maglockPin,maglockState);
                
            } 
            else 
                  {
      
                     checkNextPuzzleEntry = 0;
      
                  }            
         }
         else 
          {
            /* int i;
             for(i = 0; i < UNIQUE_PINS; i++) 
               {
                 if(digitalRead(glyphOrder[correctTaps])) 
                    { 
                      correctTaps = 0;
                     } 
  */
                  correctTaps = 0;
               }
          }
  } //end of switch-checking-led-lighting loop  

      //    Serial.println(reading[1]);
          
} //end of Void loop

//DEFINE RGB LED function
void setColor(int red, int green, int blue)  //define function
{
  #ifdef COMMON_ANODE 
  red = 255 - red;
  green = 255 - green; 
  blue = 255 - blue;
  #endif
  analogWrite(redPin, red); 
  analogWrite(greenPin, green); 
  analogWrite(bluePin, blue);
}

//DEFINE MULTIPLE WANDS CHECK
int wandCheckFunction (int reedSwitchToSkip){
  
    runesTouched = 0;
    wandCheckTimerStart = millis();
    wandCheckTimerEnd = wandCheckTimerStart + 250; //run it for 1/4 of a second
    
    do{
      
        for(i = 0; i< numberOfReedSwitches; i++)
            {
               if (reedSwitchPin[i] != reedSwitchToSkip)
                {
                   runesTouched = runesTouched + digitalRead(reedSwitchPin[i]);  
                 }
        
                 
                //
             }
    }while(millis()<wandCheckTimerEnd);

                  Serial.print("runes touched ");
                  Serial.println(runesTouched);
                  
                 return runesTouched;
}

/*
 Serial.print("i");
 Serial.print(i);
 Serial.print("  ");
 Serial.print("    reading ");
 Serial.print(reading[i]);
  Serial.print("    lastDebounceTime ");
 Serial.print(lastDebounceTime[i]);
   Serial.print("    buttonState[]");
 Serial.print(buttonState[i]);
    Serial.print("    lastbuttonState[]");
 Serial.print(lastbuttonState[i]);
 Serial.println();
  delay(1);        // delay in between reads for stability
         /*
                   Serial.print("    reading[");
                   Serial.print(i);
                   Serial.print("] ");
                   Serial.print(reading[i]); 
                   Serial.print("    buttonState[");
                   Serial.print(i);
                   Serial.print("] ");
                   Serial.print(buttonState[i]);
                   Serial.print("    lastDebounceTime[");
                   Serial.print(i);
                   Serial.print("] ");
                   Serial.print(lastDebounceTime[i]);            
                   Serial.println();
                   //delay(50);
                   Serial.print("    timeElapsed[0]");
                   Serial.print(timeElapsed[0]);                  
                   Serial.print("    timeElapsed[1]");
                   Serial.print(timeElapsed[1]);  
                   Serial.println();  
                   
                Serial.print("reading[0] ");
               Serial.print(reading[0]); 
               Serial.print("    buttonState[0]");
               Serial.print(buttonState[0]);   
                Serial.print("   reading[1] ");
               Serial.print(reading[1]); 
               Serial.print("    buttonState[1]");
               Serial.print(buttonState[1]);                                    
               Serial.println();      
                delay(1);
                 
*/
