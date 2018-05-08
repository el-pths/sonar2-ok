#define SEGMENTS 7
#define DIGITS 3

int segments[7] = {10, 4, 8, 6, 7, 11, 5};
int digits[3] = {9, 12, 13};

int soundCounter, soundPattern;

int patterns[] = {
  0b0111111, 0b0000110,
  0b1011011, 0b1001111,
  0b1100110, 0b1101101,
  0b1111101, 0b0000111,
  0b1111111, 0b1101111
};

void setup() {
  for (int p = 0; p < 7; p++) {
    pinMode(segments[p], OUTPUT);
    digitalWrite(segments[p], HIGH);
  }
  for (int d = 0; d < 3; d++) {
    pinMode(digits[d], OUTPUT);
    digitalWrite(digits[d], LOW);
  }
  pinMode(A1, OUTPUT);
  soundPattern = 0;
}

void loop() {
  int centimeters = measureDistance();
  prepareSound(centimeters);
  for (int i = 0; i < 130; i++) {
    showValueFor10ms(centimeters);
    processSound();
  }
}

int measureDistance() {
  long t;
  digitalWrite(A1, HIGH);
  delayMicroseconds(20);
  digitalWrite(A1, LOW);
  while (digitalRead(A0) == LOW) {
    // just wait for rising edge
  }
  t = micros();
  while (digitalRead(A0) == HIGH) {
    // just wait for falling edge
  }
  t = micros() - t;
  return t / 60;
}

void showValueFor10ms(int value) {
  for (int pos = 2; pos >= 0; pos--) {
    showDigit(value % 10, pos, 3);
    value /= 10;
  }
}

void showDigit(int value, int pos, int ms) {
  int p = patterns[value];
  for (int i = 0; i < SEGMENTS; i++) {
    digitalWrite(segments[i], (p & 1) ? LOW : HIGH);
    p >>= 1;
  }
  digitalWrite(digits[pos], HIGH);
  delay(ms);
  digitalWrite(digits[pos], LOW);
  for (int i = 0; i < SEGMENTS; i++) {
    digitalWrite(segments[i], HIGH);
  }
}

void prepareSound(int cm) {
  if (cm < 30) {
    soundPattern = 0b101010;
  } else if (cm < 70) {
    soundPattern = 0b110110;
  } else if (cm < 110) {
    soundPattern = 0b11110;
  } else {
    soundPattern = 0;
  }
  soundCounter = 0;
}

void processSound() {
  if (soundPattern == 0) {
    noTone(3);
    return;
  }
  if (soundCounter == 0) {
    soundCounter = 10;
    soundPattern >>= 1;
  }
  if (soundPattern & 1) {
    tone(3, 440);
  } else {
    noTone(3);
  }
  soundCounter -= 1;
}
