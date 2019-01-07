#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <algorithm>    
#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace cv;
using namespace std;

Mat compare_frame(Mat image1, Mat image2){
	Mat ret_val(image1.size(),CV_8U);
	if(image1.size()==image2.size()){
		for(int y=0;y<image1.rows;++y){
			for(int x=0;x<image1.cols;++x){

				ret_val.at<uchar>(y,x)=abs(image1.at<uchar>(y,x)-image2.at<uchar>(y,x));	
				//get the abs difference between frame 1 and frame 2 and add it to the image;
			}
	
		}
	}
	else{
		cout<<"you can't do that with two different sized images";
		return ret_val;
	}

	return ret_val;

}

float distance8(Mat image1, Mat image2, Point2i pt1, Point2i pt2){
	float ret_val=0;
	for(int i=0;i<8;++i){
		for(int j=0;j<8;++j){
			Point2i p1(pt1.x+i,pt1.y+j);
			Point2i p2(pt2.x+i,pt2.y+j);
			uchar powval=image1.at<uchar>(p1)-image2.at<uchar>(p2);
			ret_val+=pow(image1.at<uchar>(p1)-image2.at<uchar>(p2),2);
		}
	
	}//for each pixel in the 8by8 block
	ret_val=sqrt(ret_val);
	return ret_val;
}


Point2i closest_diff(Mat image1, Mat image2, Point2i pt){
	Point2i return_val(0,0);
	float lowestdist=1000000;
	for(int y=0;y<image1.rows;y+=8){
		for(int x=0;x<image1.cols;x+=8){
			float curr_dist=distance8(image1,image2,pt,Point2i(x,y));
			if(curr_dist<=lowestdist){
				return_val=Point2i(x,y);
				lowestdist=curr_dist;
			}
		}
	
	
	}

	//find the closest 8x8
	return return_val;

}



cv::Point2i nearest_diff(const cv::Mat& img1, const cv::Mat& img2, cv::Point2i p) {
    cv::Point2i nearest(0, 0);
    double minDist = std::numeric_limits<double>::max();
    for (int i = 0; i < img2.rows; i+=8) {
        for (int j = 0; j < img2.cols; j+=8) {
            double dist = distance8(img1, img2, p, cv::Point2i(j, i));
            if (dist <= minDist) {
                minDist = dist;
                nearest = cv::Point2i(j, i);
            }
        }
    }
    return nearest;
}

Mat compare_diff(Mat image1, Mat image2){
	Mat ret_val(image1.size(), CV_8U);
	for(int y=0;y<image1.rows;y+=8){
		for(int x=0;x<image1.cols;x+=8){
			Point2i curr_closest=closest_diff(image1, image2, Point2i(x,y));
			
			
			for(int i=0;i<8;++i){
				for(int j=0;j<8;++j){
					Point2i curr(x+i,y+j);
					Point2i close(curr_closest.x+i,curr_closest.y+j);
					ret_val.at<uchar>(curr)=abs(image1.at<uchar>(curr)-image2.at<uchar>(close));	
				}
	
			}//for every pixel in the 8 by 8 square

		}
	
	
	}// for every 8 by 8 square


	return ret_val;
}



Point2i normalP(Point2i pt1, Point2i pt2, int sc){
	Point2i ret_val;
	int distx=pt2.x-pt1.x;
	int disty=pt2.y-pt1.y;

	double sqroot=sqrt((distx*distx)+(disty*disty));
	if(sqroot<1){
		sqroot=ceil(sqroot);
	
	}
	else{
	       sqroot=floor(sqroot);
	
	}


	distx=distx/sqroot*sc;
	disty=disty/sqroot*sc;
	ret_val.x=pt1.x+distx;
	ret_val.y=pt1.y+disty;
	
	return ret_val;

}

Mat draw_vect(Mat image1, Mat image2){
     Mat difference(image1.size(),CV_8U);
     Mat ret_val;
     cvtColor(image2,ret_val,CV_GRAY2BGR);


     for(int y=0;y<image2.rows;y+=8){
		for(int x=0;x<image2.cols;x+=8){
			Point2i near=closest_diff(image1,image2,Point2i(x,y));
			long diff=0;	
	
			for(int i=0;i<8;++i){
				for(int j=0;j<8;++j){
					diff = diff + abs(image1.at<uchar>(y+j,x+i)-image2.at<uchar>(near.y+j,near.x+i));
				}
	
			}//for every pixel in the 8 by 8 square
			if(diff>800){
				Point2i pt(x,y);
				arrowedLine(ret_val,pt,normalP(pt,near,7),CV_RGB(255,0,0),1,CV_AA,0,0.8);
				//draw an arrow on retval
			}
		}
		
	}// for every 8 by 8 square

	return ret_val;

}

int main( int argc, char** argv ){

    //char exit;
    Mat play1 = imread("PlayFrame1_320x200.bmp", IMREAD_GRAYSCALE);   // Read the file
    Mat play2 = imread("PlayFrame2_320x200.bmp", IMREAD_GRAYSCALE);   // Read the file
    Mat tennis1 = imread("TennisFrame1_320x200.bmp", IMREAD_GRAYSCALE);   // Read the file
    Mat tennis2 = imread("TennisFrame2_320x200.bmp", IMREAD_GRAYSCALE);   // Read the file
    
    
    imshow("play diff",compare_frame(play1,play2));
    imshow("tennis diff",compare_frame(tennis1,tennis2));
    imshow("play comp",compare_diff(tennis1,tennis2)); 
    imshow("tennis comp",compare_diff(play1,play2)); 

    imshow("play arrow",draw_vect(tennis1,tennis2)); 
    imshow("tennis arrow",draw_vect(play1,play2)); 

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}


/*
 *K1 - k means
 *M1 - class thresholding see sheet
 *N1-N4 - nearest neighbor
 *T1 - manual grouping of 4 by 4
 *E - error btwn N1 to T1
 *
 *
 *row=y
 *col=x
 *
 * */

