

#pragma once

#include "ofxUIWidgetWithLabel.h"
#include "ofxUIDefines.h"
#include "ofxUIRangeSlider.h"

class scrollRangeSlider : public ofxUIRangeSlider
{
public:
    scrollRangeSlider(string _name, float _min, float _max, float _valuelow, float _valuehigh, float w, float h, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL);
    scrollRangeSlider(string _name, float _min, float _max, float *_valuelow, float *_valuehigh, float w, float h, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL);
    scrollRangeSlider(float x, float y, float w, float h, float _min, float _max, float _valuelow, float _valuehigh, string _name, int _size = OFX_UI_FONT_SMALL);
    scrollRangeSlider(float w, float h, float _min, float _max, float _valuelow, float _valuehigh, string _name, int _size = OFX_UI_FONT_SMALL);
    scrollRangeSlider(float x, float y, float w, float h, float _min, float _max, float *_valuelow, float *_valuehigh, string _name, int _size = OFX_UI_FONT_SMALL);
    scrollRangeSlider(float w, float h, float _min, float _max, float *_valuelow, float *_valuehigh, string _name, int _size = OFX_UI_FONT_SMALL);
    ~scrollRangeSlider();
    void init(string _name, float _min, float _max, float *_valuelow, float *_valuehigh, float w, float h, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL);
    virtual void update();
    virtual void setDrawPadding(bool _draw_padded_rect);
    virtual void setDrawPaddingOutline(bool _draw_padded_rect_outline);
    virtual void drawFill();
    virtual void drawFillHighlight();
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void setIncrement(float _increment);
	void input(float x, float y);
    void updateValueRef();
	void updateLabel();
    void stateChange();
	void setValueLow(float _value);
	void setValueHigh(float _value);
    float getValueLow();
    float getValueHigh();
    float getNormalizedValueLow();
    float getNormalizedValueHigh();
    float getPercentValueLow();
	float getPercentValueHigh();
	float getScaledValueLow();
	float getScaledValueHigh();
    void setLabelPrecision(int _precision);
	void setParent(ofxUIWidget *_parent);
    void setMax(float _max);
    void setMin(float _min);
    void setMaxAndMin(float _max, float _min);
    bool isDraggable();
    bool hasState(){ return true; };
#ifndef OFX_UI_NO_XML
    virtual void saveState(ofxXmlSettings *XML);
    virtual void loadState(ofxXmlSettings *XML);
#endif
    
protected:    
	float valuelow, valuehigh, increment; 
    float *valuelowRef; 
    float *valuehighRef;    
    bool useReference;         
    float hitValueLow, hitValueHigh; 
	bool hitHigh, hitLow, hitCenter; 
	float hitPoint; 
	float max, min; 
    int labelPrecision;
    string valuelowString;
    string valuehighString; 
}; 