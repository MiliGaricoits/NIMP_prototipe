//
//  patch.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/31/15.
//
//

#include "patch.h"
#include <GLUT/glut.h>

patch::patch() : ofxPatch() {
    
    selectedLink = -1;
    selectedLinkPath = -1;

    ofAddListener(ofEvents().mousePressed, this, &patch::_mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &patch::_mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &patch::_mouseReleased);
    ofAddListener(ofEvents().keyPressed, this, &patch::_keyPressed);
}

/* ================================================ */
/*                      LOOPS                       */
/* ================================================ */

void patch::update() {
    
    ofxPatch::update();

    if (videoPlayer != NULL){
        videoInterface->setVideoFrame(videoPlayer->getPosition());
    }
}

void patch::draw() {
    
    ofxPatch::draw();
    
    if (bEditMode) {
        
        // Draw video interface
        if (type.compare("ofVideoPlayer") == 0)
            videoInterface->draw();
        
        
        // Draw the links
        //
        for (int i = 0; i < outPut.size(); i++){
            if (outPut[i].to != NULL){
                ofFill();
                ofCircle(outPut[i].pos, 3);
                if (linkType == STRAIGHT_LINKS)
                    ofLine(outPut[i].pos, outPut[i].to->pos);
                else if (linkType == CURVE_LINKS) {
                    ofNoFill();
                    ofBezier(outPut[i].pos.x, outPut[i].pos.y, outPut[i].pos.x+55, outPut[i].pos.y, outPut[i].to->pos.x-55, outPut[i].to->pos.y, outPut[i].to->pos.x, outPut[i].to->pos.y);
                    ofFill();
                }
                else {
                    if (outPut[i].path_coorners.size() > 0) {
                        ofNoFill();
                        for(int j = 0; j < outPut[i].path_coorners.size(); j++){
                            ofCircle( outPut[i].path_coorners[j], 4);
                        }
                    }
                    
                    outPut[i].path_line.clear();
                    outPut[i].path_line.addVertex(outPut[i].pos);
                    if (outPut[i].path_coorners.size() > 0)
                        outPut[i].path_line.addVertices(outPut[i].path_coorners);
                    outPut[i].path_line.addVertex(outPut[i].to->pos);
                    outPut[i].path_line.draw();
                    
                    ofFill();
                }
                
                ofCircle(outPut[i].to->pos, 3);
            }
        }
    }
}


/* ================================================ */
/*                      EVENTS                      */
/* ================================================ */

void patch::_mousePressed(ofMouseEventArgs &e) {
    
    ofxPatch::_mousePressed(e);
    
    // Is over link dot ?
    //
    ofVec3f mouse = ofVec3f(e.x, e.y, 0.0);
    
    if (bEditMode){
        bool overDot = false;
        for (int i = 0; i < outPut.size() and !overDot; i++){
            
            for (int j = 0; j < outPut[i].path_coorners.size(); j++){
                
                if ( ofDist(e.x, e.y, outPut[i].path_coorners[j].x, outPut[i].path_coorners[j].y) <= 10 ){
                    if ((e.button == 2) || (glutGetModifiers() == GLUT_ACTIVE_CTRL)) {
                        outPut[i].path_coorners.erase(outPut[i].path_coorners.begin()+j);
                    }
                    else {
                        selectedLinkPath = j;
                        selectedLink = i;
                    }
                    overDot = true;
                    ofxPatch::setLinkHit(true);
                }
            }
            
            if (!overDot and linkType == PATH_LINKS and outPut.size() > 0){
                vector<ofPoint> coorners = outPut[i].path_line.getVertices();
                int addNew = -1;
                int tolerance = 3;
                
                for (int j = 0; j < coorners.size()-1; j++){
                    int next = (j+1)%coorners.size();
                    
                    if (is_between (mouse.x, coorners[j].x, coorners[j+1].x, tolerance) && is_between (mouse.y, coorners[j].y, coorners[j+1].y, tolerance))
                    {
                        if ((coorners[j+1].y - coorners[j].y) <= tolerance) // Horizontal line.
                        {
                            addNew = j;
                        }
                        
                        const float M = (coorners[j+1].y - coorners[j].y) / (coorners[j+1].x - coorners[j].x); // Slope
                        const float C = -(M * coorners[j].x) + coorners[j].y; // Y intercept
                        
                        // Checking if (x, y) is on the line passing through the end points.
                        if(std::fabs (mouse.y - (M * mouse.x + C)) <= tolerance) {
                            addNew = j;
                        }
                    }
                }
                
                if (addNew >= 0) {
                    ofxPatch::setLinkHit(true);
                    if (outPut[i].path_coorners.size() == 0)
                        outPut[i].path_coorners.push_back(mouse);
                    else if (addNew == 0)
                        outPut[i].path_coorners.insert(outPut[i].path_coorners.begin(), mouse);
                    else
                        outPut[i].path_coorners.insert(outPut[i].path_coorners.begin()+addNew, mouse);
                }
            }
        }
    }
}

void patch::_mouseDragged(ofMouseEventArgs &e) {
    
    if (!gui->getOtherSelected()) {
        ofxPatch::_mouseDragged(e);
        
        ofVec3f mouse = ofVec3f(e.x, e.y,0);
        
        if (bEditMode){
            if (selectedLink >= 0 and selectedLinkPath >= 0) {
                outPut[selectedLink].path_coorners[selectedLinkPath] = mouse;
            }
        }
    }
}

void patch::_mouseReleased(ofMouseEventArgs &e){
    
    ofxPatch::_mouseReleased(e);
    
    selectedLinkPath = -1;
    selectedLink = -1;
    ofxPatch::setLinkHit(false);
}

void patch::_keyPressed(ofKeyEventArgs &e){
    
    ofxPatch::_keyPressed(e);
    
    //if (ofGetKeyPressed(OF_KEY_CONTROL))
        cout << 'key ' << e.key << '\n';
}

void patch::_stopVideo(int &_nId) {
    videoPlayer->stop();
    videoPlayer->firstFrame();
    videoInterface->setVideoState(VIDEO_STOP);
}

void patch::_playVideo(int &_nId) {
    
    if (videoInterface->getVideoState() == VIDEO_PLAY) {
        videoPlayer->stop();
        videoInterface->setVideoState(VIDEO_PAUSE);
    } else {
        videoPlayer->play();
        videoInterface->setVideoState(VIDEO_PLAY);
    }
}


/* ================================================ */
/*       GETTERS | SETTERS | OTHER FUNCTIONS        */
/* ================================================ */

bool patch::loadFile(string _filePath, string _configFile) {
    
    ofxPatch::loadFile(_filePath, _configFile);
    
    if (type == "ofVideoPlayer") {
        videoInterface = new nodeVideoInterface(&box, &nId);
        ofAddListener( videoInterface->_stop , this, &patch::_stopVideo);
        ofAddListener( videoInterface->_play , this, &patch::_playVideo);
    }
}

ofPolyline patch::getCoorners() {
    return textureCorners;
}

void patch::setLinkType(nodeLinkType type) {
    linkType = type;
}


bool patch::is_between (float x, float bound1, float bound2, float tolerance) {
    // Handles cases when 'bound1' is greater than 'bound2' and when
    // 'bound2' is greater than 'bound1'.
    return (((x >= (bound1 - tolerance)) && (x <= (bound2 + tolerance))) ||
            ((x >= (bound2 - tolerance)) && (x <= (bound1 + tolerance))));
}

void patch::setMainCanvas(ofxUISuperCanvas* _gui) {
    this->gui = _gui;
    this->setParent(*this->gui);
}