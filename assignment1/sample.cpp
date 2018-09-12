#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <map>
#include <math.h>

using namespace cv;
using namespace std;

Mat neg(Mat image){
  for(int i=0; i<image.rows; i++){
  	for(int j=0; j<image.cols; j++){
		Vec3b currentPixel=image.at<Vec3b>(i,j);
		int newVal=((int)currentPixel[0]+(int)currentPixel[1]+(int)currentPixel[2])/3;
		newVal=255-newVal;
		Vec3b newPixel;
		newPixel[0]=newVal;
		newPixel[1]=newVal;	
		newPixel[2]=newVal;
		image.at<Vec3b>(i,j)=newPixel;	
	}
  }
  return image;

}

vector<int> calculateHist(Mat image){
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

Mat graphHist(Mat image){
 int histY=512;
 int histX=400;
 int bin_w = ceil((float)hist_w/256);
 Mat returnVal(histY,histX,CV_8UC3,Scalar(255,255,255));
 //normalise hist divide everyval by the max and times it by image rows
 //plot hist
 for(int i=0;i<256;++i){
	
	line(histogram_image, Point(bin_w*(x), hist_h), Point(bin_w*(x), hist_h - histogram[x]), Scalar(0,0,0), 1, 8, 0);
 }

 return returnVal;
}

Mat equalizeHist(Mat image){
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
      cd[i]=hist[i]+cd[i-1];
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



int main( int argc, char** argv ){
    
    if( argc != 2){
     	cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     	return -1;
    }

    Mat image,grayscale,newimage;
    image = imread(argv[1], IMREAD_GRAYSCALE);   // Read the file

    if(!image.data ){                              // Check for invalid input
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }


    imshow( "Display window", image);                   // Show our image inside it.
     
    newimage=equalizeHist(image);
    imshow( "Display window1", newimage);               // Show our image inside it.


    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}

