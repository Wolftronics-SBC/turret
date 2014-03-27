#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <windows.h>
#include <stdlib.h>

#include "serialclass.h"	// Library described above
#include <string>

#include <cstring>

#include <atlbase.h>
#include <atlconv.h>


using namespace std;
char key;

int _tmain(int argc, _TCHAR* argv[])
{
	/*cv::Mat img = cv::imread("D:/1.jpg", CV_LOAD_IMAGE_COLOR);

	if(! img.data )                              // Check for invalid input
    {
        return -1;
    }

	cv::namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
    cv::imshow( "Display window", img );                   // Show our image inside it.

    cv::waitKey(0);                                          // Wait for a keystroke in the window
    
	return 0;*/

	cvNamedWindow("Camera_Output", 1);    //Create window
	//cvNamedWindow("Camera_Output2", 2);    //Create window
    CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);  //Capture using any camera connected to your system
    while(1){ //Create infinte loop for live streaming
 
        IplImage* frame = cvQueryFrame(capture); //Create image frames from capture

		cv::Mat mat(frame); 

		cv::Mat binpic = cv::Mat::zeros(mat.rows, mat.cols, CV_32F);//binary pic

		cv::MatIterator_<cv::Vec3b> it, end;
		int i = 0;
		//for (it = mat.begin<cv::Vec3b>(), end = mat.end<cv::Vec3b>(); it != end; ++it) {
		for(int i=0; i < mat.rows; i++) {
			for(int j=0; j< mat.cols; j++) {
				/*mat.at<cv::Vec3b>(i,j)[0];
				mat.at<cv::Vec3b>(i,j)[1];
				mat.at<cv::Vec3b>(i,j)[2];*/

				//binpic.at<uchar>(i,j)[0] = 0;
			//if (((*it)[0] < 50) && ((*it)[1] < 50) && ((*it)[2] < 50)) {//bgr
			//if ((*it)[2] > 240) {//bgr


			/*if (((*it)[0] < 200) && 
				((*it)[0] > 150) && 
				((*it)[1] < 200) && 
				((*it)[1] > 150) && 
				((*it)[2] > 200)) {//bgr*/


			/*if (((*it)[0] > 230) && 
				((*it)[1] > 230) && 
				((*it)[2] > 230)) {*/
			if ((mat.at<cv::Vec3b>(i,j)[0] > 230) && 
				(mat.at<cv::Vec3b>(i,j)[1] > 230) && 
				(mat.at<cv::Vec3b>(i,j)[2] > 230)) {

					//mat.data[i*mat.cols + j] = 
					//binpic.data[i*mat.cols + j] = 0;

					//binpic.at<float>(i % binpic.cols, i / binpic.rows) = 1;
					//binpic.at<float>(i / binpic.rows, i % binpic.cols) = 1;
					//binpic.at<float>(i % binpic.rows, i / binpic.cols) = 1;
				/*(*it)[0] = 255;
				(*it)[1] = 255;
				(*it)[2] = 255;*/

				/*mat.at<cv::Vec3b>(i,j)[0] = 0;
				mat.at<cv::Vec3b>(i,j)[1] = 255;
				mat.at<cv::Vec3b>(i,j)[2] = 0;*/

				/*	mat.at<cv::Vec3b>(i,j)[0] = 0;
				mat.at<cv::Vec3b>(i,j)[1] = 0;
				mat.at<cv::Vec3b>(i,j)[2] = 0;*/
			} else {
				//binpic.at<float>(i / binpic.rows, i % binpic.cols) = 0;
				/*(*it)[0] = 0;
				(*it)[1] = 0;
				(*it)[2] = 0;*/
			}
			i++;
			}
		}


		/*for(int row = 0; row < mat.rows; ++row) {
			uchar* p = mat.ptr(row);
			for(int col = 0; col < mat.cols*3; ++col) {
				if ((*p) % 3 == 2) //b
					*p = 0;
				//if (*p > 100)
					//*p = 0;
				 *p++;  //points to each pixel B,G,R value in turn assuming a CV_8UC3 color image 
			}
		}*/

		/*cv::Mat_<cv::Vec3b> _mat = mat;
		for (int i = 0; i < mat.cols; i++) {
			for (int j = 0; j < mat.rows; j++) {
				if (_mat[i, j])
			}
		}*/


		//


        //cvShowImage("Camera_Output", frame);   //Show image frames on created window
		
		//cvShowImage("Camera_Output", img);   //Show image frames on created window
		cvShowImage("Camera_Output", frame);   //Show image frames on created window

		cv::imshow("myimg", binpic);

		//IplImage* binimg=cvCloneImage(&(IplImage)binpic);
		//cvShowImage("Camera_Output2", binimg);   //Show image frames on created window

        key = cvWaitKey(10);     //Capture Keyboard stroke
        if (char(key) == 27){
            break;      //If you hit ESC key loop will break.
        }
    }
    cvReleaseCapture(&capture); //Release capture.
    cvDestroyWindow("Camera_Output"); //Destroy Window
	//cvDestroyWindow("Camera_Output2"); //Destroy Window
}






Serial::Serial(char *portName)
{
    //We're not yet connected
    this->connected = false;

	LPCTSTR name = ATL::CA2W(portName);

    //Try to connect to the given port throuh CreateFile
    this->hSerial = CreateFile(name,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    //Check if the connection was successfull
    if(this->hSerial==INVALID_HANDLE_VALUE)
    {
        //If not success full display an Error
        if(GetLastError()==ERROR_FILE_NOT_FOUND){

            //Print Error if neccessary
            printf("ERROR: Handle was not attached. Reason: %s not available.\n", portName);

        }
        else
        {
            printf("ERROR!!!");
        }
    }
    else
    {
        //If connected we try to set the comm parameters
        DCB dcbSerialParams = {0};

        //Try to get the current
        if (!GetCommState(this->hSerial, &dcbSerialParams))
        {
            //If impossible, show an error
            printf("failed to get current serial parameters!");
        }
        else
        {
            //Define serial connection parameters for the arduino board
            dcbSerialParams.BaudRate=CBR_9600;
            dcbSerialParams.ByteSize=8;
            dcbSerialParams.StopBits=ONESTOPBIT;
            dcbSerialParams.Parity=NOPARITY;

             //Set the parameters and check for their proper application
             if(!SetCommState(hSerial, &dcbSerialParams))
             {
                printf("ALERT: Could not set Serial Port parameters");
             }
             else
             {
                 //If everything went fine we're connected
                 this->connected = true;
                 //We wait 2s as the arduino board will be reseting
                 Sleep(ARDUINO_WAIT_TIME);
             }
        }
    }

}

Serial::~Serial()
{
    //Check if we are connected before trying to disconnect
    if(this->connected)
    {
        //We're no longer connected
        this->connected = false;
        //Close the serial handler
        CloseHandle(this->hSerial);
    }
}

int Serial::ReadData(char *buffer, unsigned int nbChar)
{
    //Number of bytes we'll have read
    DWORD bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead;

    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(this->hSerial, &this->errors, &this->status);

    //Check if there is something to read
    if(this->status.cbInQue>0)
    {
        //If there is we check if there is enough data to read the required number
        //of characters, if not we'll read only the available characters to prevent
        //locking of the application.
        if(this->status.cbInQue>nbChar)
        {
            toRead = nbChar;
        }
        else
        {
            toRead = this->status.cbInQue;
        }

        //Try to read the require number of chars, and return the number of read bytes on success
        if(ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0)
        {
            return bytesRead;
        }

    }

    //If nothing has been read, or that an error was detected return -1
    return -1;

}


bool Serial::WriteData(char *buffer, unsigned int nbChar)
{
    DWORD bytesSend;

    //Try to write the buffer on the Serial port
    if(!WriteFile(this->hSerial, (void *)buffer, nbChar, &bytesSend, 0))
    {
        //In case it don't work get comm error and return false
        ClearCommError(this->hSerial, &this->errors, &this->status);

        return false;
    }
    else
        return true;
}

bool Serial::IsConnected()
{
    //Simply return the connection status
    return this->connected;
}

/*bool WriteComPort(CStrin gPortSpecifier, CString data) 
{ 
 DCB dcb; 
 DWORD byteswritten; 
 
 HANDLE hPort = CreateFile( 
 
 PortSpecifier, 
 GENERIC_WRITE, 
 0, 
 NULL, 
 OPEN_EXISTING, 
 0, 
 NULL 
 ); 
 
 if (!GetCommState(hPort,&dcb)) 
 return false; 
 
 dcb.BaudRate = CBR_9600; //9600 Baud 
 dcb.ByteSize = 8; //8 data bits 
 dcb.Parity = NOPARITY; //no parity 
 dcb.StopBits = ONESTOPBIT; //1 stop 
 
 if (!SetCommState(hPort,&dcb)) 
 return false; 
 
 bool retVal = WriteFile(hPort,data,1,&byteswritten,NULL); 
 CloseHandle(hPort); //close the handle 
 return retVal; 
} */



// application reads from the specified serial port and reports the collected data
/*int _tmain(int argc, _TCHAR* argv[])
{
	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("\\\\.\\COM10");    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	int dataLength = 256;
	int readResult = 0;


	bool isTesting = true;

	while(SP->IsConnected())
	{
		

		readResult = SP->ReadData(incomingData, dataLength);
		//printf("Bytes read: (-1 means no data available) %i\n",readResult);

		std::string test(incomingData);
		//printf("%s",incomingData);

		if (test != "") {//my
			//printf("Bytes read: (-1 means no data available) %i\n",readResult);
			printf("%s", test);
		}//
		test = "";


		Sleep(500); //20

		if (isTesting) {
			//SP->WriteData("8", 1);
			//Sleep(10);
			//SP->WriteData("15001500", 8);
			SP->WriteData("20482048", 8);
			isTesting = false;
		}
	}

	

	return 0;
}*/