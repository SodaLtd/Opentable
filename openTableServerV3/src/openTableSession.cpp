//
//  openTableSession.cpp
//  openTableServer
//
//  Created by Jonathan Jones-morris on 03/05/2016.
//
//

#include "openTableSession.h"
#include "openTableImageCollection.h"
#include "openTableBusinessModelCanvas.h"
#include "ofApp.h"

static const Json::StaticString session_title_key("session_title");
static const Json::StaticString documents_key("documents");
static const Json::StaticString session_active_key("session_active");

openTableSession::openTableSession() {
    m_type = "session";
    setup();
}
openTableSession::openTableSession(Json::Value& json) {
    setup( json );
}
openTableSession::~openTableSession() {
}
//
//
//
bool openTableSession::setup() {
    m_selected_title = -1;
    m_locked = false;
    m_selected_offset.set(0.,0.);
    //
    // TODO: create empty documents
    //
    m_session_title = "New Session";
    m_documents.push_back(new openTableImageCollection);
    m_documents.push_back(new openTableBusinessModelCanvas);
    m_current_document = m_documents[ 0 ];
	return true;
}
bool openTableSession::setup( Json::Value& json ) {
    m_selected_title = -1;
    m_selected_offset.set(0.,0.);
    m_locked = false;
    if ( openTableDocument::setup(json) ) {
        if ( json.isMember( session_title_key ) ) {
            m_session_title = json[ session_title_key ].asString();
        }
        if ( m_session_title.length() == 0 ) {
            m_session_title = "Untitled";
        }
        //
        // load documents
        //
        if ( json.isMember( documents_key ) ) {
            for ( int i = 0; i < json[ documents_key ].size(); i++ ) {
                openTableDocument* document = openTableDocument::newDocument(json[ documents_key ][i]);
                m_documents.push_back(document);
            }
        }
        m_current_document = m_documents.size() > 0 ? m_documents[ 0 ] : 0;
		return false;
    }
	return true;
}
bool openTableSession::save( Json::Value& json ) {
    if ( openTableDocument::save(json) ) {
        json[ session_title_key ] = m_session_title;
        json[ documents_key ].resize(m_documents.size());
        for ( int i = 0; i < m_documents.size(); i++ ) {
            m_documents[ i ]->save( json[ documents_key ][ i ] );
        }
        return true;
    }
    return false;
    
}
void openTableSession::update() {
    if ( m_locked ) {
        //
        // check session lock is still valid
        //
        if ( !( ( ofApp* ) ofGetAppPtr() )->m_server.isSessionConnected(m_lock_session) ) {
            unlock();
        }
    }
    if ( m_current_document ) m_current_document->update();
}

ofVec3f worldToScreen(ofVec3f WorldXYZ) {
    ofMatrix4x4 m = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW).getInverse()*ofMatrix4x4::newOrthoMatrix(0, ofGetWidth(), 0, ofGetHeight(), 0, 0);
    ofVec3f CameraXYZ = WorldXYZ * m;
    ofVec3f ScreenXYZ;
    
    ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * ofGetWidth();
    ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * ofGetHeight();
    
    ScreenXYZ.z = CameraXYZ.z;
    
    return ScreenXYZ;
    
}

void openTableSession::draw() {
    //
    // draw title
    //
    if ( m_session_title.length() > 0 ) {
        ofxFontStash* font = &( ( ( ofApp*) ofGetAppPtr() )->m_default_font );
        if ( font ) {
            ofPushStyle();
            
            float scale = ofGetHeight() / 1080.;
            float margin = 48. * scale;
            float font_size = 32.*scale;
            ofRectangle bounds = font->getBBox(m_session_title, font_size, 0, 0);
            ofVec3f top_left(-margin,-margin);
            ofVec3f bottom_right = top_left + ofVec3f(bounds.width+(margin*2),bounds.height+(margin*2));
            for ( int i = 0; i < 2; i++ ) {
                ofPushMatrix();
                if ( m_selected_title == i ) {
                    ofTranslate(m_selected_offset.x,m_selected_offset.y);
                } else {
                    ofTranslate((ofGetWidth()/2.),i==0?(margin*2.+bounds.y)+bounds.height:ofGetHeight()-((margin*2.)+bounds.height+bounds.y));
                }
                ofRotate(180.0*(1-i));
                ofTranslate(-bounds.width/2,-bounds.height/2.0);
                if ( m_locked ) {
                    ofSetColor(m_lock_colour,102);
                } else {
                    ofSetColor(0,0,0,102);
                }
                ofRect(-margin,-margin,bounds.width+(margin*2),bounds.height+(margin*2));
                ofSetColor(ofColor::white);
                font->draw(m_session_title, font_size, 0., 0.);
                ofPopMatrix();
                //
                // store bounds for hit testing
                //
                ofMatrix4x4 m;
                //
                // translate to center on origin
                //
                m.translate(-bounds.width/2,-bounds.height/2.0, 0.);
                //
                // rotate around origin
                //
                m.rotate(180.0*(1-i),0, 0, 1);
                //
                // translate to position
                //
                m.translate((ofGetWidth()/2.),i==0?(margin*2.+bounds.y)+bounds.height:ofGetHeight()-((margin*2.)+bounds.height+bounds.y),0.);

                
                ofPushStyle();
                ofSetColor(i==0?255:0, i==1?255:0, 0, 127);
                ofVec3f p0 = top_left*m;
                ofVec3f p1 = bottom_right*m;
                
                ofRectangle r(p0, p1);
                //ofRect(r);
                m_title_rect[ i ] = r;
                
                ofPopStyle();
            }
            ofPopStyle();
        }
    }
    //
    // draw document
    //
    if ( m_current_document ) m_current_document->draw();
}
//
//
//
void openTableSession::touchMove(int x, int y, int id) {
    if ( m_selected_title != -1 && m_selected_id == id ) {
        
        m_selected_offset.set(x, y);
    } else if ( m_current_document ) m_current_document->touchMove(x,y,id);
}
void openTableSession::touchDown(int x, int y, int id) {
    if ( !m_locked && m_selected_title == -1 ) {
        for ( int i = 0; i < 2; i++ ) {
            if ( m_title_rect[ i ].inside(x, y) ) {
                m_selected_offset.set(x, y);
                m_selected_title = i;
                m_selected_id = id;
                return;
            }
        }
    }
    if ( m_current_document ) m_current_document->touchDown(x,y,id);
}
void openTableSession::touchUp(int x, int y, int id) {
    if ( m_selected_title != -1 && id == m_selected_id ) {
        ofApp* app = ( ofApp* )ofGetAppPtr();
        int session_id = app->m_server.getSessionAtPoint( x, y );
        if ( session_id > 0 ) {
            //
            // send session list to session
            //
			vector< openTableDocument* > sessions = app->m_db.getDocumentsOfType("session");
            ofxJSONElement session_json;
            session_json[ "type" ] = "session";
            session_json["sessions"].resize(sessions.size());
            for ( int i = 0; i < sessions.size(); i++ ) {
                session_json["sessions"][ i ][ id_key ]     = sessions[ i ]->getId();
                session_json["sessions"][ i ][ session_title_key ]  = ( ( openTableSession* )sessions[ i ] )->getSessionTitle();
                session_json["sessions"][ i ][ session_active_key ] = sessions[ i ] == app->m_current_session;
            }
            string session_json_string = session_json.getRawString(false);
            vector< string > message;
            message.push_back("document");
            message.push_back( session_json_string );
            if ( app->m_server.sendSessionMessage(session_id, message) ) {
                //
                // lock session
                //
                m_locked = true;
                m_lock_session = session_id;
                m_lock_colour = ((ofApp*)ofGetAppPtr())->m_server.getSessionColour(session_id);
            }
        } else {
            m_locked = false;
            m_lock_session = -1;
        }
        m_selected_title = -1;
        m_selected_id = -1;
    } else if ( m_current_document ) m_current_document->touchUp(x,y,id);
}
//
//
//
void openTableSession::setCurrentDocument( int index ) {
    if ( index >= 0 && index < m_documents.size() ) {
        m_current_document = m_documents[ index ];
    }
}
//
//
//
void openTableSession::unlock() {
    m_locked = false;
    m_lock_session = -1;
}
