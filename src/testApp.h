#pragma once

#include "ofMain.h"

#include "ofxComposer.h"
#include "ofxUISuperCanvas.h"
#include "textInput.h"
#include "textInputEvent.h"
#include "enumerations.h"
#include "menuItem.h"
#include "scrollBar.h"
#include "ofxMultiTouchPad.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    void createNodeInput(float _x = ofGetMouseX(), float _y = ofGetMouseY());
    void createNode(textInputEvent &args);
    void menuEvent(ofxUIEventArgs &e);
    
    ofxComposer* composer;
    
    bool hagoZoom;
    float scale;
    
protected:
    vector<ofxUIWidget*> widgetsToDelete;
    textInput* newNodeInput;
    
    ofxUISuperCanvas* gui;
    ofxUISuperCanvas* menu;
    ofxUISuperCanvas* right_menu;
    
    ofEasyCam cam;
    
    ofxMultiTouchPad pad;
    
    scrollBar* scrollBars;
    
private:
    
    bool open_flyout;
    bool zoom_in, zoom_out;

};
