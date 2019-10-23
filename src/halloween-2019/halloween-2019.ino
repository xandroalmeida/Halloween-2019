#include <TaskScheduler.h>
#include <Chrono.h>


void scareLoop(void);

Scheduler runner;
Task scareTask(2, TASK_FOREVER, &scareLoop);


void scareLoop()  {
  static Chrono _lightChrono;
  static Chrono _waitChrono;
  

  static unsigned short _state = 0; //Estado da Task
  static unsigned long  _wtime = 0; //Tempo de espera
  static unsigned long  _ltime = 0; //Tempo de flash da luz

  switch(_state) {
    case 0: //Inicia o temporizador de ficar quieto
      _waitChrono.restart();
      _wtime = random(1000, 30000);
      _state++;
      break;
    case 1: //Testa se o tempo de ficar quite acabou
      if (_waitChrono.hasPassed(_wtime)) {
        _waitChrono.restart();
        _wtime = random(1000, 5000);
        _lightChrono.restart();
        _ltime = 0;
        _state++;
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
      }
      
      break;
    default:
      _state = 0;
    break;
     
  }
  /*
  unsigned int blinkTime = random(50,100);
  digitalWrite(3, LOW);
  while(!noiseChrono.hasPassed(ntime))  {  // change "500" for different durations in ms.
    tone(2, random(300, 1300));   // change the parameters of random() for different sound
    if(lightChrono.hasPassed(blinkTime)) {
      lightChrono.restart();
      blinkTime = random(50,100);
      digitalWrite(3, !digitalRead(3));
    }
  }
  noTone(2);
  digitalWrite(3, HIGH);
  */
}


void setup() {
  randomSeed(analogRead(0));
  //runner.init();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);

  runner.init();
  runner.addTask(scareTask);
  scareTask.enable();
  
}

void loop() {
  runner.execute();
}
