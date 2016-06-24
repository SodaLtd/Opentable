#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "openTableClientSessionServer.h"
#include "openTableDB.h"
//#include "sodaDistanceFieldFont.h"
#include "ofxFontStash.h"
#include "ofxNativeMultitouch.h"
#include "openTableSession.h"

#define k_nbuttons 8
#define k_nbuttonimages 4

class ofApp : public ofBaseApp, public ofxNativeMultitouchTarget {

public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;
    
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y ) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;

 	virtual void touchDown(int x, int y, int id) override;
	virtual void touchMove(int x, int y, int id) override;
	virtual void touchMoved(int x, int y, int id) override;
	virtual void touchUp(int x, int y, int id) override;
   
    void newDocument( int session_id, ofxJSONElement& json );
    
	void upload();

    openTableClientSessionServer    m_server;
    openTableDB                     m_db;
    openTableSession*               m_current_session;
    ofxFontStash					m_default_font;
    map< int, int >					m_selected_button;
    ofImage							m_buttons[k_nbuttonimages];
    ofRectangle						m_button_bounds[ k_nbuttons ];
    ofImage							m_background_tile;
    void layout();

	//
	//
	//
	bool					m_show_keyboard;
	ofxNativeMultitouch		m_multitouch;
};
