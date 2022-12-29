/**
 * Two Channel Receiver
 * Author: Shawn Hymel (SparkFun Electronics)
 * Editted and Stolen by: Abington Engineering Team
 * Mixes two channels for arcade drive.
 */

// Controller pins
const int CH_1_PIN = 10;
const int CH_2_PIN = 11;
const int BInPin = 9;

// Motor driver pins
const int AIN1_PIN = 2;
const int APWM_PIN = 5;
const int BIN1_PIN = 7;
const int BPWM_PIN = 6;
const int BOutPin = 3;

// Parameters
const int deadzone = 35;  // Anything between -20 and 20 is stop. This code acts as a circle's area of when the PWM values will start to be transmitted. 
//Joystick is in the circle, no PWM transmitted, when out, PWM transmitted to ESC. 
/*We may to adjust this to our controller. 
 * Some sort of a test in the setup function to see the current voltage and set the deadzone to somewhere around that.
 */
void setup() {

  // Configure pins
  pinMode(AIN1_PIN, OUTPUT);
  pinMode(APWM_PIN, OUTPUT);
  pinMode(BIN1_PIN, OUTPUT);
  pinMode(BPWM_PIN, OUTPUT);
  pinMode(BOutPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  // Read pulse width from receiver
  int y = pulseIn(CH_2_PIN, HIGH);
  int x = pulseIn(CH_1_PIN, HIGH);
  int b = pulseIn(BInPin, HIGH);

  // Convert to PWM value (-255 to 255)
  y = pulseToPWMY(y);
  x = pulseToPWMX(x);
  b = pulseToPWMB(b);
  //blade drive function with the integer of b being the input value of int b from the function. 
  bladeDrive(b);

  // Mix for arcade drive. X comes first to allow for the forward movement of the stick to actaully move the stick forward. 
  int left = x + y;
  int right = x - y;

  // Drive motor
  drive(left, right);
}

void bladeDrive(int b){
  if(b>100){
    analogWrite(BOutPin, 250);
  } else{
    analogWrite(BOutPin, 0);
  }
  
}
// Positive for forward, negative for reverse
void drive(int speed_a, int speed_b) {
  // Limit speed between -255 and 255
  speed_a = constrain(speed_a, -255, 255);
  speed_b = constrain(speed_b, -255, 255);

  // Set direction for motor A
  if ( speed_a == 0 ) {
    digitalWrite(AIN1_PIN, HIGH);
  } else if ( speed_a > 0 ) {
    digitalWrite(AIN1_PIN, LOW);
  } else {
    digitalWrite(AIN1_PIN, HIGH);
  }

  // Set direction for motor B
  if ( speed_b == 0 ) {
    digitalWrite(BIN1_PIN, HIGH);
  } else if ( speed_b > 0 ) {
    digitalWrite(BIN1_PIN, LOW);
  } else {
    digitalWrite(BIN1_PIN, HIGH);
  }
  //The digital outputs were inverted to allow the forward movement with the x to y config. 

  // Set speed
  analogWrite(APWM_PIN, abs(speed_a));
  analogWrite(BPWM_PIN, abs(speed_b));
}

// Convert RC pulse value to motor PWM value. The reason for multiple pulse readers is due to different signal ranges coming from the receiever. This allows for correct movement
int pulseToPWMY(int pulse) {
  
  // If we're receiving numbers, convert them to motor PWM
  if ( pulse > 1000 ) {
    pulse = map(pulse, 1081, 1873, -500, 500);
    pulse = constrain(pulse, -255, 255);
  } else {
    pulse = 0;
  }

  // Anything in deadzone should stop the motor
  if ( abs(pulse) <= deadzone ) {
    pulse = 0;
  }

  return pulse;
}
int pulseToPWMX(int pulse) {
  
  // If we're receiving numbers, convert them to motor PWM
  if ( pulse > 1000 ) {
    pulse = map(pulse, 1107, 1899, -500, 500);
    pulse = constrain(pulse, -255, 255);
  } else {
    pulse = 0;
  }

  // Anything in deadzone should stop the motor
  if ( abs(pulse) <= deadzone ) {
    pulse = 0;
  }

  return pulse;
}
int pulseToPWMB(int pulse) {
  
  // If we're receiving numbers, convert them to motor PWM
  if ( pulse > 1000 ) {
    pulse = map(pulse, 1490, 1884, -500, 500);
    pulse = constrain(pulse, -255, 255);
  } else {
    pulse = 0;
  }

  // Anything in deadzone should stop the motor
  if ( abs(pulse) <= deadzone ) {
    pulse = 0;
  }

  return pulse;
}
