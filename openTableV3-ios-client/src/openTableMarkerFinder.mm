//
//  openTableMarkerFinder.cpp
//  openTable
//
//  Created by Jonathan Jones-morris on 07/02/2015.
//
//

#include "openTableMarkerFinder.h"

openTableMarkerFinder::openTableMarkerFinder() {
    m_video_source = nil;
    m_marker_detector = NULL;
    m_current_frame_data.data = NULL;
}
openTableMarkerFinder::~openTableMarkerFinder() {
    if ( m_video_source ) {
        [m_video_source release];
    }
    if ( m_marker_detector ) {
        delete m_marker_detector;
    }
}
//
//
//
void openTableMarkerFinder::setup() {
}
void openTableMarkerFinder::update() {
    if ( m_frame_new ) {
        m_current_frame.loadBRGA( m_current_frame_data.data, m_current_frame_data.width, m_current_frame_data.height );
        m_frame_new =  false;
    }
}
void openTableMarkerFinder::draw() {
    if ( m_current_frame.isAllocated() ) {
        ofPushStyle();
        m_current_frame.draw(0,0);
        
        ofSetColor( ofColor::red );
        m_marker_detector->draw();
        ofPopStyle();
    }
}
//
//
//
void openTableMarkerFinder::start() {
    m_video_source = [[VideoSource alloc] init];
    if ( m_video_source ) {
        m_marker_detector = new MarkerDetector( [m_video_source getCalibration] );
        m_video_source.listener = this;
        [m_video_source startWithDevicePosition: AVCaptureDevicePositionBack];
    }
    
}
void openTableMarkerFinder::stop() {
    if ( m_video_source ) {
        m_video_source.listener = nil;
        [m_video_source release];
        m_video_source = nil;
    }
    if ( m_marker_detector ) {
        delete m_marker_detector;
        m_marker_detector = NULL;
    }
}
//
//
//
int openTableMarkerFinder::markerAt( float x, float y ) {
    if ( m_marker_detector ) {
        return m_marker_detector->markerAt( x, y );
    }
    return -1;
}
//
//
//
ofTexture& openTableMarkerFinder::getCurrentFrameReference() {
    return m_current_frame;
}
//
//
//
void openTableMarkerFinder::frameReady(BGRAVideoFrame* frame) {
    if ( m_current_frame_data.width != frame->width || m_current_frame_data.height != frame->height ) {
        m_current_frame_data.width = frame->width;
        m_current_frame_data.height = frame->height;
        m_current_frame_data.stride = frame->stride;
        m_current_frame_data.data = ( unsigned char* ) realloc( m_current_frame_data.data, frame->stride * frame->height );
    }
    memcpy( m_current_frame_data.data, frame->data, frame->stride * frame->height );
    m_frame_new = true;
    //
    //
    //
    m_marker_detector->processFrame(m_current_frame_data);
    
}
