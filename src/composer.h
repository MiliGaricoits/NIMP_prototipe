//
//  composer.h
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/28/15.
//
//

#ifndef __NIMP_Prototipe__composer__
#define __NIMP_Prototipe__composer__

#include <stdio.h>
#include "ofxComposer.h"
#include "enumerations.h"

class composer : public ofxComposer
{
    
public:
    
    composer();
    
    bool getEdit();
    void setLinkType (nodeLinkType type);
    
    void setupScrollBar();
    void updateScrollBar(ofVec3f diffVec);
    void updateHScrollBar(ofVec3f diffVec);
    
    void update();
    void draw();
    
    map<int,patch*> getPatches();
    
private:
    // Events
    void    _mouseMoved(ofMouseEventArgs &e);
    void    _keyPressed(ofKeyEventArgs &e);
    void    _mousePressed(ofMouseEventArgs &e);
    void    _mouseReleased(ofMouseEventArgs &e);
    void    _windowResized(ofResizeEventArgs &e);
    void    _mouseDragged(ofMouseEventArgs &e);
    
    // nico ScrollBar empieza
    /* Display parameters for the panel */
    float margin;
    float scrollBarWidth;
    
    float panelWidth;
    float panelHeight;
    
    // vertical scrollBar
    bool isScrollBarVisible;
    bool mouseOverGrip;
    ofRectangle scrollBarRectangle;
    ofRectangle gripRectangle;
    int mousePreviousY;
    
    // horizontal scrollBar
    bool isHScrollBarVisible;
    bool mouseOverHGrip;
    ofRectangle hScrollBarRectangle;
    ofRectangle hGripRectangle;
    int mousePreviousX;
    
    float KEY_SCROLL_SENSITIVITY = 10.f;
    int SCROLL_TOLERANCE = 10;
    // nico ScrollBar fin
    
};

#endif /* defined(__NIMP_Prototipe__composer__) */
