//
//  textInput.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/3/15.
//
//

#include "textInput.h"
#include "ofxUISuperCanvas.h"
#include <algorithm>

textInput::textInput(string _name, string _textstring, float w, float h, float x, float y, int _size) : ofxUITextInput()
{
    init(_name, _textstring, w, h, x, y, _size);
    
    nodes.push_back("gif");
    nodes.push_back("image");
    nodes.push_back("music");
    nodes.push_back("shader");
    nodes.push_back("video");
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
            string input = this->getTextString();
            this->dropdownList->clearToggles();
            for(auto n : nodes) {
                if (n.find(input) != -1)
                    this->dropdownList->addToggle(n);
            }

            if (not this->dropdownList->isOpen()) this->dropdownList->open();
        }
    }
}

void textInput::mouseDragged(int x, int y, int button) {
    
    ofxUITextInput::mouseDragged(x, y, button);
    
    if ((this->draggable) and (this->hit)) {
        this->dropdownList->getRect()->setX(x - hitPoint.x);
        this->dropdownList->getRect()->setY(y - hitPoint.y);
    }
}

void textInput::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget == this->dropdownList){
        this->setTextString(this->dropdownList->getSelected()[0]->getName());
    }
}
