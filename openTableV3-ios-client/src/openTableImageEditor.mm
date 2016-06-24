//
//  openTableImageEditor.cpp
//  openTable
//
//  Created by Jonathan Jones-morris on 10/02/2015.
//
//

#include "openTableImageEditor.h"
#include "openTableUI.h"

#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "ImageViewController.h"
#include "BMCViewController.h"

static ImageViewController* _controller = nil;

openTableImageEditor::openTableImageEditor() {
    
}
openTableImageEditor::~openTableImageEditor() {
    
    if ( _controller ) {
        [_controller.view removeFromSuperview];
        [_controller setEditor:nil];
        //[_controller release];
        //_controller = nil;
    }
    
}
//
//
//
bool openTableImageEditor::setup( ofxJSONElement& json ) {
    if ( openTableEditor::setup( json ) ) {
        m_annotation = json.isMember( "annotation" ) ? json[ "annotation" ].asString() : "";
        m_collection_images.clear();
        if ( json.isMember("images") ) {
            for ( int i = 0; i < json["images"].size(); i++ ) {
                string filename = json["images"][ i ].asString();
                string filepath = ofToDataPath("images/"+filename,true);
                m_collection_images.push_back(filepath);
            }
        }
        ofDirectory image_directory( ofToDataPath("images") );
        image_directory.allowExt("jpg");
        image_directory.allowExt("png");
        int count = image_directory.listDir();
        for ( int i = 0; i < count; i++ ) {
            string filename = image_directory.getName(i);
            string filepath = ofToDataPath("images/"+filename,true);
            if ( find(m_collection_images.begin(), m_collection_images.end(), filepath) == m_collection_images.end() ) {
                m_library_images.push_back(filepath);
            }
        }
        if ( _controller == nil ) {
            _controller	= [[ImageViewController alloc] initWithNibName:@"ImageViewController" bundle:nil];
        }
        [ofxiOSGetGLParentView() addSubview:_controller.view];
        [_controller.view setFrame:CGRectMake(0,0,ofGetWidth(),ofGetHeight())];
        [_controller setEditor: this];
        return true;
    }
    return false;
}
bool openTableImageEditor::save( ofxJSONElement& json ) {
    if ( openTableEditor::save(json) ) {
        json[ "annotation" ] = m_annotation;
        int count = m_collection_images.size();
        json[ "images" ].resize(count);
        for ( int i = 0; i < count; i++ ) {
            string filename = ofFilePath::getFileName(m_collection_images[i]);
            json[ "images" ][ i ] = filename;
        }
        return true;
    }
    return false;
}
//
//
//
int openTableImageEditor::imagesInCollection() {
    return m_collection_images.size();
}
string openTableImageEditor::pathForCollectionImage( int i ) {
    return m_collection_images[ i ];
}
int openTableImageEditor::imagesInLibrary() {
    return m_library_images.size();
}
string openTableImageEditor::pathForLibraryImage( int i ) {
    return m_library_images[ i ];
}
//
//
//
void openTableImageEditor::moveImageFromLibraryToCollection( int i ) {
    m_collection_images.push_back(m_library_images[i]);
    m_library_images.erase(m_library_images.begin()+i);
}
void openTableImageEditor::moveImageFromCollectionToLibrary( int i ) {
    m_library_images.push_back(m_collection_images[i]);
    m_collection_images.erase(m_collection_images.begin()+i);
}
