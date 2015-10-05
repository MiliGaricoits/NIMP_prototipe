#include "testApp.h"

//-------------------------------------------------------------- SETING
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetVerticalSync(false);
    
    composer.load("config.xml");
    gui = new ofxUISuperCanvas("", 0, 0, ofGetWidth(), ofGetHeight());
    gui->setColorBack(ofxUIColor(255,255,255,0));
    gui->setDraggable(false);
    
    // nico ScrollBar setup
    //composer.setup();
}

//-------------------------------------------------------------- LOOP
void testApp::update(){
    composer.update();
    
    ofSetWindowTitle( ofToString( ofGetFrameRate()));
}


void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    ofSetColor(255,255);

    composer.draw();
}


//-------------------------------------------------------------- EVENTS
void testApp::keyPressed(int key){
    
    if (key == 'n'){
        textInput *node = new textInput("My node", "", 150, 20, ofGetMouseX(), ofGetMouseY());
        vector<string> nodes;
        ofxUIDropDownList *dlist = new ofxUIDropDownList("", nodes, 150, ofGetMouseX(), ofGetMouseY());
        
        gui->addWidget(dlist);
        gui->addWidget(node);
        
        dlist->setColorBack(ofxUIColor (255,255,255,0));
        node->setColorBack(ofxUIColor (255,255,255,100));
        node->setDropdownList(*dlist);
    }
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y ){
}

void testApp::mouseDragged(int x, int y, int button){
}

void testApp::mousePressed(int x, int y, int button){
  
}

void testApp::mouseReleased(int x, int y, int button){
    
}

void testApp::windowResized(int w, int h){
    
}

void testApp::gotMessage(ofMessage msg){
}


void testApp::dragEvent(ofDragInfo dragInfo){
    if( dragInfo.files.size() > 0 ){
		for(int i = 0; i < dragInfo.files.size(); i++){
            composer.addPatchFromFile( dragInfo.files[i], dragInfo.position );
		}
	}
}