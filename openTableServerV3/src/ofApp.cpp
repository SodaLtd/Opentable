#include "ofApp.h"
#include "ARMarker.hpp"

#include "openTableImageCollection.h"
#include "openTableBusinessModelCanvas.h"

#include "sdaHTTP.h"

#include "ofxNetwork_Utils.h"


//--------------------------------------------------------------
void ofApp::setup(){
	//
	//
	//
	string hostname = ofxNet::NetworkUtils::getNodeName();
#ifdef _DEBUG
	printf("hostname: %s\n", hostname.c_str());
#endif
    //
    //
    //
    ofSetBackgroundAuto(false);
    ofEnableAntiAliasing();
    ofSetCircleResolution( 24 );
    //m_default_font.loadFont("fonts/verdana.ttf",false,0.3, 144.0);
    m_default_font.setup("fonts/verdana.ttf");
	m_buttons[0].loadImage("ui/PhotoCollection.png");
	m_buttons[1].loadImage("ui/BusinessModel.png");
	m_buttons[2].loadImage("ui/Save.png");
	m_buttons[3].loadImage("ui/Close.png");

    ofDisableArbTex();
    m_background_tile.loadImage("ui/background_tile.png");
    m_background_tile.getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
    ofEnableArbTex();
    
    layout();
    //
    //
    //
    
    m_db.setup();
    vector< openTableDocument* > sessions = m_db.getDocumentsOfType("session");
    if (sessions.size() > 0 ) {
        m_current_session = ( openTableSession* ) sessions[ 0 ];
    } else {
        m_current_session = ( openTableSession* ) m_db.add("session");
        m_db.save();
    }
    //
    // start server
    //
    m_server.setup();
    //
    //
    //
	m_show_keyboard = false;
	
	if ( !m_multitouch.setup() ) {
		ofShowCursor();
	}
	
}

//--------------------------------------------------------------
void ofApp::update(){
    //
    //
    //
    m_server.update();
    //
    //
    //
    if ( m_current_session ) {
        m_current_session->update();
    }
    /*
    if ( m_current_document ) {
        m_current_document->update();
    }
    */
}

//--------------------------------------------------------------
void ofApp::draw(){
    //
    // draw background
    //
    ofSetMatrixMode(OF_MATRIX_TEXTURE);
    ofPushMatrix();
    float scale = ofGetWidth()/m_background_tile.getWidth();
    ofScale(scale, scale);
    m_background_tile.getTextureReference().draw(0,0,ofGetWidth(),ofGetWidth());
    ofPopMatrix();
    ofSetMatrixMode(OF_MATRIX_MODELVIEW);
    ofPushStyle();
    ofRectangle safe_rect = m_server.getSafeZone();
    ofSetColor(253, 185, 0);
    ofRect(safe_rect);
    ofPopStyle();
    //
    //
    //
    m_server.draw();
    //
    //
    //
    for ( int i = 0; i < k_nbuttons; i++ ) {
        ofPushMatrix();
        ofTranslate(m_button_bounds[ i ].getCenter());
        ofRotate(m_button_bounds[i].getCenter().y < ofGetHeight() / 2.f ? 180.f : 0);
        ofTranslate(-m_button_bounds[ i ].width/2,-m_button_bounds[ i ].height/2.0);
		int selector = i % k_nbuttonimages;
		m_buttons[selector].draw(0, 0, m_button_bounds[i].width, m_button_bounds[i].height);
		/*
        if ( i == 0 || i == 2 ) {
            m_buttons[ 0 ].draw(0,0,m_button_bounds[ i ].width,m_button_bounds[ i ].height);
         } else {
            m_buttons[ 1 ].draw(0,0,m_button_bounds[ i ].width,m_button_bounds[ i ].height);
        }
		*/
        ofPopMatrix();
    }
    //
    //
    //
    if ( m_current_session ) {
        m_current_session->draw();
    }
    /*
    if ( m_current_document ) {
        m_current_document->draw();
    }
    */
}

//--------------------------------------------------------------
void ofApp::exit(){
    m_db.save();
    m_server.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch( key ) {
		/*
		case 'f' :
		case 'F' :
			ofToggleFullscreen();
			break;
		case 'k' :
			m_show_keyboard = !m_show_keyboard;
			break;
		*/
	case 'u' :
	case 'U' :
		upload();
		break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (m_multitouch.isSetup()) return;
	touchMove(x,y,-1);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (m_multitouch.isSetup()) return;
	touchDown(x,y,-1);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (m_multitouch.isSetup()) return;
	touchUp(x,y,-1);
}

void ofApp::touchDown(int x, int y, int id) {
    for ( int i = 0; i < k_nbuttons; i++ ) {
        if ( m_button_bounds[ i ].inside(x, y) ) {
            m_selected_button[ id ] = i+1;
            break;
        }
    }
    if ( !m_selected_button[ id ] && m_current_session ) {
        m_current_session->touchDown(x, y, id);
    }
    /*
    if ( !m_selected_button[ id ] && m_current_document ) {
        m_current_document->touchDown(x, y, id);
    }
    */
}
void ofApp::touchMove(int x, int y, int id) {
	if (m_current_session) {
		m_current_session->touchMove(x, y, id);
	}
	/*
	if ( m_current_document ) {
	m_current_document->touchMove(x, y, id);
	}
	*/
}
void ofApp::touchMoved(int x, int y, int id) {
	if (m_current_session) {
		m_current_session->touchMove(x, y, id);
	}
	/*
	if ( m_current_document ) {
	m_current_document->touchMove(x, y, id);
	}
	*/
}
void ofApp::touchUp(int x, int y, int id) {
    int test_session = m_server.testGetSessionAtPoint(x, y);
    if ( test_session >= 0 ) {
        //m_server.testToggleSession(test_session);
    }
    if ( m_selected_button[ id ] ) {
        int button = -1;
        for ( int i = 0; i < k_nbuttons; i++ ) {
            if ( m_button_bounds[ i ].inside(x, y) ) {
                button = i;
                break;
            }
        }
        if ( button+1 == m_selected_button[ id ] ) {
            //
            //
            //
			int selector = button % k_nbuttonimages;
			switch (selector) {
			case 0:
				m_current_session->setCurrentDocument(0);
				break;
			case 1:
				m_current_session->setCurrentDocument(1);
				break;
			case 2:
				upload();
				break;
			case 3:
				ofExit();
				break;
			}
			/*
            if ( button == 0 || button == 2 ) {
                m_current_session->setCurrentDocument( 0 );
            } else {
                m_current_session->setCurrentDocument( 1 );
            }
			*/
         }
    } else if ( m_current_session ) {
        m_current_session->touchUp(x, y, id);
    }
    m_selected_button[ id ] = 0;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    layout();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::newDocument( int session_id, ofxJSONElement& json ) {
    if ( json.isMember("type") ) {
        string type = json[ "type" ].asString();
        
        //openTableDocument* document = m_current_session->getCurrentDocument();
        if ( type == "image" ) {
            openTableImageCollection* collection = ( openTableImageCollection* ) m_current_session->getDocument( 0 );
            openTableImageGroup* group = new openTableImageGroup;
            group->setup(json);
            if ( group->nImages() > 0 ) {
				ofVec2f p = m_server.getSessionPosition(session_id);
				if (p.y < ofGetHeight() / 2) {
					p.y += 120.f;
				} else {
					p.y -= 120.f;
				}
                group->setPosition(p);
                collection->addGroup(group);
            } else {
                delete group;
            }
			m_db.save();
        } else if ( type == "text" ) {
            openTableBusinessModelCanvas* bmc = ( openTableBusinessModelCanvas* ) m_current_session->getDocument( 1 );
            bmc->updateElement(json);
			m_db.save();
        } else if( type == "session" ) {
            m_current_session->unlock();
            if ( json.isMember("sessions") ) {
                m_db.markAllDocumentsOfTypeForDelete( "session" );
                for ( int i = 0; i < json[ "sessions"].size(); i++ ) {
                    string id = json[ "sessions"][ i ].isMember( "id" ) ? json[ "sessions"][ i ][ "id" ].asString() : "";
                    string title = json[ "sessions"][ i ][ "session_title" ].asString();
                    bool active = json[ "sessions"][ i ][ "session_active" ].asBool();
                    openTableSession* session;
                    if ( id.length() > 0 ) {
                        session = ( openTableSession* )m_db.get(id);
                        //
                        // update existing
                        //
                        session->setSessionTitle( title );
                        m_db.clearPendingDelete(id);
                    } else {
                        //
                        // create new
                        //
                        session = ( openTableSession* )m_db.add( "session" );
                        session->setSessionTitle(title);
                    }
                    if ( active ) m_current_session = session;
                }
                m_db.commitPendingDeletes();
				m_db.save();
            }
            
        }
    }
}

void ofApp::upload() {
	const string url = "http://opentable-soda.rhcloud.com/replacesessions";
	string sessions = m_db.toString();
	printf("%s\n", sessions.c_str());
	/*
	function< void(string,int) > callback = [](string response, int status) {
		printf("upload : response = '%s' : status = %d\n", response.c_str(), status);
	};
	sdaHTTP::post(url, sessions, callback );
	*/
	string response;
	int status = sdaHTTP::post(url, sessions, response);
	printf("upload : response = '%s' : status = %d\n", response.c_str(), status);
}

void ofApp::layout() {
    float scale = ofGetHeight() / 1080.;
    float button_dim = 64. * scale;
    float margin = 48. * scale;
	//
	// top documents
	//
    m_button_bounds[ 0 ].x = margin;
    m_button_bounds[ 0 ].y = margin;
    m_button_bounds[ 0 ].width = button_dim;
    m_button_bounds[ 0 ].height = button_dim;
    m_button_bounds[ 1 ].x = margin + button_dim;
    m_button_bounds[ 1 ].y = margin;
    m_button_bounds[ 1 ].width = button_dim;
    m_button_bounds[ 1 ].height = button_dim;
	//
	// top quit and share
	//
	m_button_bounds[2].x = margin + button_dim * 2;
	m_button_bounds[2].y = margin;
	m_button_bounds[2].width = button_dim;
	m_button_bounds[2].height = button_dim;
	m_button_bounds[3].x = margin + button_dim * 3;
	m_button_bounds[3].y = margin;
	m_button_bounds[3].width = button_dim;
	m_button_bounds[3].height = button_dim;
	//
	// bottom documents
	//
	m_button_bounds[4].x = ofGetWidth() - (margin + button_dim);
	m_button_bounds[4].y = ofGetHeight() - (margin + button_dim);
	m_button_bounds[4].width = button_dim;
	m_button_bounds[4].height = button_dim;
	m_button_bounds[5].x = ofGetWidth() - (margin + button_dim * 2);
	m_button_bounds[5].y = ofGetHeight() - (margin + button_dim);
	m_button_bounds[5].width = button_dim;
	m_button_bounds[5].height = button_dim;
	//
	// bottom quit and share
	//
	m_button_bounds[6].x = ofGetWidth() - (margin + button_dim * 3);
	m_button_bounds[6].y = ofGetHeight() - (margin + button_dim);
	m_button_bounds[6].width = button_dim;
	m_button_bounds[6].height = button_dim;
	m_button_bounds[7].x = ofGetWidth() - (margin + button_dim * 4);
	m_button_bounds[7].y = ofGetHeight() - (margin + button_dim);
	m_button_bounds[7].width = button_dim;
	m_button_bounds[7].height = button_dim;
}