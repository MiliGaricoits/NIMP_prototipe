//
//  scrollBar.h
//  NIMP_Prototipe
//
//  Created by Nicolas Peri on 11/20/15.
//
//

#ifndef __NIMP_Prototipe__scrollBar__
#define __NIMP_Prototipe__scrollBar__

#include <stdio.h>
#include "ofMain.h"
#include "composer.h"

class scrollBar {
    
public:
    
    scrollBar();
    scrollBar(composer* composer, int eventPriority = OF_EVENT_ORDER_AFTER_APP);
//    ~scrollBar();
    
    void setup();
    void draw();
    
    // Events
    
    void    mouseMoved(ofMouseEventArgs &e);
    void    keyPressed(ofKeyEventArgs &e);
    void    mousePressed(ofMouseEventArgs &e);
    void    mouseReleased(ofMouseEventArgs &e);
    void    windowResized(ofResizeEventArgs &e);
    void    mouseDragged(ofMouseEventArgs &e);
    
private:
    void updateScrollBar(ofVec3f diffVec);
    void updateHScrollBar(ofVec3f diffVec);
    
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
    
    composer* composer;
};


#endif /* defined(__NIMP_Prototipe__scrollBar__) */
