#include <LiquidCrystal.h>

/* =====================================================
   HARDWARE CONFIGURATION
   -----------------------------------------------------
   Pin assignments for sensors, LEDs, and buzzer
   ===================================================== */

int trigPin = 9;
int echoPin = 10;
int buzzerPin = 8;

/* LED arrays
   Red LEDs indicate receding motion
   Blue LEDs indicate approaching motion */

int red[3]  = {2, 3, 4};
int blue[3] = {5, 6, 7};

/* LCD configuration
   RS, E, DB4, DB5, DB6, DB7 */

LiquidCrystal lcd(12, 11, A0, A1, A2, A3);


/* =====================================================
   GLOBAL STATE VARIABLES
   -----------------------------------------------------
   Used for velocity estimation between measurements
   ===================================================== */

float lastDistance = 0.0;
unsigned long lastTime = 0;


/* =====================================================
   DISTANCE MEASUREMENT
   -----------------------------------------------------
   Measures distance using the HC-SR04 ultrasonic sensor.
   Returns distance in centimeters.
   If no echo is detected, returns -1.
   ===================================================== */

float measureDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0)
    return -1;

  return duration * 0.0343 / 2.0;
}


/* =====================================================
   LED CONTROL
   -----------------------------------------------------
   Visualizes motion direction and intensity
   ===================================================== */

void clearLeds() {

  for (int i = 0; i < 3; i++) {
    digitalWrite(red[i], LOW);
    digitalWrite(blue[i], LOW);
  }
}

void showApproaching(int level) {

  clearLeds();

  for (int i = 0; i < level; i++)
    digitalWrite(blue[i], HIGH);
}

void showReceding(int level) {

  clearLeds();

  for (int i = 0; i < level; i++)
    digitalWrite(red[i], HIGH);
}

void showStopped() {

  clearLeds();
}


/* =====================================================
   LCD HELPER FUNCTION
   -----------------------------------------------------
   Prints a formatted floating point value with padding
   ===================================================== */

void printPaddedFloat(float value, int decimals, int width) {

  char buffer[17];
  dtostrf(value, width, decimals, buffer);
  lcd.print(buffer);
}


/* =====================================================
   BUZZER CONTROL
   -----------------------------------------------------
   Generates short sonar-like clicks based on distance.
   Click frequency increases as the object gets closer.
   Inspired by bat echolocation behaviour.
   ===================================================== */

void updateBuzzer(float distance) {

  if (distance <= 0) {
    noTone(buzzerPin);
    return;
  }

  int freq;
  int clickDuration = 12;

  if (distance > 150)
    freq = 120;
  else if (distance > 100)
    freq = 200;
  else if (distance > 60)
    freq = 300;
  else if (distance > 30)
    freq = 420;
  else
    freq = 550;

  tone(buzzerPin, freq, clickDuration);

  delay(5);
}


/* =====================================================
   SYSTEM INITIALIZATION
   ===================================================== */

void setup() {

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  for (int i = 0; i < 3; i++) {

    pinMode(red[i], OUTPUT);
    pinMode(blue[i], OUTPUT);
  }

  lcd.begin(16, 2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Doppler Demo");

  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  clearLeds();
  noTone(buzzerPin);

  delay(1000);

  lastDistance = measureDistance();

  if (lastDistance < 0)
    lastDistance = 0;

  lastTime = millis();

  lcd.clear();
}


/* =====================================================
   MAIN CONTROL LOOP
   -----------------------------------------------------
   1. Measure distance
   2. Estimate velocity
   3. Determine motion direction
   4. Update LEDs, buzzer, and LCD display
   ===================================================== */

void loop() {

  float distance = measureDistance();
  unsigned long now = millis();


  /* ----- No signal handling ----- */

  if (distance < 0) {

    clearLeds();
    noTone(buzzerPin);

    lcd.setCursor(0, 0);
    lcd.print("No signal       ");

    lcd.setCursor(0, 1);
    lcd.print("                ");

    delay(100);
    return;
  }


  /* ----- Velocity estimation ----- */

  float dt = (now - lastTime) / 1000.0;

  if (dt <= 0.05)
    return;

  float delta = distance - lastDistance;   // cm
  float speed = delta / dt;                // cm/s
  float speedMs = speed / 100.0;           // m/s


  /* ----- Motion classification ----- */

  String state = "STOP";
  int level = 0;

  if (speed < -1) {

    state = "APPROACH";

    if (speed < -15)
      level = 3;
    else if (speed < -6)
      level = 2;
    else
      level = 1;

    showApproaching(level);
  }

  else if (speed > 1) {

    state = "RECEDE";

    if (speed > 15)
      level = 3;
    else if (speed > 6)
      level = 2;
    else
      level = 1;

    showReceding(level);
  }

  else {

    state = "STOP";
    level = 0;

    showStopped();
  }


  /* ----- Acoustic feedback ----- */

  updateBuzzer(distance);


  /* ----- Serial debug output ----- */

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Delta: ");
  Serial.print(delta);
  Serial.print(" cm | Speed: ");
  Serial.print(speed);
  Serial.print(" cm/s | State: ");
  Serial.println(state);


  /* ----- LCD display update ----- */

  lcd.setCursor(0, 0);

  lcd.print("D:");
  printPaddedFloat(distance, 1, 5);
  lcd.print("cm ");

  if (state == "STOP")
    lcd.print("STOP  ");
  else if (state == "APPROACH")
    lcd.print("APRCH ");
  else
    lcd.print("RCEDE ");


  lcd.setCursor(0, 1);

  lcd.print("v:");
  printPaddedFloat(speedMs, 2, 6);
  lcd.print("m/s ");

  lcd.print("L");
  lcd.print(level);
  lcd.print(" ");


  /* ----- Update history ----- */

  lastDistance = distance;
  lastTime = now;

  delay(120);
}
