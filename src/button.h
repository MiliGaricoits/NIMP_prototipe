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
    
    button(){};
    
    button(string src, int width, int height, int x, int y, ofRectangle* container, string action) {
        
        this->btnImg.loadImage(src);
        this->w = width;
        this->h = height;
        this->x = x;
        this->y = y;
        this->action = action;
        this->container = container;
        
        setSize(width, height);

        enableMouseEvents();
        //disableAppEvents();
        
    };
    
    void setup() {};
    
    void update() {};
    
    void draw() {
        
        if(isMouseOver())
            ofSetHexColor(OVER_COLOR);
        else ofSetHexColor(NORMAL_COLOR);
        this->btnImg.draw(x + container->x, y + container->y + container->height, w, h);
        
    };
    
    bool hitTest(int tx, int ty) {
        return ((tx > x + container->x) && (tx < x + w + container->x)
                && (ty > y + container->y + container->height) && (ty < y + h + container->y + container->height));
    };
    
    virtual void onRollOver(int x, int y) {};
    virtual void onRollOut() {};
    virtual void onMouseMove(int x, int y){};
    virtual void onDragOver(int x, int y, int button) {};
    virtual void onDragOutside(int x, int y, int button) {};
    virtual void onPress(int x, int y, int button) {};
    virtual void onRelease(int x, int y, int button) {};
    virtual void onReleaseOutside(int x, int y, int button) {};
    virtual void keyPressed(int key) {};
    virtual void keyReleased(int key) {};
    
private:
    
    ofImage btnImg;
    int w, h, x, y;
    string action;
    ofRectangle* container;
};

#endif
