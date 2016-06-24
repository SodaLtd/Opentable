/*****************************************************************************
*   ARMarker.cpp
*   Example_ARMarkerBasedAR
******************************************************************************
*   by Khvedchenia Ievgen, 5th Dec 2012
*   http://computer-vision-talks.com
******************************************************************************
*   Ch2 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

#include "ARMarker.hpp"
//#include "DebugHelpers.hpp"
#include "ofMain.h"
#include "openTableClientSessionServer.h"

const int kNWords       = 4;
const int kWordLength   = 5;

// JONS: list of valid words ( any combination of these should work )
int ARMarker::s_valid_words[ kNWords ][ kWordLength ] =
{
    {1,0,0,0,0},
    {1,0,1,1,1},
    {0,1,0,0,1},
    {0,1,1,1,0}
};

ARMarker::ARMarker()
: id(-1)
{
}

bool operator<(const ARMarker &M1,const ARMarker&M2)
{
  return M1.id<M2.id;
}

cv::Mat ARMarker::rotate(cv::Mat in)
{
  cv::Mat out;
  in.copyTo(out);
  for (int i=0;i<in.rows;i++)
  {
    for (int j=0;j<in.cols;j++)
    {
      out.at<uchar>(i,j)=in.at<uchar>(in.cols-j-1,i);
    }
  }
  return out;
}


int ARMarker::hammDistMarker(cv::Mat bits)
{
    
    
  
  int dist=0;
  
  for (int y=0;y<5;y++)
  {
    int minSum=1e5; //hamming distance to each possible word
    
    for (int p=0;p<4;p++)
    {
      int sum=0;
      //now, count
      for (int x=0;x<5;x++)
      {
        sum += bits.at<uchar>(y,x) == s_valid_words[p][x] ? 0 : 1;
      }
      
      if (minSum>sum)
        minSum=sum;
    }
    
    //do the and
    dist += minSum;
  }
  
  return dist;
}

int ARMarker::mat2id(const cv::Mat &bits)
{
  int val=0;
  for (int y=0;y<5;y++)
  {
    val<<=1;
    if ( bits.at<uchar>(y,1)) val|=1;
    val<<=1;
    if ( bits.at<uchar>(y,3)) val|=1;
  }
  return val;
}

int ARMarker::getMarkerId(cv::Mat &ARMarkerImage,int &nRotations)
{
  assert(ARMarkerImage.rows == ARMarkerImage.cols);
  assert(ARMarkerImage.type() == CV_8UC1);
  
  cv::Mat grey = ARMarkerImage;

  // Threshold image
  cv::threshold(grey, grey, 125, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  //ARMarkers  are divided in 7x7 regions, of which the inner 5x5 belongs to ARMarker info
  //the external border should be entirely black
  
  int cellSize = ARMarkerImage.rows / 7;
  
  for (int y=0;y<7;y++)
  {
    int inc=6;
    
    if (y==0 || y==6) inc=1; //for first and last row, check the whole border
    
    for (int x=0;x<7;x+=inc)
    {
      int cellX = x * cellSize;
      int cellY = y * cellSize;
      cv::Mat cell = grey(cv::Rect(cellX,cellY,cellSize,cellSize));
      
      int nZ = cv::countNonZero(cell);

      if (nZ > (cellSize*cellSize) / 2)
      {
        return -1;//can not be a ARMarker because the border element is not black!
      }
    }
  }
  
  cv::Mat bitMatrix = cv::Mat::zeros(5,5,CV_8UC1);
  
  //get information(for each inner square, determine if it is  black or white)  
  for (int y=0;y<5;y++)
  {
    for (int x=0;x<5;x++)
    {
      int cellX = (x+1)*cellSize;
      int cellY = (y+1)*cellSize;
      cv::Mat cell = grey(cv::Rect(cellX,cellY,cellSize,cellSize));
      
      int nZ = cv::countNonZero(cell);
      if (nZ> (cellSize*cellSize) /2) 
        bitMatrix.at<uchar>(y,x) = 1;
    }
  }
  
  //check all possible rotations
  cv::Mat rotations[4];
  int distances[4];
  
  rotations[0] = bitMatrix;  
  distances[0] = hammDistMarker(rotations[0]);
  
  std::pair<int,int> minDist(distances[0],0);
  
  for (int i=1; i<4; i++)
  {
    //get the hamming distance to the nearest possible word
    rotations[i] = rotate(rotations[i-1]);
    distances[i] = hammDistMarker(rotations[i]);
    
    if (distances[i] < minDist.first)
    {
      minDist.first  = distances[i];
      minDist.second = i;
    }
  }
  
  nRotations = minDist.second;
  if (minDist.first == 0)
  {
      return mat2id(rotations[minDist.second]);
  }
  
  return -1;
}

int ARMarker::getValidWordCount() {
    return kNWords;
}

int* ARMarker::getValidWord( int i ) {
    if ( i >= 0 && i < kNWords ) {
        return s_valid_words[ i ];
    }
    return NULL;
}

int ARMarker::generateMarkerImage(cv::Mat& ARMarkerImage) {
    const int kCellDim = 48;
    const int kImageDim = kCellDim * 7;
    
    //
    // create image
    //
    ARMarkerImage.create(kImageDim, kImageDim, CV_8UC1);
    //
    // draw background
    //
    cv::Scalar black(0);
    cv::Scalar white(255);
    cv::Rect r( 0, 0, kImageDim, kImageDim );

    cv::rectangle(ARMarkerImage, r, black, CV_FILLED);
    //
    // draw random words
    //
    cv::Rect cell( kCellDim, kCellDim, kCellDim, kCellDim );
    for ( int i = 0; i < 5; i++ ) {
        int selector = rand() % kNWords;
        int* word = getValidWord(selector);
        cell.x = kCellDim;
        for ( int j = 0; j < 5; j++ ) {
            cv::rectangle(ARMarkerImage, cell, word[ j ] ? white : black, CV_FILLED );
            cell.x += kCellDim;
        }
        cell.y += kCellDim;
    }
    int nRotations = 0;
    return getMarkerId(ARMarkerImage, nRotations);
}

void ARMarker::drawContour(cv::Mat& image, cv::Scalar color) const
{
    float thickness = 2;
    
    cv::line(image, points[0], points[1], color, thickness, CV_AA);
    cv::line(image, points[1], points[2], color, thickness, CV_AA);
    cv::line(image, points[2], points[3], color, thickness, CV_AA);
    cv::line(image, points[3], points[0], color, thickness, CV_AA);
}

void ARMarker::draw() const {
    ofPushStyle();
    ofSetLineWidth(4);
    int r = ( id >> 4 ) & 0xF;
    int g = ( id >> 8 ) & 0xF;
    int b = ( id >> 12 ) & 0xF;
    ofSetColor(r << 4, g << 4, b << 4,127);
    
    ofPolyline poly;
    for ( int i = 0; i < 4; i++ ) {
        poly.addVertex(points[ i ].x, points[ i ].y);
    }
    poly.close();
    poly.draw();
    ofPopStyle();
}

bool ARMarker::inside( float  x, float y ) const {
    ofPolyline poly;
    for ( int i = 0; i < 4; i++ ) {
        poly.addVertex(points[ i ].x, points[ i ].y);
    }
    poly.close();
    return poly.inside( x, y );
}