#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <algorithm>    
#include <iostream>
#include <map>
#include <math.h>
#include <stack>

using namespace cv;
using namespace std;

Mat currentImage;
int sobelX[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
int sobelY[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
double UsharpMask[3][3]={{0,0.11,0},{0.11,0.11,0.11},{0,0.11,0}};
struct pixel{
   int x,y;
   pixel(){}
   pixel(int myx, int myy){
	x=myx;
        y=myy;
  }

};


int MaskPatch(const Mat& image,int x, int y, int mask[3][3]){
   int returnVal;
   returnVal=(mask[0][0]*image.at<uchar>(x-1,y-1))+(mask[0][1]*image.at<uchar>(x,y-1))+(mask[0][2]*image.at<uchar>(x+1,y-1))+(mask[1][0]*image.at<uchar>(x-1,y))+(mask[1][1]*image.at<uchar>(x,y))+(mask[1][2]*image.at<uchar>(x+1,y))+(mask[2][0]*image.at<uchar>(x-1,y+1))+(mask[2][1]*image.at<uchar>(x,y+1))+(mask[2][2]*image.at<uchar>(x+1,y+1));
   return returnVal;
}

int MaskPatchD(const Mat& image,int x, int y, double mask[3][3]){
   int returnVal;
   returnVal=(mask[0][0]*image.at<uchar>(x-1,y-1))+(mask[0][1]*image.at<uchar>(x,y-1))+(mask[0][2]*image.at<uchar>(x+1,y-1))+(mask[1][0]*image.at<uchar>(x-1,y))+(mask[1][1]*image.at<uchar>(x,y))+(mask[1][2]*image.at<uchar>(x+1,y))+(mask[2][0]*image.at<uchar>(x-1,y+1))+(mask[2][1]*image.at<uchar>(x,y+1))+(mask[2][2]*image.at<uchar>(x+1,y+1))/4;
   return returnVal;
}

Mat Sobel(Mat image){
   Mat returnImage=image.clone();
   for(int i=1;i<image.rows-2;++i){
	for(int j=1;j<image.cols-2;++j){
            int pixX=MaskPatch(image,i,j,sobelX); 
            int pixY=MaskPatch(image,i,j,sobelY); 
	    returnImage.at<uchar>(i,j)=sqrt((pixX*pixX)+(pixY*pixY));
        }
   }
   
   return returnImage;
}


Mat Usharp(const Mat& image){
   Mat returnImage=image.clone();
   for(int i=1;i<image.rows-2;++i){
	for(int j=1;j<image.cols-2;++j){
            int newval= image.at<uchar>(i,j)-MaskPatchD(image,i,j,UsharpMask);
            newval=((int)image.at<uchar>(i,j))+newval;
	    returnImage.at<uchar>(i,j)=MaskPatchD(image,i,j,UsharpMask); 
        }
   }
 
   return returnImage;
}








int main( int argc, char** argv ){

    Mat ant,basel;
    //char exit;
    ant = imread("ant_gray.bmp", IMREAD_GRAYSCALE);   
    basel = imread("basel_gray.bmp",IMREAD_GRAYSCALE);
    imshow("original basel",basel);
    //imshow("original basel",Usharp(basel));
    imshow("Usharp basel",Usharp(basel));
    //imshow("original basel",Usharp(basel));
    waitKey(0);                                          
    return 0;
}

