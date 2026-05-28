#include "helper.h"

void getUserJoysticks(int X1, int Y1, int X2, int Y2) {
  userCollection.roll     = ADS.readADC_SingleEnded(X1);
  userCollection.pitch    = ADS.readADC_SingleEnded(Y1);
  userCollection.throttle = ADS.readADC_SingleEnded(X2);
  userCollection.yaw      = ADS.readADC_SingleEnded(Y2);
}

void setFLAGS(int joystickbtn1, int joystickbtn2,int handmode, int alt_ctrl, int light_enable){
  if (!joystickbtn1) TOGGLE_BIT(userCollection.FLAGS,4);
  if (!joystickbtn2) TOGGLE_BIT(userCollection.FLAGS,5);
  if (!handmode) TOGGLE_BIT(userCollection.FLAGS,0);
  if (!alt_ctrl) TOGGLE_BIT(userCollection.FLAGS,2);
  if (!light_enable) TOGGLE_BIT(userCollection.FLAGS,1);
  
}