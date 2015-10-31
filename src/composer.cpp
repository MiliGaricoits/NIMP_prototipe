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

void composer::update(){
    this->ofxComposer::update();
}
void composer::draw(){
    this->ofxComposer::draw();
    // Add a translation to bring the panel to the good position
    ofPushMatrix();
    ofTranslate(margin, margin, 0);
    // Draw the scroll bar, is needed
    if (isScrollBarVisible) {
        ofSetColor(110);
        ofRect(scrollBarRectangle);
        if (isDraggingGrip() || mouseOverGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(180);
        }
        ofRect(gripRectangle);
    }
    
    if (isHScrollBarVisible) {
        ofSetColor(110);
        ofRect(hScrollBarRectangle);
        if (isDraggingHGrip() || mouseOverHGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(180);
        }
        ofRect(hGripRectangle);
    }
    ofPopMatrix();
}

void composer::setLinkType (nodeLinkType type) {
    for(map<int,ofxPatch*>::iterator it = this->patches.begin(); it != this->patches.end(); it++ ){
        it->second->setLinkType(type);
    }
}

void composer::setupScrollBar(){
    /*
     The "panel" is a frame. This frame contains the displayed images, and the scroll bar.
     The scroll bar contains a "grip". The user can drag the grip with the mouse.
     */
    
    gap = 10.f;               // Distance between rectangles, and between rectangles and scroll bar
    margin = 20.f;            // Distance between the edge of the screen and the panel frame
    scrollBarWidth = 20.f;
    
    // Now two rectangles, for the scroll bar and his grip placements
    // Coordinates are relative to the panel coordinates, not to the screen coordinates
    // This is a first initialisation, but we don't know many things about these placements at this state
    scrollBarRectangle = ofRectangle(ofGetWidth() - (margin*2) - scrollBarWidth, 0, scrollBarWidth, 0);
    gripRectangle = ofRectangle(ofGetWidth() - (margin*2) - scrollBarWidth, 0, scrollBarWidth, 0);
    
    hScrollBarRectangle = ofRectangle(0, ofGetHeight() - (margin*2) - scrollBarWidth, 0, scrollBarWidth);
    hGripRectangle = ofRectangle(0, ofGetHeight() - (margin*2) - scrollBarWidth, 0, scrollBarWidth);
    
    setDraggingGrip(false); // true when the user is moving the grip
    mouseOverGrip = false; // true when the mouse is over the grip
    
    mouseOverHGrip = false;
    setDraggingHGrip(false);
    
    updateScrollBar(ofVec3f(0,0,0));
    updateHScrollBar(ofVec3f(0,0,0));
}


/************************************** EVENTOS INICIO ******************************/
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
}

void composer::_mouseReleased(ofMouseEventArgs &e){
    setDraggingGrip(false);
    setDraggingHGrip(false);
}

void composer::_mousePressed(ofMouseEventArgs &e){
    // Check if the click occur on the grip
    if (isScrollBarVisible) {
        ofRectangle r = gripRectangle;
        r.translate(margin, margin); // This translation because the coordinates of the grip are relative to the panel, but the mouse position is relative to the screen
        if (r.inside(e.x, e.y)) {
            setDraggingGrip(true);
            mousePreviousY = e.y;
        }
    }
    
    if (isHScrollBarVisible) {
        ofRectangle r = hGripRectangle;
        r.translate(margin, margin); // This translation because the coordinates of the grip are relative to the panel, but the mouse position is relative to the screen
        if (r.inside(e.x, e.y)) {
            setDraggingHGrip(true);
            mousePreviousX = e.x;
        }
    }
}

void composer::_mouseMoved(ofMouseEventArgs &e){
    if (isScrollBarVisible) {
        ofRectangle r = gripRectangle;
        r.translate(margin, margin); // This translation because the coordinates of the grip are relative to the panel, but the mouse position is relative to the screen
        mouseOverGrip = r.inside(e.x, e.y);
    } else {
        mouseOverGrip = false;
    }
    
    if (isHScrollBarVisible) {
        ofRectangle r = hGripRectangle;
        r.translate(margin, margin); // This translation because the coordinates of the grip are relative to the panel, but the mouse position is relative to the screen
        mouseOverHGrip = r.inside(e.x, e.y);
    } else {
        mouseOverHGrip = false;
    }
    
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
}

void composer::_windowResized(ofResizeEventArgs &e){
    setupScrollBar();
}

/************************************** EVENTOS FIN ******************************/


void composer::updateScrollBar(ofVec3f diffVec){
    
    // TODO: con la flechita no puedo ir a los topes de la barra
    if(diffVec.y != 0){
        if(!(gripRectangle.y < 0) && !(gripRectangle.getBottom() > scrollBarRectangle.getBottom())){
            movePatches(diffVec);
        }
        
        // Check if the grip is still in the scroll bar
        if (gripRectangle.y < 0) {
            gripRectangle.y = 0;
        }
        if (gripRectangle.getBottom() > scrollBarRectangle.getBottom()) {
            gripRectangle.y = scrollBarRectangle.getBottom() - gripRectangle.height;
        }
    }
    
    
    // The size of the panel. All the screen except margins
    panelWidth = ofGetWidth() - margin * 3;
    panelHeight = ofGetHeight() - margin * 3;
    
    gripRectangle.x = scrollBarRectangle.x; // Also adjust the grip x coordinate
    int lowestCoord = getPatchesLowestCoord();  // La coordenada mas baja de un patch
    int highestCoord = getPatchesHighestCoord(); // La coordenada mas alta de un patch
    
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
    gripRectangle.y = (1-gripSizeRatioHigh)*scrollBarRectangle.height;
    
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
    panelWidth = ofGetWidth() - margin * 3;
    panelHeight = ofGetHeight() - margin * 3;
    
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










