// Radio Reciever pins
#define CH_1_PIN 10
#define CH_2_PIN 11

// Motor driver pins

#define DIR_1_PIN 4
#define PWM_1_PIN 5
#define DIR_2_PIN 7
#define PWM_2_PIN 6

#define deadzone 20
/*
  How the motor driver works: 
    1. Send a signal to the DIR pin in order to start it from Standby with a LOW/HIGH voltage (affects if it spins clockwise/counterclockwise)
    2. Use the PWM pin of each respective channel to initally set it to 0 so it doesn't move
*/
void startMotors(int pin1, int pwm1, int pin2, int pwm2);

void setup() {
  // Setting the mode for pins
  pinMode(CH_1_PIN, INPUT);
  pinMode(CH_2_PIN, INPUT);
  pinMode(DIR_1_PIN, OUTPUT);
  pinMode(PWM_1_PIN, OUTPUT);
  pinMode(DIR_2_PIN, OUTPUT);
  pinMode(PWM_2_PIN, OUTPUT);

  //Initalizing both motor channels
  startMotors(DIR_1_PIN, PWM_1_PIN, DIR_2_PIN, PWM_2_PIN);
}

void loop() {
  /*
    Procedure:
      1. Read signal from the radio controller
      2. Process signal into what direction bot has to go
      3. Control the motors to go in said direction.
  */

  //Read signal from reciever
  
  //When the signal from the channel switches form 0V to 5V, get the analog signal from it.
  int x = pulseIn(CH_1_PIN, HIGH);
  int y = pulseIn(CH_2_PIN, HIGH);
  //Convert the analog into PWM to get speed of each motor
  x = pulseToPWM(x);
  y = pulseToPWM(y);

  //Process signal to get direction
  
  //Mix signal for arcade drive
  int left = y + x;
  int right = y - x;
  /*
    Reasoning behind the math
    1. if you want to go purely forward/backward (in the y axis), both motors spin in the same direction.
    2. if you want to go purely left/right (in the x axis), both motors spin in opposite directions, thus one is +x and the other is -x.
    3. if you want to go diagonally forward to the left, you want to spin the left motor less than the right one, so that it isn't purely to the left and has some forward movement. 
      That's why the negative x value is added to the y value (shared by both left and right). And this will ensure the right motor's pwm is always higher than the left's.
    4. Same reasoning as above for diagonally right.
    5. Going diagonally left and right are reversed because why not... 
  */

  //Drive the motors
  drive(left, right);

  delay(10);
}

void startMotors(int pin1, int pwm1, int pin2, int pwm2){
  digitalWrite(pin1, LOW);
  analogWrite(pwm1, 0);
  digitalWrite(pin2, LOW);
  analogWrite(pwm2, 0);
}

// Positive for forward, negative for reverse
void drive(int speed_a, int speed_b) {
  //Motor A corresponds to left motor and Motor B corresponds to right motor.
  
  // Limit speed between -255 and 255 (PWM maxes out at 255)
  speed_a = constrain(speed_a, -255, 255);
  speed_b = constrain(speed_b, -255, 255);

  // Set direction for motor A
  if ( speed_a == 0 ) {
    //If the left motor isn't needed
    analogWrite(PWM_1_PIN, 0);
  } else if ( speed_a > 0 ) {
    //If it is spinning counterclockwise
    //PLEASE CHECK DIRECTION OF LOW/HIGH 
    digitalWrite(DIR_1_PIN, HIGH);
  } else {
    //If it is spinning clockwise
    digitalWrite(DIR_1_PIN, LOW);
  }

  if ( speed_b == 0 ) {
    //If the left motor isn't needed
    analogWrite(PWM_2_PIN, 0);
  } else if ( speed_b > 0 ) {
    //If it is spinning counterclockwise
    //PLEASE CHECK DIRECTION OF LOW/HIGH 
    digitalWrite(DIR_2_PIN, HIGH);
  } else {
    //If it is spinning clockwise
    digitalWrite(DIR_2_PIN, LOW);
  }

  //Set speed for both motors
  analogWrite(PWM_1_PIN, abs(speed_a));
  analogWrite(PWM_2_PIN, abs(speed_b));
}

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