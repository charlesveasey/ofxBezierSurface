#pragma once

#include "ofVboMesh.h"
#include "ofMain.h"

class ofxBezierSurface {
public:
    ofxBezierSurface() {};
    void setup(int w, int h, int dim, int res);
    void update();
    void draw();
    void drawWireframe();
    float ctrlPntSize;
    void reset();
    void showControls(bool b);
    vector<ofVec3f> getControlPnts();
    void setControlPnts(vector<ofVec3f>);
    
private:
    int width;
    int height;
    int cx;
    int cy;
    int rx;
    int ry;
    vector< vector<ofVec3f> > inp;
    vector< vector<ofVec3f> > outp;
    double createSurface();
    double bezierBlend(int k, double mu, int n);
    
    ofVboMesh mesh;
    
    bool bControls;
    void drawControls();
    vector<ofPoint> selectedPnts;
    bool updateSurface;
    ofPoint lastMouse;
    bool drawBox;
    ofPoint boxOrigin;
    ofPoint boxUpdate;
    bool group;
    bool shift;
    bool up;
    bool down;
    bool left;
    bool right;
    bool found;
    
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs &mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
    void keyPressed(ofKeyEventArgs& keyArgs);
    void keyReleased(ofKeyEventArgs& keyArgs);
};