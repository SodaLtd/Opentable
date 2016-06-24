//
//  openTableMenu.cpp
//  openTable
//
//  Created by Jonathan Jones-morris on 08/11/2014.
//
//

#include "openTableMenu.h"

openTableMenu::openTableMenu() {
    
}
openTableMenu::~openTableMenu() {
    
}
//
//
//
void openTableMenu::setup( string name, mui::Root* root, mui::ImageButton* icon, vector<mui::ImageButton*>& items ) {
    m_name  = name;
    m_icon  = icon;
    m_root  = root;
    m_items = items;
    m_open  = false;
    //
    //
    //
    for ( int i = 0; i < m_items.size(); i++ ) {
        m_items[ i ]->x         = m_icon->x;
        m_items[ i ]->y         = m_icon->y;
        m_items[ i ]->visible   = false;
    }
    m_root->addAnimationListener(this);
    //
    //
    //
    m_icon->onPress += Poco::Delegate<openTableMenu, ofTouchEventArgs>( this, &openTableMenu::onButtonPress );
}
//
//
//
void openTableMenu::open() {
    m_open = true;
    m_root->prepareAnimation(100);
    float x = m_icon->x + m_icon->width;
    for ( int i = 0; i < m_items.size(); i++ ) {
        m_root->animate( m_items[ i ]->x, x );
        m_items[ i ]->visible   = true;
        x += m_items[ i ]->width;
    }
    m_root->commitAnimation();
}
void openTableMenu::close() {
    m_open = false;
    m_root->prepareAnimation(100);
    for ( int i = 0; i < m_items.size(); i++ ) {
        m_root->animate( m_items[ i ]->x, m_icon->x );
    }
    m_root->commitAnimation();
}
void openTableMenu::toggle() {
    if ( m_open ) {
        close();
    } else {
        open();
    }
}
//
//
//
void openTableMenu::onButtonPress( const void* sender, ofTouchEventArgs &args ) {
    toggle();
}
//
// tweener listener interface
//
void openTableMenu::onStart(tween::TweenerParam& param) {
    if ( m_open ) {
        for ( int i = 0; i < m_items.size(); i++ ) {
            m_items[ i ]->visible   = true;
        }
    }
}
void openTableMenu::onStep(tween::TweenerParam& param) {
    
}
void openTableMenu::onComplete(tween::TweenerParam& param) {
    if ( !m_open ) {
        for ( int i = 0; i < m_items.size(); i++ ) {
            m_items[ i ]->visible   = false;
        }
    }
}
