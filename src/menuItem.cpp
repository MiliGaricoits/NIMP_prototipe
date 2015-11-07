//
//  menuItem.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 11/7/15.
//
//

#include "menuItem.h"

menuItem::menuItem(ofxUISuperCanvas* menu, string type, string name, string img, bool active) {
    
    this->label = name;
    this->counter = 0;
    
    if (type == "MultiImageButton") {
        ofxUIMultiImageButton* button = menu->addMultiImageButton(name, img, active);
        
        this->setPos(button->getRect()->getX(), button->getRect()->getY());
        this->setSize(button->getRect()->getWidth(), button->getRect()->getHeight());
    }
    else if (type == "MultiImageToggle") {
        ofxUIMultiImageToggle* toggle = menu->addMultiImageToggle(name, img, active);
        
        this->setPos(toggle->getRect()->getX(), toggle->getRect()->getY());
        this->setSize(toggle->getRect()->getWidth(), toggle->getRect()->getHeight());
    }
    
    enableMouseEvents();
}

void menuItem::draw() {
        
    if(isMouseOver()) {

        if (counter < 70)
            counter++;
        else ofDrawBitmapStringHighlight(label, ofGetMouseX()+10, ofGetMouseY()+30);
    }
    else {
        counter = 0;
    }
}
