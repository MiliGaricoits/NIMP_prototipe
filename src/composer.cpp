//
//  composer.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/28/15.
//
//

#include "composer.h"

composer::composer() : ofxComposer() {
    //  Event listeners
    //
    ofAddListener(ofEvents().mouseMoved, this, &composer::_mouseMoved);
    ofAddListener(ofEvents().mousePressed, this, &composer::_mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &composer::_mouseReleased);
    ofAddListener(ofEvents().keyPressed, this, &composer::_keyPressed);
    ofAddListener(ofEvents().windowResized, this, &composer::_windowResized);
    // nico SrollBar
    ofAddListener(ofEvents().mouseDragged, this, &composer::_mouseDragged);
}

/* ================================================ */
/*                      LOOPS                       */
/* ================================================ */

void composer::update(){
    this->ofxComposer::update();
}

void composer::draw(){
    ofPushMatrix();
    ofPushStyle();
        this->ofxComposer::draw();
    ofPopStyle();
    ofPopMatrix();
    // Add a translation to bring the panel to the good position
    ofPushMatrix();
    // Draw the scroll bar, is needed
    if (isScrollBarVisible) {
        ofSetColor(40);
        ofRect(scrollBarRectangle);
        if (isDraggingGrip() || mouseOverGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(100);
        }
        ofRect(gripRectangle);
    }
    
    if (isHScrollBarVisible) {
        ofSetColor(40);
        ofRect(hScrollBarRectangle);
        if (isDraggingHGrip() || mouseOverHGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(100);
        }
        ofRect(hGripRectangle);
    }
    ofPopMatrix();
}

/* ================================================ */
/*                      EVENTS                      */
/* ================================================ */

void composer::_mouseDragged(ofMouseEventArgs &e){
    ofVec3f mouse = ofVec3f(e.x, e.y,0);
    ofVec3f mouseLast = ofVec3f(ofGetPreviousMouseX(),ofGetPreviousMouseY(),0);
    
    ofVec3f diffVec = ofVec3f(0,0,0);

    if (isScrollBarVisible && isDraggingGrip()) {
        diffVec.y = mouseLast.y - mouse.y;
        
        // Move the grip according to the mouse displacement
        int dy = e.y - mousePreviousY;
        mousePreviousY = e.y;
        gripRectangle.y += dy;
        
    }
    if(isHScrollBarVisible && isDraggingHGrip()){
        diffVec.x = mouseLast.x - mouse.x;
        
        // Move the grip according to the mouse displacement
        int dx = e.x - mousePreviousX;
        mousePreviousX = e.x;
        hGripRectangle.x += dx;
    }
    
    updateScrollBar(diffVec);
    updateHScrollBar(diffVec);
    
    ofxComposer::_mouseDragged(e);
}

void composer::_mouseReleased(ofMouseEventArgs &e){
    setDraggingGrip(false);
    setDraggingHGrip(false);
    ofxComposer::_mouseReleased(e);
}

void composer::_mousePressed(ofMouseEventArgs &e){
    // Check if the click occur on the grip
    if (isScrollBarVisible) {
        ofRectangle r = gripRectangle;
        if (r.inside(e.x, e.y)) {
            ofxComposer::deactivateAllPatches();
            setDraggingGrip(true);
            mousePreviousY = e.y;
        }
    }
    
    if (isHScrollBarVisible) {
        ofRectangle r = hGripRectangle;
        if (r.inside(e.x, e.y)) {
            ofxComposer::deactivateAllPatches();
            setDraggingHGrip(true);
            mousePreviousX = e.x;
        }
    }
    
    ofxComposer::_mousePressed(e);
}

void composer::_mouseMoved(ofMouseEventArgs &e){
    if (isScrollBarVisible) {
        ofRectangle r = gripRectangle;
        mouseOverGrip = r.inside(e.x, e.y);
    } else {
        mouseOverGrip = false;
    }
    
    if (isHScrollBarVisible) {
        ofRectangle r = hGripRectangle;
        mouseOverHGrip = r.inside(e.x, e.y);
    } else {
        mouseOverHGrip = false;
    }
    
    ofxComposer::_mouseMoved(e);
}

void composer::_keyPressed(ofKeyEventArgs &e){
    // hacer que si es flechita mover el scroll
    ofVec3f diffVec = ofVec3f(0, 0, 0);
    if (isScrollBarVisible) {
        if (e.key == OF_KEY_UP ){
            diffVec.y = KEY_SCROLL_SENSITIVITY;
            gripRectangle.y -= KEY_SCROLL_SENSITIVITY;
        } else if (e.key == OF_KEY_DOWN){
            diffVec.y = -KEY_SCROLL_SENSITIVITY;
            gripRectangle.y += KEY_SCROLL_SENSITIVITY;
        }
    }
    if(isHScrollBarVisible){
        if (e.key == OF_KEY_LEFT ){
            diffVec.x = KEY_SCROLL_SENSITIVITY;
            hGripRectangle.x -= KEY_SCROLL_SENSITIVITY;
        } else if (e.key == OF_KEY_RIGHT ){
            diffVec.x = -KEY_SCROLL_SENSITIVITY;
            hGripRectangle.x += KEY_SCROLL_SENSITIVITY;
        }
    }
    updateScrollBar(diffVec);
    updateHScrollBar(diffVec);
    
    
    if (e.key == 'j') {
        loadSnippet("snippet.xml");
    }
    if (e.key == 'k') {
//        for(map<int,patch*>::iterator it = patches.begin(); it != patches.end(); it++ ){
//            cout << "id: " << it->second->getId() << endl;
//        }
        saveSnippet("snippetSave.xml");
    }
    
    ofxComposer::_keyPressed(e);
}

void composer::_windowResized(ofResizeEventArgs &e){
    setupScrollBar();
    ofxComposer::_windowResized(e);
}

/* ================================================ */
/*       GETTERS | SETTERS | OTHER FUNCTIONS        */
/* ================================================ */

bool composer::getEdit(){
    return this->bEditMode;
}

void composer::setLinkType (nodeLinkType type) {
    for(map<int,patch*>::iterator it = this->patches.begin(); it != this->patches.end(); it++ ){
        it->second->setLinkType(type);
    }
}

void composer::setupScrollBar(){
    /*
     The "panel" is a frame. This frame contains the displayed images, and the scroll bar.
     The scroll bar contains a "grip". The user can drag the grip with the mouse.
     */
    
    margin = 20.f;            // Distance between the edge of the screen and the panel frame
    scrollBarWidth = 20.f;
    
    // Now two rectangles, for the scroll bar and his grip placements
    // Coordinates are relative to the panel coordinates, not to the screen coordinates
    // This is a first initialisation, but we don't know many things about these placements at this state
    scrollBarRectangle = ofRectangle(ofGetWidth() - scrollBarWidth, MENU_HEIGHT, scrollBarWidth, 0);
    gripRectangle = ofRectangle(ofGetWidth() - scrollBarWidth, MENU_HEIGHT, scrollBarWidth, 0);
    
    hScrollBarRectangle = ofRectangle(0, ofGetHeight() - scrollBarWidth, 0, scrollBarWidth);
    hGripRectangle = ofRectangle(0, ofGetHeight() - scrollBarWidth, 0, scrollBarWidth);
    
    setDraggingGrip(false); // true when the user is moving the grip
    mouseOverGrip = false; // true when the mouse is over the grip
    
    mouseOverHGrip = false;
    setDraggingHGrip(false);
    
    updateScrollBar(ofVec3f(0,0,0));
    updateHScrollBar(ofVec3f(0,0,0));
}

void composer::updateScrollBar(ofVec3f diffVec){
    
    // TODO: con la flechita no puedo ir a los topes de la barra
    if(diffVec.y != 0){
        if(!(gripRectangle.y < MENU_HEIGHT) && !(gripRectangle.getBottom() > scrollBarRectangle.getBottom())){
            movePatches(diffVec);
        }
        
        // Check if the grip is still in the scroll bar
        if (gripRectangle.y < MENU_HEIGHT) {
            gripRectangle.y = 0;
        }
        if (gripRectangle.getBottom() > scrollBarRectangle.getBottom()) {
            gripRectangle.y = scrollBarRectangle.getBottom() - gripRectangle.height;
        }
    }
    
    
    // The size of the panel. All the screen except margins
    panelWidth = ofGetWidth() - margin;
    panelHeight = ofGetHeight() - margin - MENU_HEIGHT;
    
    gripRectangle.x = scrollBarRectangle.x;                   // Also adjust the grip x coordinate
    int lowestCoord = getPatchesLowestCoord() - MENU_HEIGHT;  // La coordenada mas baja de un patch
    int highestCoord = getPatchesHighestCoord() - MENU_HEIGHT;// La coordenada mas alta de un patch
    
    // Muestro la scrollBar
    isScrollBarVisible = true;
    // La altura del scroll bar = a la altura de la pantalla
    scrollBarRectangle.height = panelHeight;
    
    // estos ratios son la proporcion de lo que hay que dibujar que esta por encima y por debajo de lo que se muestra
    // al ser ratio, van de 0 a 1, y calculo dependiendo el caso
    float gripSizeRatioLow = 1.f;
    float gripSizeRatioHigh = 1.f;
    if ( (lowestCoord + SCROLL_TOLERANCE < 0)  && (highestCoord - SCROLL_TOLERANCE > panelHeight) ) {
        gripSizeRatioHigh = (float)panelHeight / (panelHeight - (float)lowestCoord);
        gripSizeRatioLow = (float)panelHeight / ( (float)highestCoord );
    } else if ( lowestCoord + SCROLL_TOLERANCE < 0 ){
        gripSizeRatioHigh = (float)panelHeight / (panelHeight - (float)lowestCoord);
    } else if ( highestCoord - SCROLL_TOLERANCE > panelHeight ) {
        gripSizeRatioLow = (float)panelHeight / ( (float)highestCoord );
    }
    
    
    // La altura del grip es el panel por los ratios fuera de la pantalla
    gripRectangle.height = panelHeight * gripSizeRatioLow * gripSizeRatioHigh;
    
    // La 'y' del grip esta en la scrollbar por la relacion de lo que queda por arriba de la pantalla
    gripRectangle.y = (1-gripSizeRatioHigh)*scrollBarRectangle.height + MENU_HEIGHT;
    
    // Si las alturas del grip y del scroll son iguales, es porque tengo todo a la vista
    // hago que la resta sea menor a 2 para dejar un margen, si no, queda a veces la barra cuando no es necesario
    if( (scrollBarRectangle.height - gripRectangle.height) < 2 ){
        isScrollBarVisible = false;
    }

}

void composer::updateHScrollBar(ofVec3f diffVec){
    
    // TODO: con la flechita no puedo ir a los topes de la barra
    if(diffVec.x != 0){
        if(!(hGripRectangle.x < 0) && !(hGripRectangle.getRight() > hScrollBarRectangle.getRight())){
            movePatches(diffVec);
        }
        
//      Check if the grip is still in the scroll bar
        if (hGripRectangle.x < 0) {
            hGripRectangle.x = 0;
        }
        if (hGripRectangle.getRight() > hScrollBarRectangle.getRight()) {
            hGripRectangle.x = hScrollBarRectangle.getRight() - hGripRectangle.width;
        }
    }
    
    
    // The size of the panel. All the screen except margins
    panelWidth = ofGetWidth() - margin;
    panelHeight = ofGetHeight() - margin;
    
    hGripRectangle.y = hScrollBarRectangle.y; // Also adjust the grip x coordinate
    int leftMostCoord = getPatchesLeftMostCoord();  // La coordenada mas baja de un patch
    int rightMostCoord = getPatchesRightMostCoord(); // La coordenada mas alta de un patch
    
    // Muestro la scrollBar
    isHScrollBarVisible = true;
    // La altura del scroll bar = a la altura de la pantalla
    hScrollBarRectangle.width = panelWidth;
    
    // estos ratios son la proporcion de lo que hay que dibujar que esta por encima y por debajo de lo que se muestra
    // al ser ratio, van de 0 a 1, y calculo dependiendo el caso
    float gripSizeRatioLeft = 1.f;
    float gripSizeRatioRight = 1.f;
    if ( (leftMostCoord + SCROLL_TOLERANCE < 0)  && (rightMostCoord - SCROLL_TOLERANCE > panelWidth) ) {
        gripSizeRatioRight = (float)panelWidth / (panelWidth - (float)leftMostCoord);
        gripSizeRatioLeft = (float)panelWidth / ( (float)rightMostCoord );
    } else if ( leftMostCoord + SCROLL_TOLERANCE < 0 ){
        gripSizeRatioRight = (float)panelWidth / (panelWidth - (float)leftMostCoord);
    } else if ( rightMostCoord - SCROLL_TOLERANCE > panelWidth ) {
        gripSizeRatioLeft = (float)panelWidth / ( (float)rightMostCoord );
    }
    
    
    // La altura del grip es el panel por los ratios fuera de la pantalla
    hGripRectangle.width = panelWidth * gripSizeRatioLeft * gripSizeRatioRight;
    
    
    // La 'x' del grip esta en la scrollbar por la relacion de lo que queda por la izquierda de la pantalla
    hGripRectangle.x = (1-gripSizeRatioRight)*hScrollBarRectangle.width;
    
    // Si las alturas del grip y del scroll son iguales, es porque tengo todo a la vista
    // hago que la resta sea menor a 2 para dejar un margen, si no, queda a veces la barra cuando no es necesario
    if( (hScrollBarRectangle.width - hGripRectangle.width) < 2 ){
        isHScrollBarVisible = false;
    }
}



/************************************** EMPIEZA SNIPPETS *********************************/
void ofxComposer::loadSnippet(string snippetName) {
    ofxXmlSettings XML;
    
    int previousPatchesSize = patches.size();
    deactivateAllPatches();
    
    if (XML.loadFile(snippetName)){
        
#ifdef USE_OFXGLEDITOR
        editor.setup(XML.getValue("general:console:font", "menlo.ttf"));
#endif
        int totalPatchs = XML.getNumTags("surface");
        
        // Load each surface present on the xml file
        //
        for(int i = 0; i < totalPatchs ; i++){
            patch *nPatch = new patch();
            bool loaded = nPatch->loadSnippetPatch(snippetName, i, previousPatchesSize);
            if (loaded){
                
#ifdef USE_OFXGLEDITOR
                if (nPatch->getType() == "ofxGLEditor"){
                    ofLog(OF_LOG_NOTICE,"ofxComposer: ofxGLEditor loaded");
                    nPatch->setTexture( editorFbo.getTextureReference(), 0);
                    bGLEditorPatch = true;
                }
#endif
                // Listen to close bottom on the titleBar
                //
                ofAddListener( nPatch->title->close , this, &ofxComposer::closePatch);
                
                // Insert the new patch into the map
                //
                patches[nPatch->getId()] = nPatch;
                
                //mili
                nPatch->setMainCanvas(this->gui);
                //
                
                nPatch->bActive = true;
            }
        }
        
        // Load links between Patchs
        //
        for(int i = 0; i < totalPatchs ; i++){
            if (XML.pushTag("surface", i)){
                int fromID = XML.getValue("id", -1);
                
                if (XML.pushTag("out")){
                    
                    int totalLinks = XML.getNumTags("dot");
                    for(int j = 0; j < totalLinks ; j++){
                        
                        if (XML.pushTag("dot",j)){
                            int toID = XML.getValue("to", 0);
                            int nTex = XML.getValue("tex", 0);
                            
                            // If everything goes ok "i" will match the position of the vector
                            // with the position on the XML, in the same place of the vector array
                            // defined on the previus loop
                            //
                            connect( fromID + previousPatchesSize, toID + previousPatchesSize, nTex);
                            
                            XML.popTag();
                        }
                    }
                    XML.popTag();
                }
                XML.popTag();
            }
        }
    }
}


bool ofxComposer::saveSnippet(string snippetName) {
    bool saveOk = true;
    
    ofxXmlSettings XML;
    
    bool a;
    bool b;
    // Delete and create xml file
    if (XML.loadFile(snippetName)) {
        XML.clear();
//        a = XML.saveFile();
    } else {
        b = XML.saveFile(snippetName);
        XML.loadFile(snippetName);
    }
    
    
    map<int,int> idMap;// = new map<int, int>;
    int idAux = 0;
    for(map<int,patch*>::iterator it = patches.begin(); it != patches.end(); it++ ){
        if(it->second->bActive) {
            idMap[it->second->getId()] = idAux;
            idAux++;
        }
    }
    for(map<int,patch*>::iterator it = patches.begin(); it != patches.end(); it++ ){
        saveOk = saveOk && it->second->saveSnippetPatch(snippetName, idMap, XML);
    }
    
    XML.saveFile(snippetName);
    
    return saveOk;
}
/*****************************************************************************************/










