#include "testApp.h"

//-------------------------------------------------------------- SETING
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetVerticalSync(false);
    
    gui = new ofxUISuperCanvas("", 0, 15, ofGetWidth(), ofGetHeight());
    gui->setColorBack(ofxUIColor(255,255,255,0));
    gui->setDraggable(false);
    gui->setOtherSelected(false);
    
    composer.setMainCanvas(gui);
    composer.load("config.xml");
    composer.setLinkType(PATH_LINKS);
    
    menu = new ofxUISuperCanvas("menu", 0, -15, ofGetWidth(), 60);
    menu->getCanvasTitle()->ofxUIWidget::setVisible(false);
    menu->setGlobalButtonDimension(30);
    menu->addImageButton("create_node", "assets/node.png", false);
    menu->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    menu->addSpacer(1,30);
    menu->addImageButton("save_snippet", "assets/save_snippet.png", false);
    menu->addImageButton("open_snippet", "assets/open_snippet.png", false);
    menu->addSpacer(1,30);
    menu->addImageToggle("straight_links", "assets/line.png", false);
    menu->addImageToggle("curved_links", "assets/curve_line.png", false);
    menu->addImageToggle("path_links", "assets/path_line.png", true);
    
    ofAddListener(menu->newGUIEvent,this,&testApp::menuEvent);

    
    // nico ScrollBar setup
    composer.setupScrollBar();
}

//-------------------------------------------------------------- LOOP
void testApp::update(){
    composer.update();
    ofSetWindowTitle( ofToString( ofGetFrameRate()));
    
    if (widgetsToDelete.size() > 0) {
        for(auto widget : widgetsToDelete) {
            if (newNodeInput == widget)
                newNodeInput = NULL;
            gui->removeWidget(widget);
        }
        widgetsToDelete.clear();
    }
}


void testApp::draw(){
    
    menu->setWidth(ofGetWidth());
    
    //ofBackgroundGradient(ofColor::gray, ofColor::black);
    //ofBackground(247, 242, 225);
    ofBackground(170, 170, 170);
    ofSetColor(156, 156, 156);
    
    for (int i = 0; i < ofGetWidth(); i+=20) {
        for (int j = 0; j < ofGetHeight(); j+=20) {
            ofCircle(i, j, 1);
        }
    }
    ofSetColor(255,255);

    composer.draw();
}


//-------------------------------------------------------------- EVENTS
void testApp::keyPressed(int key){
    
    if (key == OF_KEY_DEL) {
        
        if (((ofxUITextInput*) newNodeInput)->isClicked()) {
            gui->removeWidget(newNodeInput);
            newNodeInput = NULL;
        }
    }
    else if ((key == 'n') || (key == 'N')){
        
        if (newNodeInput == NULL)
            this->createNodeInput();
        else if (!newNodeInput->isClicked()){
            newNodeInput->getRect()->setX(ofGetMouseX());
            newNodeInput->getRect()->setY(ofGetMouseY());
            
            newNodeInput->getDropdownList()->getRect()->setX(ofGetMouseX());
            newNodeInput->getDropdownList()->getRect()->setY(ofGetMouseY());
        }
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


void testApp::createNodeInput(float _x, float _y){
    
    textInput *node = new textInput("", "", 150, 20, _x, _y);
    vector<string> nodes;
    ofxUIDropDownList *dlist = new ofxUIDropDownList("", nodes, 150, _x, _y);
    
    gui->addWidget(dlist);
    gui->addWidget(node);
    
    dlist->setColorBack(ofxUIColor (255,255,255,0));
    node->setColorBack(ofxUIColor (255,255,255,100));
    node->setDropdownList(dlist);
    
    ofAddListener( node->createNode , this, &testApp::createNode);
    
    newNodeInput = node;
}

void testApp::createNode(textInputEvent &args){
    
    if (args.type == "ofImage" || args.type == "ofVideoPlayer" || args.type == "ofShader" || args.type == "ofTexture") {
        composer.addPatchFromFile(args.path, args.point);
    }
    else {
        composer.addPatchWithOutFile(args.type, args.point);
    }
    
    ofRemoveListener(((textInput*)args.widget)->createNode , this, &testApp::createNode);

    widgetsToDelete.push_back(args.widget); // delete input from canvas
}

void testApp::menuEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
    if (name == "straight_links") {
        ((ofxUIImageToggle*)menu->getWidget("straight_links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("path_links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("curved_links"))->setValue(false);
        composer.setLinkType(STRAIGHT_LINKS);
    }
    else if (name == "curved_links") {
        ((ofxUIImageToggle*)menu->getWidget("curved_links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("path_links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("straight_links"))->setValue(false);
        composer.setLinkType(CURVE_LINKS);
    }
    else if (name == "path_links") {
        ((ofxUIImageToggle*)menu->getWidget("path_links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("curved_links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("straight_links"))->setValue(false);
        composer.setLinkType(PATH_LINKS);
    }
    else if (name == "create_node") {
        
        if (newNodeInput == NULL)
            this->createNodeInput((ofGetWidth()/2)-75, ofGetHeight()/2);
        else if (!newNodeInput->isClicked()){
            newNodeInput->getRect()->setX((ofGetWidth()/2)-75);
            newNodeInput->getRect()->setY(ofGetHeight()/2);
            
            newNodeInput->getDropdownList()->getRect()->setX((ofGetWidth()/2)-75);
            newNodeInput->getDropdownList()->getRect()->setY(ofGetHeight()/2);
        }
    }
}

