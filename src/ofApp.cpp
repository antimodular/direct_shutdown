#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    version = "direct_shutdown v17";
    ofLog()<<"version "<<version;
    ofLog()<<"remember to disable the sleep option via terminal script";
    ofLog()<<"defaults write com.apple.loginwindow PowerButtonSleepsSystem -bool no ";
    ofLog()<<"";
    
    button_object.setup();
    
    //---------visuals
    ofSetCircleResolution(50);
    //ofEnableSmoothing();
    
    textColor = 200;
    
    red = 64;
    green = 0;
    blue = 26;
    ofBackground(red,green,blue);
    
    //----stages
    bShutdownStarted = false;
    stage = -1;
    
    //------other apps
    //load names of other apps that should be exited from when shutdown is called.
    //it happens that some apps block shutdown
    
    ofLog()<<"-------read in file---------";
    ofBuffer buffer = ofBufferFromFile("otherAppsToExit.txt");
    ofLog()<<"in otherAppsToExit.txt we are looking for";
    ofLog()<<"apps to be closed upon shutdown: ";
    //sometimes apps blow the shutdown process. so it's good to exit them before.
    if(buffer.size()) {
        
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
            
            string line = *it;
            if(line.empty() == false) {
                //                vector<string> splitLine = ofSplitString(line, ",");
                otherAppNames.push_back(line);
                ofLog()<<"found "<<otherAppNames.back();
            }
        }
    }
    ofLog()<<"";
    
    //-------serial
    //communicate to Hitachi projector to turn on or off
    serial.listDevices();
    
    getSerialDevice();
    myBaud = 9600; //57600; //115200;
    ofLog()<<"serialID "<<serialID<<" , myBaud "<<myBaud;
    serial.setup(serialID,myBaud);
    
    projectorPrintInfo();
    projectorsON();
    
    ofLog()<<"";
    ofLog()<<"setup done";
}

void ofApp::exit(){
    ofLog()<<"exit in ofApp";
    button_object.exit();
}

void ofApp::exitOtherApps(){
    
    char myScript[255];
    
    for(int i=0; i<otherAppNames.size();i++){
        sprintf(myScript, "osascript -e 'quit app \"%s\"'",otherAppNames[i].c_str());
        cout<<i<<" , exitOtherApps, "<<myScript<<endl;
        std::system(myScript);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(version);
    
    if(bButtonPressed == true){
        bButtonPressed = false;
        bShutdownStarted = true;
        
        stage = 0;
        stageDuration = 1;
        
        projectorsOFF();
    }
    
    if(bShutdownStarted == true){
        if(stage == 0 && ofGetElapsedTimef() - stageTimer > stageDuration){
            //            std::cout << "\007";
            ofSystem("osascript -e 'beep'");
            stageTimer = ofGetElapsedTimef();
            stageDuration = 1;
            stage++;
        }
        if(stage == 1 && ofGetElapsedTimef() - stageTimer > stageDuration){
            ofSystem("osascript -e 'beep'");
            stageTimer = ofGetElapsedTimef();
            stageDuration = 1;
            stage++;
        }
        if(stage == 2 && ofGetElapsedTimef() - stageTimer > stageDuration){
            projectorsOFF();
            stageTimer = ofGetElapsedTimef();
            stageDuration = 3;
            stage++;
        }
        if(stage == 3 && ofGetElapsedTimef() - stageTimer > stageDuration){
            ofSystem("osascript -e 'beep'");
            ofSystem(shutdonwScript);
            stageTimer = ofGetElapsedTimef();
            stageDuration = 3;
            stage++;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    int diam = 60;
    
    ofPushMatrix();
    ofTranslate(200/2-diam/2, 200/2-50, 0);
    
    ofNoFill();
    ofSetColor(textColor, textColor, textColor);
    ofSetLineWidth(1);
    ofDrawCircle(diam/2, diam/2, diam);
    ofDrawCircle(diam/2, diam/2, diam-10);
    
    ofSetLineWidth(1);
    ofDrawCircle(diam/2, diam/2, diam-30);
    ofFill();
    ofSetColor(red,green,blue);
    ofDrawRectangle(diam/2-5, diam/2-40, 10, 25);
    
    ofSetColor(textColor, textColor, textColor);
    ofDrawRectangle(diam/2, diam/2-40, 1, 25);
    
    ofDrawBitmapString("tx/rx", diam/2-20, diam/2+4);
    //ofDrawBitmapString("~", diam/2-20, diam/2+14);
    
    /*
     //draw wave when also monitoring if power cable gets unplugged
     //new code does isnot yet able to monitor this
     float tW = 10;
     float tH = 10;
     float hOffest = 15;
     
     float x0 = diam/2-tW;
     float y0 = diam/2+hOffest+1;
     float x1 = diam/2;
     float y1 = diam/2+hOffest-tH;
     float x2 = diam/2;
     float y2 = diam/2+hOffest+tH;
     float x3 = diam/2+tW;
     float y3 = diam/2+hOffest-1;
     
     
     ofNoFill();
     ofSetLineWidth(2);
     ofBeginShape();
     ofVertex(x0,y0);
     ofBezierVertex(x1,y1,x2,y2,x3,y3);
     ofEndShape();
     ofSetLineWidth(1);
     */
    
    ofPopMatrix();
    
    float xx = (ofGetElapsedTimeMillis()/10) % ofGetWidth();
    
    ofDrawLine(xx, ofGetHeight()-10, xx, ofGetHeight());
    
    ofSetColor(textColor, textColor, textColor);
    ofDrawBitmapString("Immediate shut-down on:",12,155);
    ofDrawBitmapString("power button pressed",12,170);
    
    ofDrawBitmapString("other apps to exit:",12,190);
    for(int i=0; i<otherAppNames.size();i++){
        ofDrawBitmapString(otherAppNames[i],12,190+((i+1)*15));
    }
    
    //ofDrawBitmapString("power cord unplugged",12,185);
    
    //    ofDrawBitmapString(ofToString(ofGetFrameNum()),12,185);
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'g'){
        showGUI = !showGUI;
        
        if(showGUI){
            
        }else{
            
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

void ofApp::projectorPrintInfo(){
    
    cout<<"if(key == 'x') projectorsOFF();"<<endl;
    cout<<"if(key == 'y') projectorsON();"<<endl;
    cout<<"if(key == 'e') projectorsEcoOn();"<<endl;
    cout<<"if(key == 'n') projectorsEcoOff();"<<endl;
    
    cout<<"if(key == 'a') projector1_HDMI1();"<<endl;
    cout<<"if(key == 'b') projector1_HDMI2();"<<endl;
    cout<<"if(key == 'c') projector2_HDMI1();"<<endl;
    cout<<"if(key == 'd') projector2_HDMI2();"<<endl;
    
}

void ofApp::projectorsOFF(){
    cout<<"projectors OFF"<<endl;
    cout<<"off = 123x"<<endl;
    
    unsigned char sendCmd[4];
    sendCmd[0] = '1'; //shortSend start byte 44
    sendCmd[1] = '2';
    sendCmd[2] = '3';
    sendCmd[3] = 'x';
    serial.writeBytes(sendCmd, sizeof(sendCmd));
    
    //ofSleepMillis(2000);
    //    cout<<"projectors OFF"<<endl;
    //    serial.writeBytes(sendCmd, sizeof(sendCmd));
    //
    //    ofSleepMillis(4000);
    //    cout<<"projectors OFF"<<endl;
    //    serial.writeBytes(sendCmd, sizeof(sendCmd));
    
    
    
}


void ofApp::projectorsON(){
    cout<<"projectors ON"<<endl;
    
    
    unsigned char sendCmd[4];
    sendCmd[0] = '1'; //shortSend start byte 44
    sendCmd[1] = '2';
    sendCmd[2] = '3';
    sendCmd[3] = 'y';
    serial.writeBytes(sendCmd, sizeof(sendCmd));
    
    ofSleepMillis(2000);
    cout<<"on = 123y"<<endl;
    serial.writeBytes(sendCmd, sizeof(sendCmd));
    
    
    
}

void ofApp::projectorsEcoOn(){
    cout<<"projectorsEcoOn"<<endl;
    cout<<"ecoON = 123e"<<endl;
    
    unsigned char sendCmd[4];
    sendCmd[0] = '1'; //shortSend start byte 44
    sendCmd[1] = '2';
    sendCmd[2] = '3';
    sendCmd[3] = 'e';
    serial.writeBytes(sendCmd, sizeof(sendCmd));
    
    //ofSleepMillis(2000);
    //    cout<<"projectors OFF"<<endl;
    //    serial.writeBytes(sendCmd, sizeof(sendCmd));
    //
    //    ofSleepMillis(4000);
    //    cout<<"projectors OFF"<<endl;
    //    serial.writeBytes(sendCmd, sizeof(sendCmd));
    
    
    
    
}

void ofApp::projectorsEcoOff(){
    cout<<"projectorsEcoOff"<<endl;
    cout<<"ecoOFF = 123n"<<endl;
    
    unsigned char sendCmd[4];
    sendCmd[0] = '1'; //shortSend start byte 44
    sendCmd[1] = '2';
    sendCmd[2] = '3';
    sendCmd[3] = 'n';
    serial.writeBytes(sendCmd, sizeof(sendCmd));
    
    //ofSleepMillis(2000);
    //    cout<<"projectors OFF"<<endl;
    //    serial.writeBytes(sendCmd, sizeof(sendCmd));
    //
    //    ofSleepMillis(4000);
    //    cout<<"projectors OFF"<<endl;
    //    serial.writeBytes(sendCmd, sizeof(sendCmd));
    
    
    
}

void ofApp::projector1_HDMI1(){
    cout<<"projector1_HDMI1"<<endl;
    //X139X
    cout<<"hdmi1 = 123e"<<endl;
    
    unsigned char sendCmd[4];
    sendCmd[0] = '1'; //shortSend start byte 44
    sendCmd[1] = '2';
    sendCmd[2] = '3';
    sendCmd[3] = 'e';
    serial.writeBytes(sendCmd, sizeof(sendCmd));
}
void ofApp::projector1_HDMI2(){
    cout<<"projector1_HDMI2"<<endl;
    //X149X
    cout<<"hdmi2 = 123f"<<endl;
    
    unsigned char sendCmd[4];
    sendCmd[0] = '1'; //shortSend start byte 44
    sendCmd[1] = '2';
    sendCmd[2] = '3';
    sendCmd[3] = 'f';
    serial.writeBytes(sendCmd, sizeof(sendCmd));
}

void ofApp::projector2_HDMI1(){
    cout<<"projector2_HDMI1"<<endl;
    //X139X
    cout<<"hdmi1 = 123g"<<endl;
    
    unsigned char sendCmd[4];
    sendCmd[0] = '1'; //shortSend start byte 44
    sendCmd[1] = '2';
    sendCmd[2] = '3';
    sendCmd[3] = 'e';
    serial.writeBytes(sendCmd, sizeof(sendCmd));
}
void ofApp::projector2_HDMI2(){
    cout<<"projector2_HDMI2"<<endl;
    //X149X
    cout<<"hdmi2 = 123h"<<endl;
    
    unsigned char sendCmd[4];
    sendCmd[0] = '1'; //shortSend start byte 44
    sendCmd[1] = '2';
    sendCmd[2] = '3';
    sendCmd[3] = 'f';
    serial.writeBytes(sendCmd, sizeof(sendCmd));
}


void ofApp::getSerialDevice(){
    vector<ofSerialDeviceInfo> serialDevices = serial.getDeviceList();
    
    string deviceLine;
    for(int i=0; i<serialDevices.size();i++){
        
        deviceLine = serialDevices[i].getDeviceName().c_str();
        
        //cout<<serialDevices[i].getDeviceName().c_str()<<endl;
        
        //arduino UNO
        if(deviceLine.substr(0,12) == "tty.usbmodem"){
            serialID = "/dev/" +deviceLine;
            cout<<"arduino serial = "<<serialID<<endl;
        } else if(deviceLine.substr(0,13) == "tty.usbserial"){
            //non UNO
            serialID = "/dev/" +deviceLine;
            cout<<"arduino serial = "<<serialID<<endl;
            //            ofLog()<<"arduino serial = "<<serialID;
        }
    }
    
}
