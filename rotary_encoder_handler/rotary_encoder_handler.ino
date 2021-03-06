#include "digitalWriteFast.h"
#define ASIGNAL 2 //Pin 2 tied to GN
#define BSIGNAL 3 //Pin 3 tied to GY

volatile int a, b;

unsigned short starting_rot;       //The initial rotation.
volatile long rotation = 0;                 //The current rotation, in 1024ths of revolutions (pi/512 radians).
volatile int inertia   = 0;                 //The previous direction of rotations for cases where the direction of
                                   //change since last tick is indeterminate.



int mod(int a, int b){
  /*Return A modulo B
  */
  int r = a%b;
  return r<0 ? r+b : r;
}

int sgn(int x){
  /* returns the sign of x
   */
  if(x==0){
    return 0;
  }else{
    return x>0?1:-1;
  }
}

int extract_rel_pos(int a, int b){
  a=!!a;
  b=!!b;
  switch((a<<1) + b){
    case 0b10: return 0; // rising edge of A wave
    case 0b11: return 1; // rising edge of B wave
    case 0b01: return 2; //falling edge of A wave
    case 0b00: return 3; //falling edge of B wave
  }
  return 0;
}

int nearest_match(int rotation, int rel_pos, int inertia){
  /*Given the previous rotation and a new relative position,
   * return the best match for the change that occured to the
   * rotation (increment or decrement). 
   * If neither increment nor decrement matches, assume motion
   * continued in the direction inertia (should be 1, -1, or 0).
   */
  int prev_rel_rot = mod(rotation,4);
  if (prev_rel_rot==rel_pos){
    return 0; 
  }else if(rel_pos == mod(prev_rel_rot + 1, 4)){
      return 1;
  }else if(rel_pos == mod(prev_rel_rot - 1,4)){
    return -1;
  }else{
    return 2*inertia;
  }
}


void update_rotation(void){
  /* This is an ISR used for an interrupt, so it must be
   *  void and accept no arguments, forcing the use of
   *  some globals.
   */
    a = digitalReadFast(ASIGNAL);
    b = digitalReadFast(BSIGNAL);
    int rel_pos = extract_rel_pos(a,b);
    int delta = nearest_match(rotation,rel_pos,inertia);
    inertia = sgn(delta);
    rotation += delta;
}


void setup() {
  Serial.begin(9600);
  pinModeFast(ASIGNAL, INPUT);
  pinModeFast(BSIGNAL, INPUT);
  rotation = starting_rot = extract_rel_pos(a,b);
  attachInterrupt(digitalPinToInterrupt(ASIGNAL),update_rotation,CHANGE);
  attachInterrupt(digitalPinToInterrupt(ASIGNAL),update_rotation,CHANGE);
}



void loop() {
  delay(50);
  Serial.println(((float)(rotation - starting_rot))/1024/4);
}
