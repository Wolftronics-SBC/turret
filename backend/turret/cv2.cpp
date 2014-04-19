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

#include <math.h>

//#define DEBUG
#define RELEASE
//#define MOUSE_DEBUG

using namespace std;
char waitkey;
Serial* mySP;

std::string fixedLength(int value, int digits = 3) {
    unsigned int uvalue = value;
    if (value < 0) {
        uvalue = -uvalue;
    }
    std::string result;
    while (digits-- > 0) {
        result += ('0' + uvalue % 10);
        uvalue /= 10;
    }
    if (value < 0) {
        result += '-';
    }
    std::reverse(result.begin(), result.end());
    return result;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
	if (event == cv::EVENT_LBUTTONDOWN) {		
		int x1 = x - 320;
		int y1 = y - 240;
		double k = 745;
		int anglex = 2048 - x1 * k / 640;
		std::string str = fixedLength(anglex, 4) + "2048";
		char * writable = new char[str.size() + 1];
		std::copy(str.begin(), str.end(), writable);
		writable[str.size()] = '\0';
		printf("(x, y) = (%d, %d)\tanglex = %d res = %s\n", x1, y1, anglex, writable);
		mySP->WriteData(writable, 8);
		delete[] writable;
		Sleep(30);		
	}
}

int _tmain(int argc, _TCHAR* argv[]) {	
#ifdef RELEASE
printf("Welcome to the turret app!\n");
	mySP = new Serial("\\\\.\\COM10");
	if (mySP->IsConnected()) {
		printf("We're connected\n");
	}
	char incomingData[256] = "";			
	int dataLength = 256;
	int readResult = 0;
	bool isTesting = true;
	cvNamedWindow("Camera_Output", 1);
    CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	while (mySP->IsConnected()) {
		IplImage* frame = cvQueryFrame(capture);
		cv::Mat mat(frame); 
		cv::Mat binpic = cv::Mat::zeros(mat.rows, mat.cols, CV_32F);
		int x = 0;
		int y = 0;
		for(int i = 0; i < mat.rows; i++) {
			for(int j = 0; j < mat.cols; j++) {
				if ((mat.at<cv::Vec3b>(i,j)[0] > 150) && (mat.at<cv::Vec3b>(i,j)[0] < 250) &&
					(mat.at<cv::Vec3b>(i,j)[1] > 170) && (mat.at<cv::Vec3b>(i,j)[1] < 250) && 
					(mat.at<cv::Vec3b>(i,j)[2] > 218) && (mat.at<cv::Vec3b>(i,j)[2] <= 255)) {
						binpic.at<float>(i,j) = 255;
						x = j - mat.cols / 2;
						y = i - mat.rows / 2;
				}
			} 
		}
		double k = 745;
		int anglex = 2048 - x * k / 640;
		cvShowImage("Camera_Output", frame);   
		cv::imshow("myimg", binpic);
        waitkey = cvWaitKey(10);     
        if (char(waitkey) == 27){
            break;      
        }
		if ((x != 0) && (y != 0)) {
			std::string str = fixedLength(anglex, 4) + "2048";
			char * writable = new char[str.size() + 1];
			std::copy(str.begin(), str.end(), writable);
			writable[str.size()] = '\0';
			printf("(x, y) = (%d, %d)\tanglex = %d res = %s\n", x, y, anglex - 2048, writable);
			mySP->WriteData(writable, 8);
			delete[] writable;
			Sleep(30);	
		}
	}
	cvReleaseCapture(&capture); 
    cvDestroyWindow("Camera_Output");
#endif
#ifdef DEBUG
	cvNamedWindow("Camera_Output", 1);    //Create window
    CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);  //Capture using any camera connected to your system

	///////test cv begin
	while(1) {
		IplImage* frame = cvQueryFrame(capture); //Create image frames from capture
		cv::Mat mat(frame); 
		cv::Mat binpic = cv::Mat::zeros(mat.rows, mat.cols, CV_32F);//binary pic
		int x = 0;
		int y = 0;
		for(int i = 0; i < mat.rows; i++) {
			for(int j = 0; j < mat.cols; j++) {
				if ((mat.at<cv::Vec3b>(i,j)[0] > 150) && (mat.at<cv::Vec3b>(i,j)[0] < 250) &&
					(mat.at<cv::Vec3b>(i,j)[1] > 170) && (mat.at<cv::Vec3b>(i,j)[1] < 250) && 
					(mat.at<cv::Vec3b>(i,j)[2] > 218) && (mat.at<cv::Vec3b>(i,j)[2] <= 255)) {
						binpic.at<float>(i,j) = 255;
						x = j - mat.cols / 2;
						y = i - mat.rows / 2;
				}
			} 
		}
		double k = 745;
		int anglex = 2048 - x * k / 640;
		printf("x:\t%d\ta:\t%d\ta:\t%d\n", x, anglex, anglex - 2048);
		cvShowImage("Camera_Output", frame);   //Show image frames on created window
		cv::imshow("myimg", binpic);

		key = cvWaitKey(10);     //Capture Keyboard stroke
        if (char(key) == 27){
            break;      //If you hit ESC key loop will break.
        }
	}

	cvReleaseCapture(&capture); //Release capture.
    cvDestroyWindow("Camera_Output"); //Destroy Window
	///////test cv end
#endif
#ifdef MOUSE_DEBUG
	printf("Welcome to the turret app!\n");
	mySP = new Serial("\\\\.\\COM10");
	if (mySP->IsConnected()) {
		printf("We're connected\n");
	}
	char incomingData[256] = "";			
	int dataLength = 256;
	int readResult = 0;
	cvNamedWindow("Camera_Output", 1);    
    CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);  
	cv::setMouseCallback("Camera_Output", CallBackFunc, NULL);
	while(mySP->IsConnected()) {
		readResult = mySP->ReadData(incomingData, dataLength);
		std::string test(incomingData);
		if (-1 != readResult) {
			printf("read: %s\n", test);
		}
		test = "";
		Sleep(150);
		IplImage* frame = cvQueryFrame(capture); 
		cvShowImage("Camera_Output", frame);  
		waitkey = cvWaitKey(10);     
        if (char(waitkey) == 27){
            break;      
        }
	}
	cvReleaseCapture(&capture); 
    cvDestroyWindow("Camera_Output"); 
#endif
	return 0;
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