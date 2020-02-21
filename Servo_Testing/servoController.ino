

void moveServos(int cameraAngle, int rearAngle){
  camServo.write(cameraAngle);
  rearServo.write(rearAngle);
}
