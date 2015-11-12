//
//  nodeVideoInterface.cpp
//  ofxComposerTest
//
//  Created by Mili Garicoits on 9/8/15.
//
//

#include "nodeVideoInterface.h"
#include "ofxUISlider.h"

/* constructors */
nodeVideoInterface::nodeVideoInterface( ofRectangle* wBox, int* _windowsId){
    
    windowsBox = wBox;
    windowsId = _windowsId;
    height = 25;
    offSetWidth = 5;
    state = VIDEO_PLAY;
    videoFrame = 0;
    
    ///addButton("assets/prev.png", "prev");
    addButton("assets/pause.png", "play");
    addButton("assets/stop.png", "stop");
    //addButton("assets/next.png", "next");

    gui = new ofxUISuperCanvas("");
    gui->addSlider("", 0.0, 1.0, &videoFrame)->setLabelVisible(false);
    gui->autoSizeToFitWidgets();
    gui->setColorBack( ofxUIColor(255,255,255,0) );
};


/* setters / getters */
void nodeVideoInterface::setVideoState( videoState s ) {
    this->state = s;
    
    if (s == VIDEO_PLAY)
        buttons[0]->setSrc("assets/pause.png");
    else buttons[0]->setSrc("assets/play.png");
};
void nodeVideoInterface::setVideoFrame( float frame ) {
    this->videoFrame = frame;
};
videoState nodeVideoInterface::getVideoState() {
    return this->state;
};


/* functions */

void nodeVideoInterface::addButton( string src, string action){
    
    ofPushMatrix();
    
    int pos_x = offSetWidth + buttons.size()*20;
    int pos_y = 5;
    
    button *newButton = new button( src, 15, 15, pos_x, pos_y, windowsBox, action, windowsId);
    buttons.push_back( newButton );
    
    if (action == "stop")
        ofAddListener( newButton->stop , this, &nodeVideoInterface::stopVideo);
    else if (action == "play")
        ofAddListener( newButton->play , this, &nodeVideoInterface::playVideo);
    
    ofPopMatrix();
}

void nodeVideoInterface::draw(){

    ofPushStyle();
    
    // Draw controls
    gui->ofxUICanvas::setPosition(windowsBox->x + windowsBox->width - gui->getRect()->width , windowsBox->y + windowsBox->height - 16);
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
void nodeVideoInterface::playVideo(int &args){
    ofNotifyEvent(_play, args, this);
}

