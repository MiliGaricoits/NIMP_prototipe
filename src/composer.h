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
    float gap;
    float margin;
    float scrollBarWidth;
    
    float panelWidth;
    float panelHeight;
    bool isScrollBarVisible;
    ofRectangle scrollBarRectangle;
    ofRectangle gripRectangle;
    int mousePreviousY;
    
    float KEY_SCROLL_SENSITIVITY = 5.f;
    // nico ScrollBar fin
    
public:
    
    composer();
    
    void setLinkType (nodeLinkType type);
    void setupScrollBar();
    void updateScrollBar(ofVec3f diffVec);
    void update();
    void draw();
    
};

#endif /* defined(__NIMP_Prototipe__composer__) */
