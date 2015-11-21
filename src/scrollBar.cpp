//
//  scrollBar.cpp
//  NIMP_Prototipe
//
//  Created by Nicolas Peri on 11/20/15.
//
//

#include "scrollBar.h"
#include "ofMain.h"

scrollBar::scrollBar(){
}

scrollBar::scrollBar(class composer* comp, int eventPriority){
    this->composer = comp;
    //  Event listeners
    //
    ofAddListener(ofEvents().mouseMoved, this, &scrollBar::mouseMoved, eventPriority);
    ofAddListener(ofEvents().mousePressed, this, &scrollBar::mousePressed, eventPriority);
    ofAddListener(ofEvents().mouseReleased, this, &scrollBar::mouseReleased, eventPriority);
    ofAddListener(ofEvents().keyPressed, this, &scrollBar::keyPressed, eventPriority);
    ofAddListener(ofEvents().windowResized, this, &scrollBar::windowResized, eventPriority);
    // nico SrollBar
    ofAddListener(ofEvents().mouseDragged, this, &scrollBar::mouseDragged, eventPriority);
}


void scrollBar::setup(){
    /*
     The "panel" is a frame. This frame contains the displayed images, and the scroll bar.
     The scroll bar contains a "grip". The user can drag the grip with the mouse.
     */
    
    margin = 20.f;            // Distance between the edge of the screen and the panel frame
    scrollBarWidth = 20.f;
    
    // Now two rectangles, for the scroll bar and his grip placements
    // Coordinates are relative to the panel coordinates, not to the screen coordinates
    // This is a first initialisation, but we don't know many things about these placements at this state
    scrollBarRectangle = ofRectangle(ofGetWidth() - scrollBarWidth, MENU_HEIGHT+MENU_TOP_PADDING, scrollBarWidth, 0);
    gripRectangle = ofRectangle(ofGetWidth() - scrollBarWidth, MENU_HEIGHT+MENU_TOP_PADDING, scrollBarWidth, 0);
    
    hScrollBarRectangle = ofRectangle(0, ofGetHeight() - scrollBarWidth, 0, scrollBarWidth);
    hGripRectangle = ofRectangle(0, ofGetHeight() - scrollBarWidth, 0, scrollBarWidth);
    
    composer->setDraggingGrip(false); // true when the user is moving the grip
    mouseOverGrip = false; // true when the mouse is over the grip
    
    mouseOverHGrip = false;
    composer->setDraggingHGrip(false);
    
    updateScrollBar(ofVec3f(0,0,0));
    updateHScrollBar(ofVec3f(0,0,0));
}

void scrollBar::draw(){
    // Add a translation to bring the panel to the good position
    ofPushMatrix();
    // Draw the scroll bar, is needed
    if (isScrollBarVisible) {
        ofSetColor(40);
        ofRect(scrollBarRectangle);
        if (composer->isDraggingGrip() || mouseOverGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(100);
        }
        ofRect(gripRectangle);
    }
    
    if (isHScrollBarVisible) {
        ofSetColor(40);
        ofRect(hScrollBarRectangle);
        if (composer->isDraggingHGrip() || mouseOverHGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(100);
        }
        ofRect(hGripRectangle);
    }
    ofPopMatrix();
}

/****************************** EVENTS ****************************************/
void scrollBar::mouseDragged(ofMouseEventArgs &e){
    ofVec3f mouse = ofVec3f(e.x, e.y,0);
    ofVec3f mouseLast = ofVec3f(ofGetPreviousMouseX(),ofGetPreviousMouseY(),0);
    
    ofVec3f diffVec = ofVec3f(0,0,0);
    
    if (isScrollBarVisible && composer->isDraggingGrip()) {
        diffVec.y = mouseLast.y - mouse.y;
        
        // Move the grip according to the mouse displacement
        int dy = e.y - mousePreviousY;
        mousePreviousY = e.y;
        gripRectangle.y += dy;
        
    }
    if(isHScrollBarVisible && composer->isDraggingHGrip()){
        diffVec.x = mouseLast.x - mouse.x;
        
        // Move the grip according to the mouse displacement
        int dx = e.x - mousePreviousX;
        mousePreviousX = e.x;
        hGripRectangle.x += dx;
    }
    
    updateScrollBar(diffVec);
    updateHScrollBar(diffVec);
}

void scrollBar::mouseReleased(ofMouseEventArgs &e){
    composer->setDraggingGrip(false);
    composer->setDraggingHGrip(false);
}

void scrollBar::mousePressed(ofMouseEventArgs &e){
    // Check if the click occur on the grip
    if (isScrollBarVisible) {
        ofRectangle r = gripRectangle;
        if (r.inside(e.x, e.y)) {
            composer->deactivateAllPatches();
            composer->setDraggingGrip(true);
            mousePreviousY = e.y;
        }
    }
    
    if (isHScrollBarVisible) {
        ofRectangle r = hGripRectangle;
        if (r.inside(e.x, e.y)) {
            composer->deactivateAllPatches();
            composer->setDraggingHGrip(true);
            mousePreviousX = e.x;
        }
    }
}

void scrollBar::mouseMoved(ofMouseEventArgs &e){
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
}

void scrollBar::keyPressed(ofKeyEventArgs &e){
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

void scrollBar::windowResized(ofResizeEventArgs &e){
    this->setup();
}

/****************************** END EVENTS ****************************************/


void scrollBar::updateScrollBar(ofVec3f diffVec){
    
    // TODO: con la flechita no puedo ir a los topes de la barra
    if(diffVec.y != 0){
        if(!(gripRectangle.y < MENU_HEIGHT) && !(gripRectangle.getBottom() > scrollBarRectangle.getBottom())){
            composer->movePatches(diffVec);
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
    int lowestCoord = composer->getPatchesLowestCoord() - MENU_HEIGHT;  // La coordenada mas baja de un patch
    int highestCoord = composer->getPatchesHighestCoord() - MENU_HEIGHT;// La coordenada mas alta de un patch
    
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

void scrollBar::updateHScrollBar(ofVec3f diffVec){
    
    // TODO: con la flechita no puedo ir a los topes de la barra
    if(diffVec.x != 0){
        if(!(hGripRectangle.x < 0) && !(hGripRectangle.getRight() > hScrollBarRectangle.getRight())){
            composer->movePatches(diffVec);
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
    int leftMostCoord = composer->getPatchesLeftMostCoord();  // La coordenada mas baja de un patch
    int rightMostCoord = composer->getPatchesRightMostCoord(); // La coordenada mas alta de un patch
    
    cout << "leftMost: " << leftMostCoord << endl;
    cout << "rightMost: " << rightMostCoord << endl;
    
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