//
//  openTableBusinessModelEditor.h
//  openTable
//
//  Created by Jonathan Jones-morris on 20/02/2015.
//
//
#pragma once

#include "openTableEditor.h"

class openTableBusinessModelEditor : public openTableEditor {
public:
    openTableBusinessModelEditor();
    virtual ~openTableBusinessModelEditor();
    //
    //
    //
    virtual bool setup( ofxJSONElement& json );
    virtual bool save( ofxJSONElement& json );
    //
    //
    //
    void setTitle( string title ) { m_title = title; };
    string getTitle() { return m_title; };
    void setContent( string content ) { m_content = content; };
    string getContent() { return m_content; };
    //
    //
    //
protected:
    int     m_element_type;
    string  m_title;
    string  m_content;
};