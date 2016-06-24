//
//  ImageButton.h
//  openTable
//
//  Created by Jonathan Jones-morris on 09/11/2014.
//
//

#ifndef MUI_IMAGE_BUTTON
#define MUI_IMAGE_BUTTON

#include "MUI.h"

#include "poco/BasicEvent.h"

namespace mui{
    class ImageButton : public Image{
    public:
        ImageButton( string filename, float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 ) :
        Image( filename, x_, y_, width_, height_ ) {} ;

        bool pressed;
        Poco::BasicEvent<ofTouchEventArgs> onPress;
        Poco::BasicEvent<ofTouchEventArgs> onDoubleTapped;

        virtual void touchDown( ofTouchEventArgs &touch );
        virtual void touchMoved( ofTouchEventArgs &touch );
        virtual void touchMovedOutside( ofTouchEventArgs &touch );
        virtual void touchUp( ofTouchEventArgs &touch );
        virtual void touchUpOutside( ofTouchEventArgs &touch );
        virtual void touchDoubleTap( ofTouchEventArgs &touch );
        virtual void touchCanceled( ofTouchEventArgs &touch ); 
        
        
    };
};

#endif