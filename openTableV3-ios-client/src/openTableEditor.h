//
//  openTableEditor.h
//  openTable
//
//  Created by Jonathan Jones-morris on 11/02/2015.
//
//
#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "MUI.h"
#include "ImageButton.h"

class openTableEditor {
public:
    openTableEditor();
    virtual ~openTableEditor();
    //
    //
    //
    virtual bool setup(ofxJSONElement& json);
    virtual bool save(ofxJSONElement& json);
    virtual void close();
    //
    //
    //
    string getType() { return m_type; };
protected:
    string              m_type;
    mui::Container*     m_ui_edit_window;
};
