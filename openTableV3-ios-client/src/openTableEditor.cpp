//
//  openTableEditor.cpp
//  openTable
//
//  Created by Jonathan Jones-morris on 11/02/2015.
//
//

#include "openTableEditor.h"
#include "openTableUI.h"
//#include "ofApp.h"

openTableEditor::openTableEditor() {
    m_ui_edit_window = 0;
}

openTableEditor::~openTableEditor() {
    close();
}
//
//
//
bool openTableEditor::setup(ofxJSONElement& json) {
    mui::Root* ui_root = openTableUI::defaultUI.root();
    if ( ui_root ) {
        m_type = json[ "type" ].asString();
        
        m_ui_edit_window = new mui::Container( ofGetWidth(), 0, ofGetWidth(), ofGetHeight());
        m_ui_edit_window->opaque = true;
        m_ui_edit_window->bg.set(0);

        return true;
    }
    return false;
}
bool openTableEditor::save(ofxJSONElement& json) {
    json[ "type" ] = m_type;
    return true;
}
//
// utility to recursively remove children
//
void removeChildren( mui::Container* c ) {
    while (c->children.size()>0) {
        mui::Container* child = c->children[ 0 ];
        removeChildren(child);
        c->remove(child);
        if ( child->name != "scroll-view" ) { // JONS: let ScrollPane deal with this
            delete child;
        }
    }
}

void openTableEditor::close() {
    if ( m_ui_edit_window ) {
        mui::Root* ui_root = openTableUI::defaultUI.root();
        if ( ui_root ) {
            ui_root->remove(m_ui_edit_window);
            removeChildren(m_ui_edit_window);
            delete m_ui_edit_window;
            m_ui_edit_window = 0;
        }
    }
}

