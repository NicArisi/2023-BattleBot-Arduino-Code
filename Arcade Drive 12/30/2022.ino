const int CH_1_PIN = 10;
const int CH_2_PIN = 11;
const int BLADE_PIN = 9;

// Motor driver pins
//const int STBY_PIN = 9; //Motor driver's standyby pin
const int AIN1_PIN = 2; // Moves motor A forward
const int APWM_PIN = 5; // Motor A's speed.
const int BIN1_PIN = 7;
const int BPWM_PIN = 6;

const int BOutPin = 3;

// Parameters
const int deadzone = 28;  // Anything between -20 and 20 is stop
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
  int b = pulseIn(BLADE_PIN, HIGH);
  // Convert to PWM value (-255 to 255)
  y = pulseToPWM(y);
  x = pulseToPWM(x);
  if ((pulseIn(CH_2_PIN, HIGH) > 2000 || pulseIn(CH_2_PIN, HIGH) < 1000) || (pulseIn(CH_1_PIN, HIGH) > 2000 || pulseIn(CH_1_PIN, HIGH) < 1000) || (pulseIn(BLADE_PIN, HIGH) > 2000 || pulseIn(BLADE_PIN, HIGH) < 1000)) {
    y = 0;
    x = 0;
    b = 0;
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
  // Mix for arcade drive
  int left = y + x;
  int right = x - y;
  bladeDrive(b);
  // Drive motor
  
  drive(left, right);
  delay(25);
}

void bladeDrive(int b) {
  if (b > 1700) {
    digitalWrite(BOutPin, HIGH);
  } else {
    digitalWrite(BOutPin, LOW);
  }
}
// Positive for forward, negative for reverse
void drive(int speed_a, int speed_b) {
  
  // Limit speed between -255 and 255
  speed_a = constrain(speed_a, -255, 255);
  speed_b = constrain(speed_b, -255, 255);
  
  // Set direction for motor A
  if ( speed_a > 0 ) { // Moving right
    digitalWrite(AIN1_PIN, HIGH);
  } else {// Moving left 
    digitalWrite(AIN1_PIN, LOW);
  }

  // Set direction for motor B
  if ( speed_b > 0 ) { // Moving forward
    digitalWrite(BIN1_PIN, HIGH);
  } else { //Moving backwards
    digitalWrite(BIN1_PIN, LOW);
  }

  // Set speed
  analogWrite(APWM_PIN, abs(speed_a));
  analogWrite(BPWM_PIN, abs(speed_b));
}

// Convert RC pulse value to motor PWM value
int pulseToPWM(int pulse) {
  
  // If we're receiving numbers, convert them to motor PWM
  if ( pulse > 1000 ) {
    pulse = map(pulse, 1000, 2000, -500, 500);
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
