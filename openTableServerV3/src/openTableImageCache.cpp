//
//  openTableImageCache.cpp
//  openTableServer
//
//  Created by Jonathan Jones-morris on 19/02/2015.
//
//

#include "openTableImageCache.h"

openTableImageCache openTableImageCache::shared;
ofImage* openTableImageCache::IMAGE_NOT_FOUND = 0;

openTableImageCache::openTableImageCache() {
    
}
openTableImageCache::~openTableImageCache() {
    
}
//
//
//
ofImage* openTableImageCache::get( string name )  {
    map< string, openTableImageCacheEntry >::iterator it = m_entries.find( name );
    if ( it != m_entries.end() ) {
        it->second.addRef();
        return &( it->second.m_image );
    } else {
        ofImage image;
        if ( image.loadImage(ofToDataPath(name)) ) {
            m_entries[ name ].m_image = image;
            m_entries[ name ].addRef();
            return &( m_entries[ name ].m_image );
        }
    }
    return IMAGE_NOT_FOUND;
}
/*
void openTableImageCache::release( string name ) {
    map< string, openTableImageCacheEntry >::iterator it = m_entries.find( name );
    if ( it != m_entries.end() ) {
        if ( it->second.release() <= 0 ) {
            m_entries.erase(it);
        }
    }
}
*/

void openTableImageCache::release( ofImage* image ) {
    map< string, openTableImageCacheEntry >::iterator it = m_entries.begin();
    for ( ; it != m_entries.end(); it++ ) {
        if ( &( it->second.m_image ) == image ) {
			if ( --( it->second.m_ref ) <= 0 ) { 
				m_entries.erase(it);
			}
            break;
        }
    }
}

