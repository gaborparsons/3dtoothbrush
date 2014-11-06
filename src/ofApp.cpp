#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    position = "center";
    timer = 0;
    angle = 0;
    anglebrush = 0;
    anglespeed = 0;
    movementX = 0;
    movementY = 0;
    job = 0;

//3D MODEL SETUP
    //load the models
    typodontModel.loadModel("dentier_final/untitled.3ds", 38);
    toothbrushModel.loadModel("toothbrush/mpm_F02.3DS", 38);

    //this is sized to a MacBook Pro screen
    typodontModel.setScale(0.29, 0.29, 0.29);
    toothbrushModel.setScale(0.07, 0.07, 0.07);


//END 3D MODEL

//SERIAL SETUP
    mySerial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();

    mySerial.setup("/dev/ttyACM0", 115200);
//    mySerial.setup("/dev/cu.usbmodem411", 115200);

    mySerial.flush(); //flush the serial port once before we start
//END SERIAL

    ofSetFrameRate(60);

    ofBackground(255);

    ofSetVerticalSync(true);


    //some model / light stuff
    ofEnableDepthTest();
    glShadeModel (GL_SMOOTH);

    glEnable(GL_DEPTH_TEST); //enable depth comparisons and update the depth buffer
    ofDisableArbTex();

    ofSetCircleResolution(60);


    // fonts
    ofTrueTypeFont::setGlobalDpi(72);
    verdana20.loadFont("verdana.ttf", 20, true, true);
    verdana20.setLineHeight(24.0f);
    verdana20.setLetterSpacing(1.035);

//LIGHT SETUP

    ofSetSmoothLighting(false);
    pointLight.setDiffuseColor( ofFloatColor(.255, .255, .55) );
    pointLight.setSpecularColor( ofFloatColor(4.f, 4.f, 12.f));

    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.2f, .2f, .4f));

    pointLight3.setDiffuseColor( ofFloatColor(255.f/255.f,255.f/255.f,255.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(255.f/255.f,255.f/255.f,135.f/255.f) );

    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 120 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));

    ofSetSphereResolution(40);

}

//--------------------------------------------------------------
void ofApp::update() {

//SERIAL PART
    if(mySerial.available() >= NUM_MSG_BYTES){

        //go through all received bytes
        for(int i=0; i<NUM_MSG_BYTES; i++){

            //read this byte
            int val = mySerial.readByte();

            //store it in the array at index i
            bytesReceived[i] = val;
        }

        numMsgRecvd++;
        //check all the bytes we receive from serial
        cout << "bytesReceived[0] = " << bytesReceived[0] << endl;
        cout << "bytesReceived[1] = " << bytesReceived[1] << endl;
        cout << "bytesReceived[2] = " << bytesReceived[2] << endl;
        cout << "bytesReceived[3] = " << bytesReceived[3] << endl;
        cout << "bytesReceived[4] = " << bytesReceived[4] << endl;
        cout << "------------------------"<<endl;

        mySerial.flush();

        mySerial.writeByte('A'); //request another message!
    }
//END SERIAL

//3D MODEL SETUP
    anglespeed = timer*2;

    if(position == "center" && bytesReceived[1] >= 30 && bytesReceived[1] <= 70){
        timer++;
        if(anglespeed <=45){
            angle = anglespeed;
            anglebrush = -angle/2;
        }else{
            timer = 0;
            position = "left";
        }
    }
    if(position == "left" && (bytesReceived[1] < 30 || bytesReceived[1] > 70)){
        timer++;
        if(anglespeed <=45){
            angle = 45-anglespeed;
            anglebrush = -angle/2;
        }else{
            timer = 0;
            position = "center";
        }
    }
    if(position == "center" && bytesReceived[1] >= 160 && bytesReceived[1] <= 200){
        timer++;
        if(anglespeed <=45){
            angle = -anglespeed;
            anglebrush = -angle/2;
        }else{
            timer = 0;
            position = "right";
        }
    }
    if(position == "right" && (bytesReceived[1] < 160 || bytesReceived[1] > 200)){
        timer++;
        if(anglespeed <=45){
            angle = -45+anglespeed;
            anglebrush = -angle/2;
        }else{
            timer = 0;
            position = "center";
        }
    }
    //CIRCULAR MOVEMENT
    if(bytesReceived[2] >20 && bytesReceived[2] <210){//if the brush is not standing straight
        if((position == "right" && (bytesReceived[3]<25 || bytesReceived[3]>37)) || (position == "left" && (bytesReceived[3]<42 || bytesReceived[3]>60)) ){
            if(job<56){
                job++;
                if(job<14){
                    movementX = job;
                    movementY = job/2;
                }else if(job<28){
                    movementX = job;
                    movementY = (14-(job-14))/2;
                }else if(job<42){
                    movementX = 28-(job-28);
                    movementY = (14-(job-14))/2;
                }else{
                    movementX = 28-(job-28);
                    movementY = (-56+job)/2;
                }
            }
            else{
                job=0;
            };
        }//end of byteReceived[3] if function
    }//end of byteReceived[2] if function

//END 3D MODEL

    pointLight.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    pointLight2.setPosition(ofGetWidth(), ofGetHeight(), 100);
    pointLight3.setPosition(200, 300, 200);

}

//--------------------------------------------------------------
void ofApp::draw() {

    ofSetColor(255);

    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

//SERIAL PART
    ofSetColor(255);

    //draw out our received data
    for(int i=0; i<NUM_MSG_BYTES; i++){
        string whichByte = "bytesReceived["+ofToString(i)+"] = ";
        verdana20.drawString(whichByte+ofToString(bytesReceived[i]), 50, 30+30*i);
    }
//   verdana20.drawString(ofToString(bytesReceived[1]), 50, 30);

    verdana20.drawString("position: "+ position, 50, 220);

    //just for debugging
//    verdana20.drawString("num messages received total: "+ ofToString(numMsgRecvd), 50, 250);
//    verdana20.drawString("position: "+ position, 50, 300);
//    verdana20.drawString("timer: "+ ofToString(timer), 50, 350);
//    verdana20.drawString("j: "+ ofToString(job), 50, 400);

//END SERIAL

    ofEnableDepthTest();
    ofEnableLighting();
    pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();

    material.begin();

    ofSetColor(180);
    ofNoFill();

    ofDrawSphere(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth());

//3D MODEL
    ofSetColor(255, 255, 255, 255);

    glPushMatrix();
        glTranslatef(ofGetWidth()/2, ofGetHeight()/2+160, -40);
        glRotatef(angle , 0, 1, 0);
        glRotatef(170 , 0, 1, 0);
                typodontModel.draw();
        glRotatef(-170 , 0, 1, 0);
        glRotatef(-angle , 0, 1, 0);
            glTranslatef(-200, -150, 160);
            glRotatef(anglebrush , 0, 1, 0);
            glRotatef(270 , 0, 0, 1);
            glRotatef(180 , 0, 1, 0);
            glTranslatef(0, -angle*2, angle*2);

            for(int i=0; i<20; i++){
                glTranslatef(movementY, movementX, 0);
                toothbrushModel.draw();
                glTranslatef(-movementY, -movementX, 0);
            }
            glTranslatef(0, angle*2, -angle*2);
            glRotatef(-270 , 0, 0, 1);
            glRotatef(-180 , 0, 1, 0);
            glRotatef(-anglebrush , 0, 1, 0);
            glTranslatef(200, 150, -160);
        glTranslatef(-(ofGetWidth()/2), -(ofGetHeight()/2+160), 40);
    glPopMatrix();
//End 3D MODEL

    ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    bMousePressed = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    bMousePressed = false;
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
