//
//  patch.h
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/31/15.
//
//

#ifndef __NIMP_Prototipe__patch__
#define __NIMP_Prototipe__patch__

#include <stdio.h>
#include "ofxPatch.h"
#include "enumerations.h"

class patch : public ofxPatch {
    
public:
    
    patch();
    
    void update();
    void draw();
    
    bool loadFile(string _filePath, string _configFile = "none");
    
    void _stopVideo(int &_nId);
    void _playVideo(int &_nId);
    
    void _mousePressed(ofMouseEventArgs &e);
    void _mouseDragged(ofMouseEventArgs &e);
    void _mouseReleased(ofMouseEventArgs &e);
    
    ofPolyline getCoorners();
    ofRectangle getBox() { return box; };
    void setLinkType(nodeLinkType type);
    
    
private:
    
    nodeVideoInterface *videoInterface;
    nodeLinkType linkType;
};

#endif /* defined(__NIMP_Prototipe__patch__) */
