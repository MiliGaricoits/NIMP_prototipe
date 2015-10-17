#pragma once

#include "ofMain.h"

#include "ofxComposer.h"
#include "ofxUISuperCanvas.h"
#include "textInput.h"
#include "textInputEvent.h"
#include "scrollRangeSlider.h"

class testApp : public ofBaseApp{
private:
    // nico scroll sliders
    float minVerticalSlider;
    float maxVerticalSlider;
    float lowVerticalSlider;
    float highVerticalSlider;
    void updateVerticalSliderValues();
    
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
    
protected:
    vector<ofxUIWidget*> widgetsToDelete;
};
