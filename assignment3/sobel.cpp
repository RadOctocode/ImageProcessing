#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <algorithm>    
#include <iostream>
#include <map>
#include <math.h>
#include <stack>
#include <cmath>
using namespace cv;
using namespace std;

Mat currentImage;
int sobelX[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
int sobelY[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
double UsharpMask[3][3]={{0.0625,0.125,0.0625},{0.125,0.25,0.125},{0.0625,0.125,0.0625}};
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
   returnVal=(mask[0][0]*image.at<Vec3b>(x-1,y-1)[0])+(mask[0][1]*image.at<Vec3b>(x,y-1)[0])+(mask[0][2]*image.at<Vec3b>(x+1,y-1)[0])+(mask[1][0]*image.at<Vec3b>(x-1,y)[0])+(mask[1][1]*image.at<Vec3b>(x,y)[0])+(mask[1][2]*image.at<Vec3b>(x+1,y)[0])+(mask[2][0]*image.at<Vec3b>(x-1,y+1)[0])+(mask[2][1]*image.at<Vec3b>(x,y+1)[0])+(mask[2][2]*image.at<Vec3b>(x+1,y+1)[0]);
   return returnVal;
}



Mat Sobel(Mat image){
   Mat returnImage=image.clone();
   for(int i=1;i<image.rows-2;++i){
	for(int j=1;j<image.cols-2;++j){
            int pixX=MaskPatch(image,i,j,sobelX); 
            int pixY=MaskPatch(image,i,j,sobelY); 
	    	returnImage.at<Vec3b>(i,j)[0]=sqrt((pixX*pixX)+(pixY*pixY));
		returnImage.at<Vec3b>(i,j)[1]=sqrt((pixX*pixX)+(pixY*pixY));
		returnImage.at<Vec3b>(i,j)[2]=sqrt((pixX*pixX)+(pixY*pixY));

	
	
	}
   }
   
   return returnImage;
}



