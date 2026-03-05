// install the SevSeg library from Arduino IDE → Tools → Manage Libraries → search "SevSeg" by Dean Reading
#include <SevSeg.h>

SevSeg sevseg;

// ---- Pomodoro Timer Settings ----
const int WORK_MINUTES        = 25;
const int SHORT_BREAK_MINUTES = 5;
const int LONG_BREAK_MINUTES  = 15;
const int ROUNDS_BEFORE_LONG  = 4;
// ---------------------------------

int minutes = WORK_MINUTES;
int seconds = 0;
unsigned long lastTick = 0;

bool isWorkSession = true;
int roundCount = 0;

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {12, 9, 8, 6};
  byte segmentPins[] = {11, 7, 4, 2, 1, 10, 5, 3};

  bool resistorsOnSegments = false;
  bool updateWithDelays = false;
  bool leadingZeros = true;
  bool disableDecPoint = false;

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins,
               resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);

  sevseg.setBrightness(90);
}

void loop() {
  unsigned long now = millis();

  if (now - lastTick >= 1000) {
    lastTick = now;

    if (seconds == 0 && minutes == 0) {
      if (isWorkSession) {
        roundCount++;
        isWorkSession = false;
        minutes = (roundCount % ROUNDS_BEFORE_LONG == 0) ? LONG_BREAK_MINUTES : SHORT_BREAK_MINUTES;
        seconds = 0;
      } else {
        isWorkSession = true;
        minutes = WORK_MINUTES;
        seconds = 0;
      }
    } else {
      if (seconds == 0) {
        minutes--;
        seconds = 59;
      } else {
        seconds--;
      }
    }
  }

  int displayNum = minutes * 100 + seconds;
  sevseg.setNumber(displayNum, 2);
  sevseg.refreshDisplay();
}