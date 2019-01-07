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
int average4x4(Mat image,int y, int x){
	int retVal=0;
	for(int i=y;i<y+4;++i){
		for(int j=x;j<x+4;++j){
			retVal+=image.at<uchar>(i,j);	
		
		}
	
	}
	return retVal/16;

}

float distance(Mat image, Point2i pt1, Point2i pt2){
	float ret_val=0;
	for(int i=0; i<4 ; ++i){
		for(int j=0 ; j<4 ; ++j){
			Point2i start(pt1.x+i, pt1.y+j);
			Point2i finish(pt2.x+i, pt2.y+j);
			ret_val += ((image.at<uchar>(start) - image.at<uchar>(finish))*(image.at<uchar>(start) - image.at<uchar>(finish)));
		
		
		}
	
	}
	ret_val=sqrt(ret_val);
	return ret_val;
}


Point2i NN_dist(Mat image, Point2i start){
	Point2i ret_val(0,0);
	float minium=1000000;
	for(int i=0;i<image.rows/2;i+=4){
		for(int j=0;j<image.cols;j+=4){
			float current=distance(image,start,Point2i(j,i));
			if(current<minium){
				minium=current;
				ret_val=Point2i(j,i);
			}
		
		}
	
	}
	return ret_val;
}

Point2i NN(Mat image, uchar val){
	Point2i ret_val(0,0);
	int minium=256;
	for(int i=0;i<image.rows;++i){
		for(int j=0;j<image.cols;++j){
			uchar current=image.at<uchar>(i,j);
			if(abs(current-val)<minium){
				minium=abs(current-val);
				ret_val=Point2i(j,i);
			}
		
		}
	
	}
	return ret_val;



}

vector<int> class_avg(Mat image){
	vector<int> classsum(3,0);
	vector<int> classct(3,0);

	for(int y=0;y<image.rows/2;y+=4){
		for(int x =0;x<image.cols;x+=4){
			int intensity=average4x4(image,y,x);
		 	int index=0;
			if(intensity<125){
				index=0;
			}
			else if(intensity<175){
				index=1;
			
			}

			else{
				index=2;
			
			}
			classsum[index] += intensity;
			++classct[index];	
		}
	
	}
	for(int i=0;i<3;++i){
		if(classct[i]>0){
		
			classsum[i]=classsum[i]/classct[i];
		}
	}
	

	return classsum;
}

Mat M1(Mat image,int start,int end){
    Mat ret_val=image.clone();
    for(int y=start;y<end;y+=4){
    	for(int x = 0 ; x < image.cols ; x+=4){
	     	
	     int avginten=average4x4(image,y,x);
	     int set_val=0;
     	     
	     /////////////set the value///////////////
	     if((avginten>=0)&&(avginten<125)){
	     	set_val=10;
	     }	     
	     else if((avginten>=125)&&(avginten<175)){
	    	set_val=130;
	     }
	     else{
	    	set_val=255;
	     } 
             //////////set the value///////////////


            /////////////set//////////////
	     for(int i=y;i<y+4;++i){
		for(int j=x;j<x+4;++j){
			ret_val.at<uchar>(i,j)=set_val;	
		
		}
	
	     }
	    /////////////set///////////////

	}
    }
    return ret_val;
}


Mat N1(Mat image){
   Mat ret_val=image.clone();
   for(int y=(image.rows/2);y<image.rows;y+=4){
   	for(int x=0;x<image.cols;x+=4){
		//find the nearest point
		//determine the average intensity of of that nearest point

	     Point2i nearest=NN_dist(image,Point2i(x,y));
             int avginten=average4x4(image,nearest.y,nearest.x);
	     int set_val=0; 
	     /////////////set the value///////////////
	     if((avginten>=0)&&(avginten<125)){
	     	set_val=10;
	     }	     
	     else if((avginten>=125)&&(avginten<175)){
	    	set_val=130;
	     }
	     else{
	    	set_val=255;
	     } 
             //////////set the value///////////////


            /////////////set//////////////
	     for(int i=y;i<y+4;++i){
		for(int j=x;j<x+4;++j){
			ret_val.at<uchar>(i,j)=set_val;	
		
		}
	
	     }
	    /////////////set///////////////
		//set the intesity of the current 4x4
	}
   
   }

   return ret_val;
}

Mat N2(Mat image){
   Mat ret_val=image.clone();
   for(int y=(image.rows/2);y<image.rows;y+=4){
   	for(int x=0;x<image.cols;x+=4){
		//find the nearest point
		Point2i nearest=NN_dist(image,Point2i(x,y));
		//each pixel is real to the nearest pixel
	     for(int i=0;i<4;++i){
		for(int j=0;j<4;++j){
			ret_val.at<uchar>(y+i,x+j)=image.at<uchar>(nearest.y+i,nearest.x+j);	
		
		}
	
	     }

	}
   
   }

   return ret_val;


}

Mat N3(Mat image, Mat training_image){
   Mat ret_val=image.clone();
   for(int y=(image.rows/2);y<image.rows;y+=4){
   	for(int x=0;x<image.cols;x+=4){
		//find the average intensity of the current row/col
		uchar inten=average4x4(image,y,x);
		Point2i neighbor=NN(training_image,inten);



            /////////////set//////////////
	     for(int i=y;i<y+4;++i){
		for(int j=x;j<x+4;++j){
			ret_val.at<uchar>(i,j)=training_image.at<uchar>(neighbor);	
		
		}
	
	     }
	    /////////////set///////////////
		//find nn in the training image based on the intensity
		//set the intensity of the 4x4 based on the intensity of the nn 
	}
   
   }

   return ret_val;


}

Mat N4(Mat image){
   Mat ret_val=image.clone();
   vector<int> averages=class_avg(image);

   for(int y=(image.rows/2);y<image.rows;y+=4){
   	for(int x=0;x<image.cols;x+=4){
		//find the nearest point
		Point2i close=NN_dist(image,Point2i(x,y));
		//determine the average intensity of of that nearest point
		int inten=average4x4(image,close.y,close.x);
		//set the intesity of the current 4x4
                int index=0;
	        if(inten<125){
			index=0;
		}
		else if(inten<175){
			index=1;
			
		}

		else{
			index=2;
			
		}
            /////////////set//////////////
	     for(int i=y;i<y+4;++i){
		for(int j=x;j<x+4;++j){
			ret_val.at<uchar>(i,j)=(uchar)averages[index];	
		
		}
	
	     }
	    /////////////set///////////////
		

	}
   
   }

   return ret_val;


}


vector<uchar> locate_center(Mat image, vector<Point2i>points){
	vector<long> centernew(16,0);
	for(int i=0;i<points.size();++i){
		for(int a=0;a<4;++a){
			for(int b=0;b<4;++b){
				centernew[b*4+a] += image.at<uchar>(Point2i(points[i].x + a,points[i].y + b));	
			
			}
		}	
	}
	vector<uchar> totalnew(16,0);
	for(int i=0;i<16;++i){
		totalnew[i]=(uchar)(centernew[i]/points.size());
	
	}
	return totalnew;
}



float kmeans_dist(Mat image, Point2i point, vector<uchar> value){
	float distance=0;
	for(int a=0;a<4;++a){
		for(int b=0;b<4;++b){
			Point2i current(point.x+a, point.y+b);
			if((current.x<image.cols)&&(current.y<image.rows)){
				uchar current_val=value[a+b*4];
				distance = distance + ((image.at<uchar>(current)-current_val)*(image.at<uchar>(current)-current_val));
			
			}
		}
	}



	return sqrt(distance);
}


int minindft(vector<float> ft_vals){
	int retval=0;
	float min=ft_vals[0];
	for(int i=1;i<ft_vals.size();++i){
		if(min > ft_vals[i]){
			retval=i;
			min=ft_vals[i];
		}
	
	}
	return retval;
}

int minindint(vector<int> char_vals){
	int retval=0;
	uchar min=char_vals[0];
	for(int i=1;i<char_vals.size();++i){
		if(min > char_vals[i]){
			retval=i;
			min=char_vals[i];
		}
	
	}
	return retval;
}

double calc_error(Mat image1,Mat image2){
	int vCount=0;
	double dCount=0;
	for(int i=image1.rows/2;i<image1.rows;i+=4){
		for(int j=image1.cols/2;j<image1.cols;j+=4){
			++vCount;
			if(image1.at<uchar>(i,j)!=image2.at<uchar>(i,j)){
				++dCount;	
			}	
		
		
		}
	
	}
	return dCount/vCount;
}


Mat K1(Mat image){
	vector<uchar> labels (3);
	Mat ret_val=image.clone();
	vector<Point2i> center_def = {Point2i(0,0), Point2i(6,0), Point2i(12,0)};
	vector<vector<Point2i>> class_def(3);
	bool center_move=true;

	//////////////////////////////////////////start kmeans rep steps////////////////////////////////////////////////
	while(center_move) {

      
	
	////////////////////////////clean classes///////////////////////////////
        for(int i = 0; i < 3; ++i) {
            class_def[i].clear();
            class_def[i].push_back(center_def[i]);
        }
	//////////////////////////////////////////////////////////////////////


        ///////////////////////////// sort into classes/////////////////////////////////////////////
        for(int row = 0; row < image.rows/2; row+=4){
            for(int col = 0; col < image.cols; col+=4) {
                Point2i p(col, row);
                
		if(p == center_def[0] || p == center_def[1] || p == center_def[2]){
		       	continue;
		}//ignore centers

                vector<float> mins = {distance(image,p,center_def[0]), distance(image,p,center_def[1]), distance(image,p,center_def[2])};
                class_def[minindft(mins)].push_back(p);
		//calculate distances and get the smallest one
            }
	}
	////////////////////////////////////////////////////////////////////////////////////////////


        center_move = false;


        vector<Point2i> new_center_def(3);

        /////////////////////////////////////// calc new center///////////////////////////////////////////
        for(int i = 0; i < 3; ++i) {
            auto newCenter = locate_center(image, class_def[i]);

            double min = 10000000;
            Point2i newPoint = center_def[i];

            for(int row = 0; row < image.rows/2; ++row)
                for(int col = 0; col < image.cols; ++col) {
                    double dist = kmeans_dist(image, Point2i(col, row), newCenter);
                    if(min > dist) {
                        newPoint = Point2i(col, row);
                        min = dist;
                    }
                }



            new_center_def[i] = newPoint;
        }

	//////////////////////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////check if centers changed if so continue with while///////////////////////////////////////////////
        for(auto p : new_center_def) {
            if(p != center_def[0] && p != center_def[1] && p != center_def[2]) {
                center_move = true;
                break;
            }
        }///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        center_def = new_center_def;

    }/////////////////////////////////////////////end of while loop/////////////////////////////////////////////////////////








    ////////////////////////////////////////// clean classes///////////////////////////////////////////////////
    for(int i = 0; i < 3; ++i) {
        class_def[i].clear();
        class_def[i].push_back(center_def[i]);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////// add values///////////////////////////////////////
    for(int i = 0; i < image.rows/2; i+=4){
        for(int j = 0; j < image.cols; j+=4) {
            Point2i p(j, i);

            if(p == center_def[0] || p == center_def[1] || p == center_def[2]){ 
		    continue;
	    }

            vector<float> miniums = {distance(image,p,center_def[0]), distance(image,p,center_def[1]), distance(image,p,center_def[2])};
            class_def[minindft(miniums)].push_back(p);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    vector<int> intens = {average4x4(image, center_def[0].y, center_def[0].x),average4x4(image, center_def[1].y, center_def[1].x),average4x4(image, center_def[2].y, center_def[2].x)};
    

    ///////////////////////////////////////determine labels///////////////////////////////////////////////
    int smallest = minindint(intens);

    labels[smallest] = 0;
    if(intens[(smallest+1)%3] > intens[(smallest+2)%3]) {
        labels[(smallest+1)%3] = 255;
        labels[(smallest+2)%3] = 128;
    }
    else {
        labels[(smallest+2)%3] = 255;
        labels[(smallest+1)%3] = 128;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////color clusters//////////////////////////////////////////////
    int label = 0;
    for(int v=0;v<class_def.size();++v) {
        for(auto p : class_def[v]){

	     for(int i=0;i<4;++i){
		for(int j=0;j<4;++j){
			ret_val.at<uchar>(p.y+i,p.x+j)=labels[label];	
		
		}
	
	     }

	}
	label++;
    }
    /////////////////////////////////////////////////////////////////////////////////////////
	


return ret_val;

}

int main( int argc, char** argv ){

    Mat odd,even,thresEven,thresOdd;
    //char exit;
    even = imread("TestImage-even-width.bmp", IMREAD_GRAYSCALE);   // Read the file
    odd = imread("TestImage-odd-width.bmp", IMREAD_GRAYSCALE);   // Read the file
    Mat test=even.clone();
    Mat fullycorrect=M1(test,test.rows/2,test.rows);
    Mat traindata=M1(test,0,test.rows/2);
    //imshow("My M1",traindata);
    //imshow("My N1",N1(even));
    //imshow("My N2",N2(even));
    //imshow("My N3",N3(even,traindata));
    //imshow("My N4",N4(even));
    imshow("My K1",K1(even));
    //imshow("My T1",fullycorrect);
    cout<<"-------------------------error rate-------------------------\n"<<calc_error(fullycorrect,N1(even))*100<<"\n"<<"-----------------------------------------";

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

