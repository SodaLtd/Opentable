//
//  openTableSessionClient.h
//  openTable
//
//  Created by Jonathan Jones-morris on 05/02/2015.
//
//

#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"

class openTableSessionClient {
public:
    openTableSessionClient();
    virtual ~openTableSessionClient();
    //
    //
    //
    void setup( string server_address, int port = 3000 );
    void update();
    //
    // search for free session marker
    //
    void aquireSession(int session_id);
    void releaseSession();
    bool hasSession() { return m_session_id > 0; }
    bool isConnected();
    int getSessionId() { return m_session_id; }
    ofColor& getSessionColour() { return m_session_colour; };
    //
    //
    //
    bool sendMessage( vector< string > message );
    bool sendMessage( string message );
protected:
    string          m_server_address;
    int             m_port;
    ofxTCPClient    m_tcp;
    int             m_session_id;
    ofColor         m_session_colour;
};