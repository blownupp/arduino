/*
 * Inspired by Rui Santos @ gist.github.com/ruisantos16/
 * and Adafruit Industries. Everything is open and free
 * and all that jazz, if you even see this at some point
 * be sure to keep this intact and share!
 */
 
const int data = 7;
const int latch = 9;
const int clock = 8;

int newestDigit[6] = {};
int digitScan = 0;
 
unsigned long previousTmillis = 0;
unsigned long previousMillis = 0;
unsigned long refreshRate = 10; // millis()
unsigned long oneSecond = 1000; // one second = 1000 millis
 
boolean newDigit = false;
 
const byte value[] = {B11000000,  // 0
                      B11111001,  // 1
                      B10100100,  // 2..
                      B10110000,  // 3
                      B10011001,  // 4
                      B10010010,  // 5
                      B10000010,  // 6
                      B11111000,  // 7
                      B10000000,  // 9
                      B10010000};   // 9
 
const byte digit[] = {B00000100, // hours
                      B00001000, // hours
                      B00010000, // minutes
                      B00100000, // minutes
                      B01000000, // seconds
                      B10000000};  // seconds


void setup() {
  pinMode(data, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);

// Serial.begin(9600) // once I get the clock working

};

// updates display
void updateDisp()
  {
    newDigit = false;
    
    shiftThis(B11111111, B11111111);
    
    shiftThis(digit[digitScan], value[newestDigit[digitScan]]);
    
    digitScan++;
       
    if (digitScan > 5)
      {
       digitScan = 0;
      }; 
  };
  
void shiftThis(byte a, byte b) // shiftOut for each '595 attached
  {
    digitalWrite(latch, LOW);
      shiftOut(data, clock, MSBFIRST, a);
      shiftOut(data, clock, MSBFIRST, b);
    digitalWrite(latch, HIGH);
  };

// Set this to current time (24hr clock)

int oneSec = 0;
int tenSec = 0;
int oneMin = 3;
int tenMin = 1;
int oneHour = 9;
int tenHour = 1;

/*     This will get called at the beginning of each main loop().
     * Being a state machine - not depending on delay() for program flow,
     * other functions can carry on while we wait for a full second to pass.
     *    After a second has passed, the counter is increased and the newDigit var
     * is set true (if inbetween updates, we force a new one to display the new digit). 
     *    This continues, adding in a variable for each collumn:
     * tens of seconds, minutes, tens of minutes and so on. This way, we only have
     * to keep track of the duration of one second along with a running total of the
     * other values.
     *    Well, that's the idea at least.
     */

void Time() { 
    unsigned long currentTmillis = millis();
    
    if (currentTmillis - previousTmillis >= oneSecond)
    {
      previousTmillis = currentTmillis;
      oneSec += 1;
      newDigit = true;
    };
    
        if (oneSec > 9)
        {
          oneSec = 0;
          tenSec += 1;
          newDigit = true;
        };
        
        if (tenSec > 5)
        {
          tenSec = 0;
          oneMin += 1;
          newDigit = true;
        };
        
            if (oneMin > 9)
            {
              oneMin = 0;
              tenMin += 1;
              newDigit = true;
            };
            
            if (tenMin > 5)
            {
              tenMin = 0;
              oneHour += 1;
              newDigit = true;
            };
            
                if ((tenHour >= 2) && (oneHour > 3))
                {
                  tenSec,oneMin,tenMin,oneHour,tenHour=0;
                  oneSec += 1;
                  newDigit = true;
                }
                else if (oneHour > 9)
                {
                  oneHour = 0;
                  tenHour += 1;
                  newDigit = true;
                };
  };
        
   
void loop()
  {
    newestDigit[5] = oneSec;
    newestDigit[4] = tenSec;
    newestDigit[3] = oneMin;
    newestDigit[2] = tenMin;
    newestDigit[1] = oneHour;
    newestDigit[0] = tenHour;
        
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= refreshRate)
      {
        previousMillis = currentMillis;
        updateDisp();
      };
    
    if (newDigit == true)
    {
      updateDisp();
    };
    
    Time();
    
            
    };
