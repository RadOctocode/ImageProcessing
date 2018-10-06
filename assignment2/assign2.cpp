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
   returnVal=(mask[0][0]*image.at<uchar>(x-1,y-1))+(mask[0][1]*image.at<uchar>(x,y-1))+(mask[0][2]*image.at<uchar>(x+1,y-1))+(mask[1][0]*image.at<uchar>(x-1,y))+(mask[1][1]*image.at<uchar>(x,y))+(mask[1][2]*image.at<uchar>(x+1,y))+(mask[2][0]*image.at<uchar>(x-1,y+1))+(mask[2][1]*image.at<uchar>(x,y+1))+(mask[2][2]*image.at<uchar>(x+1,y+1));
   return returnVal;
}

int MaskPatchD(const Mat& image,int x, int y, double mask[3][3]){
   int returnVal;
   returnVal=(mask[0][0]*image.at<uchar>(x-1,y-1))+(mask[0][1]*image.at<uchar>(x,y-1))+(mask[0][2]*image.at<uchar>(x+1,y-1))+(mask[1][0]*image.at<uchar>(x-1,y))+(mask[1][1]*image.at<uchar>(x,y))+(mask[1][2]*image.at<uchar>(x+1,y))+(mask[2][0]*image.at<uchar>(x-1,y+1))+(mask[2][1]*image.at<uchar>(x,y+1))+(mask[2][2]*image.at<uchar>(x+1,y+1))/4;
   return returnVal;
}

int MaskPatchF11(const Mat& image,int x, int y, vector<vector<float>> mask){
   int returnVal;
   returnVal=0;
   return returnVal;
}

int MaskPatchF7(const Mat& image,int x, int y, vector<vector<double>> mask){
   int row1,row2,row3,row4,row5,row6,row7;
   row1=(mask[0][0]*image.at<uchar>(x-3,y-3))+(mask[0][1]*image.at<uchar>(x-2,y-3))+(mask[0][
2]*image.at<uchar>(x-1,y-3))+(mask[0][3]*image.at<uchar>(x,y-3))+(mask[0][4]*image.at<uchar>(x+1,y-3))+(mask[0][5]*image.at<uchar>(x+2,y-3))+(mask[0][6]*image.at<uchar>(x+3,y-3));
   row2=(mask[1][0]*image.at<uchar>(x-3,y-2))+(mask[1][1]*image.at<uchar>(x-2,y-2))+(mask[1][
2]*image.at<uchar>(x-1,y-2))+(mask[1][3]*image.at<uchar>(x,y-2))+(mask[1][4]*image.at<uchar>(x+1,y-2))+(mask[1][5]*image.at<uchar>(x+2,y-2))+(mask[1][6]*image.at<uchar>(x+3,y-2));
   row3=(mask[2][0]*image.at<uchar>(x-3,y-1))+(mask[2][1]*image.at<uchar>(x-2,y-1))+(mask[2][
2]*image.at<uchar>(x-1,y-1))+(mask[2][3]*image.at<uchar>(x,y-1))+(mask[2][4]*image.at<uchar>(x+1,y-1))+(mask[2][5]*image.at<uchar>(x+2,y-1))+(mask[2][6]*image.at<uchar>(x+3,y-1));
   row4=(mask[3][0]*image.at<uchar>(x-3,y))+(mask[3][1]*image.at<uchar>(x-2,y))+(mask[3][
2]*image.at<uchar>(x-1,y))+(mask[3][3]*image.at<uchar>(x,y))+(mask[3][4]*image.at<uchar>(x+1,y))+(mask[3][5]*image.at<uchar>(x+2,y))+(mask[3][6]*image.at<uchar>(x+3,y));
   row5=(mask[4][0]*image.at<uchar>(x-3,y+1))+(mask[4][1]*image.at<uchar>(x-2,y+1))+(mask[4][
2]*image.at<uchar>(x-1,y+1))+(mask[4][3]*image.at<uchar>(x,y+1))+(mask[4][4]*image.at<uchar>(x+1,y+1))+(mask[4][5]*image.at<uchar>(x+2,y+1))+(mask[4][6]*image.at<uchar>(x+3,y+1));
   row6=(mask[5][0]*image.at<uchar>(x-3,y+2))+(mask[5][1]*image.at<uchar>(x-2,y+2))+(mask[5][
2]*image.at<uchar>(x-1,y+2))+(mask[5][3]*image.at<uchar>(x,y+2))+(mask[5][4]*image.at<uchar>(x+1,y+2))+(mask[5][5]*image.at<uchar>(x+2,y+2))+(mask[5][6]*image.at<uchar>(x+3,y+2));
   row6=(mask[6][0]*image.at<uchar>(x-3,y+3))+(mask[6][1]*image.at<uchar>(x-2,y+3))+(mask[6][
2]*image.at<uchar>(x-1,y+3))+(mask[6][3]*image.at<uchar>(x,y+3))+(mask[6][4]*image.at<uchar>(x+1,y+3))+(mask[6][5]*image.at<uchar>(x+2,y+3))+(mask[6][6]*image.at<uchar>(x+3,y+3));
   return row1+row2+row3+row4+row5+row6+row7;
}


int MaskPatchF11(const Mat& image,int x, int y, vector<vector<double>> mask){
   int offset= -5;
   int total=0;
for(int i=0;i<11;++i){
   total =(mask[i][0]*image.at<uchar>(x-5,y+offset))+(mask[i][1]*image.at<uchar>(x-4,y+offset))+(mask[i][2]*image.at<uchar>(x-3,y+offset))+(mask[i][3]*image.at<uchar>(x-2,y+offset))+(mask[i][4]*image.at<uchar>(x-1,y+offset))+(mask[i][5]*image.at<uchar>(x,y+offset))+(mask[i][6]*image.at<uchar>(x+1,y+offset))+(mask[i][7]*image.at<uchar>(x+2,y+offset))+(mask[i][8]*image.at<uchar>(x+3,y+offset))+(mask[i][9]*image.at<uchar>(x+4,y+offset))+(mask[i][10]*image.at<uchar>(x+5,y+offset))+total;
++offset;
 }

   return total;
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
	    returnImage.at<uchar>(i,j)=newval; 
        }
   }
 
   return returnImage;
}


Mat Blur(const Mat& image){
   Mat returnImage=image.clone();
   for(int i=1;i<image.rows-2;++i){
	for(int j=1;j<image.cols-2;++j){
	    returnImage.at<uchar>(i,j)=MaskPatchD(image,i,j,UsharpMask); 
        }
   }
 
   return returnImage;
}


Mat Sig7(const Mat& image,vector<vector<double>> mask){
   Mat returnImage=image.clone();
   for(int i=3;i<image.rows-6;++i){
	for(int j=3;j<image.cols-6;++j){
	    returnImage.at<uchar>(i,j)=MaskPatchF7(image,i,j,mask); 
        }
   }
 
   for(int i=0;i<image.rows;++i){
	for(int j=0;j<image.cols;++j){
	    if((int)(returnImage.at<uchar>(i,j))<100){
               returnImage.at<uchar>(i,j)=255; 

            } 
	    else{
               returnImage.at<uchar>(i,j)=0; 

            }
        }
   }

   return returnImage;
}

Mat Sig11(const Mat& image,vector<vector<double>> mask){
   Mat returnImage=image.clone();
   for(int i=5;i<image.rows-10;++i){
	for(int j=5;j<image.cols-10;++j){
	    returnImage.at<uchar>(i,j)=MaskPatchF11(image,i,j,mask); 

        }
   }
   for(int i=0;i<image.rows;++i){
	for(int j=0;j<image.cols;++j){
	    if((int)(returnImage.at<uchar>(i,j))<100){
               returnImage.at<uchar>(i,j)=255; 

            } 
	    else{
               returnImage.at<uchar>(i,j)=0; 

            }
        }
   }
 
   return returnImage;
}

vector<vector<double>> createLoGMask(double sigma, int size, int offset, int scale, int mapScale){
	
	int min =((-1.0 * (double)size / (double)2));
	int max = (((double)size / (double)2));
	vector<vector<double>> mask(size,vector<double>(size,0.0));
	int x = 0;
	double total = 0;
	double newTotal = 0;
        double sigConst,baseConst,expConst;

        //--------------------calculate formula----------------------------------- 
	for (int i = min*scale; i <= max*scale; i +=scale) {
		int y = 0;
		for (int j = min*scale; j <= max*scale; j+=scale) {
			sigConst = 1/(M_PI*pow(sigma, 4));
			baseConst = (pow(i,2) + pow(j,2)) / (2*pow(sigma, 2));
			mask[x][y] = ((-1*sigConst)*(1-baseConst)*(pow(exp(1),(-1*baseConst))))*mapScale;
			total = total +mask[x][y];
			++y;
		}
		++x;
	}
        //--------------------end calculate formula-----------------------------------


	//-------------------------start total---------------------------------
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("%lf ", mask[i][j]);
		}
		printf("\n");
	}
	printf("original %d kernel: %lf\n", total,size);
        //---------------------------end total-----------------------------------


	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (mask[i][j] > 0) {
				mask[i][j]=mask[i][j]-(total / (double)(pow(size, 2) - (double)pow(offset, 2)));
			}
			newTotal=newTotal+mask[i][j];
			printf("%lf ", mask[i][j]);
		}
		printf("\n");
	}

	printf("new %d kernel: %lf\n", newTotal,size);

  return mask;

}

int main( int argc, char** argv ){

    Mat ant,basel;
    //char exit;
    ant = imread("ant_gray.bmp", IMREAD_GRAYSCALE);   
    basel = imread("basel_gray.bmp",IMREAD_GRAYSCALE);
    imshow("Usharp basel",Usharp(basel));//blur take difference then add on top of the original picture
    imshow("Usharp ant",Usharp(ant));//blur take difference then add on top of the original picture
    imshow("Sobel basel",Sobel(Blur(basel)));//blur take difference then add on top of the original picture
    imshow("Sobel ant",Sobel(Blur(ant)));//blur take difference then add on top of the original picture
    vector<vector<double>> mask7 = createLoGMask(1.4,7,3,1,1);
    vector<vector<double>> mask11 = createLoGMask(5.0,11,5,2,100);
    imshow("LoG7 basel",Sig7(basel,mask7));//blur take difference then add on top of the original
    imshow("LoG11 basel",Sig11(basel,mask11));//blur take difference then add on top of the original
    imshow("LoG7 ant",Sig7(ant,mask7));//blur take difference then add on top of the original
    imshow("LoG11 ant",Sig11(ant,mask11));//blur take difference then add on top of the original
    waitKey(0);                                          
    return 0;
}

