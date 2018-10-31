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
#include "sobel.cpp"

#define pi 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679
#define smallerpi 3.1415926
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

double alpha2(int a){

	if(a==0){
		return 1.0/sqrt(2.0);
	
	}
	return 1.0;

}
double alpha(int i, int n){
	if(i!=0){
		return (sqrt(2.0)/(double)n);
	
	
	}
	return (sqrt(1.0)/(double)n);


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

Mat hueImage(const Mat& image){
	Mat returnImage=image.clone();
	for(int i =0;i<image.rows;++i){
        	for(int j=0;j<image.cols;++j){
			int inten;
			inten=image.at<Vec3b>(i,j)[0];
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

vector<float> dctVect(int pt, vector<int> img){
	vector<double> intensity;
	intensity.resize(pt);
	
	vector<float> returnVal;
	returnVal.resize(pt*pt);

	vector<vector<double>> cosine;
	vector<double> row;
	row.resize(pt);
	
	for(int i=0; i<pt; ++i){
		cosine.push_back(row);
		intensity[i]=img[i];	

	}

	double I2N= 1.0/(double)pt*2.0;

	for(int i=0;i<pt;++i){
		for(int j=0;j<pt;++j){
			cosine[i][j]=cos(smallerpi*i*(2.0*j+1)*I2N);
		}
	}


	for(int i=0;i<pt;++i){
		for(int j=0;j<pt;++j){
			for(int k=0;k<pt;++k){
				returnVal[j*pt+k]=0;
				for(int l=0;l<pt;++l){
					for(int m=0;m<pt;++m){
						float currentOut=cosine[j][l]*cosine[k][m];
						returnVal[j*pt+k]+=(currentOut*intensity[m]);
					
					}
				
				}

				returnVal[j*pt+k]*=alpha(j,pt)*alpha(k,pt);	
			}
		
		}
	
	}

	return returnVal;

}


Mat DCTImage_DC(Mat intensityImage){
	int height=intensityImage.rows;
	int width=intensityImage.cols;
	height-=(intensityImage.rows%8);
	width-=(intensityImage.cols%8);


	Mat retVal = intensityImage.clone();
	
	for(int x = 0; x < height; x+= 8){
		for(int y = 0; y < width; y+= 8){
			//for each block
			for(int i = x; i < x+8; i++){
				for(int j = y; j < y+8; j++){
					//for every pixel
					if(i != x || j != y){
						retVal.at<Vec3b>(i,j)[0] = 0;
						retVal.at<Vec3b>(i,j)[1] = 0;
						retVal.at<Vec3b>(i,j)[2] = 0;					
					}
				}
			}
		}
	}
	
	return retVal;
}//only keep one pixel

Mat DCTImage_9(Mat intensityImage){
	int height=intensityImage.rows;
	int width=intensityImage.cols;
	height-=(intensityImage.rows%8);
	width-=(intensityImage.cols%8);


	Mat retVal = intensityImage.clone();
	
	for(int x = 0; x < height; x+= 8){
		for(int y = 0; y < width; y+= 8){
			//for block 8 pixels
			for(int i = x; i < x+8; i++){
				for(int j = y; j < y+8; j++){
					//for every pixel
					for(int p = x; p < x+8; p++){
						for(int q = y; q < y+8; q++){
							//for every pixel
							if(!(i == x && j == y)&&!(i == x+2 && j == y+1)&&!(i == x+2 && j == y)&&!(i == x+1 && j == y+2)&&!(i == x+1 && j == y+1)&&!(i == x+1 && j == y)&&!(i == x && j == y+3)&&!(i == x && j == y+2)&&!(i == x && j == y +1)){
								retVal.at<Vec3b>(i,j)[0] = 0;								
								retVal.at<Vec3b>(i,j)[1] = 0;
								retVal.at<Vec3b>(i,j)[2] = 0;
					
							}
						}
					}
				}
			}
		}
	}
	
	return retVal;
}//keep 9 pixels


Mat DCTImage(const Mat& intensityImage, bool DC, bool l9){
        Mat retVal=intensityImage.clone();	
	retVal.setTo(Scalar(0,0,0));

	int height=intensityImage.rows;
	int width=intensityImage.cols;
	height-=(intensityImage.rows%8);
	width-=(intensityImage.cols%8);

	for(int bigblockx = 0; bigblockx < height; bigblockx+= 8){
		for(int bigblocky = 0; bigblocky < width; bigblocky+= 8){
			//8 pixel block	
			for(int smalli = bigblockx; smalli < bigblockx+8; ++smalli){
				for(int smallj = bigblocky; smallj < bigblocky+8; ++smallj){
					float currentVal = 0.0;
					
					for(int pixelp = bigblockx; pixelp < bigblockx+8; ++pixelp){
						for(int pixelq = bigblocky; pixelq < bigblocky+8; ++pixelq){
							float p2=2*pixelp+1;
							float q2=2*pixelq+1;
							float pii=smalli*pi;
							float pij=smallj*pi;
							currentVal+=(intensityImage.at<Vec3b>(pixelp,pixelq)[2])*cos(p2*pii/16)*(cos(q2*pij/16));	
						}
					}
					currentVal *= (0.25) * (alpha2(smalli) * alpha2(smallj));
					retVal.at<Vec3b>(smalli, smallj)[0] = ceil(currentVal);
					retVal.at<Vec3b>(smalli, smallj)[1] = ceil(currentVal);
					retVal.at<Vec3b>(smalli, smallj)[2] = ceil(currentVal);
				}
			}
		}
	}


	if(DC){
	     retVal=DCTImage_DC(retVal);

	}
	else if(l9){
	     retVal=DCTImage_9(retVal);

	}
	
    return retVal;
}


Mat IDCTImage(const Mat& intensityImage){
	Mat retVal=intensityImage.clone();	
	retVal.setTo(Scalar(0,0,0));

	int height=intensityImage.rows;
	int width=intensityImage.cols;
	height-=(intensityImage.rows%8);
	width-=(intensityImage.cols%8);

	
	for(int blockx = 0; blockx < height; blockx+= 8){
		for(int blocky = 0; blocky < width; blocky+= 8){
			//for each block
			for(int i = blockx; i < blockx+8; i++){
				for(int j = blocky; j < blocky+8; j++){
					float currentVal = 0.0;

					for(int p = blockx; p < blockx+8; p++){
						for(int q = blocky; q < blocky+8; q++){
							float i2=2*i+1;
							float j2=2*j+1;
							float pip=p*pi;
							float piq=q*pi;

							currentVal += (intensityImage.at<Vec3b>(p,q)[2])*(cos(((i2)) * pip / 16))*(cos(j2 * piq / 16))*(alpha2(i)*alpha2(j));
						}
					}
					currentVal *= (0.25);
					retVal.at<Vec3b>(i, j)[0] = ceil(currentVal);
					retVal.at<Vec3b>(i, j)[1] = ceil(currentVal);
					retVal.at<Vec3b>(i, j)[2] = ceil(currentVal);
				}
			}
		}
	}
	return retVal;

}

int main( int argc, char** argv ){

    Mat basel,intensity,HSI,build,plate;
    vector<int> row18={10,11,12,11,12,13,12,11};
    vector<int> row28={10,-10,8,-7,8,-8,7,-7};
    vector<int> row116={10,11,12,11,12,13,12,11,10,-10,8,-7,8,-8,7,-7};
    vector<float> row1=dctVect(8,row18);
    vector<float> row2=dctVect(8,row28);
    vector<float> row3=dctVect(16,row116);
    cout<<"/////////////////////////////////two rows//////////////////////////////////////////////\n";
    for(int i=0;i<8;++i){
	printf("%.5f, ",row1[i]);
	
    }
    for(int i=0;i<8;++i){
    	printf("%.5f, ",row2[i]);

    }
		cout<<"\n"<<"\n";
 cout<<"/////////////////////////////////one row//////////////////////////////////////////////\n";
    for(int i=0;i<16;++i){
	printf("%.5f, ",row3[i]);
    }

    //char exit;
    build = imread("Building1.bmp", IMREAD_COLOR);   
    plate = imread("Disk.bmp",IMREAD_COLOR);
    basel = imread("basel3.bmp",IMREAD_COLOR);
    plate=hueImage(rbgToHsi(plate));
   
    HSI=rbgToHsi(basel);
    intensity=intensityImage(HSI);
    Mat dctBaselL9=DCTImage(intensity,false,true);
    Mat dctBaselDC=DCTImage(intensity,true,false);
    Mat dctBasel=DCTImage(intensity,false,false);
    
    imshow("ROI BUILD",Sobel(build));
    imshow("ROI PLATE",Sobel(plate));
    imshow("HSI",HSI);
    imshow("intensity",intensity);
    imshow("DCT L9",dctBaselL9);
    imshow("DCT DC",dctBaselDC);
    imshow("DCT",dctBasel);
    imshow("IDCT",IDCTImage(dctBasel));
    imshow("IDCT L9",IDCTImage(dctBaselL9));
    imshow("IDCT DC",IDCTImage(dctBaselDC));
    waitKey(0);                                          
    return 0;
}
