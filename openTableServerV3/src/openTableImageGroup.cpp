//
//  openTableImageGroup.cpp
//  openTableServer
//
//  Created by Jonathan Jones-morris on 09/02/2015.
//
//

#include "openTableImageGroup.h"
#include "ofApp.h"

static const Json::StaticString images_key("images");
static const Json::StaticString annotation_key("annotation");
static const Json::StaticString position_key("position");
static const Json::StaticString rotation_key("rotation");
static const Json::StaticString x_key("x");
static const Json::StaticString y_key("y");

openTableImageGroup::openTableImageGroup() {
    m_type = "image";
	m_rotation = 0.0;
}
openTableImageGroup::~openTableImageGroup() {
    
}
//
//
//
bool openTableImageGroup::setup( Json::Value& json ) {
    if ( openTableDocument::setup( json ) ) {
        if ( json.isMember(images_key) ) {
            for ( int i = 0; i < json[ images_key ].size(); i++ ) {
                m_images.push_back( openTableImage( json[ images_key ][ i ].asString() ) );
                //m_images.resize(m_images.size()+1);
                //m_images.back().m_filename = json[ images_key ][ i ].asString();
                
            }
        }
        if ( json.isMember(annotation_key) ) {
            m_annotation = json[ annotation_key ].asString();
        }
        if ( json.isMember(position_key) ) {
            m_position.x = json[ position_key ][ x_key ].asFloat();
            m_position.y = json[ position_key ][ y_key ].asFloat();
        }
        if ( json.isMember(rotation_key) ) {
            m_rotation = json[ rotation_key ].asFloat();
        }
        return true;
    }
    return false;
}
bool openTableImageGroup::save( Json::Value& json ) {
    if ( openTableDocument::save(json) ) {
        if ( m_images.size() > 0 ) {
            json[ images_key ].resize( m_images.size() );
            for ( int i = 0; i < m_images.size(); i++ ) {
                json[ images_key ][ i ] = m_images[ i ].m_filename;
            }
        }
        json[ annotation_key ] = m_annotation;
        json[ position_key ][ x_key ] = m_position.x;
        json[ position_key ][ y_key ] = m_position.y;
        json[ rotation_key ] = m_rotation;
		return true;
    }
	return false;
}
void openTableImageGroup::update() {
    for ( int i = 0; i < m_images.size(); i++ ) {
        if ( !m_images[ i ].isLoaded() ) {
            m_images[ i ].load();
        }
    }
}
void openTableImageGroup::draw() {
	
    float current_rotation = m_rotation - 45.0;
    float rotation_inc = 90.0 / m_images.size();
	ofPath path;
	ofPoint axis( 0, 0, 1.0);
    for ( int i = 0; i < m_images.size(); i++ ) {
        if ( m_images[ i ].isLoaded() ) {
            ofPushMatrix();
			ofPoint dim(m_images[ i ].m_image->getWidth(),m_images[ i ].m_image->getHeight());
            float scale = MIN(160./dim.x, 120./dim.y);
			dim *= scale;
            ofTranslate( m_position );
            ofRotate( current_rotation );
            m_images[ i ].m_image->draw( 0, 0, dim.x, dim.y );
            ofPopMatrix();
			//
			// update outline path
			//
			dim /= 2.;
			ofPoint topleft( -dim.x, -dim.y );
			ofPoint topright( dim.x, -dim.y );
			ofPoint bottomright( dim.x, dim.y );
			ofPoint bottomleft( -dim.x, dim.y );
			topleft.rotate(current_rotation, axis);
			topleft += m_position;
			topright.rotate(current_rotation, axis);
			topright += m_position;
			bottomright.rotate(current_rotation, axis);
			bottomright += m_position;
			bottomleft.rotate(current_rotation, axis);
			bottomleft += m_position;
			path.newSubPath();
			path.moveTo( topleft );
			path.lineTo( topright );
			path.lineTo( bottomright );
			path.lineTo( bottomleft );
			path.close();
			//
			//
			//
			current_rotation += rotation_inc;
        }
    }

    if ( m_annotation.length() > 0 ) {
        ofApp* app = ( ofApp* ) ofGetAppPtr();
        //ofRectangle bounds = app->m_default_font.getStringBoundingBox(m_annotation, 0, 0, 24.);
        ofRectangle bounds = app->m_default_font.getBBox(m_annotation, 24., 0, 0);
		ofPoint dim( bounds.width, bounds.height );
        ofPushStyle();
        ofPushMatrix();
        ofTranslate( m_position );
        ofRotate( m_rotation );
        ofTranslate( -( dim.x/2. ), 120.0  );
        ofSetColor(255, 255, 255, 127);
        ofRect( -8, -( dim.y / 2 + 16 ), dim.x + 16, dim.y + 16 );
        ofSetColor(ofColor::black);
        //app->m_default_font.drawString(m_annotation, 0, 0, 24.);
        app->m_default_font.draw(m_annotation, 24., 0, 0);
        ofPopMatrix();
        ofPopStyle();
		//
		// add to outline
		//
		ofPoint position = m_position;
		dim.x += 16;
		dim.y += 16;
		dim /= 2.;
		ofPoint topleft( -dim.x, -dim.y + 112. );
		ofPoint topright( dim.x, -dim.y + 112. );
		ofPoint bottomright( dim.x, dim.y + 112. );
		ofPoint bottomleft( -dim.x, dim.y + 112. );
		topleft.rotate(m_rotation, axis);
		topleft += position;
		topright.rotate(m_rotation, axis);
		topright += position;
		bottomright.rotate(m_rotation, axis);
		bottomright += position;
		bottomleft.rotate(m_rotation, axis);
		bottomleft += position;
		path.newSubPath();
		path.moveTo( topleft );
		path.lineTo( topright );
		path.lineTo( bottomright );
		path.lineTo( bottomleft );
		path.close();
    }
	//
	// convert path to polyline for hit test
	//
	m_hit_lock.lock();
	m_outline = path.getOutline();
	m_hit_lock.unlock();

	/*
	ofPushStyle();
	path.setStrokeWidth( 1 );
	path.setStrokeColor( ofColor::red );
	path.setFillColor( ofColor( 255, 0, 0, 128 ) );
	path.draw();
	
	ofSetColor( ofColor::green );
	ofNoFill();
	
	for ( auto& poly : m_outline ) {
		poly.draw();
	}
	ofPopStyle();
	*/
	
}
//
//
//

//
//
//
void openTableImageGroup::addImage( string filename ) {
    //
    // check for duplicate
    //
    for ( int i = 0; i < m_images.size(); i++ ) {
        if ( m_images[ i ].m_filename == filename ) {
            return; // TODO: return error
        }
    }
    //
    //
    //
    //m_images.push_back(openTableImage(filename));
    m_images.resize(m_images.size()+1);
    m_images.back().m_filename = filename;
}
void openTableImageGroup::removeImage( string path ) {
    
}
string openTableImageGroup::getAnnotation() {
    return m_annotation;
}
void openTableImageGroup::setAnnotation( string annotation ) {
    m_annotation = annotation;
}
//
//
//
int openTableImageGroup::getHitPart( ofVec2f p ) {
	m_hit_lock.lock();
	for ( auto& poly : m_outline ) {
		if (poly.size()>1&&poly.inside(p) ) {
			float distance = m_position.distance(p);
			if ( distance < 160./4.) {
				m_hit_lock.unlock();
				return 1;
			} else {
				m_hit_lock.unlock();
				return 2;
			}
		}
	}
	m_hit_lock.unlock();
	return 0;
}

