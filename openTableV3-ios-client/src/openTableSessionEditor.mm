//
//  openTableSessionEditor.cpp
//  openTable
//
//  Created by Jonathan Jones-morris on 07/05/2016.
//
//

#include "openTableSessionEditor.h"
#include "openTableUI.h"

#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "SessionViewController.h"

static SessionViewController* _controller;

openTableSessionEditor::openTableSessionEditor() {
    
}
openTableSessionEditor::~openTableSessionEditor() {
    
    if ( _controller ) {
        [_controller.view removeFromSuperview];
        [_controller setEditor:nil];
        //[_controller release];
        //_controller = nil;
    } 
    
}
//
//
//
bool openTableSessionEditor::setup( ofxJSONElement& json ) {
    if ( openTableEditor::setup( json ) ) {
        if ( json.isMember("sessions") ) {
            for ( int i = 0; i < json["sessions"].size(); i++ ) {
                string session_id = json["sessions"][ i ]["id"].asString();
                string name = json["sessions"][ i ]["session_title"].asString();
                bool active = json["sessions"][ i ]["session_active"].asBool();
                m_sessions.push_back(sessionEntry( session_id, name, active ));
            }
            if ( _controller == nil ) {
                _controller	= [[SessionViewController alloc] initWithNibName:@"SessionViewController" bundle:nil];
            }
            [ofxiOSGetGLParentView() addSubview:_controller.view];
            [_controller.view setFrame:CGRectMake(0,0,ofGetWidth(),ofGetHeight())];
            [_controller setEditor:(void*)this];
        }
        return true;
    }
    return false;
}
bool openTableSessionEditor::save( ofxJSONElement& json ) {
    if ( openTableEditor::save(json) ) {
        int count = m_sessions.size();
        json["sessions"].resize(count);
        for ( int i = 0; i < count; i++ ) {
            json["sessions"][i]["id"]               = m_sessions[ i ].session_id;;
            json["sessions"][i]["session_title"]    = m_sessions[ i ].name.length() > 0 ? m_sessions[ i ].name : "Untitled";
            json["sessions"][i]["session_active"]   = m_sessions[ i ].active;
        }
        return true;
    }
    return false;
}
