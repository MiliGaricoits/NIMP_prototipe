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
    cam.update();
    composer.update();
    
    ofSetWindowTitle( ofToString( ofGetFrameRate()));
}


void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    ofSetColor(255,255);
    
    float scale = cam.getScale();
//    ofVec3f translate = cam.getTranslation();

    ofPushMatrix();
//    ofVec3f translate = cam.getTranslation();
//    composer.translatePatches(translate);
//        ofTranslate(cam.getTranslation().x, cam.getTranslation().y);
//        ofScale(cam.getScale(), cam.getScale());
//        composer.scalePatches(cam.getScale());
        composer.draw();
    ofPopMatrix();
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
        
        ofAddListener( node->createNode , this, &testApp::createNode);
    }
    
    if (key == 's'){
        composer.scalePatches(ZOOM_IN_SCALE);
    }
    
    if (key == 'd'){
        composer.scalePatches(ZOOM_OUT_SCALE);
    }
    
    if (key == 'w'){
        translateX += 0.5f;
        translateY += 0.5f;
        ofVec3f translate = cam.getTranslation();
        composer.translatePatches(translate);
        //        cout << "Hola, estoy en zoomin" << endl;
                composer.scalePatches(.95f);
    }
    
    if (key == 'e'){
        translateX -= 0.5f;
        translateY -= 0.5f;
        //        cout << "Hola, estoy en zoomin" << endl;
        //        composer.scalePatches(.95f);
    }
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y ){
}

void testApp::mouseDragged(int x, int y, int button){
    cam.mouseDragged(x, y);
    ofVec3f translateVec = cam.getTranslation();
    float scale = cam.getScale();
//    composer.translatePatches(translateVec);
//    composer.scalePatches(scale);
//    composer.translatePatches(translateVec);
    
    
    cout << "mouse coords: " << x << " " << y << endl;
    map<int, ofxPatch*> patches = composer.getPatches();
    for(map<int,ofxPatch*>::iterator it = patches.begin(); it != patches.end(); it++ ){
        ofxPatch* patchAux = it->second;
        cout << "patch: " << patchAux->getId() << endl;
            cout << "\t vertice x rectangulo: " << patchAux->getBox().x << endl;
            cout << "\t vertice y rectangulo: " << patchAux->getBox().y << endl;
            cout << "\t altura rectangulo: " << patchAux->getBox().height << endl;
            cout << "\t ancho rectangulo: " << patchAux->getBox().width << endl;
//            ofVec3f *newPoint = new ofVec3f(it->second->getX() - translateVec.x, it->second->getY() - translateVec.y);
//            ofPoint *point = new ofPoint(x,y);
//            it->second->move(*point);
//            cout << "punto Viejo: " << it->second->getX() << " " << it->second->getY() << endl;
//            cout << "punto Nuevo: " << newPoint->x << " " << newPoint->y << endl;
    }

    cout << endl;

//    cout << "scaled float: " << cam.getScale() << endl;
    
//    map<int,ofxPatch*> patches = composer.getPatches();
//    for(map<int,ofxPatch*>::iterator it = patches.begin(); it != patches.end(); it++ ){
//        ofVec3f *newPoint = new ofVec3f(it->second->getX() - translateVec.x, it->second->getY() - translateVec.y);
//        it->second->move(newPoint);
//        cout << "punto Viejo: " << it->second->getX() << " " << it->second->getY() << endl;
//        cout << "punto Nuevo: " << newPoint->x << " " << newPoint->y << endl;
//    }
    
}

void testApp::mousePressed(int x, int y, int button){
//    cout << "mouse coords: " << x << " " << y << endl;
//    
//    cout << "algun patch hit: " << composer.isAnyPatchHit(x, y) << endl;
//    
//    // print patches coords
//    map<int, ofxPatch*> patches = composer.getPatches();
//    for(map<int,ofxPatch*>::iterator it = patches.begin(); it != patches.end(); it++ ){
//        ofxPatch* patchAux = it->second;
//        cout << "patch: " << patchAux->getId() << endl;
//            cout << "\t vertice x rectangulo: " << patchAux->getBox().x << endl;
//            cout << "\t vertice y rectangulo: " << patchAux->getBox().y << endl;
//            cout << "\t altura rectangulo: " << patchAux->getBox().height << endl;
//            cout << "\t ancho rectangulo: " << patchAux->getBox().width << endl;
//        ofPoint punto = *new ofPoint(patchAux->getBox().x, patchAux->getBox().y);
//        cout << "\t hola: " << patchAux->getSurfaceToScreen(punto) << endl;
//    }
//
//    cout << endl;
//
//    
    if(!composer.isAnyPatchHit(x, y)){
        cam.mousePressed(x, y, button);
    }
}

void testApp::mouseReleased(int x, int y, int button){
    cam.mouseReleased(x, y);
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
    
    if (args.type == "image" || args.type == "video") {
        composer.addPatchFromFile(args.path, args.point);
    }
    else {
        composer.addPatchWithOutFile(args.type, args.point);
    }
    
    gui->removeWidget(args.widget);
}
