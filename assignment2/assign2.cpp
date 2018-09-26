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
struct pixel{
   int x,y;
   pixel(){}
   pixel(int myx, int myy){
	x=myx;
        y=myy;
  }

};

int CalculatePixel(Mat image,int x, int y){
   int returnVal,pixX,pixY;
   pixX=(sobelX[0][0])+(sobelX[0][1])+(sobelX[0][2])+(sobelX[1][0])+(sobelX[1][1])+(sobelX[1][2])+(sobelX[2][0])+(sobelX[2][1])+(sobelX[2][2]);
   pixY=()+()+()+()+()+()+()+()+();
   returnVal=sqrt((pixX*pixX)+(pixY*pixY));
   return returnVal;
}

Mat Sobel(Mat image){
   Mat returnImage=image.clone();
   for(int i=1;i<image.rows-2;++i){
	for(int j=1;j<image.cols-2;++j){
 

            returnimage.at<uchar>(i,j)=CalculatePixel(image,i,j); 




        }

   }
   

   
   return returnImage;
}









int main( int argc, char** argv ){

    Mat ant,basel;
    //char exit;
    ant = imread("ant_gray.bmp", IMREAD_GRAYSCALE);   
    basel = imread("basel_gray.bmp",IMREAD_GRAYSCALE);
    imshow("original ant",ant);
    imshow("original basel",basel);
    waitKey(0);                                          
    return 0;
}

