#ifndef LIB_CS
#include "IrisLandmark.hpp"

#include <iostream>

#include <opencv2/highgui.hpp>
#include "FaceMeshCpp.h"


#if SHOW_FPS
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

#if SHOW_FPS
    float sum = 0;
    int count = 0;
#endif

	GetFaceMesh()->Init("./models");

    while (success)
    {
        cv::Mat rframe, frame;
        success = cap.read(rframe); // read a new frame from video

        if (success == false)
            break;
        
        //cv::flip(rframe, rframe, 1);

        #if SHOW_FPS
            auto start = std::chrono::high_resolution_clock::now();
        #endif

		GetFaceMesh()->DetectAndDraw(rframe);

        cv::imshow("Face detector", rframe);

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