//
//  button.h
//  ofxComposerTest
//
//  Created by Mili Garicoits on 9/8/15.
//
//

#ifndef ofxComposerTest_button_h
#define ofxComposerTest_button_h

#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"

#define		OVER_COLOR		0xFF0000
#define     NORMAL_COLOR    0xFFFFFF

class button : public ofxMSAInteractiveObject {

public:
    
    /* Constructors */
    
    button(){};
    
    button(string src, int width, int height, int x, int y, ofRectangle* container, string action, int* windowsId) {
        
        this->btnImg.loadImage(src);
        this->w = width;
        this->h = height;
        this->x = x;
        this->y = y;
        this->action = action;
        this->container = container;
        this->windowsId = windowsId;
        
        setSize(width, height);

        enableMouseEvents();
        //disableAppEvents();
    };
    
    
    /* Setters / Getters */
    
    void setSrc(string s) {
        this->btnImg.loadImage(s);
    }
    
    ofEvent<int> stop;
    ofEvent<int> play;
    
    void setup() {};
    void update() {};
    void draw() {
            
        if(isMouseOver())
            ofSetHexColor(OVER_COLOR);
        else ofSetHexColor(NORMAL_COLOR);
        this->btnImg.draw(x + container->x, y + container->y + container->height, w, h);
        
    };
    
    
    /* ofxMSAInteractiveObject events*/
    
    bool hitTest(int tx, int ty) {
        return ((tx > x + container->x) && (tx < x + w + container->x)
                && (ty > y + container->y + container->height) && (ty < y + h + container->y + container->height));
    };
    
    virtual void onRollOver(int x, int y) {};
    virtual void onRollOut() {};
    virtual void onMouseMove(int x, int y){};
    virtual void onDragOver(int x, int y, int button) {};
    virtual void onDragOutside(int x, int y, int button) {};
    virtual void onPress(int x, int y, int button) {
        if (action.compare("stop") == 0) {
            ofNotifyEvent(stop, *windowsId, this);
        } else if (action.compare("play") == 0) {
            ofNotifyEvent(play, *windowsId, this);
        }
    };
    virtual void onRelease(int x, int y, int button) {};
    virtual void onReleaseOutside(int x, int y, int button) {};
    virtual void keyPressed(int key) {};
    virtual void keyReleased(int key) {};
    
private:
    
    ofImage btnImg;
    int w, h, x, y;
    int* windowsId;
    string action;
    ofRectangle* container;
};

#endif
