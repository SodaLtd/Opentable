//
//  openTableClientSession.h
//  openTableServer
//
//  Created by Jonathan Jones-morris on 04/02/2015.
//
//

#pragma once

#include "ofMain.h"

class openTableClientSession {
public:
    openTableClientSession();
    virtual ~openTableClientSession();
    //
    //
    //
    void setup( int session_id, ofImage& marker_image );
    void update();
    void draw( ofPoint& p, float width = 0, float height = 0 );
    void draw( float x, float y, float width = 0, float height = 0 );
    //
    //
    //
    int getSessionId() { return m_session_id; };
    int getClientId() { return m_client_id; };
	int getStation() { return m_station; };
	void setStation(int i) { m_station = i; };
	ofColor getColour() { return m_session_colour; };
	void setColour(ofColor& colour) { m_session_colour = colour; };
    //
    //
    //
    void aquire( int client_id ) { m_client_id = client_id; }
    void release() { m_client_id = -1; };
    bool isAvailable() { return m_client_id == -1; }
    //
    //
    //
protected:
    int     m_session_id;
    ofImage m_marker_image;
    int     m_client_id;
    ofColor m_session_colour;
    ofImage* m_session_indicator;
    float   m_indicator_alpha;
	int		m_station;
};