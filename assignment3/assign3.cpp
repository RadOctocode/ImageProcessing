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

float intensityValue(int red, int green, int blue){
	return (red+green+blue)/3;
}

float saturationValue(int red, int green, int blue){
	int minVal=min(red,blue);
	minVal=min(minVal,green);
	float s=1-3*(minVal/(red+green+blue+0.00001));
	if(s<0.00001){
		s=0;
	}
	else if(s>0.99999){
		s=1;
	}

	return s;

}

int hueValue(int red, int green, int blue){
	float numer= 0.5 *((red-green)+(red-blue));
	float denomer= sqrt(((red-green)*(red-green))+((red-blue)*(green-blue)));
	float hue=acos(numer/denomer);
	if(blue>green){
		hue=((360*3.14159)/180)-hue;
	
	}
	
	return hue;

}

Mat rbgToHsi(const Mat& image){
    Mat returnImage=image.clone();
    for(int i =0;i<image.rows;++i){
        for(int j=0;j<image.cols;++j){
		Vec3b currentPixel=image.at<Vec3b>(i,j);
		int b,g,r;
		b=currentPixel[0];	
		g=currentPixel[1];
		r=currentPixel[2];
		int sat,hue,inten;
	      	sat=saturationValue(r,g,b)*100;
		inten=intensityValue(r,g,b);
		if(sat!=0){
			hue=hueValue(r,g,b)*180/3.14159;
		}
		returnImage.at<Vec3b>(i,j)[0]=hue;
		returnImage.at<Vec3b>(i,j)[1]=sat;
		returnImage.at<Vec3b>(i,j)[2]=inten;
	}
    }

    return returnImage;
}

int main( int argc, char** argv ){

    Mat basel;
    //char exit;
    //ant = imread("ant_gray.bmp", IMREAD_COLOR);   
    basel = imread("test.jpg",IMREAD_COLOR);
   imshow("basel",basel);//blur take difference then add on top of the original picture
   //imshow("ant",ant);//blur take difference then add on top of the original picture
  imshow("basel 2",rbgToHsi(basel)); 
   waitKey(0);                                          
    return 0;
}

