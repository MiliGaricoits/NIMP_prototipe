//
//  button.cpp
//  ofxComposerTest
//
//  Created by Mili Garicoits on 9/9/15.
//
//

#include "button.h"

#define		OVER_COLOR		0x00FF00

button::button(string src, int width, int height, string action) {
    
    this->btnImg.loadImage(src);
    this->width = width;
    this->height = height;
    this->action = action;
    
    enableMouseEvents();
    
};

void button::draw(int x, int y) {
    
    if(isMouseOver())
        ofSetHexColor(OVER_COLOR);
    this->btnImg.draw(x,y, this->width, this->height);
}