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

	IKinectSensor* pSensor; // KinectV2 ���g�����߂�Sensor�C���^�t�F�[�X
	IBodyFrameSource* pBodySource; // Body�t���[���̂��߂�Source�C���^�t�F�[�X
	IColorFrameSource* pColorSource; // Color�t���[���̂��߂�Source�C���^�t�F�[�X
	IBodyFrameReader* pBodyReader; // Body�t���[���̂��߂�Reader�C���^�t�F�[�X
	IColorFrameReader* pColorReader; // Color�t���[���̂��߂�Reader�C���^�t�F�[�X
	ICoordinateMapper* pCoordinateMapper; // �t���[���Ԃ̈ʒu���킹�̂��߂̃C���^�t�F�[�X

protected:
};