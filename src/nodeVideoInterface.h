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
#include "enumVideoState.h"
#include "ofxUISuperCanvas.h"

class nodeVideoInterface {
public:
    
    /* constructor */
    nodeVideoInterface ( ofRectangle* wBox, int* _windowsId);
    
    /* setters / getters */
    void setVideoState( videoState s );
    void setVideoFrame( float frame );
    videoState getVideoState();
    
    /* functions */
    void addButton( string src, string action );
    void draw();
    void stopVideo(int & args);
    void playVideo(int & args);
    
    /* events */
    ofEvent<int> _stop;
    ofEvent<int> _play;
    
private:
    
    ofRectangle    tittleBox;
    ofRectangle    *windowsBox;
    vector<button*> buttons;
    int *windowsId, offSetWidth, height;
    float videoFrame;
    videoState state;
    ofxUISuperCanvas *gui;
    ofxUISlider *slider;
};

#endif
