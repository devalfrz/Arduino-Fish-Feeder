/*
  Automating fish feeder
  2017/06/19
  Alfredo Rius
  alfredo.rius@gmail.com
*/

#include <Servo.h>

#define FEED_PERIOD (unsigned long)12*60*60 //Every 12 hours
#define FEED_INITIAL_PERIOD 10 //10 Seconds to avoid hot start

#define SERVO_MAX 85
#define SERVO_MIN 65

#define SERVO 6

#define BUTTON 3


Servo servo;
uint8_t feed_state = 0;
unsigned long time_elapsed;
unsigned long time_left;

uint8_t led_counter;
uint8_t tmp;

void setup() {
  
  // Start Servo
  servo.attach(SERVO);
  // Set servo in a defined location
  servo.write(SERVO_MIN);
  
  // Button
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON), set_feed_state, FALLING);

  // Time indicator
  pinMode(LED_BUILTIN, OUTPUT);

  // Initial period
  reset_timer(FEED_INITIAL_PERIOD);
}

void loop() {
  if(feed_state){
    feed(2,10);
  }
  time_elapsed = time_left - millis();
  if(time_elapsed > FEED_PERIOD*(unsigned long)1000){
    feed_state = 1;
  }
  delay(50);
  
  // Just a simple time indicator, will blink faster when
  // feed time is near
  tmp = time_elapsed/(60*60)/1000;
  if(led_counter >= tmp*20){
    led_counter = 0;
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  }
  led_counter ++;
}

void set_feed_state(){
  feed_state = 1;
}

void feed(int times, int d){
  int pos,i;
  
  // Reset everything
  feed_state = 0;
  reset_timer(FEED_PERIOD);
  led_counter = 0;

  // Feed fish
  for(i=0;i<times;i++){
    for (pos = SERVO_MIN; pos <= SERVO_MAX; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(d);                       // waits 15ms for the servo to reach the position
    }
    for (pos = SERVO_MAX; pos >= SERVO_MIN; pos -= 1) { // goes from 180 degrees to 0 degrees
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(d);                       // waits 15ms for the servo to reach the position
    }
  }
}


void reset_timer(long seconds){
  time_left = millis()+(seconds*1000);
}


