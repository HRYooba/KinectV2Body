#pragma once

#include "ofMain.h"
#include "Kinect.h"

class ofKinectV2 {

public:
	ofKinectV2();
	virtual ~ofKinectV2();
	void setup();
	void update();
	void drawColor();
	void drawJoint();
	ofVec2f getJointPos(int count, int type);
private:
	int width;
	int height;
	int bufferSize;

	ofImage colorImage;
	ofVec2f jointPos[BODY_COUNT][JointType::JointType_Count];

	IKinectSensor* pSensor; // KinectV2 を使うためのSensorインタフェース
	IBodyFrameSource* pBodySource; // BodyフレームのためのSourceインタフェース
	IColorFrameSource* pColorSource; // ColorフレームのためのSourceインタフェース
	IBodyFrameReader* pBodyReader; // BodyフレームのためのReaderインタフェース
	IColorFrameReader* pColorReader; // ColorフレームのためのReaderインタフェース
	ICoordinateMapper* pCoordinateMapper; // フレーム間の位置合わせのためのインタフェース

protected:
};