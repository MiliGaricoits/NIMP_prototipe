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
        if (isDraggingGrip() || isMouseOverGrip()) {
            ofSetColor(230);
        } else {
            ofSetColor(180);
        }
        ofRect(gripRectangle);
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
    
    setDraggingGrip(false); // true when the user is moving the grip
    setMouseOverGrip(false); // true when the mouse is over the grip
    
    updateScrollBar();
}


/************************************** EVENTOS INICIO ******************************/
void composer::_mouseDragged(ofMouseEventArgs &e){
    ofVec3f mouse = ofVec3f(e.x, e.y,0);
    ofVec3f mouseLast = ofVec3f(ofGetPreviousMouseX(),ofGetPreviousMouseY(),0);
    

    if (isScrollBarVisible && isDraggingGrip()) {
        // Move the grip according to the mouse displacement
        int dy = e.y - mousePreviousY;
        mousePreviousY = e.y;
        gripRectangle.y += dy;
        
        // si no estoy en ninguno de los 2 bordes, muevo los patches
        if(!(gripRectangle.y < 0) && !(gripRectangle.getBottom() > scrollBarRectangle.getBottom())){
            ofVec3f aux = ofVec3f(0, mouseLast.y - mouse.y, 0);
            movePatches(aux);
        }
        
        // Check if the grip is still in the scroll bar
        if (gripRectangle.y < 0) {
            gripRectangle.y = 0;
        }
        if (gripRectangle.getBottom() > scrollBarRectangle.getBottom()) {
            gripRectangle.y = scrollBarRectangle.getBottom() - gripRectangle.height;
        }
        
    }
    updateScrollBar();
}

void composer::_mouseReleased(ofMouseEventArgs &e){
    setDraggingGrip(false);
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
}

void composer::_mouseMoved(ofMouseEventArgs &e){
    if (isScrollBarVisible) {
        ofRectangle r = gripRectangle;
        r.translate(margin, margin); // This translation because the coordinates of the grip are relative to the panel, but the mouse position is relative to the screen
        setMouseOverGrip(r.inside(e.x, e.y));
    } else {
        setMouseOverGrip(false);
    }
}

void composer::_keyPressed(ofKeyEventArgs &e){
    // hacer que si es flechita mover el scroll
}

void composer::_windowResized(ofResizeEventArgs &e){
    setupScrollBar();
}

/************************************** EVENTOS FIN ******************************/


void composer::updateScrollBar(){
    // The size of the panel. All the screen except margins
    panelWidth = ofGetWidth() - margin * 2;
    panelHeight = ofGetHeight() - margin * 2;
    
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
    if ( (lowestCoord < 0)  && (highestCoord > panelHeight) ) {
        gripSizeRatioHigh = (float)panelHeight / (panelHeight - (float)lowestCoord);
        gripSizeRatioLow = (float)panelHeight / ( (float)highestCoord );
    } else if ( lowestCoord < 0 ){
        gripSizeRatioHigh = (float)panelHeight / (panelHeight - (float)lowestCoord);
    } else if ( highestCoord > panelHeight ) {
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
