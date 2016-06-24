//
//  openTableSessionClient.cpp
//  openTable
//
//  Created by Jonathan Jones-morris on 05/02/2015.
//
//

#include "openTableSessionClient.h"
#include "ofApp.h"
//
//
//        
openTableSessionClient::openTableSessionClient() {
    m_session_id = -1;
}

openTableSessionClient::~openTableSessionClient() {
    
}
//
//
//
void openTableSessionClient::setup( string server_address, int port ) {
    m_tcp.setup(server_address, port);
}
void openTableSessionClient::update() {
    string message = m_tcp.receive();
    if ( message.length() > 0 ) {
        //
        // process message
        //
        ofApp* app = ( ofApp* ) ofGetAppPtr();
        vector< string > parts = ofSplitString(message, "[~COMMAND~]");
        if ( parts.size() == 2 ) {
            string command = parts[ 0 ];
            vector<string> parameters = ofSplitString(parts[ 1 ], "[~PARAM~]");
            printf( "%s\n", message.c_str());
            if ( command == "connect" ) {
                int session_id = ofToInt( parameters[ 0 ] );
                if ( parameters[ 1 ] == "connected" ) {
                    printf( "connected to session %d\n", session_id );
                    m_session_id = session_id;
                    ofxJSONElement colour;
                    if ( colour.parse(parameters[ 2 ]) ) {
                        m_session_colour.r = colour[ "r" ].asInt();
                        m_session_colour.g = colour[ "g" ].asInt();
                        m_session_colour.b = colour[ "b" ].asInt();
                    }
                } else {
                    printf( "unable to connect to session %d\n", m_session_id );
                    m_session_id = -1;
                }
            } else if ( command == "disconnect" ) {
                int session_id = ofToInt( parameters[ 0 ] );
                if ( parameters[ 1 ] == "disconnected" && session_id == m_session_id ) {
                    printf( "disconnected from session %d\n", m_session_id );
                    m_session_id = -1;
                } else {
                    printf( "unable to disconnect from session %d\n", m_session_id );
                }
            } else if ( command == "document" ) {
                app->loadDocument(parameters[ 0 ]);
            }
            
        }
    }

}
//
// search for free session marker
//
void openTableSessionClient::aquireSession(int session_id) {
    if ( m_tcp.isConnected() ) {
        //
        // show marker finder interface
        //
        vector< string > message;
        message.push_back("connect");
        message.push_back(ofToString(session_id));
        sendMessage(message);
    }
}
void openTableSessionClient::releaseSession() {
    if ( m_tcp.isConnected() && m_session_id > 0 ) {
        //
        // show marker finder interface
        //
        vector< string > message;
        message.push_back("disconnect");
        message.push_back(ofToString(m_session_id));
        sendMessage(message);
    }
    m_session_id = 0;
}
bool openTableSessionClient::isConnected() {
    return m_tcp.isConnected();
}
//
//
//
bool openTableSessionClient::sendMessage( vector< string > message ) {
    if ( m_tcp.isConnected() ) {
        ostringstream message_string;
        for ( int i = 0; i < message.size(); i++ ) {
            message_string << message[ i ];
            if ( i == 0 ) {
                message_string << "[~COMMAND~]";
            } else if ( i < message.size() - 1 ) {
                message_string << "[~PARAM~]";
            }
        }
        return m_tcp.send(message_string.str());
    }
    return false;
}
bool openTableSessionClient::sendMessage( string message ) {
    if ( m_tcp.isConnected() ) {
        return m_tcp.send(message);
    }
    return false;
}
