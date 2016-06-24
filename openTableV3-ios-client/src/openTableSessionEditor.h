//
//  openTableSessionEditor.h
//  openTable
//
//  Created by Jonathan Jones-morris on 07/05/2016.
//
//

#pragma once

#include "openTableEditor.h"

class openTableSessionEditor : public openTableEditor {
public:
    class sessionEntry {
    public:
        sessionEntry( string _session_id, string _name, bool _active = false ) {
            session_id  = _session_id;
            name        = _name;
            active      = _active;
        }
        string session_id;
        string name;
        bool   active;
    };
    openTableSessionEditor();
    virtual ~openTableSessionEditor();
    //
    //
    //
    virtual bool setup( ofxJSONElement& json );
    virtual bool save( ofxJSONElement& json );
    //
    //
    //
    int sessionCount() { return m_sessions.size(); };
    void setSessionName( int i, string name ) { m_sessions[ i ].name = name; };
    string getSessionName( int i ) { return m_sessions[ i ].name; };
    string getSessionId( int i ) { return m_sessions[ i ].session_id; };
    bool getSessionActive( int i ) { return m_sessions[ i ].active; };
    void setActiveSession( int i ) {
        for ( int j = 0; j < m_sessions.size(); j++ ) {
            m_sessions[ j ].active = ( i == j );
        }
    }
    int getActiveSession() {
        for ( int i = 0; i < m_sessions.size(); i++ ) {
            if ( m_sessions[ i ].active ) return i;
        }
        return -1;
    }
    int addSession( string name ) {
        m_sessions.push_back(sessionEntry( "", name ));
        return m_sessions.size() - 1;
    }
    void removeSession( int i ) { m_sessions.erase(m_sessions.begin()+i); };
protected:    
    //
    //
    //
    vector<sessionEntry>    m_sessions;
};

