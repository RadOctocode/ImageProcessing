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

int mask[3][3]={{0,1,0},{1,1,1},{0,1,0}};



Mat thres(const Mat& image){

   Mat returnimage=image.clone();
   int threshold=0;
   


   for(int i=0; i<image.rows; i++){
  	for(int j=0; j<image.cols; j++){
	   threshold +=(int)image.at<uchar>(i,j);
	}
   }
 
   threshold=threshold/(image.rows*image.cols);

   for(int i=0; i<returnimage.rows; i++){
  	for(int j=0; j<returnimage.cols; j++){
	   int currentPix=(int)returnimage.at<uchar>(i,j);
	   if(currentPix<=threshold){
		returnimage.at<uchar>(i,j)=0;		

	   }//bg
	   else{									   			
		   returnimage.at<uchar>(i,j)=255;		
	   }//foreg

	}
   } 

   return returnimage;
}

bool HitMiss(const Mat& image, int x, int y){
	bool boolAnd=true;
	bool boolOr=false;
	/*if(((x-1)>0)&&((y-1)>0)){
	    boolAnd=boolAnd&&(mask[0][0]*image.at<uchar>(x-1,y-1));
	    boolOr=boolOr||(mask[0][0]*image.at<uchar>(x-1,y-1));

	}*/
	if((y-1)>0){
	    boolAnd=boolAnd&&(mask[0][1]*image.at<uchar>(x,y-1));
	    boolOr=boolOr||(mask[0][1]*image.at<uchar>(x,y-1));

	}
	/*if(((x+1)<image.cols)&&((y-1)>0)){	
	    boolAnd=boolAnd&&(mask[0][2]*image.at<uchar>(x+1,y-1));
	    boolOr=boolOr||(mask[0][2]*image.at<uchar>(x+1,y-1));
	
	}*/
	if((x-1)>0){			
	   boolAnd=boolAnd&&(mask[1][0]*image.at<uchar>(x-1,y));
	   boolOr=boolOr||(mask[1][0]*image.at<uchar>(x-1,y));
	}
	boolAnd=boolAnd&&(mask[1][1]*image.at<uchar>(x,y));
	boolOr=boolOr||(mask[1][1]*image.at<uchar>(x,y));			
	
	if((x+1)>image.cols){			
	    boolAnd=boolAnd&&(mask[1][2]*image.at<uchar>(x+1,y));
	    boolOr=boolOr||(mask[1][2]*image.at<uchar>(x+1,y));
	}
	/*if(((x-1)>0)&&((y+1)<image.rows)){			
	    boolAnd=boolAnd&&(mask[2][0]*image.at<uchar>(x-1,y+1));
	    boolOr=boolOr||(mask[2][0]*image.at<uchar>(x-1,y+1));
	}*/
	if((y+1)<image.rows){			
	   boolAnd=boolAnd&&(mask[1][2]*image.at<uchar>(x,y+1));
	   boolOr=boolOr||(mask[1][2]*image.at<uchar>(x,y+1));
	}
	/*if(((x+1)<image.cols)&&((y+1)<image.rows)){			
	   boolAnd=boolAnd&&(mask[2][2]*image.at<uchar>(x+1,y+1));
	   boolOr=boolOr||(mask[2][2]*image.at<uchar>(x+1,y+1));
	}*/

	if(boolOr&&(!boolAnd)){
		return true;
	}
	return false;
/*
 1 2 3
 4 5 6
 7 8 9
*/
}


Mat erosionThres(const Mat& image){   
   Mat returnVal=image.clone();
   for(int i=0;i<image.rows;++i){
	for(int j=0;j<image.cols;++j){
            bool hitbool= HitMiss(image,i,j);
	    if(hitbool){
		returnVal.at<uchar>(i,j)=0;
	    }
	    

        }
   }
   return returnVal;

}/*finding fits*/


Mat dilationThres(const Mat& image){
   Mat returnVal=image.clone();
   for(int i=0;i<image.rows;++i){
	for(int j=0;j<image.cols;++j){
            bool hitbool= HitMiss(image,i,j);
	    if(hitbool){
		returnVal.at<uchar>(i,j)=255;
	    }
        }
   }
   return returnVal;
}


Mat closingOp(const Mat& image){
	Mat dImage,eImage;
	dImage=dilationThres(image);
	eImage=erosionThres(dImage);

	return eImage;

}//dilation then erosion

Mat openingOp(const Mat& image){
	Mat dImage,eImage;
	eImage=erosionThres(image);
	dImage=dilationThres(eImage);

	return dImage;
}//erosion then dilation

Mat runFunction(const Mat& image,int times, Mat (*a)(const Mat&)){
	Mat result=image.clone();
	for(int i=0;i<times;++i){
		result=a(result);	
	}

	return result;
}


int main( int argc, char** argv ){

    Mat odd,even,thresEven,thresOdd;
    //char exit;
    even = imread("TestImage-even-width.bmp", IMREAD_GRAYSCALE);   // Read the file
    odd = imread("TestImage-odd-width.bmp", IMREAD_GRAYSCALE);   // Read the file
    thresEven=thres(even);
    thresOdd=thres(odd);
    Mat (*openAdr)(const Mat&)=&openingOp;
    Mat (*closeAdr)(const Mat&)=&closingOp;
    Mat (*eroAdr)(const Mat&)=&erosionThres;
    Mat (*dilAdr)(const Mat&)=&dilationThres;

    /*imshow( "original image even", (thres(even))); 
    
    imshow( "erosion image even", (erosionThres(thres(even))));               // Show enhanced image.   
 
    imshow( "dilation image even", ((dilationThres(thres(even)))));               // Show enhanced image.
 
    imshow( "original image odd", (thres(odd))); 
    
    imshow( "erosion image odd", (erosionThres(thres(odd))));               // Show enhanced image.   
 
    imshow( "dilation image odd", ((dilationThres(thres(odd)))));               // Show enhanced image.
 */
   /* imshow( "original image odd", (thres(odd))); 
    
    imshow( "closing image odd", (closingOp(thres(odd))));               // Show enhanced image.   
 
    imshow( "opening image odd", ((openingOp(thres(odd)))));               // Show enhanced image.
   */
    imshow( "original image even", thresEven); 
    
    imshow( "closing image even", runFunction(thresEven,1,closeAdr));               // Show enhanced image.   
 
    imshow( "opening image even", ((openingOp(thres(even)))));               // Show enhanced image.
   
    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}

