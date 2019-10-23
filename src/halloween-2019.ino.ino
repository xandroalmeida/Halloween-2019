#include <Chrono.h>



void noise(unsigned int ntime)  {
  Chrono noiseChrono;
  Chrono lightChrono;
  
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
}

void scare() {
  noise(random(2000, 5000));
  
}


void setup() {
  randomSeed(analogRead(0));
  //runner.init();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
}

void loop() {
  scare();  
  delay(random(10000,30000));
}
