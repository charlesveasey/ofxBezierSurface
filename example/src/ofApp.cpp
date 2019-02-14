#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);
    int width = 1024;
    int height = 768;
    
    ofImage image;
    image.load("1024_768.jpg");
    image.resize(width, height);
    
    texture.clear();
    texture.allocate(width, height, GL_RGB);
    texture.loadData(image.getPixels());
    
    surface.setup(width, height, 6, 6);
//    surface.addListeners();

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
    
    if(key == 'e'){
        bEnableEdit = !bEnableEdit;
        if(bEnableEdit){
            surface.addListeners();
        }else{
            surface.removeListeners();
        }
    }
    if (key == 32) // space
        surface.reset();
    else if (key == 115){ // s = save
        xml.clear();
//        xml.addChild("points");
//        xml.setTo("points");

       xml.appendChild("points"); //.set("points");
        
        vector<ofVec3f> vec = surface.getControlPnts();
        for (int i = 0; i<vec.size(); i++) {
//            xml.addChild("point");
//            xml.setToChild(i);
//            xml.setAttribute("xyz", ofToString(vec[i].x) + "," + ofToString(vec[i].y) + "," + ofToString(vec[i].z));
//            xml.setToParent();
            
            auto pt = xml.getChild("points").appendChild("point");
            pt.setAttribute("x", vec[i].x);
            pt.setAttribute("y", vec[i].y);
            pt.setAttribute("z", vec[i].z);
        }
//        xml.setToParent();
        xml.save("settings.xml");
    }
    else if (key == 108){ // l = load
        string str;
        vector<ofVec3f> vec;
        xml.load("settings.xml");

        auto point_parents = xml.find("/points");
        for(auto & one_parent : point_parents){
            auto all_point_children = one_parent.getChildren("point");
            
//            int a = 0;
            for(auto & pt: all_point_children){
                
            
//            if (xml.exists("point["+ofToString(i)+"][@xyz]")) {
//                str = xml.getAttribute("point["+ofToString(i)+"][@xyz]");
//                int x = ofToInt(ofSplitString(str, ",")[0]);
//                int y = ofToInt(ofSplitString(str, ",")[1]);
//                int z = ofToInt(ofSplitString(str, ",")[2]);
                
                auto x = pt.getAttribute("x").getIntValue();
                auto y = pt.getAttribute("y").getIntValue();
                auto z = pt.getAttribute("z").getIntValue();
                vec.push_back(ofVec3f(x,y,z));
//            }
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
