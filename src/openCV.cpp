/*
 * openCV.cpp
 *
 *  Created on: Jul 26, 2015
 *      Author: odroid
 */

#include "openCV.h"

int testSobel(void){
	IplImage *inputImg;// = cvLoadImage("hsv-color-model.png");


    //VideoCapture cap(-1); // open the video file for reading
    CvCapture* capture = cvCaptureFromCAM(0);
    cvGrabFrame( capture );
	inputImg = cvRetrieveFrame( capture );
	IplImage *outputImg = cvCreateImage(cvSize(inputImg->width, inputImg->height),IPL_DEPTH_8U, 3);
	IplImage *rImg = cvCreateImage(cvSize(inputImg->width, inputImg->height),IPL_DEPTH_8U, 3);
	IplImage *gImg = cvCreateImage(cvSize(inputImg->width, inputImg->height),IPL_DEPTH_8U, 3);
	IplImage *bImg = cvCreateImage(cvSize(inputImg->width, inputImg->height),IPL_DEPTH_8U, 3);

	IplImage *rImgResize = cvCreateImage(cvSize(inputImg->width/2, inputImg->height/2),IPL_DEPTH_8U, 3);
	IplImage *gImgResize = cvCreateImage(cvSize(inputImg->width/2, inputImg->height/2),IPL_DEPTH_8U, 3);
	IplImage *bImgResize = cvCreateImage(cvSize(inputImg->width/2, inputImg->height/2),IPL_DEPTH_8U, 3);
    //if ( !cap.isOpened() )  // if not success, exit program
    //{
    //     cout << "Cannot open the video file" << endl;
    //     return -1;
    //}

    //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

    //double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

     //cout << "Frame per seconds : " << fps << endl;

     cvNamedWindow("MyVideo", CV_WINDOW_AUTOSIZE);
     cvNamedWindow("RImag", CV_WINDOW_AUTOSIZE);
     cvNamedWindow("BImag", CV_WINDOW_AUTOSIZE);
     cvNamedWindow("GImag", CV_WINDOW_AUTOSIZE);
    //namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

     //cvResizeWindow("MyVideo", inputImg->width/2, inputImg->height/2);
     cvResizeWindow("RImag", inputImg->width/2, inputImg->height/2);
     cvResizeWindow("BImag", inputImg->width/2, inputImg->height/2);
     cvResizeWindow("GImag", inputImg->width/2, inputImg->height/2);


    while(1){
        //Mat frame;

        //bool bSuccess = cap.read(frame); // read a new frame from video
        cvGrabFrame( capture );
        inputImg = cvRetrieveFrame( capture );
        cvFlip(inputImg,NULL,1);
         //if (!bSuccess) //if not success, break loop
        //{
        //                cout << "Cannot read the frame from video file" << endl;
        //               break;
        //}

        cvCvtColor(inputImg, outputImg, CV_BGR2HSV);
        //frame = cvarrToMat(outputImg);

        for(int row = 0; row < inputImg->height; row++){
          for(int col = 0; col < inputImg->width; col++){
            int valH = outputImg->imageData[inputImg->widthStep * row + col * 3];
            int valS = outputImg->imageData[inputImg->widthStep * row + col * 3+1];
            if((valH < 5 || valH > 175) && valS > 1){
            	rImg->imageData[inputImg->widthStep * row + col * 3] = 0;
            	rImg->imageData[inputImg->widthStep * row + col * 3 + 1] = 0;
            	rImg->imageData[inputImg->widthStep * row + col * 3 + 2] = 255;
            }
            else{
            	rImg->imageData[inputImg->widthStep * row + col * 3] = 0;
				rImg->imageData[inputImg->widthStep * row + col * 3 + 1] = 0;
				rImg->imageData[inputImg->widthStep * row + col * 3 + 2] = 0;
            }

            if(valH > 29 && valH < 90){
				gImg->imageData[inputImg->widthStep * row + col * 3] = 0;
				gImg->imageData[inputImg->widthStep * row + col * 3 + 1] = 255;
				gImg->imageData[inputImg->widthStep * row + col * 3 + 2] = 0;
			}
            else{
            	gImg->imageData[inputImg->widthStep * row + col * 3] = 0;
				gImg->imageData[inputImg->widthStep * row + col * 3 + 1] = 0;
				gImg->imageData[inputImg->widthStep * row + col * 3 + 2] = 0;
            }

            if(valH > 89 && valH < 150){
				bImg->imageData[inputImg->widthStep * row + col * 3] = 0;
				bImg->imageData[inputImg->widthStep * row + col * 3 + 1] = 0;
				bImg->imageData[inputImg->widthStep * row + col * 3 + 2] = 255;
			}
            else{
				bImg->imageData[inputImg->widthStep * row + col * 3] = 0;
				bImg->imageData[inputImg->widthStep * row + col * 3 + 1] = 0;
				bImg->imageData[inputImg->widthStep * row + col * 3 + 2] = 0;
            }
          }
        }

        cvResize(rImg, rImgResize);
        cvResize(gImg, gImgResize);
        cvResize(bImg, bImgResize);

        cvShowImage("MyVideo", inputImg); //show the frame in "MyVideo" window
        cvShowImage("RImag", rImgResize);
        cvShowImage("BImag", bImgResize);
        cvShowImage("GImag", gImgResize);

        if(waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
       {
                cout << "esc key is pressed by user" << endl;
                break;
       }
    }

    cvReleaseCapture( &capture );
    cvDestroyWindow( "MyVideo" );
    cvDestroyWindow( "RImag" );
    cvDestroyWindow( "BImag" );
    cvDestroyWindow( "GImag" );

    return 0;
}


