#define ASIGNAL 2 //Pin 2 tied to GN
#define BSIGNAL 3 //Pin 3 tied to GY

int a, b;
long rotation = 0;  //the number of quater-1024ths of a rotation from starting position
int inertia   = 0;  //the previous direction of motion for cases where the direction of
                    //change since last tick is indeterminate.

int mod(int a, int b){
  /*Return A modulo B
  */
  int r = a%b;
  return r<0 ? r+b : r;
}

int sgn(int x){
  if(x==0){
    return 0;
  }else{
    return x>0?1:-1;
  }
}

int extract_rel_pos(int a, int b){
  if(a){
    if(!b){
      return 0;
    }else{
      return 1;
    }
  }else{
    if(b){
      return 2;
    }else{
      return 3;
    }
  }
}

int nearest_match(int rotation, int rel_pos, int inertia){
  /*Given the previous rotation and a new relative position,
   * return the best match for the change that occured to the
   * rotation (increment or decrement). 
   * If neither increment nor decrement matches, assume motion
   * continued in the direction inertia, in {1, -1, 0}.
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

void setup() {
  Serial.begin(9600);
  pinMode(ASIGNAL, INPUT);
  pinMode(BSIGNAL, INPUT);
}



void loop() {
  int i = 0;
  for(i=0;i<50;i++){
    int rel_pos, delta, i;
    a = digitalRead(ASIGNAL);
    b = digitalRead(BSIGNAL);
    rel_pos = extract_rel_pos(a,b);
    delta = nearest_match(rotation,rel_pos,inertia);
    inertia = sgn(delta);
    rotation += delta;
  }Serial.println(((double)(rotation))/1024);
}
