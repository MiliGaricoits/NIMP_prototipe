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
    
    // nico scrollBar setup
    minVerticalSlider = 0.f;
    maxVerticalSlider = 10.f;
    lowVerticalSlider = 2.f;
    highVerticalSlider = 4.f;
    float widthVerticalSlider = 15.f;
    float heightVerticalSlider = ofGetHeight() - 40.f;
    float widthHorizontalSlider = ofGetWidth() - 40.f;
    float heightHorizontalSlider = 15.f;
    
//    scrollRangeSlider* verticalSlider = new scrollRangeSlider("VERTICAL_RANGE_SLIDER", minVerticalSlider, maxVerticalSlider, lowVerticalSlider, highVerticalSlider, 300.f, 400.f);
//    gui->addWidget(verticalSlider);
//    ofxUIRangeSlider()
    gui->addWidget(new ofxUIRangeSlider("Original", minVerticalSlider, maxVerticalSlider, lowVerticalSlider, highVerticalSlider, widthVerticalSlider, heightVerticalSlider, 100.f, 80.f));
    
    
    scrollRangeSlider* verticalSlider = new scrollRangeSlider("", minVerticalSlider, maxVerticalSlider, lowVerticalSlider, highVerticalSlider, widthVerticalSlider, heightVerticalSlider, ofGetWidth() - 30.f, 10.f);
    scrollRangeSlider* horizontalSlider = new scrollRangeSlider("", minVerticalSlider, maxVerticalSlider, lowVerticalSlider, highVerticalSlider, widthHorizontalSlider, heightHorizontalSlider, 10.f, ofGetHeight() - 30.f);
    
    verticalSlider->setDrawOutline(true);
    horizontalSlider->setDrawOutline(true);
    cout << "height pantalla: " << ofGetHeight() << endl;
    cout << "width pantalla: " << ofGetWidth() << endl;
    
    gui->addWidget(verticalSlider);
    gui->addWidget(horizontalSlider);
    
    // nico ScrollBar setup
    //composer.setup();

}

//-------------------------------------------------------------- LOOP
void testApp::update(){
    composer.update();
    ofSetWindowTitle( ofToString( ofGetFrameRate()));
    
    if (widgetsToDelete.size() > 0) {
        for(auto widget : widgetsToDelete) {
            gui->removeWidget(widget);
        }
        widgetsToDelete.clear();
    }
}


void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    ofSetColor(255,255);

    composer.draw();
}


//-------------------------------------------------------------- EVENTS
void testApp::keyPressed(int key){
    
    vector<ofxUIWidget*> all_nodes;
    
    if (key == OF_KEY_DEL) {
        
        all_nodes = gui->getWidgetsOfType(OFX_UI_WIDGET_TEXTINPUT);
        for (int i = 0; i < all_nodes.size(); i++) {
            if (((ofxUITextInput*) all_nodes[i])->isClicked())
                gui->removeWidget(all_nodes[i]);
        }
    }
    else if ((key == 'n') || (key == 'N')){
        
        all_nodes = gui->getWidgetsOfType(OFX_UI_WIDGET_TEXTINPUT);
        
        int i = 0;
        while (i < all_nodes.size()) {
            
            if (((ofxUITextInput*) all_nodes[i])->isClicked())
                return;
            i++;
        };
        
        textInput *node = new textInput("", "", 150, 20, ofGetMouseX(), ofGetMouseY());
        vector<string> nodes;
        ofxUIDropDownList *dlist = new ofxUIDropDownList("", nodes, 150, ofGetMouseX(), ofGetMouseY());
        
        gui->addWidget(dlist);
        gui->addWidget(node);
        
        dlist->setColorBack(ofxUIColor (255,255,255,0));
        node->setColorBack(ofxUIColor (255,255,255,100));
        node->setDropdownList(dlist);
        
        ofAddListener( node->createNode , this, &testApp::createNode);
    }
    
    // delete memory
    if (!all_nodes.empty()) {
        all_nodes.clear();
        vector<ofxUIWidget*>().swap(all_nodes);
    }
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y ){
}

void testApp::mouseDragged(int x, int y, int button){
}

void testApp::mousePressed(int x, int y, int button){
    cout << "x: " << x << endl;
    cout << "y: " << y << endl;
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

void testApp::createNode(textInputEvent &args){
    
    if (args.type == "ofImage" || args.type == "ofVideoPlayer" || args.type == "ofShader" || args.type == "ofTexture") {
        composer.addPatchFromFile(args.path, args.point);
    }
    else {
        composer.addPatchWithOutFile(args.type, args.point);
    }
    
    ofRemoveListener(((textInput*)args.widget)->createNode , this, &testApp::createNode);
    
    widgetsToDelete.push_back(args.widget);
}


// nico scroll slider
void testApp::updateVerticalSliderValues(){
    
}
