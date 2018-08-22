#ifndef Dispatch_h
#define Dispatch_h

#include "Arduino.h"
#include "Queue.h"
#include "RelayController.h"
//#include "Timer.h"


    // setup up PIN mode
   // void setupPinMode(void);
  // dispatch data from Q to different controlers
    void taskDispatch(void * parameter);
  
#endif