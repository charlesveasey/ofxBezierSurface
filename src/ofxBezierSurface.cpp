#include "ofxBezierSurface.h"

void ofxBezierSurface::setup(int w, int h, int dim, int res) {
    width = w;
    height = h;
    cx = dim-1;
    cy = dim-1;
    rx = res;
    ry = res;
    
    int i,j;
    
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

    vector<ofVec3f> verts = mesh.getVertices();
    for (i=0; i<verts.size(); i++) {
        mesh.setTexCoord(i, ofVec2f(verts[i].x, verts[i].y));
    }
    
    // interface
    bControls = true;
    updateSurface = false;
    ctrlPntSize = 10;
    drawBox = false;
    shift = false;
    up = false;
    down = false;
    left = false;
    right = false;
    found  = false;
    
    ofAddListener(ofEvents().mousePressed, this, &ofxBezierSurface::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &ofxBezierSurface::mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &ofxBezierSurface::mouseReleased);
    ofAddListener(ofEvents().keyPressed, this, &ofxBezierSurface::keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxBezierSurface::keyReleased);

}

void ofxBezierSurface::draw() {
    mesh.draw();
    if (bControls)
        drawControls();
}

void ofxBezierSurface::drawWireframe() {
    mesh.drawWireframe();
    if (bControls)
        drawControls();
}

void ofxBezierSurface::update() {
    if (updateSurface) {
        createSurface();
        updateSurface = false;
    }
}

void ofxBezierSurface::drawControls() {
    ofFill();
    ofSetColor(100,100,100);

    for (int i=0;i<=cx;i++) {
        for (int j=0;j<=cy;j++) {
            
            for (int k=0;k<selectedPnts.size();k++) {
                if (selectedPnts[k].x == i && selectedPnts[k].y == j) {
                    ofSetColor(0,0,255);
                    break;
                }
                else
                    ofSetColor(100,100,100);
            }
            
            ofCircle(inp[i][j].x, inp[i][j].y, inp[i][j].z, ctrlPntSize);
        }
    }
    
    if (drawBox) {
        ofNoFill();
        glLineWidth(2);
        ofSetHexColor(0xFFFFFF);
        ofRect(boxOrigin.x, boxOrigin.y, boxUpdate.x-boxOrigin.x, boxUpdate.y-boxOrigin.y);
    }
}

void ofxBezierSurface::showControls(bool b) {
    bControls = b;
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


//----------------------------------------------------- interaction.
void ofxBezierSurface::mousePressed(ofMouseEventArgs& mouseArgs) {
    float distance;
    ofPoint tmp;
    bool missing = true;

    for (int i=0;i<=cx;i++) {
        for (int j=0;j<=cy;j++) {
            distance = ofDist(inp[i][j].x, inp[i][j].y, mouseArgs.x, mouseArgs.y);
            if (distance < ctrlPntSize) {
                tmp.set(i,j);
                found = true;
            }
        }
    }
    
    if (found) {
        for (int i=0; i<selectedPnts.size(); i++) {
            if (selectedPnts[i].x == tmp.x && selectedPnts[i].y == tmp.y) {
                missing = false;
            }
        }
        if (missing)
            selectedPnts.push_back(tmp);
    }
    else {
        selectedPnts.clear();
        boxOrigin = ofPoint(mouseArgs.x, mouseArgs.y);
        boxUpdate = boxOrigin;
    }
    lastMouse = ofPoint(mouseArgs.x, mouseArgs.y);
}

void ofxBezierSurface::mouseDragged(ofMouseEventArgs& mouseArgs) {
    if (!found) {
        drawBox = true;
        boxUpdate = ofPoint(mouseArgs.x, mouseArgs.y);
        group = true;
    }
    else if (selectedPnts.size() > 0) {
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
    drawBox = false;
    found = false;
    int ix, iy;
    
    if (group) {
        ofRectangle rect = ofRectangle(boxOrigin.x, boxOrigin.y, boxUpdate.x-boxOrigin.x, boxUpdate.y-boxOrigin.y);
        for (int i=0;i<=cx;i++) {
            for (int j=0;j<=cy;j++) {
                ix = inp[i][j].x;
                iy = inp[i][j].y;
                if (ix + ctrlPntSize >= rect.getTopLeft().x && ix - ctrlPntSize <= rect.getBottomRight().x) {
                    if (iy + ctrlPntSize >= rect.getTopLeft().y && iy - ctrlPntSize <= rect.getBottomRight().y)
                        selectedPnts.push_back(ofPoint(i,j));
                }
                
            }
        }
        group = false;
    }
}

void ofxBezierSurface::keyPressed(ofKeyEventArgs& keyArgs) {
    int key = keyArgs.key;

    if (key == OF_KEY_SHIFT) shift = true;
    
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
    if (key == OF_KEY_SHIFT) shift = false;
    else if (key == OF_KEY_UP) up = false;
    else if (key == OF_KEY_DOWN) down = false;
    else if (key == OF_KEY_LEFT) left = false;
    else if (key == OF_KEY_RIGHT) right = false;
}

//----------------------------------------------------- bezier.
double ofxBezierSurface::createSurface() {
    double mui,muj;
    double bi, bj;
    float x,y,z;
    
    // calculate bezier surface
    for (int i=0;i<rx;i++) {
        mui = i / (double)(rx-1);
        for (int j=0;j<ry;j++) {
            muj = j / (double)(ry-1);
            
            outp[i][j].x = 0;
            outp[i][j].y = 0;
            outp[i][j].z = 0;

            for (int ki=0;ki<=cx;ki++) {
                bi = bezierBlend(ki,mui,cx);
                for (int kj=0;kj<=cy;kj++) {
                    bj = bezierBlend(kj,muj,cy);
                    outp[i][j].x += (inp[ki][kj].x * bi * bj);
                    outp[i][j].y += (inp[ki][kj].y * bi * bj);
                    outp[i][j].z += (inp[ki][kj].z * bi * bj);
                }
            }
        }
    }
    
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
