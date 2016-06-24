#include "ofApp.h"
#include "openTableUI.h"
#include "openTableImageEditor.h"
#include "openTableBusinessModelEditor.h"
#include "openTableSessionEditor.h"
#include "connectDialog.h"

#include "Poco/Net/HostEntry.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Environment.h"
#include "Poco/Exception.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/DNS.h"
#include "Poco/Net/HostEntry.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

 Poco::Net::HostEntry getHostByName(const std::string& hostname) {
     Poco::Net::HostEntry hostEntry;
     
     try {
        hostEntry = Poco::Net::DNS::hostByName(hostname);
     } catch (const Poco::Net::HostNotFoundException& exc) {
        ofLogError("NetworkUtils::getHostByName") << exc.displayText();
        return hostEntry;
     } catch (const Poco::Net::NoAddressFoundException& exc) {
        ofLogError("NetworkUtils::getHostByName") << exc.displayText();
        return hostEntry;
     } catch (const Poco::Net::DNSException& exc) {
        ofLogError("NetworkUtils::getHostByName") << exc.displayText();
        return hostEntry;
     } catch (const Poco::IOException& exc) {
        ofLogError("NetworkUtils::getHostByName") << exc.displayText();
        return hostEntry;
     } catch (...) {
        ofLogError("NetworkUtils::getHostByName") << "Unknown Exception: " << hostname;
        return hostEntry;
     }
     
     return hostEntry;
 }

Poco::Net::HostEntry table_hostentry;

//--------------------------------------------------------------
void ofApp::setup(){
    table_hostentry = getHostByName("OPENTABLE");
    /*
    m_connect_dialog = [[ConnectDialog alloc] initWithNibName:@"ConnectDialog" bundle:nil];
    [ofxiOSGetGLParentView() addSubview:m_connect_dialog.view];
    */
    float width = ofGetWidth();
    float height = ofGetHeight();
    printf( "[%f,%f]\n", width, height );
    //ofSetBackgroundAuto(false);
    ofSetBackgroundColor(ofColor::white);
    //
    //
    //
    m_logo.loadImage("ui/Logo.png");
    m_logo.setAnchorPercent(.5, .5);
    ofDisableArbTex();
    m_background_tile.loadImage("ui/Background.png");
    m_background_tile.getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
    ofEnableArbTex();
    //
    //
    //
    m_editor = 0;
    //
    //
    //
    m_marker_finder.setup();
    m_marker_finder.start();
    m_mode = ModeIdle;
    //
    // scan for table on local network
    //
    m_udp.Create();
    m_udp.SetNonBlocking(true);
    m_udp.Bind(8888);
    m_mode = ModeConnecting;
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( ofxiOSGetGLView().frame.size.width != [[UIScreen mainScreen] bounds].size.width ) {
        ofxiOSGetGLView().frame = CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, [[UIScreen mainScreen] bounds].size.height);
    }
    if ( m_session_client.isConnected() ) {
        m_session_client.update();
        if ( !m_session_client.hasSession() ) {
            switch ( m_mode ) {
                case ModeJoinSession :
                    m_marker_finder.update();
                    break;
            }
        } else {
            if ( m_mode == ModeJoinSession ) m_mode = ModeIdle;
        }
        if ( !openTableUI::defaultUI.isSetup() ) {
            openTableUI::defaultUI.setup();
        }
        openTableUI::defaultUI.update();
    } else {
        /*
        if ( !connection_dialog_visible ) {
            if ( table_hostentry.addresses().size() > 0 ) {
                vector<Poco::Net::IPAddress> addresses = table_hostentry.addresses();
                for ( auto& address : addresses ) {
                    string address_string = address.toString();
                    printf( "connecting to : %s\n", address_string.c_str() );
                    connect( address_string );
                    if ( m_mode == ModeJoinSession ) break;
                }
            } else {
                table_hostentry = getHostByName("OPENTABLE");
            }
        }
        */
        char buffer[ 256 ];
        memset( buffer, 0, 255 );
        int recieved = m_udp.Receive(buffer, 255);
        if ( recieved > 0 ) {
            size_t length = strlen(buffer);
            string server_address;
            for ( int i = 1;  i < length-1; i++ ) {
                if ( buffer[ i ] == ']' ) break;
                server_address.push_back( buffer[ i ] );
            }
            if ( server_address.length() > 0 ) {
                connect( server_address );
            }
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //
    // draw background
    //
    /*
    ofDisableDepthTest();
    ofSetMatrixMode(OF_MATRIX_TEXTURE);
    ofPushMatrix();
    float max_dim = max(ofGetWidth(),ofGetHeight());
    float scale = max_dim/m_background_tile.getWidth();
    ofScale(scale, scale);
    m_background_tile.getTextureReference().draw(0,0,max_dim,max_dim);
    ofPopMatrix();
    ofSetMatrixMode(OF_MATRIX_MODELVIEW);
    */
    //
    //
    //
    if ( m_session_client.isConnected() ) {
        if ( !m_session_client.hasSession() ) {
            ofPushMatrix();
            ofPushStyle();
            switch ( m_mode ) {
                case ModeJoinSession :
                    ofSetColor(ofColor::white);
                    ofVec2f scale( ofGetWidth() / m_marker_finder.getCurrentFrameReference().getWidth(), ofGetHeight() / m_marker_finder.getCurrentFrameReference().getHeight());
                    ofScale( min(scale.x,scale.y), min(scale.x,scale.y));
                    m_marker_finder.draw();
                    break;
            }
            ofPopStyle();
            ofPopMatrix();
        } else {
            //
            // draw global ui
            //
            openTableUI::defaultUI.draw();
            //
            //
            //
            ofPushStyle();
            ofSetColor(m_session_client.getSessionColour());
            ofCircle( 32, 32, 14 );
            ofPopStyle();
        }
    } else {
        if ( m_session_client.hasSession() ) {
            m_session_client.releaseSession();
            m_mode = ModeConnecting;
        }
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2.,ofGetHeight()/2.);
        ofRotate(ofGetElapsedTimef()*40.);
        m_logo.draw(0,0);
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    m_marker_finder.stop();
    m_session_client.releaseSession();
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

    ofTouchEventArgs adjusted;
    getAdjustedCoords( touch, adjusted );
    if ( openTableUI::defaultUI.isSetup() ) {
        openTableUI::defaultUI.handleTouchDown(adjusted);
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    
    ofTouchEventArgs adjusted;
    getAdjustedCoords( touch, adjusted );
    if ( openTableUI::defaultUI.isSetup() ) {
        openTableUI::defaultUI.handleTouchMoved(adjusted);
    }
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
    ofTouchEventArgs adjusted;
    getAdjustedCoords( touch, adjusted );
    if ( m_mode == ModeJoinSession ) {
        ofVec2f scale( ofGetWidth() / m_marker_finder.getCurrentFrameReference().getWidth(), ofGetHeight() / m_marker_finder.getCurrentFrameReference().getHeight());
        //ofScale(scale.x, scale.y);
        int marker_id = m_marker_finder.markerAt(adjusted.x / scale.x, adjusted.y / scale.y);
        if ( marker_id >= 0 ) {
            printf( "marker id = %d\n", marker_id );
            m_session_client.aquireSession(marker_id);
        } else {
            printf( "no marker at [%f,%f]", adjusted.x, adjusted.y);
        }
    }
    if ( openTableUI::defaultUI.isSetup() ) {
        openTableUI::defaultUI.handleTouchUp(adjusted);
    }
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    /*
    ofTouchEventArgs adjusted;
    getAdjustedCoords( touch, adjusted );
    openTableUI::defaultUI.handleTouchDoubleTap(adjusted);
    */
    if ( m_mode == ModeConnecting && !connection_dialog_visible ) {
        showConnectDialog();
    }
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    ofTouchEventArgs adjusted;
    getAdjustedCoords( touch, adjusted );
    if ( openTableUI::defaultUI.isSetup() ) {
        openTableUI::defaultUI.handleTouchCancelled(adjusted);
    }
}

//--------------------------------------------------------------
void ofApp::lostFocus(){
    saveCurrentDocument();
}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    if ( openTableUI::defaultUI.isSetup() ) {
        openTableUI::defaultUI.root()->width = ofGetWidth();
        openTableUI::defaultUI.root()->height = ofGetHeight();
    }
}

void ofApp::panGesture( ofVec2f velocity ) {
    if ( velocity.y < -1000 ) {
        if ( m_editor ) {
            saveCurrentDocument( velocity );
        }
    }
}

void ofApp::connect( string ip_address ) {
    m_mode = ModeConnecting;
    m_session_client.setup(ip_address);
    if ( !m_session_client.isConnected() ) {
        //m_mode = ModeIdle;
    } else {
        m_mode = ModeJoinSession;
    }
}

void ofApp::newDocument( string type ) {
    ofxJSONElement json;
    json[ "type" ] = type;
    if ( m_editor ) {
        //delete m_editor;
        //m_editor = 0;
        saveCurrentDocument();
    }
    if ( type == "image" ) {
        m_editor = new openTableImageEditor();
        m_mode = ModeEditImage;
    }
    if ( m_editor ) {
        m_editor->setup( json );
    }
}

void ofApp::loadDocument( string document ) {
    ofxJSONElement json;
    if ( json.parse(document) ) {
        if ( json.isMember( "type" ) ) {
            if ( m_editor ) {
                //delete m_editor;
                //m_editor = 0;
                saveCurrentDocument();
            }
            string type = json[ "type" ].asString();
            if ( type == "image" ) {
                m_editor = new openTableImageEditor;
                m_mode = ModeEditImage;
            } else if ( type == "text" ) {
                m_editor = new openTableBusinessModelEditor;
                m_mode = ModeEditText;
            } else if ( type == "session" ) {
                m_editor = new openTableSessionEditor;
                m_mode = ModeEditSession;
            }
            if ( m_editor ) {
                m_editor->setup( json );
            }
        }
    }
}

void ofApp::saveCurrentDocument( ofVec2f velocity ) {
    if ( m_editor ) {
        ofxJSONElement json;
        if ( m_editor->save( json ) ) {
            vector< string > message;
            message.push_back( "document" );
            message.push_back( json.getRawString() );
            m_session_client.sendMessage(message);
            delete m_editor;
            m_editor = 0;
        }
    }
}

void ofApp::getAdjustedCoords( ofTouchEventArgs& original, ofTouchEventArgs& adjusted ) {
    adjusted = original;
    //
    // JONS: should be no need to adjust now hardware orientation is sorted
    //
    return;
    
    ofOrientation orientation = ofGetOrientation();
    switch (orientation) {
        case OF_ORIENTATION_DEFAULT :
            break;
        case OF_ORIENTATION_180 :
            adjusted.y = ofGetHeight() - original.y;
            break;
        case OF_ORIENTATION_90_LEFT :
            adjusted.x = original.y;
            adjusted.y = ofGetHeight() - original.x;
            break;
        case OF_ORIENTATION_90_RIGHT :
            adjusted.x = ofGetWidth() - original.y;
            adjusted.y = original.x;
            break;
        case OF_ORIENTATION_UNKNOWN :
            break;
    }
}
