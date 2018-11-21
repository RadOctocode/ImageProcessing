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

struct pixel{
    int x;
    int y;
    pixel(int myx, int myy){
    	x=myx;
	y=myy;
    }
    
};

struct centroid{
    vector<pixel> pixels;
    pixel center;
    group(vector<pixel> mypixels, pixel mycenter){
    	center=mycenter;
	pixels=mypixels;
    }
};
int point_distances(const pixel& p1,const pixel& p2){
	return abs(sqrt(pow((p2.x-p1.x),2)+pow((p2.y-p1.y),2)));

}

Mat k_means(int k, Mat image){
/*
 *1) pick K random clusters as centers
 *2) assign each data point to to the nearest cluster
 *3) take the average of the assigned pts and make that the new center
 *4) repeat until none of the cluster arrangements change. 
 * */

}

Mat difference (Mat screen1, Mat screen2){




}

Mat classify(Mat image){



}

Mat blockmatch(Mat screen1, Mat screen2){
//use blockmath algo


}


int main( int argc, char** argv ){

    Mat odd,even,thresEven,thresOdd;
    //char exit;
    even = imread("TestImage-even-width.bmp", IMREAD_GRAYSCALE);   // Read the file
    odd = imread("TestImage-odd-width.bmp", IMREAD_GRAYSCALE);   // Read the file
 

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}


/*
 *K1 - k means
 *M1 - class thresholding see sheet
 *N1-N4 - nearest neighbor
 *T1 - manual grouping
 *E - error btwn N1 to T1
 * */
