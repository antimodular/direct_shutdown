#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "PowerButtonDetector.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void exit();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofParameter<bool> bButtonPressed;
    bool old_bButtonPressed;
       
    //
    PowerButtonDetector button_object;
    string version;
    
    //----stages
    bool bShutdownStarted;
    float stageTimer;
    float stageDuration;
    int stage;
    
    bool showGUI;
    
    //visuals
    int bgColor;
    int textColor;
    int red, green, blue;
    
    //other apps
    void exitOtherApps();
    vector<string> otherAppNames;
    
    //serial communication to arduino to control projector functions
    int myBaud;
    string serialID;
    
    ofSerial    serial;
    
    void projectorPrintInfo();
    
    void projectorsON();
    void projectorsOFF();
    
    void projectorsEcoOn();
    void projectorsEcoOff();
    
    void projector1_HDMI1();
    void projector1_HDMI2();
    void projector2_HDMI1();
    void projector2_HDMI2();
    
    void getSerialDevice();
    
    
};
