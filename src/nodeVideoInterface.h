//
//  nodeVideoInterface.h
//  ofxComposerTest
//
//  Created by Mili Garicoits on 9/8/15.
//
//

#ifndef ofxComposerTest_nodeVideoInterface_h
#define ofxComposerTest_nodeVideoInterface_h

#include "ofMain.h"
#include "button.h"

class nodeVideoInterface {
public:
    
    nodeVideoInterface ( ofRectangle* wBox, int* _windowsId);
    
    void addButton( string src, string action );
    void draw();
    void stopVideo(int & args);
    
    ofEvent<int> _stop;
    
private:
    
    ofRectangle    tittleBox;
    ofRectangle    *windowsBox;
    vector<button*> buttons;
    int *windowsId, offSetWidth, height;
};

#endif
