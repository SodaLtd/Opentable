/*****************************************************************************
*   VideoSource.h
*   Example_MarkerBasedAR
******************************************************************************
*   by Khvedchenia Ievgen, 5th Dec 2012
*   http://computer-vision-talks.com
******************************************************************************
*   Ch2 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import <AVFoundation/AVFoundation.h>

////////////////////////////////////////////////////////////////////
// File includes:
#include "BGRAVideoFrame.h"
#include "CameraCalibration.hpp"

class VideoSourceListener {
public:
    virtual void frameReady(BGRAVideoFrame* frame) = 0;
};

@protocol VideoSourceDelegate<NSObject>

-(void)frameReady:(BGRAVideoFrame) frame;

@end

@interface VideoSource : NSObject<AVCaptureVideoDataOutputSampleBufferDelegate>
{
  
}

@property (nonatomic, retain) AVCaptureSession        * captureSession;
@property (nonatomic, retain) AVCaptureDeviceInput    * deviceInput;
@property (nonatomic, retain) id<VideoSourceDelegate>   delegate;
@property (nonatomic) VideoSourceListener*              listener;

- (bool) startWithDevicePosition:(AVCaptureDevicePosition)devicePosition;
- (CameraCalibration) getCalibration;
- (CGSize) getFrameSize;

@end

