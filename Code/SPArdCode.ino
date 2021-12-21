#include <arduino.h>
#include <Servo.h>

#define KEY_RANGE 14

//Pin Information
#define SOL_PINS 2
#define W_SOL 0
#define B_SOL 1
#define SERVO 9
#define BUTTON 4

#define MS_PER_KEY_RIGHT 280
#define MS_PER_KEY_LEFT 240

// File root;
Servo servo;
// const uint8_t CS = 10;

//Song manual is formatted {note, note, ..., 255}
#define MAN_QUARTER_BEAT_MS 250  //How many ms in one quarter beat
#define MAN_NOTE_HOLD_QB 1
#define MAN_NOTE_DELAY_QB 1

int8_t song_man[] = {2, 1, 0, 1, 2, 2, 2, -1,
                      1, 1, 1, -1, 2, 2, 2, -1,
                      2, 1, 0, 1, 2, 2, 2,
                      2, 1, 1, 2, 1, 0, 127}; //Mary had a little Lamb

const uint8_t sol_pins[] = {2, 3};
uint8_t curr_idx = 0;
volatile uint8_t playF = 0;
uint8_t testF = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  // timer_init();
  servo.attach(SERVO);
  servo.write(90);
  for (int i = 0; i < SOL_PINS; i++)
    pinMode(sol_pins[i], OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

// power is a number from 0 to 9 that indicates power or speed
// negative numbers reverse the direction.
void turn(int8_t power) {
  uint8_t num = 90 + (10 * (power % 10));
  servo.write(num);
}

int getTurnTime(uint8_t target_i) {
  int dif = target_i - curr_idx;
  if (dif > 0)
    return dif * MS_PER_KEY_RIGHT;
  else
    return dif * MS_PER_KEY_LEFT;
}

//Move servo to note location and activate solenoid if white key
void playNote(uint8_t note, uint8_t var, int delay_ms) {
  uint8_t idx = note;
  int turn_time;

  if (idx >= 0 && idx < KEY_RANGE)
  {
    //Move to Note
    if (curr_idx != idx)
    {
      turn_time = getTurnTime(idx);
      if (turn_time > 0)
        turn(4);
      else
        turn(-4);
      turn_time = abs(turn_time);
      delay(turn_time);
      turn(0);
      if (delay_ms > turn_time)
        delay_ms -= turn_time;
    }
    delay(delay_ms);
    curr_idx = idx;
    digitalWrite(sol_pins[W_SOL], var);
  }
  else
    delay(delay_ms);
}

void test(int f)
{
  if (f)
  {//Right
    turn(4);
    delay(280);
    turn(0);    
  }
  else
  {//Left
    turn(-4);
    delay(240);
    turn(0); 
  }
}

void playSong_man() {
  int8_t *ptr = song_man;
  int delay_ms = MAN_NOTE_DELAY_QB * MAN_QUARTER_BEAT_MS;
  int hold_ms = MAN_NOTE_HOLD_QB * MAN_QUARTER_BEAT_MS;

  while (*ptr != 127)
  {
    playNote(*ptr, HIGH, delay_ms);
    delay(hold_ms);
    playNote(*ptr, LOW, 0);
    ptr++;
  }
}

void loop() {
  if (digitalRead(BUTTON) == LOW)
  {
    playSong_man();
    while (digitalRead(BUTTON) == LOW);
  }
}
