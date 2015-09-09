//
//  nodeVideoInterface.cpp
//  ofxComposerTest
//
//  Created by Mili Garicoits on 9/8/15.
//
//

#include "nodeVideoInterface.h"

nodeVideoInterface::nodeVideoInterface( ofRectangle* wBox, int* _windowsId ){
    
    windowsBox = wBox;
    windowsId = _windowsId;
    height = 25;
    offSetWidth = 5;
};

void nodeVideoInterface::addButton( string src, string action){
    
    button newButton( src, 15, 15, action);
    buttons.push_back( newButton );
}

void nodeVideoInterface::draw(){

    ofPushStyle();

    // Draw controls
    //
    ofFill();
    ofSetColor(0,250, 130);
    ofRect(windowsBox->x, windowsBox->y + windowsBox->height, windowsBox->width, height);
    
    for (int i = 0; i < buttons.size(); i++){
        ofSetColor(100);
        buttons[i].draw(windowsBox->x + offSetWidth + i*20, windowsBox->y + windowsBox->height + 5);
    }
    ofPopStyle();
};
