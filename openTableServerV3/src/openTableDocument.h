//
//  openTableDocument.h
//  openTableServer
//
//  Created by Jonathan Jones-morris on 09/02/2015.
//
//

#pragma once

#include "ofMain.h"
#include "ofxJSONElement.h"

static const Json::StaticString id_key("id");
static const Json::StaticString type_key("type");

class openTableSelectableItem {
public:
	virtual void select() {};
	virtual void deselect() {};
};
//
//
//
class openTableDocumentSelection {
public:
	openTableDocumentSelection() {};
	~openTableDocumentSelection() {};
	//
	//
	//
	openTableSelectableItem*	m_item;
	int							m_action;
	ofVec2f						m_last_position;
};

class openTableDocument {
public:
	//
	//
	//
    openTableDocument() {
        //m_id = ofToString( ofGetUnixTime() );
        m_id = ofToString( ofGetSystemTimeMicros() );
		m_pending_delete = false;
    };
    virtual ~openTableDocument() {};
    //
    //
    //
    virtual bool setup() {
        return true;
    }
    virtual bool setup( Json::Value& json ) {
        if (  json.isMember( type_key ) ) {
            m_id = json.isMember( id_key ) ? json[ id_key ].asString() : ofToString( ofGetUnixTime() );
            m_type = json[ type_key ].asString();
            return true;
        }
        return false;
    }
    virtual bool save( Json::Value& json ) {
        json[ id_key ]      = m_id;
        json[ type_key ]    = m_type;
        return true;
    }
    virtual void update() {};
    virtual void draw() {};
    //
    //
    //
    string getId() { return m_id; };
    string getType() { return m_type; };
    //
    //
    //
	virtual void touchMove(int x, int y, int id) {};
	virtual void touchDown(int x, int y, int id) {};
	virtual void touchUp(int x, int y, int id) {};
    //
    //
    //
    void markForDelete() { m_pending_delete = true; };
    void clearMarkForDelete() { m_pending_delete = false; };
    bool isPendingDelete() { return m_pending_delete; };
    //
    //
    //
    static openTableDocument* newDocument( string type );
    static openTableDocument* newDocument( Json::Value& json );
protected:
    string m_id;
    string m_type;
	bool isSelected( openTableSelectableItem* item );
	map< int, openTableDocumentSelection >	m_selection;
    //
    //
    //
    bool    m_pending_delete;
};