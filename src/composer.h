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
    composer(int eventPriority = OF_EVENT_ORDER_AFTER_APP);
    
    bool getEdit();
    void setLinkType (nodeLinkType type);
    
    void update();
    void customDraw();
    
    map<int,patch*> getPatches();
    
private:
    // Events
    void    _mouseMoved(ofMouseEventArgs &e);
    void    _keyPressed(ofKeyEventArgs &e);
    void    _mousePressed(ofMouseEventArgs &e);
    void    _mouseReleased(ofMouseEventArgs &e);
    void    _windowResized(ofResizeEventArgs &e);
    void    _mouseDragged(ofMouseEventArgs &e);
    
};

#endif /* defined(__NIMP_Prototipe__composer__) */
