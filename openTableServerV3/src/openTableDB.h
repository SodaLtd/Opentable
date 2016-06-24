//
//  openTableDB.h
//  openTableServer
//
//  Created by Jonathan Jones-morris on 09/02/2015.
//
//

#pragma once

#include "ofMain.h"
#include "ofxJSONElement.h"
#include "openTableDocument.h"

class openTableDB {
public:
    openTableDB();
    virtual ~openTableDB();
    //
    //
    //
    void setup();
    //
    //
    //
    bool save();
    bool load();
    //
    //
    //
    openTableDocument* get( string document_id );
    openTableDocument* add( string type );
    void remove( string document_id );
    vector< openTableDocument* > getDocumentsOfType( string type );
    //
    //
    //
    void setPendingDelete( string document_id );
    void clearPendingDelete( string document_id );
    void markAllDocumentsOfTypeForDelete( string type );
    void commitPendingDeletes();
	//
	//
	//
	string toString();
protected:
    //
    // TODO: make this a shared_ptr
    //
    map< string, openTableDocument* > m_documents;
};