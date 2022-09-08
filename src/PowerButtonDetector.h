//
//  PowerButtonDetector.h
//  BootManager
//
//  Created by Akira on 4/7/15.
//  modified by Stephan on 7/1/2019
//
//https://gist.github.com/Akira-Hayasaka/863ff80c2d95c7ca1572

#ifndef BootManager_PowerButtonDetector_h
#define BootManager_PowerButtonDetector_h


#include "ofMain.h"

//static const string shutdonwScript = "osascript -e \'tell app \"System Events\" to shut down\'";

//https://ss64.com/osx/shutdown.html
//static const string shutdonwScript = "osascript -e 'do shell script \"sudo /sbin/shutdown -h now\"'";
static const string shutdonwScript = "osascript -e 'do shell script \"sudo /sbin/shutdown -h now\" password \"hemmer\" with administrator privileges'";
//static const string shutdonwScript = "do shell script "shutdown -h now" with administrator privileges username "<youradminusername>" password "<youradminpassword>"
//static const string shutdonwScript = "osascript -e 'do shell script \"sudo /sbin/shutdown -h +1\"'";


static bool bButtonPressed;

static void onPwrBtnPressed(CFNotificationCenterRef center,
                            void *observer,
                            CFStringRef name,
                            const void *object,
                            CFDictionaryRef userInfo)
{
    
            bButtonPressed = true;
    ofLog() << "Power Button Pressed";
//    ofSystem(shutdonwScript);
}




class PowerButtonDetector: public ofThread
{
    
    
private:
    
    enum STATE
    {
        INIT,
        RUN
    };
    STATE state;
    
public:
 
    ~PowerButtonDetector(){
        stop();
        waitForThread(false);
    }
    
    void setup()
    {
        state = INIT;
        bButtonPressed = false;
        threadedFunction();
        start();
    }
    
    /// Start the thread.
    void start(){
        startThread();
    }
    
    void stop(){
        std::unique_lock<std::mutex> lck(mutex);
        stopThread();
        condition.notify_all();
    }
    
    int getThreadFrameNum(){
        return threadFrameNum;
    }
    
    bool getButtonPressed(){
        return threaded_bButtonPressed;
    }
    void update(){
        // if we didn't lock here we would see
        // tearing as the thread would be updating
        // the pixels while we upload them to the texture
        std::unique_lock<std::mutex> lock(mutex);
//        tex.loadData(pixels);
        condition.notify_all();
    }
    
    void threadedFunction(){
        
        while(isThreadRunning()){
            threadFrameNum++;
            threaded_bButtonPressed = bButtonPressed;
            
            std::unique_lock<std::mutex> lock(mutex);
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
            condition.wait(lock);
        }//end while(isThreadRunning())
    }
    
protected:
    
    std::condition_variable condition;
    int threadFrameNum = 0;
    bool threaded_bButtonPressed;
    
    
};

#endif
