void Stop() {
  motorRight.run(RELEASE);
  motorLeft.run(RELEASE);
}

void Forward(byte speed) {
  motorRight.run(FORWARD);
  motorLeft.run(FORWARD);
  motorLeft.setSpeed(speed);
  motorRight.setSpeed(speed);
}
 
void AntiClockwise(byte speed) {
  motorRight.run(FORWARD);
  motorLeft.run(BACKWARD);    
  motorLeft.setSpeed(speed);
  motorRight.setSpeed(speed);
}
 
void Clockwise(byte speed) {
  motorRight.run(BACKWARD);
  motorLeft.run(FORWARD);
  motorLeft.setSpeed(speed);
  motorRight.setSpeed(speed);
}

void Backward() {
  motorRight.run(BACKWARD);
  motorLeft.run(BACKWARD);
  motorLeft.setSpeed(speedMax);
  motorRight.setSpeed(speedMax);
} 

void Impulse() {  
  Forward(speedMax);
  delay(100);
  Backward();
  delay(100);
}

void Wait5seconds() {
  delay(3500);  
}
