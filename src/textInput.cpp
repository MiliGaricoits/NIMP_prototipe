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
    this->dropdownList->setAutoClose(true);
    
    ofAddListener(((ofxUISuperCanvas*) this->dropdownList->getCanvasParent())->newGUIEvent,this,&textInput::guiEvent);
}

void textInput::keyPressed(int key) {
    
    ofxUITextInput::keyPressed(key);
    if(clicked)
    {
        if (this->getTextString().length() > 2)
        {
            this->dropdownList->addToggle(ofGetTimestampString());
            if (not this->dropdownList->isOpen()) this->dropdownList->open();
        }
    }
}

void textInput::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget == this->dropdownList){
        this->setTextString(this->dropdownList->getSelected()[0]->getName());
    }
}