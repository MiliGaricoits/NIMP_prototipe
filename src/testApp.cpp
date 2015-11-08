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
    menu->setColorBack(ofxUIColor(140, 140, 140,255));
    menu->setGlobalButtonDimension(30);
    
    new menuItem(menu, "MultiImageButton", "New Patcher", "assets/new_file.png", false);
    menu->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    new menuItem(menu, "MultiImageButton", "Open Patcher", "assets/open_file.png", false);
    new menuItem(menu, "MultiImageButton", "Save Patcher", "assets/save_file.png", false);
    menu->addSpacer(1,30)->setColorFill(ofxUIColor(120, 120, 120,200));
    new menuItem(menu, "MultiImageButton", "Create Node", "assets/node.png", false);
    menu->addSpacer(1,30)->setColorFill(ofxUIColor(120, 120, 120,200));
    new menuItem(menu, "MultiImageButton", "Save Snippet", "assets/save_snippet.png", false);
    new menuItem(menu, "MultiImageButton", "Open Snippet", "assets/open_snippet.png", false);
    menu->addSpacer(1,30)->setColorFill(ofxUIColor(120, 120, 120,200));
    new menuItem(menu, "MultiImageToggle", "Straight Links", "assets/line.png", false);
    new menuItem(menu, "MultiImageToggle", "Curved Links", "assets/curve_line.png", false);
    new menuItem(menu, "MultiImageToggle", "Segmented Links", "assets/path_line.png", true);
    menu->addSpacer(1,30)->setColorFill(ofxUIColor(120, 120, 120,200));
    new menuItem(menu, "MultiImageToggle", "Edit Mode on/off", "assets/edit_mode.png", false);
    
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
    ofBackground(180, 180, 180);
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
    
    if ((key == 'n') || (key == 'N')){
        
        if (newNodeInput != NULL && !newNodeInput->isClicked())
            newNodeInput->setFocus(true);
    }
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
    
    //dlist->setColorBack(ofxUIColor (200,200,200,20));
    node->setColorBack(ofxUIColor (80,80,80,100));
    node->setColorOutlineHighlight(ofxUIColor(150,150,250));
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
    if (name == "Straight Links") {
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
        composer.setLinkType(STRAIGHT_LINKS);
    }
    else if (name == "Curved Links") {
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
        composer.setLinkType(CURVE_LINKS);
    }
    else if (name == "Segmented Links") {
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
        composer.setLinkType(PATH_LINKS);
    }
    else if (name == "Create Node") {
        
        if (newNodeInput == NULL)
            this->createNodeInput((ofGetWidth()/2)-75, ofGetHeight()/2);
        else if (!newNodeInput->isClicked()){
            newNodeInput->getRect()->setX((ofGetWidth()/2)-75);
            newNodeInput->getRect()->setY(ofGetHeight()/2);
            
            newNodeInput->getDropdownList()->getRect()->setX((ofGetWidth()/2)-75);
            newNodeInput->getDropdownList()->getRect()->setY(ofGetHeight()/2);
            
            newNodeInput->setFocus(true);
        }
    }
    else if (name == "Edit Mode on/off") {
        
        if (composer.getEdit())
            composer.setEdit(false);
        else composer.setEdit(true);
    }
}

