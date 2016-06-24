/*****************************************************************************
*   MarkerDetector.hpp
*   Example_MarkerBasedAR
******************************************************************************
*   by Khvedchenia Ievgen, 5th Dec 2012
*   http://computer-vision-talks.com
******************************************************************************
*   Ch2 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

#ifndef Example_MarkerBasedAR_MarkerDetector_hpp
#define Example_MarkerBasedAR_MarkerDetector_hpp

#undef MIN
#undef MAX

////////////////////////////////////////////////////////////////////
// Standard includes:
#include <vector>
#include <opencv2/opencv.hpp>

////////////////////////////////////////////////////////////////////
// File includes:
#include "BGRAVideoFrame.h"
#include "CameraCalibration.hpp"
#include "ARMarker.hpp"

#include "ofMain.h"

////////////////////////////////////////////////////////////////////
// Forward declaration:
//class ARMarker;

/**
 * A top-level class that encapsulate marker detector algorithm
 */
class MarkerDetector {
public:
    typedef std::vector<cv::Point>    PointsVector;
    typedef std::vector<PointsVector> ContoursVector;


    /**
    * Initialize a new instance of marker detector object
    * @calibration[in] - Camera calibration (intrinsic and distortion components) necessary for pose estimation.
    */
    MarkerDetector(CameraCalibration calibration);

    //! Searches for markes and fills the list of transformation for found markers
    void processFrame(const BGRAVideoFrame& frame);

    const std::vector<Transformation>& getTransformations() const;

    void draw();
    int markerAt( float x, float y );
    
protected:

    //! Main marker detection routine
    bool findMarkers(const BGRAVideoFrame& frame, std::vector<ARMarker>& detectedMarkers);

    //! Converts image to grayscale
    void prepareImage(const cv::Mat& bgraMat, cv::Mat& grayscale) const;

    //! Performs binary threshold
    void performThreshold(const cv::Mat& grayscale, cv::Mat& thresholdImg) const;

    //! Detects appropriate contours
    void findContours(cv::Mat& thresholdImg, ContoursVector& contours, int minContourPointsAllowed) const;

    //! Finds marker candidates among all contours
    void findCandidates(const ContoursVector& contours, std::vector<ARMarker>& detectedMarkers);

    //! Tries to recognize markers by detecting marker code 
    void recognizeMarkers(const cv::Mat& grayscale, std::vector<ARMarker>& detectedMarkers);

    //! Calculates marker poses in 3D
    void estimatePosition(std::vector<ARMarker>& detectedMarkers);

    //
    //
    //
    void lock()  { m_lock.lock(); } ;
    void unlock()  { m_lock.unlock(); } ;
private:
    float m_minContourLengthAllowed;
    ofMutex   m_lock;
    cv::Size markerSize;
    cv::Mat camMatrix;
    cv::Mat distCoeff;
    std::vector<Transformation>   m_transformations;
    std::vector<ARMarker>         m_markers;
    cv::Mat m_grayscaleImage;
    cv::Mat m_thresholdImg;  
    cv::Mat canonicalMarkerImage;

    ContoursVector           m_contours;
    std::vector<cv::Point3f> m_markerCorners3d;
    std::vector<cv::Point2f> m_markerCorners2d;
};

#endif
