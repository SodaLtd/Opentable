/*****************************************************************************
*   ARMarker.hpp
*   Example_ARMarkerBasedAR
******************************************************************************
*   by Khvedchenia Ievgen, 5th Dec 2012
*   http://computer-vision-talks.com
******************************************************************************
*   Ch2 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

#ifndef Example_ARMarkerBasedAR_ARMarker_hpp
#define Example_ARMarkerBasedAR_ARMarker_hpp

#undef MIN
#undef MAX

////////////////////////////////////////////////////////////////////
// Standard includes:
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

////////////////////////////////////////////////////////////////////
// File includes:
#include "GeometryTypes.hpp"

/**
 * This class represents a ARMarker
 */
class ARMarker
{  
public:
    ARMarker();

    friend bool operator<(const ARMarker &M1,const ARMarker&M2);
    friend std::ostream & operator<<(std::ostream &str,const ARMarker &M);

    static cv::Mat rotate(cv::Mat  in);
    static int hammDistMarker(cv::Mat bits);
    static int mat2id(const cv::Mat &bits);
    static int getMarkerId(cv::Mat &in,int &nRotations);
    //
    //
    //
    static int getValidWordCount();
    static int* getValidWord( int i );

    static int generateMarkerImage(cv::Mat& bits);

    // Id of  the ARMarker
    int id;

    // ARMarker transformation with regards to the camera
    Transformation transformation;

    std::vector<cv::Point2f> points;

    // Helper function to draw the ARMarker contour over the image
    void drawContour(cv::Mat& image, cv::Scalar color = CV_RGB(0,250,0)) const;
    
    void draw() const;
    bool inside( float x, float y ) const;
    //
    //
    //
    static int s_valid_words[ 4 ][ 5 ];
};

#endif
