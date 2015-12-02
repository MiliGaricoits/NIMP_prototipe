#include "testApp.h"

//-------------------------------------------------------------- SETING
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetVerticalSync(false);
    
    open_flyout = false;
    
    //*** TRACKPAD SETUP ***//
    
    pad = ofxMultiTouchPad();
    
    
    //*** CAMERA SETUP ***//
    
    cam.setDistance(600);
    cam.disableMouseInput();
    cam.enableOrtho();
    cam.setVFlip(true);
    scale = 1.f;
    
    
    //*** TOP MENU ***//
    
    menu = new ofxUISuperCanvas("menu", 0, MENU_TOP_PADDING, ofGetWidth(), MENU_HEIGHT);
    menu->getCanvasTitle()->ofxUIWidget::setVisible(false);
    menu->setColorBack(ofxUIColor(140, 140, 140,255));
    ofxUISpacer* spacer;
    
    new menuItem(menu, "MultiImageButton", "New Patcher", "assets/new_file.png", false, 35, 20);
    new menuItem(menu, "MultiImageButton", "Open Patcher", "assets/open_file.png", false, 65, 20);
    new menuItem(menu, "MultiImageButton", "Save Patcher", "assets/save_file.png", false, 95, 20);
    spacer = new ofxUISpacer(130, 20, 1,25);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageButton", "Create Node", "assets/node.png", false, 140, 20);
    spacer = new ofxUISpacer(175, 20, 1,25);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageButton", "Save Snippet", "assets/save_snippet.png", false, 185, 20);
    new menuItem(menu, "MultiImageButton", "Open Snippet", "assets/open_snippet.png", false, 215, 20);
    spacer = new ofxUISpacer(250, 20, 1,25);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageToggle", "Straight Links", "assets/line.png", false, 260, 20);
    new menuItem(menu, "MultiImageToggle", "Curved Links", "assets/curve_line.png", false, 290, 20);
    new menuItem(menu, "MultiImageToggle", "Segmented Links", "assets/path_line.png", true, 320, 20);
    spacer = new ofxUISpacer(355, 20, 1,25);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageToggle", "Edit Mode on/off", "assets/edit_mode.png", false, 365, 20);
    ofAddListener(menu->newGUIEvent,this,&testApp::menuEvent);
    
    
    //*** RIGHT MENU ***//
    
    right_menu = new ofxUISuperCanvas("menu", 0, MENU_HEIGHT + MENU_TOP_PADDING, RIGHT_MENU_WIDTH, ofGetHeight() - (MENU_HEIGHT + MENU_TOP_PADDING));
    right_menu->getCanvasTitle()->ofxUIWidget::setVisible(false);
    right_menu->setColorBack(ofxUIColor(140, 140, 140,255));
    
    new menuItem(right_menu, "MultiImageButton", "Zoom In", "assets/zoom_in.png", false, 5, right_menu->getRect()->getHeight()-30);
    new menuItem(right_menu, "MultiImageButton", "Zoom Out", "assets/zoom_out.png", false, 5, right_menu->getRect()->getHeight()-60);
    new menuItem(right_menu, "MultiImageToggle", "Inspect", "assets/open_flyout.png", false, 5, right_menu->getRect()->getHeight()-100);
    
    ofAddListener(right_menu->newGUIEvent,this,&testApp::menuEvent);
    
    
    //*** MAIN CANVAS ***//
    
    gui = new ofxUISuperCanvas("", RIGHT_MENU_WIDTH, MENU_HEIGHT + MENU_TOP_PADDING, ofGetWidth() - RIGHT_MENU_WIDTH, ofGetHeight() - (MENU_HEIGHT +MENU_TOP_PADDING));
    gui->setColorBack(ofxUIColor(255,255,255,0));
    gui->setDraggable(false);
    gui->setOtherSelected(false);
    
    
    //*** COMPOSER AND PATCHES SETUP ***//
    
    composer = new composer::composer(COMPOSER_EVENT_PRIORITY);
    composer->setMainCanvas(gui);
    composer->load("config.xml");
    composer->setLinkType(PATH_LINKS);
    
    composer->setParent(cam);
    map<int,patch*> patches = composer->getPatches();
    for(map<int,patch*>::iterator it = patches.begin(); it != patches.end(); it++ ){
        it->second->setParent(cam);
    }
    
    
    //*** SCROLL BAR SETUP ***//
    
    this->scrollBars = new scrollBar(this->composer, &this->pad, SCROLL_BAR_EVENT_PRIORITY);
    scrollBars->setup();
}

//-------------------------------------------------------------- LOOP
void testApp::update() {
    
    
    if (zoom_in) {
        scale -= 10*SCALE_SENSITIVITY;
        cam.setScale(scale);
        ofVec3f diffVec = ofVec3f(ZOOM_DIFF, ZOOM_DIFF,0);
        scrollBars->updateHScrollBar(diffVec);
        scrollBars->updateScrollBar(diffVec);
    }
    else if (zoom_out) {
        scale += 10*SCALE_SENSITIVITY;
        cam.setScale(scale);
        ofVec3f diffVec = ofVec3f(-ZOOM_DIFF, -ZOOM_DIFF,0);
        scrollBars->updateHScrollBar(diffVec);
        scrollBars->updateScrollBar(diffVec);
    }
    
    scrollBars->update();
    composer->update();
    
    ofSetWindowTitle( ofToString( ofGetFrameRate()));
    
    if (widgetsToDelete.size() > 0) {
        for(auto widget : widgetsToDelete) {
            if (newNodeInput == widget)
                newNodeInput = NULL;
            gui->removeWidget(widget);
        }
        widgetsToDelete.clear();
    }
    
    if (open_flyout){
        right_menu->setWidth(RIGHT_MENU_LONG_WIDTH);
    }
    else right_menu->setWidth(RIGHT_MENU_WIDTH);
    
    right_menu->getWidget("Zoom In")->getRect()->setY(right_menu->getRect()->getHeight()-30);
    right_menu->getWidget("Zoom Out")->getRect()->setY(right_menu->getRect()->getHeight()-60);
    right_menu->getWidget("Inspect")->getRect()->setY(right_menu->getRect()->getHeight()-100);
}


void testApp::draw(){
    
    menu->setWidth(ofGetWidth());
    right_menu->setHeight(ofGetHeight() - (MENU_HEIGHT + MENU_TOP_PADDING));
    
    ofBackground(180, 180, 180);
    ofSetColor(156, 156, 156);
    
    for (int i = 0; i < ofGetWidth(); i+=20) {
        for (int j = 0; j < ofGetHeight(); j+=20) {
            ofCircle(i, j, 1);
        }
    }
    ofSetColor(255,255);
    
    //cam.lookAt(*gui);
    cam.begin();
    //ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    composer->customDraw();
    cam.end();

    scrollBars->draw();
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
    if(hagoZoom){
        ofVec3f mouse = ofVec3f(x, y,0);
        ofVec3f mouseLast = ofVec3f(ofGetPreviousMouseX(),ofGetPreviousMouseY(),0);
        float dify = mouse.y - mouseLast.y;
        scale += dify*SCALE_SENSITIVITY;
        cam.setScale(scale);
    }
}

void testApp::mousePressed(int x, int y, int button){
    if(button == 2){
        hagoZoom = true;
    }
}

void testApp::mouseReleased(int x, int y, int button){
    hagoZoom = false;
    zoom_in = false;
    zoom_out = false;
}

void testApp::windowResized(int w, int h){
}

void testApp::gotMessage(ofMessage msg){
}


void testApp::dragEvent(ofDragInfo dragInfo){
    if( dragInfo.files.size() > 0 ){
		for(int i = 0; i < dragInfo.files.size(); i++){
            composer->addPatchFromFile( dragInfo.files[i], dragInfo.position );
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
        composer->addPatchFromFile(args.path, args.point);
    }
    else {
        composer->addPatchWithOutFile(args.type, args.point);
    }
    
    ofRemoveListener(((textInput*)args.widget)->createNode , this, &testApp::createNode);

    widgetsToDelete.push_back(args.widget); // delete input from canvas
}

void testApp::menuEvent(ofxUIEventArgs &e)
{
    zoom_in = false;
    zoom_out = false;
    
    string name = e.getName();
    if (name == "Straight Links") {
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
        composer->setLinkType(STRAIGHT_LINKS);
    }
    else if (name == "Curved Links") {
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
        composer->setLinkType(CURVE_LINKS);
    }
    else if (name == "Segmented Links") {
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
        composer->setLinkType(PATH_LINKS);
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
        
        if (composer->getEdit())
            composer->setEdit(false);
        else composer->setEdit(true);
    }
    else if (name == "Inspect"){
        if (open_flyout) open_flyout = false;
        else open_flyout = true;
    }
    else if (name == "Zoom In"){
        
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            zoom_in = true;
            scale -= 10*SCALE_SENSITIVITY;
            cam.setScale(scale);
        }
    }
    else if (name == "Zoom Out"){
        
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            zoom_out = true;
            scale += 10*SCALE_SENSITIVITY;
            cam.setScale(scale);
        }
    }
    else if (name == "Save Snippet"){
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            composer->saveSnippet();
        }
    }
    else if (name == "Open Snippet"){
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            composer->loadSnippet();
        }
    }
}

