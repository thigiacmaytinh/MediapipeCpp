#pragma once
#include <vector>
#include "IrisLandmark.hpp"

#ifdef _DEBUG
#define SHOW_FPS
#endif

#ifdef SHOW_FPS
#include <chrono>
#endif

#define GetFaceMesh FaceMeshCpp::GetInstance

class FaceMeshObj
{
public:
	enum FaceAngle {
		Straight = 0,
		Left,
		Right,
		Unknown		
	};
	std::vector<cv::Point> landmarks;
	std::vector<cv::Point> leftEyes;
	std::vector<cv::Point> rightEyes;
	FaceAngle faceAngle = FaceAngle::Unknown;
};

class FaceMeshCpp
{
	static FaceMeshCpp* m_instance;
	my::IrisLandmark* irisLandmarker;

	FaceMeshObj::FaceAngle GetFaceAngle(FaceMeshObj facemesh, float threshold=-1);
	double CalcDistance(cv::Point point1, cv::Point point2);

	bool m_inited = false;
public:	

	bool m_enableDetectIris = false;
	float m_threshold = 1.7;

	FaceMeshCpp();
	~FaceMeshCpp();

	void Init(std::string modelsDir);

	static FaceMeshCpp* GetInstance();
	FaceMeshObj Detect(cv::Mat frame, float threshold=-1);
	FaceMeshObj DetectAndDraw(cv::Mat frame, float threshold = -1);
	
};

