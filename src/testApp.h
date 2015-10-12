#pragma once

#include "ofMain.h"

#include "ofxComposer.h"
#include "ofxUISuperCanvas.h"
#include "textInput.h"
#include "textInputEvent.h"

class testApp : public ofBaseApp{
    
private:
    static const float ZOOM_UNIT = 0.05f;
    static const float ZOOM_OUT_SCALE = 1.f - ZOOM_UNIT;
    static const float ZOOM_IN_SCALE = 1.f + ZOOM_UNIT;
    float translateX;
    float translateY;
    
    ofx2DCam cam;
    
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
    
    void createNode(textInputEvent &args);
    
    ofxComposer composer;
    ofxUISuperCanvas *gui;
};
