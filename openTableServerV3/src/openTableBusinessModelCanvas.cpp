//
//  openTableBusinessModelCanvas.cpp
//  openTableServer
//
//  Created by Jonathan Jones-morris on 19/02/2015.
//
//

#include "openTableBusinessModelCanvas.h"
#include "openTableImageCache.h"
#include "ofApp.h"
//
//
//

//
//
//
const int kCanvasColumns    = 5;
const int kCanvasRows       = 3;
const float kElementHeaderFontSize = 24.0;
const float kElementBodyFontSize = 14.0;

//
//
//
/*
 openTableBusinessModelCanvasElementParteners,
 openTableBusinessModelCanvasElementActivities,
 openTableBusinessModelCanvasElementResources,
 openTableBusinessModelCanvasElementProposition,
 openTableBusinessModelCanvasElementCustomerRelationships,
 openTableBusinessModelCanvasElementDistributionChannels,
 openTableBusinessModelCanvasElementCustomerSegment,
 openTableBusinessModelCanvasElementCostStructure,
 openTableBusinessModelCanvasElementRevenueStream,
 openTableBusinessModelCanvasElementCount
*/
const string kElementTitles[ openTableBusinessModelCanvasElement::openTableBusinessModelCanvasElementCount ] = {
    "Partners",
    "Activities",
    "Resources",
    "Proposition",
    "Relationships",
    "Channels",
    "Segment",
    "Cost",
    "Revenue"
};
//
// element bounds expressed in columns and rows
//
const float kElementBounds[ openTableBusinessModelCanvasElement::openTableBusinessModelCanvasElementCount ][ 4 ] = {
    { 0, 0, 1, 2 },     // partners
    { 1, 0, 1, 1 },     // activities
    { 1, 1, 1, 1 },     // resources
    { 2, 0, 1, 2 },     // propostion
    { 3, 0, 1, 1 },     // relationships
    { 3, 1, 1, 1 },     // distribution
    { 4, 0, 1, 2 },     // segment
    { 0, 2, 2.5, 1 },   // cost
    { 2.5, 2, 2.5, 1 }  // revenue
};
//
//
//
static const Json::StaticString elements_key("elements");
static const Json::StaticString element_type_key("element_type");
static const Json::StaticString element_title_key("title");
static const Json::StaticString element_content_key("content");
//
//
//
openTableBusinessModelCanvasElement::openTableBusinessModelCanvasElement()  {
    m_flip = 0;
    m_orientation = 0;
    m_locked = false;
}
openTableBusinessModelCanvasElement::~openTableBusinessModelCanvasElement() {
    openTableImageCache::shared.release(m_flip);
}
//
//
//
bool openTableBusinessModelCanvasElement::setup( int type ) {
    if ( type < openTableBusinessModelCanvasElementCount ) {
        m_type = type;
        m_title = kElementTitles[ type ];
        return true;
    }
	return false;
}
bool openTableBusinessModelCanvasElement::setup( Json::Value& json ) {
    /*
    int     m_type;
    string  m_title;
    string  m_content;
    */
    m_type = json[ element_type_key ].asInt();
    m_title = json[ element_title_key ].asString();
    m_content = json[ element_content_key ].asString();
	return true;
}
bool openTableBusinessModelCanvasElement::save( Json::Value& json ) {
    json[ "type" ]                = "text";
    json[ element_type_key ]      = m_type;
    json[ element_title_key ]     = m_title;
    json[ element_content_key ]   = m_content;
	return true;
}
void openTableBusinessModelCanvasElement::update() {
    if ( m_locked ) {
        //
        // check lock is still valid
        //
        if ( !((ofApp*) ofGetAppPtr() )->m_server.isSessionConnected(m_lock_session) ) {
            unlock();
        }
    }
    if ( m_flip == 0 ) {
        m_flip = openTableImageCache::shared.get("ui/FlipElement.png");
    }
}
void openTableBusinessModelCanvasElement::draw() {
	start_clip();
    float scale = ofGetHeight() / 1080.;
    float margin = 8. * scale;
    float margin2 = margin * 2.;
    ofRectangle outline = m_bounds;
    outline.x += margin; outline.y += margin;
    outline.width -= margin2; outline.height -= margin2;
    if ( m_locked ) {
        ofPushStyle();
        ofRectangle outline = m_bounds;
        outline.x += margin; outline.y += margin;
        outline.width -= margin2; outline.height -= margin2;
        ofSetColor(m_lock_colour,127);
        ofFill();
        ofRect( outline );
        ofNoFill();
        ofRect( outline );
        
        ofPopStyle();
    } else {
        ofPushMatrix();
        ofPushStyle();
        //
        // setup transform ( for flip )
        //
        bool moving = m_offset.x != 0 || m_offset.y != 0;
        int alpha = 255;
        ofPoint centre = m_bounds.getCenter();
        if ( moving ) {
            alpha = 127;
            ofTranslate(m_offset.x, m_offset.y);
        } else {
            ofTranslate(centre.x, centre.y);
        }
        ofRotate( m_orientation==0 ? 0.0 : 180.0 );
        ofTranslate(-centre.x, -centre.y);
        //
        // background and frame
        //
        ofFill();
        ofSetColor(255,255,255,alpha);
        ofRect( outline );
        ofNoFill();
        ofSetColor(0,0,0,alpha);
        ofRect( outline );
        //
        // title
        //
		ofFill();
		float font_size = kElementHeaderFontSize * scale;
        ofApp* app = ( ofApp* ) ofGetAppPtr();
        float y_offset = m_flip->getHeight()*scale - ( m_flip->getHeight()*scale - font_size ) / 2.;
        
        //app->m_default_font.drawString(m_title, outline.x + margin/2., outline.y + y_offset, font_size, outline.width - m_flip->getWidth()*scale );
		app->m_default_font.draw(m_title, font_size, outline.x + margin/2., outline.y + y_offset);
        //
        // content
        //
        if ( m_content.length() > 0 ) {
            y_offset = ( m_flip->getHeight() + margin ) *scale;
            font_size = kElementBodyFontSize * ( ofGetWidth() / 1920.0 );
            //app->m_default_font.drawString(m_content, outline.x + margin, outline.y + y_offset, font_size, outline.width - margin2 );
            int num_lines;
            app->m_default_font.drawMultiLineColumn(
                                                    m_content,
                                                    font_size,
                                                    outline.x + margin, outline.y + y_offset,
                                                    outline.width - margin2,
                                                    num_lines );
        }
        //
        // flip button
        //
        m_flip->draw( outline.getRight() - (m_flip->getWidth()*scale), outline.getTop(), (m_flip->getWidth()*scale), (m_flip->getHeight()*scale) );
        //
        //
        //
        ofPopStyle();
        ofPopMatrix();
    }
	end_clip();
}
void openTableBusinessModelCanvasElement::start_clip() {
	ofPoint center = m_offset.length() > 0. ? m_offset : m_bounds.getCenter();
	ofRectangle clip_rect = m_bounds;
	clip_rect.x = center.x - ( clip_rect.getWidth() / 2. );
	clip_rect.y = center.y - ( clip_rect.getHeight() / 2. );
	//
	// adjust for scaled margin
	//
	float scale = ofGetHeight() / 1080.;
	float margin = 8. * scale;
	float margin2 = margin * 2.;
	clip_rect.x += margin; clip_rect.y += margin;
	clip_rect.width -= margin2; clip_rect.height -= margin2;

	GLint view[4];
	glGetIntegerv(GL_VIEWPORT, &view[0]);
	clip_rect.y = view[3] - (clip_rect.y + clip_rect.getHeight());
	glScissor(clip_rect.x, clip_rect.y, clip_rect.getWidth(), clip_rect.getHeight());
	glEnable(GL_SCISSOR_TEST);
}
void openTableBusinessModelCanvasElement::end_clip() {
	glDisable(GL_SCISSOR_TEST);
}
//
// interaction
//
int openTableBusinessModelCanvasElement::hitPart( float x, float y ) {
    if ( !m_locked && m_bounds.inside(x, y) ) {
        //
        // test for flip
        //
        ofRectangle r;
        r.x = m_bounds.x + ( m_orientation == 0 ? m_bounds.width - 32 : 0 );
        r.y = m_bounds.y + ( m_orientation == 0 ? 0 : m_bounds.height - 32 );
        r.width = 64; r.height = 64;
        return r.inside(x, y) ? openTableBusinessModelCanvasElementFlip : openTableBusinessModelCanvasElementMove;
    }
    return -1;
}
//
//
//
openTableBusinessModelCanvas::openTableBusinessModelCanvas() {
    m_type = "business_model";
    setup();
}
openTableBusinessModelCanvas::openTableBusinessModelCanvas(Json::Value& json) {
    m_type = "business_model";
    setup(json);
}

openTableBusinessModelCanvas::~openTableBusinessModelCanvas() {
    
}
//
//
//
bool openTableBusinessModelCanvas::setup() {
    for ( int i = 0; i < openTableBusinessModelCanvasElement::openTableBusinessModelCanvasElementCount; i++ ) {
        m_elements[ i ].setup(i);
    }
    return true;
}
bool openTableBusinessModelCanvas::setup( Json::Value& json ) {
    if ( openTableDocument::setup(json) ) {
        if ( json.isMember( elements_key ) ) {
            for ( int i = 0; i < json[ elements_key ].size(); i++ ) {
                m_elements[ i ].setup(json[ elements_key ][ i ]);
            }
        } else {
            for ( int i = 0; i < openTableBusinessModelCanvasElement::openTableBusinessModelCanvasElementCount; i++ ) {
                m_elements[ i ].setup(i);
            }
        }
        //
        // force layout on first update
        //
        m_width = m_height = -1;
        return true;
    }
    return false;
}
bool openTableBusinessModelCanvas::save( Json::Value& json ) {
    if ( openTableDocument::save(json) ) {
        int count = m_elements.size();
        json[ elements_key ].resize(count);
        for ( int i = 0; i < count; i++ ) {
            m_elements[ i ].save( json[ elements_key ][ i ] );
        }
        return true;
    }
	return false;
}
void openTableBusinessModelCanvas::update() {
    //
    // update layout
    //
    if ( m_width != ofGetWidth() || m_height != ofGetHeight() ) {
        layout();
    }
    //
    //
    //
    for ( auto& element : m_elements ) {
        element.second.update();
    }
}
void openTableBusinessModelCanvas::draw() {
    for ( auto& element : m_elements ) {
        if ( !isSelected( &( element.second ) ) ) element.second.draw();
    }
	for ( auto& selection : m_selection ) {
		if ( selection.second.m_item ) {
			((openTableBusinessModelCanvasElement*)selection.second.m_item)->draw();
		}
	}
}
//
//
//
void openTableBusinessModelCanvas::touchMove(int x, int y, int id) {
	if (m_selection.find(id) == m_selection.end()) return;
	if ( m_selection[ id ].m_item && m_selection[ id ].m_action == openTableBusinessModelCanvasElement::openTableBusinessModelCanvasElementMove ) {
		((openTableBusinessModelCanvasElement*)m_selection[ id ].m_item)->moveTo(x, y);
	}
}

void openTableBusinessModelCanvas::touchDown(int x, int y, int id) {
	m_selection[ id ].m_item = 0;
	m_selection[ id ].m_action = 0;
    for ( auto& element : m_elements ) {
        int part = element.second.hitPart(x, y);
        if ( part > 0 ) {
			m_selection[ id ].m_item = &( element.second );
			m_selection[ id ].m_action = part;
            break;
        }
    }
}

void openTableBusinessModelCanvas::touchUp(int x, int y, int id) {
    //
    // check if drop is over marker
    //
    if ( m_selection[ id ].m_item ) {
        if ( m_selection[ id ].m_action == openTableBusinessModelCanvasElement::openTableBusinessModelCanvasElementMove ) {
            ofApp* app = ( ofApp* )ofGetAppPtr();
            if ( app ) {
                int session_id = app->m_server.getSessionAtPoint( x, y );
                if ( session_id > 0 ) {
                    //
                    // send group to session
                    //
                    ofxJSONElement group_json;
                    ((openTableBusinessModelCanvasElement*)m_selection[ id ].m_item)->save(group_json);
                    string group_json_string = group_json.getRawString(false);
                    vector< string > message;
                    message.push_back("document");
                    message.push_back( group_json_string );
                    if ( app->m_server.sendSessionMessage(session_id, message) ) {
                        
                        //
                        // lock element
                        //
                        ofColor session_colour = ((ofApp*)ofGetAppPtr())->m_server.getSessionColour(session_id);
                       ((openTableBusinessModelCanvasElement*)m_selection[ id ].m_item)->lock(session_id,session_colour);
                    }
                }
            }
        } else if ( m_selection[ id ].m_action == openTableBusinessModelCanvasElement::openTableBusinessModelCanvasElementFlip ){
			int part = ((openTableBusinessModelCanvasElement*)m_selection[ id ].m_item)->hitPart( x, y );
			if ( part > 0 ) {
				((openTableBusinessModelCanvasElement*)m_selection[ id ].m_item)->flip();
			}
        }
        ((openTableBusinessModelCanvasElement*)m_selection[ id ].m_item)->reset();
    }
    //
    //
    //
	m_selection.erase( id );
	//m_selection[ id ].m_item = 0;
	//m_selection[ id ].m_action = 0;
}
void openTableBusinessModelCanvas::updateElement( Json::Value& json ) {
    int element_type = json.isMember("element_type") ? json["element_type"].asInt() : -1;
    if ( element_type >= 0 ) {
        m_elements[ element_type ].setup(json);
        m_elements[ element_type ].unlock(); // JONS: assume we unlock at this point
    }
}
//
//
//
void openTableBusinessModelCanvas::layout() {
    m_width     = ofGetWidth();
    m_height    = ofGetHeight();
    ofApp* app = ( ofApp* )ofGetAppPtr();
    ofRectangle safe_rect = app->m_server.getSafeZone();
    float column_width  = safe_rect.width / kCanvasColumns;
    float row_height    = safe_rect.height / kCanvasRows;
    for ( int i = 0; i < openTableBusinessModelCanvasElement::openTableBusinessModelCanvasElementCount; i++ ) {
        m_elements[ i ].setBounds(
                                                   safe_rect.x+kElementBounds[i][0]*column_width,
                                                   safe_rect.y+kElementBounds[i][1]*row_height,
                                                   kElementBounds[i][2]*column_width,
                                                   kElementBounds[i][3]*row_height);
    }
}
