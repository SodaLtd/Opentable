//
//  openTableDocument.cpp
//  openTableServer
//
//  Created by Jonathan Jones-morris on 09/02/2015.
//
//

#include "openTableDocument.h"
/*
openTableDocument* openTableDocument::documentFromJson( Json::Value& json ) {
    if ( json.isMember( type_key ) ) {
        string type = json[ type_key ].asString();
    }
    return false;
}
*/

bool openTableDocument::isSelected( openTableSelectableItem* item ) {
	for ( auto& selection : m_selection ) {
		if ( selection.second.m_item == item ) {
			return true;
		}
	}
	return false;
}

#include "openTableSession.h"
#include "openTableImageCollection.h"
#include "openTableBusinessModelCanvas.h"

openTableDocument* openTableDocument::newDocument( Json::Value& json ) {
    if ( json.isMember(type_key) ) {
        string type = json[ type_key ].asString();
        if ( type == "session" ) {
            return new openTableSession( json );
        } else if ( type == "image_collection" ) {
            return new openTableImageCollection( json );
        } else if ( type == "business_model" ) {
            return new openTableBusinessModelCanvas( json );
        }
    }
    return 0;
}
openTableDocument* openTableDocument::newDocument( string type ) {
    if ( type == "session" ) {
        return new openTableSession;
    } else if ( type == "image_collection" ) {
        return new openTableImageCollection;
    } else if ( type == "business_model" ) {
        return new openTableBusinessModelCanvas;
    }
    return 0;
}
