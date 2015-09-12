//
//  nodeVideoInterface.cpp
//  ofxComposerTest
//
//  Created by Mili Garicoits on 9/8/15.
//
//

#include "nodeVideoInterface.h"

nodeVideoInterface::nodeVideoInterface( ofRectangle* wBox, int* _windowsId){
    
    windowsBox = wBox;
    windowsId = _windowsId;
    height = 25;
    offSetWidth = 5;
    
    addButton("assets/prev.png", "prev");
    addButton("assets/play.png", "play");
    addButton("assets/stop.png", "stop");
    addButton("assets/next.png", "next");
    
    ofAddListener( buttons[2]->stop , this, &nodeVideoInterface::stopVideo);
};

void nodeVideoInterface::addButton( string src, string action){
    
    ofPushMatrix();
    
    int pos_x = windowsBox->getHeight() + offSetWidth + buttons.size()*20;
    int pos_y = 5;
    
    button *newButton = new button( src, 15, 15, pos_x, pos_y, windowsBox, action, windowsId);
    buttons.push_back( newButton );
    
    ofPopMatrix();
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
        buttons[i]->setPosition(windowsBox->x + offSetWidth + i*20, windowsBox->y + windowsBox->height + 5);
        buttons[i]->draw();
    }
    ofPopStyle();
};

void nodeVideoInterface::stopVideo(int &args){
    ofNotifyEvent(_stop, args, this);
}

