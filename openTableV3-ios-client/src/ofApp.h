#pragma once

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#include "openTableMarkerFinder.h"
#include "openTableSessionClient.h"
#include "ofxJSON.h"
#include "openTableEditor.h"

class ofApp : public ofxiOSApp {
	
public:
    enum {
        ModeConnecting,
        ModeIdle,
        ModeJoinSession,
        ModeEditImage,
        ModeEditText,
        ModeEditSession
    };
    void setup();
    void update();
    void draw();
    void exit();

    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);

    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);

    void panGesture( ofVec2f velocity );
    
    void connect( string ip_address );
    void newDocument( string type );
    void loadDocument( string document );
    void saveCurrentDocument( ofVec2f velocity = ofVec2f() );
    
    void getAdjustedCoords( ofTouchEventArgs& original, ofTouchEventArgs& adjusted );
    
    ofxUDPManager           m_udp;
    openTableMarkerFinder   m_marker_finder;
    openTableSessionClient  m_session_client;
    
    int m_mode;
    openTableEditor*        m_editor;
    
    ofImage                 m_background_tile;
    ofImage                 m_logo;
    
};


