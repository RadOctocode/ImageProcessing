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

struct pixel{
   int x,y;
   pixel(){}
   pixel(int myx, int myy){
	x=myx;
        y=myy;
  }

};


struct region{
   int size;
   pixel startPix;
   region(){}
   region(int mysize, pixel myPix){
	size=mysize;
	startPix=myPix;
   }

   region(int mysize, int myx, int myy){
	size=mysize;
	pixel startPix=pixel(myx,myy);

   }

};
bool regComp(region r1, region r2){

  return r1.size<r2.size;

}

bool find_vect(vector<pixel> myvect,pixel myPixel){
   for(auto i:myvect){
	if((i.x==myPixel.x)&&(i.y==myPixel.y)){
		return true;

        }
   }
   return false;
}

Mat neg(Mat image){
  for(int i=0; i<image.rows; i++){
  	for(int j=0; j<image.cols; j++){
		image.at<uchar>(i,j)=255-image.at<uchar>(i,j);
	}
  }
  return image;

}

vector<int> calculateHist(const Mat& image){
  vector<int> histogram (256,0);
  for(int i=0; i<image.rows; i++){
  	for(int j=0; j<image.cols; j++){
		Vec3b currentPixel=image.at<Vec3b>(i,j);
		int newVal=(int)currentPixel[0];
		histogram[newVal]+=1;
	}
  }


  return histogram;
}

Mat graphHist(const Mat& image){
  
 vector<int> hist=calculateHist(image);
 int histY=512;
 int histX=400;
 int max=0;
 int bin_w = ceil((float)histX/256);
 Mat returnVal(histY,histX,CV_8UC3,Scalar(0,0,0));
 for(int i=0;i<256;++i){
	if(max<hist[i]){
		max=hist[i];	
	}
 }

 for(int i=0;i<256;++i){
	hist[i]=((float)hist[i]/max)*image.rows;
 }
 //normalise hist divide everyval by the max and times it by image rows
 //plot hist
 for(int i=0;i<256;++i){
	
	line(returnVal, Point(bin_w*(i), histX), Point(bin_w*(i), histX - hist[i]), Scalar(255,255,255), 4, 8, 0);
 }

 return returnVal;
}

Mat equalizeHist(const Mat& image){
  vector<int> hist=calculateHist(image);
  int totalPixels=0;
  vector<float> pdf (256,0);
  vector<float> cd (256,0);
  vector<float> cdf (256,0);
  vector<int> finalMapping (256,0);
  Mat returnimage=image.clone();

  for(auto i:hist){
	totalPixels+=i;
  }

  for(int i=0;i<256;++i){
	pdf[i]=(double)hist[i]/totalPixels;	
  }
  
  cd[0]=hist[0];
  for(int i=1;i<256;++i){
      cd[i]=(hist[i]+cd[i-1]);
  }

  for(int i = 0; i < 256; i++){
		cdf[i] = cd[i]/totalPixels;
  }


  for(int i=0;i<256;++i){
	int finalVal=ceil(cdf[i]*255);
	finalMapping[i]=finalVal;
  }
  for(int i=0; i<image.rows; i++){
  	for(int j=0; j<image.cols; j++){
		int currentHue=finalMapping[image.at<uchar>(i,j)];
		returnimage.at<uchar>(i,j)=currentHue;
	}
  }
  
  //calculate what group changes to what values
  //then based on the old value change it to the new value

 return returnimage;

}

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
		returnimage.at<uchar>(i,j)=255;		

	   }//bg
	   else{									   			returnimage.at<uchar>(i,j)=0;		
	   }//foreg

	}
   } 

   return returnimage;
}

int countSection(int x, int y, const Mat& image){
   pixel startPix=pixel(x,y);
   vector<pixel> seenPixels;
   stack<pixel> pixelStack;
   pixelStack.push(startPix);
   seenPixels.push_back(startPix);
   int currentColor=(int)image.at<uchar>(x,y);
   int returnVal=0;
   //cout<<image.rows*image.cols<<"\n";
   while(!pixelStack.empty()){
	//cout<<pixelStack.size()<<"\n";
	pixel topP=pixelStack.top();
	pixelStack.pop();
        returnVal++;
	currentImage.at<uchar>(topP.x,topP.y)=120;
	
        if((topP.x-1>=0)&&(topP.x+1<image.rows)&&(topP.y-1>=0)&&(topP.y+1<image.cols)){
		if((!find_vect(seenPixels,pixel(topP.x+1,topP.y)))&&(image.at<uchar>(topP.x+1,topP.y)==currentColor)){
			pixelStack.push(pixel(topP.x+1,topP.y));
			seenPixels.push_back(pixel(topP.x+1,topP.y));
		}
		if((!find_vect(seenPixels,pixel(topP.x-1,topP.y)))&&(image.at<uchar>(topP.x-1,topP.y)==currentColor)){                
			pixelStack.push(pixel(topP.x-1,topP.y));
			seenPixels.push_back(pixel(topP.x-1,topP.y));
		}
		if((!find_vect(seenPixels,pixel(topP.x,topP.y-1)))&&((image.at<uchar>(topP.x,topP.y-1)==currentColor))){
			pixelStack.push(pixel(topP.x,topP.y-1));
			seenPixels.push_back(pixel(topP.x,topP.y-1));
		}
		if((!find_vect(seenPixels,pixel(topP.x,topP.y+1)))&&(image.at<uchar>(topP.x,topP.y+1)==currentColor)){
			pixelStack.push(pixel(topP.x,topP.y+1));
			seenPixels.push_back(pixel(topP.x,topP.y+1));
		}


        }


   }


   return returnVal;

}

Mat colorImage(int x, int y, int color, const Mat& image){
   Mat returnImage=image.clone();
   pixel startPix=pixel(x,y);
   vector<pixel> seenPixels;
   stack<pixel> pixelStack;
   pixelStack.push(startPix);
   seenPixels.push_back(startPix);
   int currentColor=(int)image.at<uchar>(x,y);
   int returnVal=0;
   while(!pixelStack.empty()){
	pixel topP=pixelStack.top();
	pixelStack.pop();
        returnImage.at<uchar>(topP.x,topP.y)=color;
	
        if((topP.x-1>=0)&&(topP.x+1<image.rows)&&(topP.y-1>=0)&&(topP.y+1<image.cols)){
		if((!find_vect(seenPixels,pixel(topP.x+1,topP.y)))&&(image.at<uchar>(topP.x+1,topP.y)==currentColor)){
			pixelStack.push(pixel(topP.x+1,topP.y));
			seenPixels.push_back(pixel(topP.x+1,topP.y));
		}
		if((!find_vect(seenPixels,pixel(topP.x-1,topP.y)))&&(image.at<uchar>(topP.x-1,topP.y)==currentColor)){                
			pixelStack.push(pixel(topP.x-1,topP.y));
			seenPixels.push_back(pixel(topP.x-1,topP.y));
		}
		if((!find_vect(seenPixels,pixel(topP.x,topP.y-1)))&&((image.at<uchar>(topP.x,topP.y-1)==currentColor))){
			pixelStack.push(pixel(topP.x,topP.y-1));
			seenPixels.push_back(pixel(topP.x,topP.y-1));
		}
		if((!find_vect(seenPixels,pixel(topP.x,topP.y+1)))&&(image.at<uchar>(topP.x,topP.y+1)==currentColor)){
			pixelStack.push(pixel(topP.x,topP.y+1));
			seenPixels.push_back(pixel(topP.x,topP.y+1));
		}


        }


   }
   return returnImage;

}


Mat rankImage(const Mat& image){
    currentImage=image.clone();
    vector<region> segments; 
    bool mycolor=false; //false white true black
    Mat returnImage=image.clone();
    for(int i=0; i<image.rows; i++){
  	for(int j=0; j<image.cols; j++){
		if((currentImage.at<uchar>(i,j)==0)){
			mycolor=true;			
			int area=countSection(i,j,returnImage);
			if(area!=1){
				pixel myP=pixel(i,j);
				region currentRegion=region(area,myP);

				segments.push_back(currentRegion);
			}			
		}
		else if((currentImage.at<uchar>(i,j)==255)){
			mycolor=false;
		}
	}
   }
   cout<<"done calculating"<<"\n";
   sort(segments.begin(), segments.end(), regComp); // 12 32 45 71(26 33 53 80)
   //cout<<"done ranking "<<"smallest:"<<(segments[0]).size<<" largest:"<<(segments.back()).size<<"\n";
   pixel startG=(segments.back()).startPix;
   Mat colorGreat=colorImage(startG.x,startG.y,200,currentImage);
   Mat fullcolor=colorImage(((segments.front()).startPix).x, ((segments.front()).startPix).y, 60, colorGreat);
  
   return fullcolor; 
}


int main( int argc, char** argv ){

    Mat nycimg,houseimg,shapesimg,guideimg,histogramorg,newimage,histogramnew,negimage,histogramneg;
    //char exit;
    nycimg = imread("NYC.bmp", IMREAD_GRAYSCALE);   // Read the file
    houseimg = imread("house.bmp", IMREAD_GRAYSCALE);   // Read the file
    shapesimg = imread("shapes.bmp", IMREAD_GRAYSCALE);   // Read the file
    guideimg = imread("guide.bmp", IMREAD_GRAYSCALE);   // Read the file
   
    //house

    imshow( "original house", houseimg);               // Show enhanced image.
    imshow( "histogram of original house", graphHist(houseimg));          // Show enhanced histogram.

    imshow( "enhanced house", equalizeHist(houseimg));               // Show enhanced image.
    imshow( "enhanced histogram house", graphHist(equalizeHist(houseimg)));          // Show enhanced histogram.

    imshow( "negati ve house", neg(equalizeHist(houseimg)));               // Show enhanced image.
    imshow( "negative histogram house", graphHist(neg(equalizeHist(houseimg))));          // Show enhanced histogram.

	
    //newimage=equalizeHist(image);
    //histogramnew=graphHist(newimage);
    //imshow( "enhanced window2", newimage);               // Show enhanced image.
    //imshow( "enhanced histogram2", histogramnew);          // Show enhanced histogram.

    //negimage=neg(newimage);
    //histogramneg=graphHist(negimage);
    //imshow( "negative window3", negimage);               // Show negative image.
    //imshow( "negative histogram3", histogramneg);          // Show negative histogram.
    //Mat thresimg=thres(image);
    //Mat newimage1=colorImage(37, 36,120, thresimg);
    //histogramneg=graphHist(negimage);
    //imshow( "negative window3", thresimg);               // Show negative image.
    //imshow( "negative histogram3", histogramneg);          // Show negative histogram.
    
    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}

