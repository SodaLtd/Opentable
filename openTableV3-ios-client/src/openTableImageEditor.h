//
//  openTableImageEditor.h
//  openTable
//
//  Created by Jonathan Jones-morris on 10/02/2015.
//
//
#pragma once

#include "openTableEditor.h"

class openTableImageEditor : public openTableEditor {
public:
    openTableImageEditor();
    virtual ~openTableImageEditor();
    //
    //
    //
    virtual bool setup( ofxJSONElement& json );
    virtual bool save( ofxJSONElement& json );
    //
    //
    //
    int imagesInCollection();
    string pathForCollectionImage( int i );
    int imagesInLibrary();
    string pathForLibraryImage( int i );
    //
    //
    //
    void moveImageFromLibraryToCollection( int i );
    void moveImageFromCollectionToLibrary( int i );
    //
    //
    //
    void setAnnotation( string annotation ) { m_annotation = annotation; };
    string getAnnotation() { return m_annotation; };
protected:
    string              m_annotation;
    vector< string >    m_collection_images;
    vector< string >    m_library_images;
};