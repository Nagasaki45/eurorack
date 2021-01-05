#include <uClock.h>
#define ANALOG_SYNC_RATIO 12  // From 24 to 2 PPQN

#define MIDI_CLOCK_START 0xFA
#define MIDI_CLOCK_STOP 0xFC
#define MIDI_CLOCK_TICK 0xF8

#define SOFT_VCC_PIN 6
#define MIDI_RUNNING_PIN 10
#define TEMPO_PIN A0
#define CLOCK_PIN 3

bool isMidiRunning = false;
bool isMidiWaiting = false;

void clockOutput96PPQN(uint32_t* tick) {
  if ((*tick % ANALOG_SYNC_RATIO ) == 0) {
    digitalWrite(CLOCK_PIN, HIGH);
    if (isMidiWaiting) {
      Serial.write(MIDI_CLOCK_START);
      isMidiWaiting = false;
      isMidiRunning = true;
    }
  } else {
    digitalWrite(CLOCK_PIN, LOW);
  }
  if (isMidiRunning) {
    Serial.write(MIDI_CLOCK_TICK);
  }
}

void updateTempo() {
  int tempoPot = analogRead(TEMPO_PIN);
  float tempo = (tempoPot / 1024.0) * 210.0 + 30.0;
  uClock.setTempo(tempo);  
}

void stopMidi() {
  Serial.write(MIDI_CLOCK_STOP);
  isMidiRunning = false;
  isMidiWaiting = false;
}

void updateMidiRunning() {
  int midiRunningSwitchState = digitalRead(MIDI_RUNNING_PIN);
  if (midiRunningSwitchState != isMidiRunning) {
    if (isMidiRunning) {
      stopMidi();
    } else {
      isMidiWaiting = true;
    }
  }
}

void setup() {
  Serial.begin(31250);

  pinMode(SOFT_VCC_PIN, OUTPUT);
  digitalWrite(SOFT_VCC_PIN, HIGH);

  pinMode(MIDI_RUNNING_PIN, INPUT_PULLUP);
  pinMode(CLOCK_PIN, OUTPUT);
  
  uClock.init();
  uClock.setClock96PPQNOutput(clockOutput96PPQN);
  uClock.start();
}

void loop() {
  updateTempo();
  updateMidiRunning();
}
