//
//  openTableUI.h
//  openTable
//
//  Created by Jonathan Jones-morris on 09/11/2014.
//
//

#ifndef __openTable__openTableUI__
#define __openTable__openTableUI__

#include "ofMain.h"
#include "openTableMenu.h"

class openTableUI {
public:
    openTableUI();
    virtual ~openTableUI();
    //
    //
    //
    void setup();
    bool isSetup() { return m_setup; };
    //
    //
    //
    void update();
    void draw();
    //
    //
    //
    void openSettings(const void* sender, ofTouchEventArgs &args );
    void newPhotoCollection(const void* sender, ofTouchEventArgs &args );
    void newBusinessModel(const void* sender, ofTouchEventArgs &args );
    //
    //
    //
    void onNewCollection( const void* sender, ofTouchEventArgs &args );
    //
    //
    //
    //
    //
    //
    mui::Root* root() { return m_ui_root; };
    //
    //
    //
    virtual mui::Container * handleTouchDown( ofTouchEventArgs &touch ) { return m_ui_root->handleTouchDown( touch ); };
    virtual mui::Container * handleTouchMoved( ofTouchEventArgs &touch ) { return m_ui_root->handleTouchMoved( touch ); };
    virtual mui::Container * handleTouchUp( ofTouchEventArgs &touch ) { return m_ui_root->handleTouchUp( touch ); };
    virtual mui::Container * handleTouchDoubleTap( ofTouchEventArgs &touch ) { return m_ui_root->handleTouchDoubleTap( touch ); };
    virtual mui::Container * handleTouchCancelled( ofTouchEventArgs &touch ) { return m_ui_root->handleTouchCancelled( touch ); };
    //
    //
    //
    static openTableUI defaultUI;
protected:
    //
    // UI
    //
    mui::Root*                  m_ui_root;
    openTableMenu               m_ui_main_menu;
    bool m_setup;
    //
    //
    //
};

#endif /* defined(__openTable__openTableUI__) */
