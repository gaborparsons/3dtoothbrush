#pragma once

#include "ofMain.h"
#include "ofx3DModelLoader.h"
//#include "ofxAssimpModelLoader.h"

#define NUM_MSG_BYTES 6  //how many bytes we are expecting to receive


class ofApp : public ofBaseApp{

public:

    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // fonts
    bool bFirst;
    string typeStr;
    ofTrueTypeFont	verdana20;

    //SERIAL PART
    ofSerial mySerial;

    //set this array's length to the number of msg
    //bytes we are expecting to receive
    int bytesReceived[NUM_MSG_BYTES];

    //we'll use this to count how many msgs have been received so far
    //(for debugging)
    long numMsgRecvd;
    //SERIAL END

    //might not be useful
    bool bFill;
    bool bWireframe;
    bool bDrawNormals;
    bool bDrawAxes;
    bool bDrawLights;
    bool bInfoText;
    bool bMousePressed;
    bool bSplitFaces;

    ofImage texture;

    int mode;
    string position;
    int timer;
    int angle;
    int anglebrush;
    float anglespeed;
    int movementX;
    int movementY;
    int job;

    ofSpherePrimitive sphere;
    ofIcoSpherePrimitive icoSphere;
    ofPlanePrimitive plane;
    ofCylinderPrimitive cylinder;
    ofConePrimitive cone;
    ofBoxPrimitive box;

    // place to store the sides of the box
    ofVboMesh boxSides[ofBoxPrimitive::SIDES_TOTAL];
    ofVboMesh deformPlane;
    ofVboMesh topCap, bottomCap, body;
    vector<ofMeshFace> triangles;

    //LIGHT SETUP
    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    ofMaterial material;

//3D model
    ofx3DModelLoader typodontModel;
    ofx3DModelLoader toothbrushModel;


};

