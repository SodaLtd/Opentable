//
//  openTableClientSessionServer.h
//  openTableServer
//
//  Created by Jonathan Jones-morris on 04/02/2015.
//
//

#pragma once

#include "ofMain.h"
#include "openTableClientSession.h"
#include "ofxNetwork.h"
#include "openTableLED.h"

class NetInterface {
public:
	string name;
	string ip_address;
	string subnet_mask;
	string broadcast_address;
	string toString() {
		ostringstream os;
		os << "name : " << name;
		os << " ip address : " << ip_address;
		os << " subnet mask : " << subnet_mask;
		os << " broadcast address : " << broadcast_address;
		return os.str();
	};
};

class openTableClientSessionServer {
public:
    openTableClientSessionServer();
    virtual ~openTableClientSessionServer();
    //
    //
    //
    void setup( int port = 3000, int n_sessions = 4 );
    void close();
    void update();
    void draw();
    //
    //
    //
    ofRectangle getSafeZone();
    int getSessionAtPoint( float x, float y );
    bool sendSessionMessage( int session_id, vector< string > message );
    bool sendSessionMessage( int session_id, string message );
    ofVec2f getSessionPosition( int session_id );
    bool isSessionConnected( int session_id );
	ofColor getSessionColour(int session_id);
    //
    //
    //
    int testGetSessionAtPoint( float x, float y );
    bool testToggleSession( int session_id );
    //
    //
    //
protected:
    bool connectClient( int client_id, vector< string >& parameters );
    bool disconnectClient( int client_id );
    int getClientSession( int client_id );
    //
    //
    //
    ofxTCPServer                    m_tcp;
	ofxUDPManager					m_udp;
    map<string, NetInterface>       m_addresses;
    map< int, openTableClientSession >    m_sessions;
    float                           m_last_broadcast;
	openTableLED					m_led;
};