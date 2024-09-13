#ifndef LIB_CS
#include "IrisLandmark.hpp"

#include <iostream>

#include <opencv2/highgui.hpp>
#include "FaceMeshCpp.h"


#ifdef SHOW_FPS
    #include <chrono>
#endif


int main(int argc, char* argv[]) {

    //my::IrisLandmark irisLandmarker("./models");
	//my::IrisLandmark* irisLandmarker = new my::IrisLandmark("./models");
    cv::VideoCapture cap(0);

    bool success = cap.isOpened();
    if (success == false)
    {
        std::cerr << "Cannot open the camera." << std::endl;
        return 1;
    }

	GetFaceMesh()->Init("./models");

    while (success)
    {
        cv::Mat frame;
        success = cap.read(frame); // read a new frame from video

        if (success == false)
            break;
        
        //cv::flip(rframe, rframe, 1);

		GetFaceMesh()->DetectAndDraw(frame);

        cv::imshow("Facemesh detector", frame);

        if (cv::waitKey(10) == 27)
            break;
    }

    //#if SHOW_FPS
    //    std::cout << "Average inference time: " << sum / count << "ms " << std::endl;
    //#endif

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
#endif