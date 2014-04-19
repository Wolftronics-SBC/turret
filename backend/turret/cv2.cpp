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

using namespace std;
char key;

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

char* myitoa(int i, char b[], int l){
    char const digit[] = "0123456789";
    char* p = b;
	
	for (int j = 0; j < l; j++) {
		++p;
	}
	
    *p = '\0';
	for (int j = 0; j < l; j++) {
		if (i % 10 != 0) {
			*--p = digit[i % 10];
			i = i / 10;	
		} else {
			*--p = digit[0];
		}
	}

    return b;
}

// application reads from the specified serial port and reports the collected data
int _tmain(int argc, _TCHAR* argv[])
{	
#ifndef DEBUG

	long int debug_delay = 100;

	//Arduino

	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("\\\\.\\COM10");    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	int dataLength = 256;
	int readResult = 0;


	bool isTesting = true;

	//CV
	cvNamedWindow("Camera_Output", 1);    //Create window
    CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);  //Capture using any camera connected to your system

	while(SP->IsConnected())
	{
		//debug
		//debug_delay--;


		//Arduino

		readResult = SP->ReadData(incomingData, dataLength);
		//printf("Bytes read: (-1 means no data available) %i\n",readResult);
		std::string test(incomingData);
		//printf("%s",incomingData);
		if (test != "") {//my
			//printf("Bytes read: (-1 means no data available) %i\n",readResult);
			
			//printf("%s", test);//
		}//
		test = "";
		//Sleep(20); //20 or 500

		//CV
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

		//int anglex = x * 68.27 / 640;
		//int anglex = 2048 + x * 68.27 / 320;
		//double k = 68.27;
		//double k = 200;
		//double k = 455.11;
		double k = 745;
		//double k = 455.11;
		//double k = 60;
		//int anglex = 2048 - x * k / 640;
		//anglex = anglex - x * k / 640;
		int anglex = 2048 - x * k / 640;
		/*double anglexd = 2048 - x * k / 640;
		int anglex = anglexd;*/
		//int anglex = x / 2 * 68.27 / 640;

		

		cvShowImage("Camera_Output", frame);   //Show image frames on created window
		cv::imshow("myimg", binpic);

        key = cvWaitKey(10);     //Capture Keyboard stroke
        if (char(key) == 27){
            break;      //If you hit ESC key loop will break.
        }
		
		//if (debug_delay == 0) {
			//debug_delay = 100;

			if (isTesting) {
				//Sleep(10);
				//SP->WriteData("20482048", 8);
				//SP->WriteData("20482048", 8);
				isTesting = false;
			} else {
				if ((x != 0) && (y != 0)) {
					char str[4];
					char result[8];
					//myitoa(anglex, str, 4);			
					myitoa(anglex, str, 4);			
					strcpy(result, str); 
					strcat(result, "2048");

					//printf("str%sstr", str);
					//printf("result%sresult", result);
					SP->WriteData(result, 8);
			
					//printf("%s", result);
					char *two = result;
					//printf("%s \n", two);
					printf("x:\t%d\ta:\t%d\tresult:\t%s\n", x, anglex - 2048, two);
					//SP->WriteData(two, 8);

					//Sleep(200);
					//Sleep(20);
					Sleep(30);
					//Sleep(300);
				}
			}

		//}

	}
	cvReleaseCapture(&capture); //Release capture.
    cvDestroyWindow("Camera_Output"); //Destroy Window
	//cvDestroyWindow("Camera_Output2"); //Destroy Window
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