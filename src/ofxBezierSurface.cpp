#include "ofxBezierSurface.h"

void ofxBezierSurface::setup(int w, int h, int dim, int res) {
    width = w;
    height = h;
    cx = dim-1;
    cy = dim-1;
    rx = res;
    ry = res;
    
    int i,j;
    
    bHasListener = false;
    
    // create point vectors
    for (i=0; i<=cx; i++) {
        inp.push_back(vector<ofVec3f>());
        for (j=0; j<=cy; j++) {
            inp[i].push_back(ofVec3f());
        }
    }
    for (i=0; i<rx; i++) {
        outp.push_back(vector<ofVec3f>());
        for (j=0; j<ry; j++) {
            outp[i].push_back(ofVec3f());
        }
    }
    
    // load default points
    for (i=0;i<=cx;i++) {
        for (j=0;j<=cy;j++) {
            inp[i][j].x = ofMap(i, 0, cx, 0, width);
            inp[i][j].y = ofMap(j, 0, cy, 0, height);
            inp[i][j].z = 0;
        }
    }
    
    // create default surface
    mesh = ofMesh::plane(width, height, rx, ry, OF_PRIMITIVE_TRIANGLES);
    createSurface();

    vector<glm::vec3> verts = mesh.getVertices();
    for (i=0; i<verts.size(); i++) {
        mesh.setTexCoord(i, ofVec2f(verts[i].x, verts[i].y));
    }
    
    // interface
    updateSurface = false;
    ctrlPntSize = 10;
    up = false;
    down = false;
    left = false;
    right = false;
    found  = false;
}

void ofxBezierSurface::draw() {
    mesh.draw();
}

void ofxBezierSurface::drawWireframe() {
    mesh.drawWireframe();
}

void ofxBezierSurface::update() {
    if (updateSurface) {
        createSurface();
        updateSurface = false;
    }
}

void ofxBezierSurface::drawControls() {
    for (int i=0;i<=cx;i++) {
        for (int j=0;j<=cy;j++) {
            
            for (int k=0;k<selectedPnts.size();k++) {
                if (selectedPnts[k].x == i && selectedPnts[k].y == j) {
                    ofSetColor(200,200,0);
                    break;
                }
                else
                    ofSetColor(200,200,200);
            }
            ofPoint temp_inP(inp[i][j].x, inp[i][j].y, inp[i][j].z);
                        ofPoint temp_outP(outp[i][j].x, outp[i][j].y, outp[i][j].z);
                        
            ofDrawCircle(temp_inP, ctrlPntSize);
            
           
              ofSetColor(0,0,200);
            ofDrawCircle(temp_outP, ctrlPntSize/2);
             ofSetColor(255);
            ofDrawLine(temp_inP,temp_outP);
            
        }
    }
}


//vector<ofVec3f> ofxBezierSurface::getVertices(){
//    return mesh.getVertices();
//}

vector<glm::vec3> ofxBezierSurface::getVertices(){
    return mesh.getVertices();
}

void ofxBezierSurface::setVertices(vector<ofVec3f> & verts){
    for (int i=0; i<verts.size(); i++) {
        mesh.setVertex(i, verts[i]);
    }
}

vector<ofVec3f> ofxBezierSurface::getControlPnts(){
    vector<ofVec3f> vec;
    for (int i=0; i<=cx; i++) {
        for (int j=0; j<=cy; j++) {
            vec.push_back(inp[i][j]);
        }
    }
    return vec;
}
void ofxBezierSurface::setControlPnts(vector<ofVec3f> vec){
    int c=0;
    for (int i=0; i<=cx; i++) {
        for (int j=0; j<=cy; j++) {
            inp[i][j] = vec[c];
            c++;
        }
    }
    createSurface();
}

int ofxBezierSurface::getControlPntDim(){
    return cx+1;
}

void ofxBezierSurface::setControlPntDim(int dim){
    int oldcx = cx;
    int oldcy = cy;
    
    cx = dim-1;
    cy = dim-1;
    
    vector< vector<ofVec3f> > oldInp;
    
    for (int i=0;i<=oldcx;i++) {
        oldInp.push_back(vector<ofVec3f>());

        for (int j=0;j<=oldcy;j++) {
            oldInp[i].push_back(ofVec3f());
            oldInp[i][j].x = inp[i][j].x;
            oldInp[i][j].y = inp[i][j].y;
            oldInp[i][j].z = inp[i][j].z;
        }
    }
    
    
    inp.clear();
    
    for (int i=0; i<=cx; i++) {
        inp.push_back(vector<ofVec3f>());
        for (int j=0; j<=cy; j++) {
            inp[i].push_back(ofVec3f());
        }
    }
    
 
    // load default points
    for (int i=0;i<=cx;i++) {
        for (int j=0;j<=cy;j++) {
            inp[i][j].x = ofMap(i, 0, cx, 0, width);
            inp[i][j].y = ofMap(j, 0, cy, 0, height);
            inp[i][j].z = 0;
        }
    }
    
    inp = calculateSurface(oldInp, inp, oldcx, oldcy, cx+1, cy+1);
    
    createSurface();
}

void ofxBezierSurface::reset() {
    // load default points
    for (int i=0;i<=cx;i++) {
        for (int j=0;j<=cy;j++) {
            inp[i][j].x = ofMap(i, 0, cx, 0, width);
            inp[i][j].y = ofMap(j, 0, cy, 0, height);
            inp[i][j].z = 0;
        }
    }
    createSurface();
}

void ofxBezierSurface::addListeners(){
    
    bHasListener = true;
    
    ofAddListener(ofEvents().mousePressed, this, &ofxBezierSurface::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &ofxBezierSurface::mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &ofxBezierSurface::mouseReleased);
    ofAddListener(ofEvents().keyPressed, this, &ofxBezierSurface::keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxBezierSurface::keyReleased);
}
void ofxBezierSurface::removeListeners(){
     bHasListener = false;
    ofRemoveListener(ofEvents().mousePressed, this, &ofxBezierSurface::mousePressed);
    ofRemoveListener(ofEvents().mouseDragged, this, &ofxBezierSurface::mouseDragged);
    ofRemoveListener(ofEvents().mouseReleased, this, &ofxBezierSurface::mouseReleased);
    ofRemoveListener(ofEvents().keyPressed, this, &ofxBezierSurface::keyPressed);
    ofRemoveListener(ofEvents().keyReleased, this, &ofxBezierSurface::keyReleased);
}


//----------------------------------------------------- interaction.
void ofxBezierSurface::mousePressed(ofMouseEventArgs& mouseArgs) {
    float distance = 0;
    ofPoint tmp;
    bool missing = true;
    
    tmp.set(findPoint(mouseArgs));
    
    if (!ofGetKeyPressed(OF_KEY_SHIFT)) {
        selectedPnts.clear();
    }
    
    for (int i=0; i<selectedPnts.size(); i++) {
        if (selectedPnts[i].x == tmp.x && selectedPnts[i].y == tmp.y)
            missing = false;
    }
    
    if (missing)
        selectedPnts.push_back(tmp);
    
    lastMouse = ofPoint(mouseArgs.x, mouseArgs.y);
}

ofPoint ofxBezierSurface::findPoint(ofMouseEventArgs mouseArgs){
    ofPoint pnt;
    float distance = 0;
    float nearest = -1;

    
    for (int i=0; i<=cx; i++) {
        for (int j=0; j<=cy; j++) {
            distance = ofDist(inp[i][j].x, inp[i][j].y, mouseArgs.x, mouseArgs.y);
            if (nearest == -1)
                nearest = distance;
            if (distance < nearest) {
                pnt.set(i,j);
                nearest = distance;
            }
        }
    }
    return pnt;
}

void ofxBezierSurface::mouseDragged(ofMouseEventArgs& mouseArgs) {
   if (selectedPnts.size() > 0) {
        ofPoint mouse = ofPoint(mouseArgs.x, mouseArgs.y);
        ofPoint d = ofPoint(mouse.x - lastMouse.x, mouse.y - lastMouse.y);
        for (int i=0; i<selectedPnts.size(); i++) {
            inp[selectedPnts[i].x][selectedPnts[i].y].x += d.x;
            inp[selectedPnts[i].x][selectedPnts[i].y].y += d.y;
        }
        updateSurface = true;
        lastMouse = mouse;
    }
}

void ofxBezierSurface::mouseReleased(ofMouseEventArgs& mouseArgs) {
}

void ofxBezierSurface::keyPressed(ofKeyEventArgs& keyArgs) {
    int key = keyArgs.key;
    
    float dx=0;
    float dy=0;
    if (key == OF_KEY_UP || key == OF_KEY_DOWN || key == OF_KEY_LEFT || key == OF_KEY_RIGHT ) {
        if (key == OF_KEY_UP) up = true;
        else if (key == OF_KEY_DOWN) down = true;
        else if (key == OF_KEY_LEFT) left = true;
        else if (key == OF_KEY_RIGHT) right = true;
        
        if (up) dy = -1;
        if (down) dy = 1;
        if (left) dx = -1;
        if (right) dx = 1;
        
        if (selectedPnts.size() > 0) {
            for (int i=0; i<selectedPnts.size(); i++) {
                inp[selectedPnts[i].x][selectedPnts[i].y].x += dx;
                inp[selectedPnts[i].x][selectedPnts[i].y].y += dy;
            }
            updateSurface  = true;
        }
    }

}

void ofxBezierSurface::keyReleased(ofKeyEventArgs& keyArgs){
    int key = keyArgs.key;
    if (!ofGetKeyPressed(OF_KEY_SHIFT)){
        if (key == OF_KEY_UP) up = false;
        else if (key == OF_KEY_DOWN) down = false;
        else if (key == OF_KEY_LEFT) left = false;
        else if (key == OF_KEY_RIGHT) right = false;
    }
}

//----------------------------------------------------- bezier.
void ofxBezierSurface::createSurface() {
    outp = calculateSurface(inp, outp, cx, cy, rx, ry);
    
    int i,j;
    int cnt;
    vector<ofVec3f> verts;
    for (i=0;i<rx;i++) {
        for (j=0;j<ry;j++) {
            verts.push_back(outp[i][j]);
        }
    }
    for (i=0; i<verts.size(); i++) {
        mesh.setVertex(i, verts[i]);
    }
}

vector< vector<ofVec3f> > ofxBezierSurface::calculateSurface(vector< vector<ofVec3f> > ip, vector< vector<ofVec3f> > op,
                                                             int cpx, int cpy, int rpx, int rpy ){
    double mui,muj;
    double bi, bj;
    float x,y,z;
    
    // calculate bezier surface
    for (int i=0;i<rpx;i++) {
        mui = i / (double)(rpx-1);
        for (int j=0;j<rpy;j++) {
            muj = j / (double)(rpy-1);
            
            op[i][j].x = 0;
            op[i][j].y = 0;
            op[i][j].z = 0;
            
            for (int ki=0;ki<=cpx;ki++) {
                bi = bezierBlend(ki,mui,cpx);
                for (int kj=0;kj<=cpy;kj++) {
                    bj = bezierBlend(kj,muj,cpy);
                    op[i][j].x += (ip[ki][kj].x * bi * bj);
                    op[i][j].y += (ip[ki][kj].y * bi * bj);
                    op[i][j].z += (ip[ki][kj].z * bi * bj);
                }
            }
        }
    }
    
    return op;
}

double ofxBezierSurface::bezierBlend(int k, double mu, int n) {
    int nn,kn,nkn;
    double blend=1;
    
    nn = n;
    kn = k;
    nkn = n - k;
    
    while (nn >= 1) {
        blend *= nn;
        nn--;
        if (kn > 1) {
            blend /= (double)kn;
            kn--;
        }
        if (nkn > 1) {
            blend /= (double)nkn;
            nkn--;
        }
    }
    if (k > 0)
        blend *= pow(mu,(double)k);
    if (n-k > 0)
        blend *= pow(1-mu,(double)(n-k));
    
    return(blend);
}
