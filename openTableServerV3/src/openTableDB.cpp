//
//  openTableDB.cpp
//  openTableServer
//
//  Created by Jonathan Jones-morris on 09/02/2015.
//
//

#include "openTableDB.h"
#include "openTableImageCollection.h"
#include "openTableBusinessModelCanvas.h"

const string kDefaultDB = "opentable.db";

openTableDB::openTableDB() {
    
}
openTableDB::~openTableDB() {
    
}
//
//
//
void openTableDB::setup() {
    load();
}
//
//
//
bool openTableDB::save() {
    ofxJSONElement json;
    if ( m_documents.size() > 0 ) {
        json.resize(m_documents.size());
        map< string, openTableDocument* >::iterator it = m_documents.begin();
        for ( int i = 0; i < m_documents.size(); i++, it++ ) {
            it->second->save( json[ i ] );
        }        
    }
    return json.save(kDefaultDB);
}
bool openTableDB::load() {
    ofxJSONElement json;
    if ( json.open(ofToDataPath(kDefaultDB)) ) {
        for ( int i = 0; i < json.size(); i++ ) {
            if ( json[ i ].isMember(id_key) ) {
                string id = json[ i ][ id_key ].asString();
                m_documents[ id ] = openTableDocument::newDocument(json[i]);
            }
        }
		return true;
    } else {
        /*
        //
        // create and save default db
        //
        openTableImageCollection* collection = ( openTableImageCollection* ) add( "image_collection" );
        openTableImageGroup* group = new openTableImageGroup;
        group->setPosition(ofVec2f(ofGetWidth() / 2.0, ofGetHeight() / 2.0));
        for ( int i = 0; i < 4; i++ ) {
            string filename = "image" + ofToString(i, 2, '0') + ".jpg";
            group->addImage(filename);
        }
        collection->addGroup(group);
        
        openTableBusinessModelCanvas* canvas = ( openTableBusinessModelCanvas* ) add( "business_model" );
        canvas->setup();
        
        return save();
        */
        return false;
    }
}
//
//
//
openTableDocument* openTableDB::get( string document_id ) {
    if ( m_documents.find(document_id) != m_documents.end() ) {
        return m_documents[ document_id ];
    }
    return 0;
}

vector< openTableDocument* > openTableDB::getDocumentsOfType( string type ) {
    vector< openTableDocument* > results;
    map< string, openTableDocument* >::iterator it = m_documents.begin();
    for ( ; it != m_documents.end(); it++ ) {
        if ( it->second->getType() == type ) {
            results.push_back(it->second);
        }
    }
    return results;
}

openTableDocument* openTableDB::add( string type )  {
    openTableDocument* document = openTableDocument::newDocument(type);
    if ( document ) {
        m_documents[ document->getId() ] = document;
    }
    return document;
}

void openTableDB::remove( string document_id ) {
    m_documents.erase( document_id );
}
//
//
//
void openTableDB::setPendingDelete( string document_id ) {
    if ( m_documents.find(document_id) != m_documents.end() ) {
        m_documents[ document_id ]->markForDelete();
    }
}
void openTableDB::clearPendingDelete( string document_id ) {
    if ( m_documents.find(document_id) != m_documents.end() ) {
        m_documents[ document_id ]->clearMarkForDelete();
    }
}
void openTableDB::markAllDocumentsOfTypeForDelete( string type ) {
    map< string, openTableDocument* >::iterator it = m_documents.begin();
    for ( ; it != m_documents.end(); it++ ) {
        if ( it->second->getType() == type ) {
            it->second->markForDelete();
        }
    }
}
void openTableDB::commitPendingDeletes() {
    for ( int i = 0; i < m_documents.size(); ) {
        map< string, openTableDocument* >::iterator it = m_documents.begin();
        advance(it, i);
        if ( it->second->isPendingDelete() ) {
            m_documents.erase(it);
        } else {
            i++;
        }
    }
}
//
//
//
string openTableDB::toString() {
	ofxJSONElement json;
	map< string, openTableDocument* >::iterator it = m_documents.begin();
	for (; it != m_documents.end(); it++) {
		Json::Value document_json;
		it->second->save(document_json);
		json.append(document_json);
	}
	return json.getRawString(false);
}

