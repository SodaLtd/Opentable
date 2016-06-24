//
//  openTableImageCollection.h
//  openTableServer
//
//  Created by Jonathan Jones-morris on 09/02/2015.
//
//

#pragma once

#include "openTableImageGroup.h"

class openTableImageCollection : public openTableDocument {
public:
    openTableImageCollection();
    openTableImageCollection(Json::Value& json);
    virtual ~openTableImageCollection();
    //
    //
    //
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
    void addGroup( openTableImageGroup* group );
    void removeGroup( string id );
protected:
    void clear();
    vector< openTableImageGroup* > m_groups;
    ofMutex                 m_group_lock;
    void lock() { m_group_lock.lock(); };
    void unlock() { m_group_lock.unlock(); };
    //
    //
    //
	/*
    openTableImageGroup*    m_selected_group;
    int                     m_selected_action;
    ofVec2f                 m_last_point;
	*/
};