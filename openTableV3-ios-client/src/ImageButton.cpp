//
//  ImageButton.cpp
//  openTable
//
//  Created by Jonathan Jones-morris on 09/11/2014.
//
//

#include "ImageButton.h"

//--------------------------------------------------------------
void mui::ImageButton::touchDown( ofTouchEventArgs &touch ){
    pressed = true;
}


//--------------------------------------------------------------
void mui::ImageButton::touchMoved( ofTouchEventArgs &touch ){
    pressed = true;
}


//--------------------------------------------------------------
void mui::ImageButton::touchMovedOutside( ofTouchEventArgs &touch ){
    pressed = false;
}


//--------------------------------------------------------------
void mui::ImageButton::touchUp( ofTouchEventArgs &touch ){
    pressed = false;
    onPress( this, touch );
}


//--------------------------------------------------------------
void mui::ImageButton::touchUpOutside( ofTouchEventArgs &touch ){
    pressed = false;
}


//--------------------------------------------------------------
void mui::ImageButton::touchDoubleTap( ofTouchEventArgs &touch ){
    onDoubleTapped( this, touch );
}


//--------------------------------------------------------------
void mui::ImageButton::touchCanceled( ofTouchEventArgs &touch ){
    pressed = false;
}
