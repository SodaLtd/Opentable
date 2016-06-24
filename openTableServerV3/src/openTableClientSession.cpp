//
//  openTableClientSession.cpp
//  openTableServer
//
//  Created by Jonathan Jones-morris on 04/02/2015.
//
//

#include "openTableClientSession.h"
#include "openTableImageCache.h"
#include "openTableClientSessionServer.h"

openTableClientSession::openTableClientSession() {
    m_session_id    = -1;
    m_client_id     = -1;
    m_session_indicator = openTableImageCache::IMAGE_NOT_FOUND;
}
openTableClientSession::~openTableClientSession() {
    openTableImageCache::shared.release(m_session_indicator);
}
//
//
//
void openTableClientSession::setup( int session_id, ofImage& marker_image) {
    m_session_id = session_id;
    m_marker_image = marker_image;
    m_marker_image.setAnchorPercent(0.5, 0.5);
    m_session_colour = ofColor::black;
    m_session_indicator = openTableImageCache::shared.get("ui/SessionIndicator.png");
	m_station = 0;

}

void openTableClientSession::update() {
    
}
void openTableClientSession::draw( ofPoint& p, float width, float height ) {
    draw(p.x,p.y, width, height);
}
void openTableClientSession::draw( float x, float y, float width, float height ) {
    if ( isAvailable() ) {
        //
        //
        //
        m_marker_image.draw( x, y,
                            width > 0 ? width : m_marker_image.getWidth(),
                            height > 0 ? height : m_marker_image.getHeight());
    } else {
        if ( m_session_indicator != openTableImageCache::IMAGE_NOT_FOUND ) {
            //
            //
            //
            ofPushStyle();
            ofSetColor(ofColor::white);
            m_session_indicator->draw(x-(width/2), y-(height/2), width, height);
            ofSetColor(m_session_colour);
            m_session_indicator->draw(x-(width/2), y-(height/2), width, height);
            ofPopStyle();
        }
    }
}

