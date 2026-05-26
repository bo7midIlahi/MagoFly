#include "helper.h"

void getUserJoysticks(int X1, int Y1, int X2, int Y2) {
  userCollection.roll     = ADS.readADC_SingleEnded(X1);
  userCollection.pitch    = ADS.readADC_SingleEnded(Y1);
  userCollection.throttle = ADS.readADC_SingleEnded(X2);
  userCollection.yaw      = ADS.readADC_SingleEnded(Y2);
}