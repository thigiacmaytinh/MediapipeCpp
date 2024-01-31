#include "FaceMeshCpp.h"
#include "IrisLandmark.hpp"
#include <math.h>
#include <iostream>

#define BLACK cv::Scalar(0,0,0)
#define BLUE cv::Scalar(255,0,0)
#define CYAN cv::Scalar(255,255,0)
#define GREY cv::Scalar(128,128,128)
#define GREEN cv::Scalar(0,255,0)
#define ORANGE cv::Scalar(0,165,255)
#define PINK cv::Scalar(147,20,255)
#define PURPLE cv::Scalar(255,0,255)
#define RED cv::Scalar(0,0,255)
#define WHITE cv::Scalar(255,255,255)
#define YELLOW cv::Scalar(0,255,255)

FaceMeshCpp* FaceMeshCpp::m_instance = nullptr;

///////////////////////////////////////////////////////////////////////////////////////////////////

FaceMeshCpp::FaceMeshCpp()
{
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////

FaceMeshCpp::~FaceMeshCpp()
{
	irisLandmarker = nullptr;
	delete irisLandmarker;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

FaceMeshCpp* FaceMeshCpp::GetInstance()
{
	if (!m_instance)
		m_instance = new FaceMeshCpp();
	return m_instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void FaceMeshCpp::Init(std::string modelsDir)
{
	irisLandmarker = new my::IrisLandmark(modelsDir);
	m_inited = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

FaceMeshObj FaceMeshCpp::Detect(cv::Mat frame)
{
	FaceMeshObj facemesh;

	if (irisLandmarker == nullptr || !m_inited)
		return facemesh;

	irisLandmarker->loadImageToInput(frame);
	irisLandmarker->runInference();
	
	facemesh.landmarks = irisLandmarker->getAllFaceLandmarks();
	if (m_enableDetectIris)
	{
		facemesh.leftEyes = irisLandmarker->getAllFaceLandmarks();
		facemesh.rightEyes = irisLandmarker->getAllFaceLandmarks();
	}
	
	facemesh.faceAngle = GetFaceAngle(facemesh);
	return facemesh;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

FaceMeshObj FaceMeshCpp::DetectAndDraw(cv::Mat frame)
{
	FaceMeshObj facemesh = Detect(frame);

	cv::Scalar color = facemesh.faceAngle == FaceMeshObj::FaceAngle::Straight ? GREEN : RED;

	for (auto point : facemesh.landmarks) {		
		cv::circle(frame, point, 2, color, -1);
	}

	for (auto point : facemesh.leftEyes) {
		cv::circle(frame, point, 2, color, -1);
	}

	for (auto point : facemesh.rightEyes) {
		cv::circle(frame, point, 2, color, -1);
	}

#if SHOW_FPS
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	float inferenceTime = duration.count() / 1e3;
	sum += inferenceTime;
	count += 1;
	int fps = (int) 1e3 / inferenceTime;

	cv::putText(frame, std::to_string(fps), cv::Point(20, 70), cv::FONT_HERSHEY_PLAIN, 3, cv::Scalar(0, 196, 255), 2);
#endif

	return facemesh;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

double FaceMeshCpp::CalcDistance(cv::Point point1, cv::Point point2)
{
	return sqrt(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2));	
}
	
///////////////////////////////////////////////////////////////////////////////////////////////////

FaceMeshObj::FaceAngle FaceMeshCpp::GetFaceAngle(FaceMeshObj facemesh)
{
	if (facemesh.landmarks.size() == 0)
		return FaceMeshObj::FaceAngle::Unknown;

	double distanceLeft = CalcDistance(facemesh.landmarks[5], facemesh.landmarks[234]);
	double distanceRight = CalcDistance(facemesh.landmarks[5], facemesh.landmarks[454]);


	FaceMeshObj::FaceAngle result = FaceMeshObj::FaceAngle::Straight;

	if (distanceLeft < distanceRight)
	{
		double ratio = distanceRight / distanceLeft;
		if (ratio > m_threshold)
			result = FaceMeshObj::FaceAngle::Left;
	}
	else if (distanceRight < distanceLeft)
	{
		double ratio = distanceLeft / distanceRight;
		if (ratio > m_threshold)
			result = FaceMeshObj::FaceAngle::Right;
	}	


	return result;
}