const int startPausePin = 2;
const int resetPin      = 3;
const int greenLED      = 4;
const int yellowLED     = 5;
const int redLED        = 6;
const int buzzerPin     = 7;

const long STUDY_TIME  = 5;
const long SHORT_BREAK = 3;
const long LONG_BREAK  = 5;

bool running   = false;
bool onStudy   = true;
int  pomRound  = 1;
long timeLeft  = STUDY_TIME;

unsigned long lastTick    = 0;
bool lastStartState       = HIGH;
bool lastResetState       = HIGH;

void setup() {
  pinMode(startPausePin, INPUT_PULLUP);
  pinMode(resetPin,      INPUT_PULLUP);
  pinMode(greenLED,      OUTPUT);
  pinMode(yellowLED,     OUTPUT);
  pinMode(redLED,        OUTPUT);
  pinMode(buzzerPin,     OUTPUT);
  digitalWrite(buzzerPin, LOW);
  Serial.begin(9600);
  updateLEDs();
  printStatus();
}

void loop() {
  handleButtons();
  if (!running) return;

  if (millis() - lastTick >= 1000) {
    lastTick = millis();
    timeLeft--;
    printStatus();

    if (timeLeft <= 0) {
      notify();
      advancePhase();
    }
  }
}

void handleButtons() {
  bool startState = digitalRead(startPausePin);
  bool resetState = digitalRead(resetPin);

  if (lastStartState == HIGH && startState == LOW) {
    delay(50);
    running = !running;
    Serial.println(running ? "Running" : "Paused");
  }
  lastStartState = startState;

  if (lastResetState == HIGH && resetState == LOW) {
    delay(50);
    resetTimer();
    Serial.println("Reset");
  }
  lastResetState = resetState;
}

void advancePhase() {
  if (onStudy) {
    onStudy  = false;
    timeLeft = (pomRound == 4) ? LONG_BREAK : SHORT_BREAK;
  } else {
    if (pomRound == 4) pomRound = 1;
    else pomRound++;
    onStudy  = true;
    timeLeft = STUDY_TIME;
  }
  updateLEDs();
  printStatus();
}

void notify() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  }
}

void updateLEDs() {
  allLEDsOff();
  if (onStudy) {
    digitalWrite(greenLED, HIGH);
  } else if (pomRound == 4) {
    digitalWrite(redLED, HIGH);
  } else {
    digitalWrite(yellowLED, HIGH);
  }
}

void allLEDsOff() {
  digitalWrite(greenLED,  LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED,    LOW);
}

void resetTimer() {
  pomRound = 1;
  onStudy  = true;
  timeLeft = STUDY_TIME;
  running  = false;
  updateLEDs();
  printStatus();
}

void printStatus() {
  int mins = timeLeft / 60;
  int secs = timeLeft % 60;
  String phase = onStudy ? "STUDY" : (pomRound == 4 ? "LONG BREAK" : "SHORT BREAK");
  Serial.print("Round "); Serial.print(pomRound);
  Serial.print(" | "); Serial.print(phase);
  Serial.print(" | ");
  if (mins < 10) Serial.print("0");
  Serial.print(mins); Serial.print(":");
  if (secs < 10) Serial.print("0");
  Serial.println(secs);
}
