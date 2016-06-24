//
//  openTableBusinessModelCanvas.h
//  openTableServer
//
//  Created by Jonathan Jones-morris on 19/02/2015.
//
//
#pragma once

#include "openTableDocument.h"

class openTableBusinessModelCanvasElement : public openTableSelectableItem {
public:
    enum {
        openTableBusinessModelCanvasElementParteners,
        openTableBusinessModelCanvasElementActivities,
        openTableBusinessModelCanvasElementResources,
        openTableBusinessModelCanvasElementProposition,
        openTableBusinessModelCanvasElementCustomerRelationships,
        openTableBusinessModelCanvasElementDistributionChannels,
        openTableBusinessModelCanvasElementCustomerSegment,
        openTableBusinessModelCanvasElementCostStructure,
        openTableBusinessModelCanvasElementRevenueStream,
        openTableBusinessModelCanvasElementCount
    };
    enum {
        openTableBusinessModelCanvasElementFlip = 1,
        openTableBusinessModelCanvasElementMove
    };
    //
    //
    //
    openTableBusinessModelCanvasElement();
    virtual ~openTableBusinessModelCanvasElement();
    //
    //
    //
    virtual bool setup( int type );
    virtual bool setup( Json::Value& json );
    virtual bool save( Json::Value& json );
    virtual void update();
    virtual void draw();
	virtual void start_clip();
	virtual void end_clip();
    //
    // layout
    //
    virtual void setBounds( float x, float y, float width, float height ) {
        m_bounds.x = x;
        m_bounds.y = y;
        m_bounds.width = width;
        m_bounds.height = height;
    }
    virtual ofRectangle& getBounds() { return m_bounds; }
    virtual void flip() { m_orientation = !m_orientation; }
    //
    // interaction
    //
    virtual void lock( int lock_session, ofColor& lock_colour ) {
        m_locked = true;
        m_lock_session = lock_session;
        m_lock_colour = lock_colour;
    }
    virtual void unlock() {
        m_locked = false;
        m_lock_session = -1;
    }
    virtual void moveTo( float x, float y ) { m_offset.set( x, y ); };
    virtual void reset() { m_offset.x = m_offset.y = 0; }
    virtual int hitPart( float x, float y );
protected:
    //
    // data
    //
    int     m_type;
    string  m_title;
    string  m_content;
    //
    // display
    //
    ofImage*        m_flip;
    bool            m_locked;
    int             m_lock_session;
    ofColor         m_lock_colour;
    int             m_orientation;
    ofRectangle     m_bounds;
    ofVec2f         m_offset;
    //
    //
    //
};

class openTableBusinessModelCanvas : public openTableDocument {
public:
    openTableBusinessModelCanvas();
    openTableBusinessModelCanvas(Json::Value& json);
    virtual ~openTableBusinessModelCanvas();
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
    void updateElement( Json::Value& json );
protected:
    //
    //
    //
    map< int, openTableBusinessModelCanvasElement >     m_elements;
	/*
    int                                                 m_selected_element;
    int                                                 m_selected_action;
	*/
    //
    //
    //
    float m_width;
    float m_height;
    void layout();
    //
    // TODO: move locking to super class
    //
    bool                                                m_locked;
    int                                                 m_lock_session;
};
