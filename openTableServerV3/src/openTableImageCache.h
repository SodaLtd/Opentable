//
//  openTableImageCache.h
//  openTableServer
//
//  Created by Jonathan Jones-morris on 19/02/2015.
//
//
#pragma once

#include "ofMain.h"


class openTableImageCacheEntry {
public:
    openTableImageCacheEntry() {
        m_ref = 0;
    }
    void addRef() { m_ref++; };
    int release() { return --m_ref; };
    //
    //
    //
    ofImage m_image;
    int     m_ref;
};

class openTableImageCache {
public:
    openTableImageCache();
    ~openTableImageCache();
    //
    //
    //
    ofImage* get( string name );
    //void release( string name );
    void release( ofImage* image );
    //
    //
    //
    static openTableImageCache shared;
    static ofImage* IMAGE_NOT_FOUND;
protected:
    map< string, openTableImageCacheEntry > m_entries;
};