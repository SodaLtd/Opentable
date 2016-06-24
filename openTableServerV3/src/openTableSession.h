//
//  openTableSession.h
//  openTableServer
//
//  Created by Jonathan Jones-morris on 03/05/2016.
//
//

#pragma once

#include "openTableDocument.h"

class openTableSession : public openTableDocument {
public:
    openTableSession();
    openTableSession(Json::Value& json);
    virtual ~openTableSession();
    //
    //
    //
    virtual bool setup() override;
    virtual bool setup( Json::Value& json ) override;
    virtual bool save( Json::Value& json ) override;
    virtual void update() override;
    virtual void draw() override;
    //
    //
    //
    virtual void touchMove(int x, int y, int id) override;
    virtual void touchDown(int x, int y, int id) override;
    virtual void touchUp(int x, int y, int id) override;
    //
    //
    //
    void setCurrentDocument( int index );
    openTableDocument* getCurrentDocument() { return m_current_document; };
    openTableDocument* getDocument( int i ) { return m_documents[ i ]; };
    //
    //
    //
    void setSessionTitle( const string& title ) { m_session_title = title; };
    string getSessionTitle() { return m_session_title; };
    //
    //
    //
    void unlock();
protected:
    //
    //
    //
    string                          m_session_title;
    //
    //
    //
    openTableDocument*              m_current_document;
    vector< openTableDocument* >    m_documents;
    //
    //
    //
    ofRectangle                     m_title_rect[2];
    int                             m_selected_id;
    int                             m_selected_title;
    ofPoint                         m_selected_offset;
    bool                            m_locked;
    int                             m_lock_session;
    ofColor                         m_lock_colour;
};

