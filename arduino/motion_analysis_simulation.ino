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

float lastDistance = -1.0;
unsigned long lastTime = 0;

int stableApproachCount = 0;
int stableRecedeCount = 0;

/* =====================================================
   DISTANCE MEASUREMENT
   -----------------------------------------------------
   Measures distance using the HC-SR04 ultrasonic sensor.
   Returns distance in centimeters.
   If no echo is detected, returns -1.
   ===================================================== */
float readRawDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000);

  if (duration == 0) return -1;

  float distance = duration * 0.0343 / 2.0;

  if (distance < 2 || distance > 300) return -1;

  return distance;
}

/* =========================
   Filtered distance reading
   ========================= */
float measureDistanceFiltered() {
  const int samples = 5;
  float sum = 0;
  int validCount = 0;

  for (int i = 0; i < samples; i++) {
    float d = readRawDistance();
    if (d > 0) {
      sum += d;
      validCount++;
    }
    delay(10);
  }

  if (validCount < 3) return -1;

  return sum / validCount;
}

/* =====================================================
  return sum / validCount;
   LED CONTROL
}
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
  for (int i = 0; i < level && i < 3; i++) {
    digitalWrite(blue[i], HIGH);
  }
}

void showReceding(int level) {
  clearLeds();
  for (int i = 0; i < level && i < 3; i++) {
    digitalWrite(red[i], HIGH);
  }
}

void showStopped() {
  clearLeds();
}

/* =====================================================
   LCD HELPER FUNCTION
   -----------------------------------------------------
   Prints a formatted floating point value with padding
 =====================================================*/

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
void playApproachBuzzer(float distance) {
  if (distance <= 0 || distance > 100) {
    noTone(buzzerPin);
    return;
  }

  int freq;
  int clickDuration = 25;

  if (distance > 70)
    freq = 300;
  else if (distance > 40)
    freq = 450;
  else if (distance > 20)
    freq = 650;
  else
    freq = 850;

  tone(buzzerPin, freq, clickDuration);
}

void playRecedeBuzzer(float distance) {
  if (distance <= 0 || distance > 100) {
    noTone(buzzerPin);
    return;
  }

  int freq;
  int clickDuration = 25;

  if (distance > 70)
    freq = 180;
  else if (distance > 40)
    freq = 220;
  else if (distance > 20)
    freq = 260;
  else
    freq = 320;

  tone(buzzerPin, freq, clickDuration);
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

  lastDistance = measureDistanceFiltered();
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
  float distance = measureDistanceFiltered();
  unsigned long now = millis();

  if (distance < 0) {
    clearLeds();
    noTone(buzzerPin);

    lcd.setCursor(0, 0);
    lcd.print("No signal       ");
    lcd.setCursor(0, 1);
    lcd.print("                ");

    stableApproachCount = 0;
    stableRecedeCount = 0;

    delay(100);
    return;
  }

  if (lastDistance < 0) {
    lastDistance = distance;
    lastTime = now;
    return;
  }

  float dt = (now - lastTime) / 1000.0;
  if (dt < 0.12) return;

  float delta = distance - lastDistance;


  if (abs(delta) > 20) {
    lastDistance = distance;
    lastTime = now;
    return;
  }

  if (abs(delta) < 1.0) {
    delta = 0;
  }

  float speed = delta / dt;      // cm/s
  float speedMs = speed / 100.0; // m/s

  String state = "STOP";
  int level = 0;

  if (speed < -3) {
    stableApproachCount++;
    stableRecedeCount = 0;
  }
  else if (speed > 3) {
    stableRecedeCount++;
    stableApproachCount = 0;
  }
  else {
    stableApproachCount = 0;
    stableRecedeCount = 0;
  }

  if (stableApproachCount >= 1) {
    state = "APPROACH";

    if (speed < -18)
      level = 3;
    else if (speed < -8)
      level = 2;
    else
      level = 1;

    showApproaching(level);
  }
  else if (stableRecedeCount >= 1) {
    state = "RECEDE";

    if (speed > 18)
      level = 3;
    else if (speed > 8)
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

  if (state == "APPROACH" && distance < 100) {
    playApproachBuzzer(distance);
  }
  else if (state == "RECEDE" && distance < 100) {
    playRecedeBuzzer(distance);
  }
  else {
    noTone(buzzerPin);
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Delta: ");
  Serial.print(delta);
  Serial.print(" cm | Speed: ");
  Serial.print(speed);
  Serial.print(" cm/s | State: ");
  Serial.print(state);
  Serial.print(" | A:");
  Serial.print(stableApproachCount);
  Serial.print(" | R:");
  Serial.println(stableRecedeCount);

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

  lastDistance = distance;
  lastTime = now;

  delay(70);
}
