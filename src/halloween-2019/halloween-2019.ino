#include <TaskScheduler.h>
#include <Chrono.h>
#include "pitches.h"


void scareLoop(void);
void thereminLoop(void);

Scheduler runner;
Task scareTask(2, TASK_FOREVER, &scareLoop);
Task thereminTask(12, TASK_FOREVER, &thereminLoop);

boolean thereminOn = true;

void thereminLoop(void) {
  static Chrono _noteChrono;
  static Chrono _onOffChrono;
  static boolean _onOff = false;
  static unsigned long _onOffTime = 0;
  
  static unsigned int melody[] = {NOTE_CS6,NOTE_FS5,NOTE_FS5,NOTE_CS6,NOTE_FS5,NOTE_FS5,NOTE_CS6,NOTE_D5,NOTE_FS5,
               NOTE_CS6,NOTE_FS5,NOTE_FS5,NOTE_CS6,NOTE_FS5,NOTE_FS5,NOTE_CS6,NOTE_D5,NOTE_FS5,
               NOTE_C6,NOTE_F5,NOTE_F5,NOTE_C6,NOTE_F5,NOTE_F5,NOTE_C6,NOTE_F5,NOTE_CS5,NOTE_F5,
               NOTE_C6,NOTE_F5,NOTE_F5,NOTE_C6,NOTE_F5,NOTE_F5,NOTE_C6,NOTE_F5,NOTE_CS5,NOTE_F5,
               NOTE_B5,NOTE_E5,NOTE_E5,NOTE_B5,NOTE_E5,NOTE_E5,NOTE_B5,NOTE_E5,NOTE_C6,NOTE_E5,
               NOTE_B5,NOTE_E5,NOTE_E5,NOTE_B5,NOTE_E5,NOTE_E5,NOTE_B5,NOTE_E5,NOTE_C6,NOTE_E5};
  static unsigned short noteDurations[] = {4,4,4,4,4,4,4,4,4,
                      4,4,4,4,4,4,4,4,4,
                      4,4,4,4,4,4,4,4,4,4,
                      4,4,4,4,4,4,4,4,4,4,
                      4,4,4,4,4,4,4,4,4,4,
                      4,4,4,4,4,4,4,4,4,4};
  static unsigned short note = 0;

  static unsigned int vibrato[] = {0,4,8,11,13,15,15,15,13,11,8,4};
  static unsigned short vibratoLen = 12;
  static unsigned short x = 0;

  static int noteDuration = 0;
  static boolean playNote = true;

  if (_onOffChrono.hasPassed(_onOffTime)) {
    _onOffChrono.restart();
    _onOff = !_onOff;
    if (_onOff) {
      _onOffTime = random(10000, 15000);
    } else {
      _onOffTime = random(30000, 90000);
    }
  }

  if (!_onOff) {
    noTone(4);
    return;
  }

  if (thereminOn && playNote && melody[note]) {
    //tone(4, melody[note]);
    tone(4, melody[note]+vibrato[x++]*(melody[note]/300));
    if (x == 12) {
      x = 0;
    }
  }

  if (_noteChrono.hasPassed(noteDuration)) {
    _noteChrono.restart();
    if (playNote) {
      noTone(4);
      playNote = false;
      noteDuration *= 0.05;
    } else {
      note++;
      if (note == 58) {
        note = 0;
      }
      noteDuration = 1200 / noteDurations[note];
      playNote = true;
      x = 0;
    }
  } 
}

void scareLoop()  {
  static Chrono _lightChrono;
  static Chrono _waitChrono;
  
  static unsigned short _state = 0; //Estado da Task
  static unsigned long  _wtime = 0; //Tempo de espera
  static unsigned long  _ltime = 0; //Tempo de flash da luz

  switch(_state) {
    case 0: //Inicia o temporizador de ficar quieto
      _waitChrono.restart();
      _wtime = random(15000, 90000);
      _state++;
      break;
    case 1: //Testa se o tempo de ficar quite acabou
      if (_waitChrono.hasPassed(_wtime)) {
        _waitChrono.restart();
        _wtime = random(1000, 3000);
        _lightChrono.restart();
        _ltime = 0;
        _state++;
        thereminOn=false;
        noTone(4);
        
      }
      break;
    case 2:
      tone(2, random(300, 1300));
      if (_ltime == 0) {
        _ltime = random(50,100);
        digitalWrite(3, !digitalRead(3));
      }

      if (_lightChrono.hasPassed(_ltime)) {
        digitalWrite(3, !digitalRead(3));
        _lightChrono.restart();
        _ltime = random(50,100);
      }
      
      if (_waitChrono.hasPassed(_wtime)) {
        _state = 0;
        noTone(2);
        digitalWrite(3, HIGH);
        thereminOn=true;
      }
      
      break;
    default:
      _state = 0;
    break;
     
  }
}

void setup() {
  randomSeed(analogRead(0));

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);

  runner.init();
  runner.addTask(scareTask);
  scareTask.enable();
  runner.addTask(thereminTask);
  thereminTask.enable();
  
}

void loop() {
  runner.execute();
}
