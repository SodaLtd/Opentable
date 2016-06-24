//
//  openTableImageGroup.h
//  openTableServer
//
//  Created by Jonathan Jones-morris on 09/02/2015.
//
//

#pragma once

#include "openTableDocument.h"
#include "openTableImageCache.h"

class openTableImage {
public:
    openTableImage() {
        m_image = openTableImageCache::IMAGE_NOT_FOUND;
    }
    
    openTableImage( string filename ) {
        m_filename = filename;
        m_image = openTableImageCache::IMAGE_NOT_FOUND;
    }
    
    ~openTableImage() {
        openTableImageCache::shared.release(m_image);
        m_image = openTableImageCache::IMAGE_NOT_FOUND;
    }
    openTableImage& operator=(const openTableImage other) {
        m_filename = other.m_filename;
        if ( other.isLoaded() ) {
            m_image = openTableImageCache::shared.get("images/" + m_filename);
        } else {
            m_image = openTableImageCache::IMAGE_NOT_FOUND;
        }
        return *this;
    }
    /*
    openTableImage& operator=(openTableImage& other) {
        m_filename = other.m_filename;
        m_image = openTableImageCache::shared.get("images/" + m_filename);
        return *this;
    }
    */
    bool isLoaded() const {
        return m_image != openTableImageCache::IMAGE_NOT_FOUND;
    }
    void load() {
        m_image = openTableImageCache::shared.get("images/" + m_filename);
		if ( m_image ) {
			m_image->setAnchorPercent(0.5,0.5);
		}
    }
    string  m_filename;
    ofImage* m_image;
};

class openTableImageGroup : public openTableDocument, public openTableSelectableItem {
public:
    openTableImageGroup();
    openTableImageGroup(Json::Value& json);
    virtual ~openTableImageGroup();
    //
    //
    //
    bool setup( Json::Value& json );
    bool save( Json::Value& json );
    //
    //
    //
    void update();
    void draw();
    //
    //
    //
    void setPosition( ofVec2f position ) { m_position = position; }
    void setRotation( float rotation ) { m_rotation = rotation; }
    ofVec2f& getPosition() { return m_position; };
    float getRotation() { return m_rotation; };
    int getHitPart( ofVec2f p );
    //
    //
    //
    int nImages() { return m_images.size(); };
    void addImage( string filename );
    void removeImage( string filename );
    string getAnnotation();
    void setAnnotation( string annotation );
protected:
    vector< openTableImage >    m_images;
    string                      m_annotation;
    ofVec2f                     m_position;
    float                       m_rotation;
	//
	//
	//
	ofMutex						m_hit_lock;
	vector<ofPolyline>			m_outline;
};