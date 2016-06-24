//
//  openTableMenu.h
//  openTable
//
//  Created by Jonathan Jones-morris on 08/11/2014.
//
//

#ifndef __openTable__openTableMenu__
#define __openTable__openTableMenu__

#include "ofMain.h"
#include "MUI.h"
#include "ImageButton.h"

class openTableMenu : public tween::TweenerListener {
public:
    openTableMenu();
    virtual ~openTableMenu();
    //
    //
    //
    void setup( string name, mui::Root* root, mui::ImageButton* icon, vector<mui::ImageButton*>& items );
    //
    //
    //
    void open();
    void close();
    void toggle();
    //
    //
    //
    void onButtonPress( const void* sender, ofTouchEventArgs &args );
    //
    // tweener listener interface
    //
    virtual void onStart(tween::TweenerParam& param);
    virtual void onStep(tween::TweenerParam& param);
    virtual void onComplete(tween::TweenerParam& param);

protected:
    string                  m_name;
    mui::Root*              m_root;
    mui::ImageButton*             m_icon;
    vector<mui::ImageButton*>     m_items;
    bool                    m_open;
    
};

#endif /* defined(__openTable__openTableMenu__) */
