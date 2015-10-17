//
//  textInput.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/3/15.
//
//

#include "textInput.h"
#include "textInputEvent.h"
#include "ofxComposer.h"
#include "ofxUISuperCanvas.h"

textInput::textInput(string _name, string _textstring, float w, float h, float x, float y, int _size) : ofxUITextInput()
{
    init(_name, _textstring, w, h, x, y, _size);
    
    nodes.push_back("gif");
    nodes.push_back("image");
    nodes.push_back("music");
    nodes.push_back("shader");
    nodes.push_back("video");
    nodes.push_back("camera");
}

void textInput::setDropdownList(ofxUIDropDownList* dl) {
    
    dl->open();
    dl->setVisible(false);
    dl->setAutoClose(true);
    
    dl->addToggles(nodes);
    dl->setToggleVisibility(false);
    
    this->addEmbeddedWidget(dl);
    this->dropdownList = dl;
    
    ofAddListener(((ofxUISuperCanvas*) dl->getCanvasParent())->newGUIEvent,this,&textInput::guiEvent);
}

ofxUIDropDownList* textInput::getDropdownList() {
    return this->dropdownList;
}

void textInput::keyPressed(int key) {
    
    ofxUITextInput::keyPressed(key);
    if(clicked)
    {
        string input = this->getTextString();
        for(auto n : this->dropdownList->getToggles()) {
            if (n->getName().find(input) != -1)
                n->setVisible(true);
            else n->setVisible(false);
        }

        if (not this->dropdownList->isOpen()) this->dropdownList->open();
    }
}

void textInput::mouseDragged(int x, int y, int button) {
    
    ofxUITextInput::mouseDragged(x, y, button);
    
    if ((this->draggable) and (this->hit)) {
        this->dropdownList->getRect()->setX(x - hitPoint.x);
        this->dropdownList->getRect()->setY(y - hitPoint.y);
    }
}

void textInput::guiEvent(ofxUIEventArgs &e){
    
    if(e.widget == this->dropdownList){
        this->setTextString(this->dropdownList->getSelected()[0]->getName());
        
        textInputEvent e;
        e.point.set(this->getRect()->getX(), this->getRect()->getY());
        e.widget = this;
        e.type = this->dropdownList->getSelected()[0]->getName();
        
        ofFileDialogResult openFileResult;
        
        if (e.type == "camera") {
            
            e.type = "ofVideoGrabber";
        }
        else if (e.type == "image"){
            openFileResult = ofSystemLoadDialog("Select an image or gif");
            
            if (openFileResult.bSuccess){
                
                ofFile file (openFileResult.getPath());
                
                if (file.exists()){
                    
                    string fileExtension = ofToUpper(file.getExtension());
                    
                    //We only want images
                    if (fileExtension == "JPG"  ||
                        fileExtension == "PNG"  ||
                        fileExtension == "JPEG" ||
                        fileExtension == "GIF"  ||
                        fileExtension == "BMP"  ) {
                        e.path = openFileResult.getPath();
                    }
                    else return;
                }
                file.close();
            }
            else return;
        }
        else if (e.type == "video") {
            openFileResult = ofSystemLoadDialog("Select a video");
            
            if (openFileResult.bSuccess){
                
                ofFile file (openFileResult.getPath());
                
                if (file.exists()){
                    
                    string fileExtension = ofToUpper(file.getExtension());
                    
                    //We only want videos
                    if (fileExtension == "MOV" ||
                        fileExtension == "MPG" ||
                        fileExtension == "MP4" ||
                        fileExtension == "M4V" ) {
                        e.path = openFileResult.getPath();
                    }
                    else return;
                }
                file.close();
            }
            else return;

        }
        
        ofNotifyEvent(createNode, e , this);
        
    }
}
