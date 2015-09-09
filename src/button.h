//
//  button.h
//  ofxComposerTest
//
//  Created by Mili Garicoits on 9/8/15.
//
//

#ifndef ofxComposerTest_button_h
#define ofxComposerTest_button_h

#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"

class button : public ofxMSAInteractiveObject {

public:
    
    button(string src, int width, int height, string action);
    void draw(int x, int y);
    
private:
    
    ofImage btnImg;
    int width, height;
    string action;
};

#endif
