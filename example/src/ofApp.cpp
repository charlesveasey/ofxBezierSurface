#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    int width = 1024;
    int height = 768;
    
    ofImage image;
    image.loadImage("1024_768.jpg");
    image.resize(width, height);
    
    texture.clear();
    texture.allocate(width, height, GL_RGB);
    texture.loadData(image.getPixelsRef());
    
    surface.setup(width, height, 4, 20);
    surface.addListeners();

}

//--------------------------------------------------------------
void ofApp::update(){
    surface.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255);
    texture.bind();
        surface.draw();
    texture.unbind();
    surface.drawControls();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 32) // space
        surface.reset();
    else if (key == 115){ // s = save
        xml.clear();
        xml.addChild("points");
        xml.setTo("points");
        vector<ofVec3f> vec = surface.getControlPnts();
        for (int i = 0; i<vec.size(); i++) {
            xml.addChild("point");
            xml.setToChild(i);
            xml.setAttribute("xyz", ofToString(vec[i].x) + "," + ofToString(vec[i].y) + "," + ofToString(vec[i].z));
            xml.setToParent();
        }
        xml.setToParent();
        xml.save("settings.xml");
    }
    else if (key == 108){ // l = load
        string str;
        vector<ofVec3f> vec;
        xml.load("settings.xml");
        for (int i = 0; i<xml.getNumChildren(); i++) {
            
            if (xml.exists("point["+ofToString(i)+"][@xyz]")) {
                str = xml.getAttribute("point["+ofToString(i)+"][@xyz]");
                int x = ofToInt(ofSplitString(str, ",")[0]);
                int y = ofToInt(ofSplitString(str, ",")[1]);
                int z = ofToInt(ofSplitString(str, ",")[2]);
                vec.push_back(ofVec3f(x,y,z));
            }
        }
        surface.setControlPnts(vec);
    }
    else if (key == 45){ // - = decrease points
        surface.setControlPntDim(surface.getControlPntDim()-1);
    }
    else if (key == 61){ // - = increase points
        surface.setControlPntDim(surface.getControlPntDim()+1);
    }
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
