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

#define pi 3.142857 
using namespace cv;
using namespace std;



vector<int> calculateHist(const Mat& image){
  vector<int> histogram (256,0);
  for(int i=0; i<image.rows; i++){
  	for(int j=0; j<image.cols; j++){
		Vec3b currentPixel=image.at<Vec3b>(i,j);
		int newVal=(int)currentPixel[2];//use int
		histogram[newVal]+=1;
	}
  }

  return histogram;
}


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

Mat intensityImage(const Mat& image){
	Mat returnImage=image.clone();
	for(int i =0;i<image.rows;++i){
        	for(int j=0;j<image.cols;++j){
			int inten;
			inten=image.at<Vec3b>(i,j)[2];
			returnImage.at<Vec3b>(i,j)[0]=inten;
			returnImage.at<Vec3b>(i,j)[1]=inten;
			returnImage.at<Vec3b>(i,j)[2]=inten;
		
		}
	}


	return returnImage;
}

float calcFV(vector<int> currentHist,int thres,float mean){
	float returnVal;	
	for (int i=0;i<256;++i){
		if(i>=thres){
			returnVal+=(((i-mean)*(i-mean))+currentHist[i]);
		}
	}
	return returnVal;
}

float calcBV(vector<int> currentHist,int thres,float mean){
	float returnVal;	
	for (int i=0;i<256;++i){
		if(i<thres){
			returnVal+=(((i-mean)*(i-mean))+currentHist[i]);
		}
	}
	return returnVal;
}

int ostuMethod(const Mat& image){
	int returnVal=140;
	vector<float> classVar (256,1000.0);
	vector<int> currentHist=calculateHist(image);
	int totalnum=image.cols*image.rows;
	for (int i=0;i<256;++i){
		float forgW=0;
		float backgW=0;
		float forgM=0;
		float backgM=0;
		int backP=0;
		int forP=0;
		for (int j=0;j<256;++j){
			if(j<i){
				backgW+=currentHist[j];
				backgM+=(j*currentHist[j]);
				
			}//back
			else{
				forgW+=currentHist[j];
				forgM+=(j*currentHist[j]);				
			}//fore
			
		}
		backP=backgW;
		forP=forgW;	
		forgW/=totalnum;
		backgW/=totalnum;
		forgM/=forP;
		backgM/=backP;
		int forgV=calcFV(currentHist,i,forgM)/forP;
		int backgV=calcBV(currentHist,i,backgM)/backP;
		//calculate
		classVar[i]=((forgW)*(forgV))+((backgW)*(backgV));
	}//for every index
	for(int i=0;i<256;++i){
		if(classVar[i]<classVar[returnVal]){
			returnVal=i;
		}
	}
	cout<<returnVal<<'\n';
	return returnVal;
}


Mat intensityThreshold(const Mat& image){
	int huethres=ostuMethod(image);
	Mat returnImage=image.clone();

	for(int i =0;i<image.rows;++i){
        	for(int j=0;j<image.cols;++j){
			if(((int)image.at<Vec3b>(i,j)[2])>huethres){
				returnImage.at<Vec3b>(i,j)[0]=255;
				returnImage.at<Vec3b>(i,j)[1]=255;
				returnImage.at<Vec3b>(i,j)[2]=255;

			}
			else{

				returnImage.at<Vec3b>(i,j)[0]=0;
				returnImage.at<Vec3b>(i,j)[1]=0;
				returnImage.at<Vec3b>(i,j)[2]=0;
			}
			
		
		}
	}

	return returnImage;

}


Mat dctTransform(int matrix[][8]){ 
  
    // dct will store the discrete cosine transform 
    float dct[8][8]; 
    float ci, cj, dct1, sum; 
    for (int i = 0; i < 8; ++i) { 
        for (int j = 0; j < 8; ++j) { 
  
            // ci and cj depends on frequency as well as 
            // number of row and columns of specified matrix 
            if (i == 0) 
                ci = 1 / sqrt(8); 
            else
                ci = sqrt(2) / sqrt(8); 
            if (j == 0) 
                cj = 1 / sqrt(8); 
            else
                cj = sqrt(2) / sqrt(8); 
  
            // sum will temporarily store the sum of  
            // cosine signals 
            sum = 0; 
            for (int k = 0; k < 8; ++k) { 
                for (int l = 0; l < 8; ++l) { 
                    dct1 = matrix[k][l] * cos((2 * k + 1) * i * pi / (2 * 8))*cos((2 * l + 1) * j * pi / (2 * 8)); 
                    sum += dct1; 
                } 
            } 
            dct[i][j] = ci * cj * sum; 
        } 
    } 

    Mat test(8,8,CV_64F);
    memcpy(test.data,dct,8*8*sizeof(int));
    imwrite("test2.jpeg",test);
    return test;
} 

int main() 
{ 
    int matrix[8][8] = { { 255, 255, 255, 255, 255, 255, 255, 255 }, 
                         { 255, 255, 255, 255, 255, 255, 255, 255 }, 
                         { 255, 255, 255, 255, 255, 255, 255, 255 }, 
                         { 255, 255, 255, 255, 255, 255, 255, 255 }, 
                         { 255, 255, 255, 255, 255, 255, 255, 255 }, 
                         { 255, 255, 255, 255, 255, 255, 255, 255 }, 
                         { 255, 255, 255, 255, 255, 255, 255, 255 }, 
                         { 255, 255, 255, 255, 255, 255, 255, 255 } }; 
    Mat test(8,8,CV_64F);
    memcpy(test.data,matrix,8*8*sizeof(int));
    imwrite("test.bmp",test);
    imshow("help",test);
    dctTransform(matrix); 
    return 0; 
} 



/*int main( int argc, char** argv ){

    Mat basel,gray;
    //char exit;
    //ant = imread("ant_gray.bmp", IMREAD_COLOR);   
    //basel = imread("test.jpg",IMREAD_COLOR);
    basel = imread("Building1.bmp",IMREAD_COLOR);
   //imshow("basel",basel);//blur take difference then add on top of the original picture
   //imshow("ant",ant);//blur take difference then add on top of the original picture
  //imshow("basel 2",rbgToHsi(basel));
  gray=rbgToHsi(basel);
  imshow("basel 3",gray);
  imshow("basel 4",intensityThreshold(gray));
   waitKey(0);                                          
    return 0;
}*/
