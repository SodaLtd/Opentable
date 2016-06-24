//
//  openTableImageCollection.cpp
//  openTableServer
//
//  Created by Jonathan Jones-morris on 09/02/2015.
//
//

#include "openTableImageCollection.h"
#include "ofApp.h"

static const Json::StaticString groups_key("groups");

openTableImageCollection::openTableImageCollection() {
    m_type = "image_collection";
}
openTableImageCollection::openTableImageCollection(Json::Value& json) {
    setup(json);
}
openTableImageCollection::~openTableImageCollection() {
    clear();
}
//
//
//
bool openTableImageCollection::setup( Json::Value& json ) {
    if ( openTableDocument::setup(json) ) {
        if ( json.isMember( groups_key ) ) {
            for ( int i = 0; i < json[ groups_key ].size(); i++ ) {
                //m_groups.push_back(openTableImageGroup(json[ groups_key ][ i ]));
                openTableImageGroup* group = new openTableImageGroup;
                group->setup(json[ groups_key ][ i ]);
                m_groups.push_back(group);
            }
        }
        return true;
    }
    return false;
}
bool openTableImageCollection::save( Json::Value& json ) {
    if ( openTableDocument::save(json) ) {
        json[ groups_key ].resize(m_groups.size());
        for ( int i = 0; i < m_groups.size(); i++ ) {
            m_groups[ i ]->save( json[ groups_key ][ i ] );
        }
        return true;
    }
    return false;
}
void openTableImageCollection::update() {
    lock();
    for ( auto& group : m_groups ) {
        group->update();
    }
    unlock();
}
void openTableImageCollection::draw() {
    lock();
    for ( auto& group : m_groups ) {
        if ( !isSelected(group) ) {
            group->draw();
        }
    }
    ofPushStyle();
	for ( auto& selection : m_selection ) {
		if ( selection.second.m_item ) {
			if ( selection.second.m_action == 1 ) ofSetColor(255, 255, 255, 127);
			((openTableImageGroup*)selection.second.m_item)->draw();
		}
	}
    ofPopStyle();
    unlock();
}
//
// TODO: move mouse / touch handling into imageGroup
//
void openTableImageCollection::touchMove(int x, int y, int id) {
	if (m_selection.find(id) == m_selection.end()) return;
    ofVec2f p( x, y );
    if ( m_selection[ id ].m_item ) {
        switch( m_selection[ id ].m_action ) {
            case 1 :
                ((openTableImageGroup*)m_selection[ id ].m_item)->setPosition(p);
                break;
            case 2 : {
                ofVec2f posn = ((openTableImageGroup*)m_selection[ id ].m_item)->getPosition();
                float last_angle = ofRadToDeg( atan2( m_selection[ id ].m_last_position.y - posn.y, m_selection[ id ].m_last_position.x - posn.x ) );
                float this_angle = ofRadToDeg( atan2( p.y - posn.y, p.x - posn.x ) );
                float dr = this_angle - last_angle;
                ((openTableImageGroup*)m_selection[ id ].m_item)->setRotation(((openTableImageGroup*)m_selection[ id ].m_item)->getRotation() + dr);
				printf( "rotating: %f\n", ((openTableImageGroup*)m_selection[ id ].m_item)->getRotation() );
            }
        }
    }
    m_selection[ id ].m_last_position = p;
}
void openTableImageCollection::touchDown(int x, int y, int id) {
    ofVec2f p( x, y );
	m_selection[ id ].m_item = 0;
	m_selection[ id ].m_action = 0;
    for ( auto& group : m_groups ) {
		if (!isSelected(group)) {
			int part = group->getHitPart(p);
			if (part > 0) {
				m_selection[id].m_item = group;
				m_selection[id].m_action = part;
				m_selection[id].m_last_position = p;
				break;
			}
		}
    }
}
void openTableImageCollection::touchUp(int x, int y, int id) {
	if (m_selection.find(id) == m_selection.end()) return;
    //
    // check if drop is over marker
    //
    if ( m_selection[ id ].m_action == 1 ) {
        ofApp* app = ( ofApp* )ofGetAppPtr();
        if ( app ) {
            int session_id = app->m_server.getSessionAtPoint( x, y );
            if ( session_id > 0 ) {
                //
                // send group to session
                //
                ofColor session_colour = ((ofApp*) ofGetAppPtr())->m_server.getSessionColour(session_id);
                ofxJSONElement group_json;
				((openTableImageGroup*)m_selection[ id ].m_item)->save(group_json);
                string group_json_string = group_json.getRawString();
                vector< string > message;
                message.push_back("document");
                message.push_back( group_json_string );
                if ( app->m_server.sendSessionMessage(session_id, message) ) {
                    //
                    // remove group from collection
                    //
					printf( "removing group\n" );
                    removeGroup(((openTableImageGroup*)m_selection[ id ].m_item)->getId());
                }
            }
        }
    }
    //
    //
    //
	m_selection.erase( id );
    //m_selection[ id ].m_item = 0;
    //m_selection[ id ].m_action = 0;
}
//
//
//
void openTableImageCollection::addGroup( openTableImageGroup* group ) {
    lock();
    m_groups.push_back(group);
    unlock();
}
void openTableImageCollection::removeGroup( string id ) {
    lock();
    for ( int i = 0; i < m_groups.size(); i++ ) {
        if ( m_groups[ i ]->getId() == id ) {
            delete m_groups[ i ];
            m_groups.erase(m_groups.begin()+i);
            break;
        }
    }
    unlock();
}
void openTableImageCollection::clear() {
    lock();
    for ( int i = 0; i < m_groups.size(); i++ ) {
        delete m_groups[ i ];
    }
    m_groups.clear();
    unlock();
}