#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <map>
#include <math.h>

using namespace cv;
using namespace std;

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

Mat colorImage(){






}



int main( int argc, char** argv ){
    
    if( argc != 2){
     	cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     	return -1;
    }

    Mat image,histogramorg,newimage,histogramnew,negimage,histogramneg;
    image = imread(argv[1], IMREAD_GRAYSCALE);   // Read the file

    if(!image.data ){                              // Check for invalid input
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    histogramorg=graphHist(image);
    imshow( "original", image);                   // Show original image.
    imshow( "original histogram", histogramorg);           // Show original histogram.

    newimage=equalizeHist(image);
    histogramnew=graphHist(newimage);
    imshow( "enhanced window2", newimage);               // Show enhanced image.
    imshow( "enhanced histogram2", histogramnew);          // Show enhanced histogram.

    negimage=neg(newimage);
    histogramneg=graphHist(negimage);
    imshow( "negative window3", negimage);               // Show negative image.
    imshow( "negative histogram3", histogramneg);          // Show negative histogram.
    

    
    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}

