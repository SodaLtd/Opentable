//
//  openTableBusinessModelEditor.cpp
//  openTable
//
//  Created by Jonathan Jones-morris on 20/02/2015.
//
//

#include "openTableBusinessModelEditor.h"
//#include "openTableUI.h"

#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "BMCViewController.h"

static BMCViewController* _controller = nil;

openTableBusinessModelEditor::openTableBusinessModelEditor() {
    
}
openTableBusinessModelEditor::~openTableBusinessModelEditor() {
    
    if ( _controller ) {
        [_controller.view removeFromSuperview];
        [_controller setEditor: nil];
        //[_controller release];
        //_controller = nil;
    }
    
}
//
//
//
bool openTableBusinessModelEditor::setup( ofxJSONElement& json ) {
    if ( openTableEditor::setup( json ) ) {
        m_element_type = json.isMember( "element_type" ) ? json[ "element_type" ].asInt() : -1;
        m_title = json.isMember( "title" ) ? json[ "title" ].asString() : "";
        m_content = json.isMember( "content" ) ? json[ "content" ].asString() : "";
        if ( _controller == nil ) {
            _controller	= [[BMCViewController alloc] initWithNibName:@"BMCViewController" bundle:nil];
        }
        [ofxiOSGetGLParentView() addSubview:_controller.view];
        [_controller.view setFrame:CGRectMake(0,0,ofGetWidth(),ofGetHeight())];
        [_controller setEditor:(void*)this];
        return true;
    }
    return false;
}
bool openTableBusinessModelEditor::save( ofxJSONElement& json ) {
    if ( openTableEditor::save(json) ) {
        json[ "element_type" ] = m_element_type;
        json[ "title" ] = m_title;
        json[ "content" ] = m_content;
        return true;
    }
    return false;
    
}
