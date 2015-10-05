//
//  textInput.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/3/15.
//
//

#include "textInput.h"
#include "ofxUISuperCanvas.h"

textInput::textInput(string _name, string _textstring, float w, float h, float x, float y, int _size) : ofxUITextInput()
{
    init(_name, _textstring, w, h, x, y, _size);
}

void textInput::setDropdownList(ofxUIDropDownList &dl) {
    this->dropdownList = &dl;
    this->dropdownList->open();
    this->dropdownList->setVisible(false);
}

void textInput::keyPressed(int key) {
    
    ofxUITextInput::keyPressed(key);
    if(clicked)
    {
        if (this->getTextString().length() > 2)
        {
            this->dropdownList->addToggle(ofGetTimestampString());
        }
    }
}