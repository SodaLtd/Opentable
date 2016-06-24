//
//  openTableMarkerFinder.h
//  openTable
//
//  Created by Jonathan Jones-morris on 07/02/2015.
//
//
#pragma once

#include "ofMain.h"

#import "VideoSource.h"
#include "MarkerDetector.hpp"

class openTableMarkerTexture : public ofTexture {
public:
    void loadBRGA( unsigned char* data, int w, int h ) {
        if(w > texData.tex_w || h > texData.tex_h) {
            allocate(w, h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
            enableTextureTarget();
            
            glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
            glTexImage2D(texData.textureTarget, 0, GL_RGBA, (GLint)texData.tex_w, (GLint)texData.tex_h, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);  // init to black...
            disableTextureTarget();
        }
        enableTextureTarget();
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glBindTexture(GL_TEXTURE_2D, texData.textureID);
        glTexSubImage2D(texData.textureTarget, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, data);
        disableTextureTarget();
    }
};

class openTableMarkerFinder : public VideoSourceListener {
public:
    openTableMarkerFinder();
    ~openTableMarkerFinder();
    //
    //
    //
    void setup();
    void update();
    void draw();
    //
    //
    //
    void start();
    void stop();
    //
    //
    //
    void frameReady(BGRAVideoFrame* frame);
    //
    //
    //
    int markerAt( float x, float y );
    //
    //
    //
    ofTexture& getCurrentFrameReference();
protected:
    VideoSource*    m_video_source;
    MarkerDetector* m_marker_detector;
    //
    //
    //
    bool                        m_frame_new;
    openTableMarkerTexture      m_current_frame;
    BGRAVideoFrame              m_current_frame_data;
};