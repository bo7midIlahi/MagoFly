#include "helper.h"

void getUserJoysticks(int joystickbtn1, int joystickbtn2,int X1, int Y1, int X2, int Y2) {
  if (CHECK_BIT(userCollection.FLAGS,0))
  {
    userCollection.roll     = ADS.readADC_SingleEnded(X1);
    userCollection.pitch    = ADS.readADC_SingleEnded(Y1);
    //userCollection.throttle = ADS.readADC_SingleEnded(Y2);
    if (!CHECK_BIT(userCollection.FLAGS,4))
    {
      if ((ADS.readADC_SingleEnded(Y2)>30000) && (userCollection.throttle<100))
      {
        userCollection.throttle += 1;
      }
      if ((ADS.readADC_SingleEnded(Y2)<100) && (userCollection.throttle>0))
      {
        userCollection.throttle -= 1;
      }
    }
    userCollection.yaw      = ADS.readADC_SingleEnded(X2);
    if (!joystickbtn2) TOGGLE_BIT(userCollection.FLAGS,4);
    if (!joystickbtn1) TOGGLE_BIT(userCollection.FLAGS,5);
  }else
  {
    userCollection.roll     = ADS.readADC_SingleEnded(X2);
    userCollection.pitch    = ADS.readADC_SingleEnded(Y2);
    //userCollection.throttle = ADS.readADC_SingleEnded(Y1);
    if (!CHECK_BIT(userCollection.FLAGS,4))
    {
      if ((ADS.readADC_SingleEnded(Y1)>30000) && (userCollection.throttle<100))
      {
        userCollection.throttle += 1;
      }
      if ((ADS.readADC_SingleEnded(Y1)<100) && (userCollection.throttle>0))
      {
        userCollection.throttle -= 1;
      }
    }
    userCollection.yaw      = ADS.readADC_SingleEnded(X1);
    if (!joystickbtn1) TOGGLE_BIT(userCollection.FLAGS,4);
    if (!joystickbtn2) TOGGLE_BIT(userCollection.FLAGS,5);
  }
  
  
}

void setFLAGS(int handmode, int alt_ctrl, int light_enable,int engine_kill){

  if (!handmode) TOGGLE_BIT(userCollection.FLAGS,0);
  if (!alt_ctrl) TOGGLE_BIT(userCollection.FLAGS,2);
  if (!light_enable) TOGGLE_BIT(userCollection.FLAGS,1);
  engine_kill ? SET_BIT(userCollection.FLAGS,3) : CLEAR_BIT(userCollection.FLAGS,3);
  
}