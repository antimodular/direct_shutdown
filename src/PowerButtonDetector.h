//
//  PowerButtonDetector.h
//  BootManager
//
//  Created by Akira on 4/7/15.
//
//
//https://gist.github.com/Akira-Hayasaka/863ff80c2d95c7ca1572

#ifndef BootManager_PowerButtonDetector_h
#define BootManager_PowerButtonDetector_h


#include "ofMain.h"
//#include "ofApp.h"


//static const string shutdonwScript = "osascript -e \'tell app \"System Events\" to shut down\'";

//https://ss64.com/osx/shutdown.html
//static const string shutdonwScript = "osascript -e 'do shell script \"sudo /sbin/shutdown -h now\"'";
static const string shutdonwScript = "osascript -e 'do shell script \"sudo /sbin/shutdown -h +1\"'";
static bool bButtonPressed;


static void onPwrBtnPressed(CFNotificationCenterRef center,
                            void *observer,
                            CFStringRef name,
                            const void *object,
                            CFDictionaryRef userInfo)
{
//    ofApp *app = (ofApp*)ofGetAppPtr();
//    //        app->exitOtherApps();
//    app->projectorsOFF();
    
            bButtonPressed = true;
    ofLog() << "Power Button Pressed";
//    ofSystem(shutdonwScript);
}




class PowerButtonDetector //: public ofThread
{
public:
 
    void setup()
    {
       
  
        state = INIT;
        bButtonPressed = false;
        threadedFunction();
//        startThread();
    }
    
    void exit()
    {
//        waitForThread();
    }
    
protected:
    

    
    void threadedFunction(){
        
//        while(isThreadRunning()){
            
        if (state == INIT){ //} && lock()) {
                CFNotificationCenterRef distCenter;
                CFStringRef evtName = CFSTR("com.apple.shutdownInitiated");
                distCenter = CFNotificationCenterGetDistributedCenter();
                if (NULL == distCenter){
                    ofLog()<<"A";
                    return 1;
                }
                CFNotificationCenterAddObserver(distCenter, NULL, &onPwrBtnPressed, evtName, NULL, CFNotificationSuspensionBehaviorDeliverImmediately);
                 ofLog()<<"B";
                CFRunLoopRun();
                state = RUN;
//                unlock();
            }
        
//        }//end while(isThreadRunning())
    }
    
private:
    
    enum STATE
    {
        INIT,
        RUN
    };
    STATE state;
};

#endif
