#include "ofKinectV2.h"

// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

ofKinectV2::ofKinectV2() {

}

void ofKinectV2::setup() {
	width = 1920;
	height = 1080;
	bufferSize = width * height * 4 * sizeof(unsigned char);
	colorImage.allocate(width, height, OF_IMAGE_COLOR_ALPHA);

	HRESULT hResult = S_OK;
	// Sensor
	hResult = GetDefaultKinectSensor(&pSensor);
	if (FAILED(hResult)) {
		cerr << "Error : GetDefaultKinectSensor" << endl;
		return;
	}
	hResult = pSensor->Open();
	if (FAILED(hResult)) {
		cerr << "Error : IKinectSensor::Open()" << endl;
		return;
	}

	// Sorce
	hResult = pSensor->get_BodyFrameSource(&pBodySource);
	if (FAILED(hResult)) {
		cerr << "Error : IKinectSensor::get_BodyFrameSource()" << endl;
		return;
	}
	hResult = pSensor->get_ColorFrameSource(&pColorSource);
	if (FAILED(hResult)) {
		cerr << "Error : IKinectSensor::get_BodyFrameSource()" << endl;
		return;
	}

	// Reader
	hResult = pBodySource->OpenReader(&pBodyReader);
	if (FAILED(hResult)) {
		cerr << "Error : IBodyFrameSource::OpenReader()" << endl;
		return;
	}
	hResult = pColorSource->OpenReader(&pColorReader);
	if (FAILED(hResult)) {
		cerr << "Error : IBodyFrameSource::OpenReader()" << endl;
		return;
	}

	// Coordinate Mapper
	hResult = pSensor->get_CoordinateMapper(&pCoordinateMapper);
	if (FAILED(hResult)) {
		cerr << "Error : IKinectSensor::get_CoordinateMapper()" << endl;
		return;
	}
}

void ofKinectV2::update() {
	if (!pBodyReader || !pColorReader) {
		return;
	}

	HRESULT hResult = S_OK;
	// Color Frame
	IColorFrame* pColorFrame = nullptr;
	hResult = pColorReader->AcquireLatestFrame(&pColorFrame);
	if (SUCCEEDED(hResult)) {
		BYTE* data = colorImage.getPixels();
		hResult = pColorFrame->CopyConvertedFrameDataToArray(bufferSize, data, ColorImageFormat_Rgba); // ofImageにKinectのcolorを代入
		if (SUCCEEDED(hResult)) {
			colorImage.update();
		}
	}

	// Body Frame
	IBodyFrame* pBodyFrame = nullptr;
	hResult = pBodyReader->AcquireLatestFrame(&pBodyFrame);
	if (SUCCEEDED(hResult)) {
		// FrameからBodyを取得
		IBody* pBody[BODY_COUNT] = { 0 };
		hResult = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, pBody);
		if (SUCCEEDED(hResult)) {
			for (int count = 0; count < BODY_COUNT; count++) {
				// トラッキングできているか
				BOOLEAN bTraked = false;
				hResult = pBody[count]->get_IsTracked(&bTraked);
				if (SUCCEEDED(hResult) && bTraked) {
					// 人物からJoint(節)を取得
					Joint joint[JointType::JointType_Count];
					hResult = pBody[count]->GetJoints(JointType::JointType_Count, joint);
					// joint
					for (int type = 0; type < JointType::JointType_Count; type++) {
						// 描画するためにJointのCamera座標系からColor座標系へ位置合わせ
						ColorSpacePoint colorSpacePoint = { 0 };
						pCoordinateMapper->MapCameraPointToColorSpace(joint[type].Position, &colorSpacePoint);
						int x = static_cast<int>(colorSpacePoint.X);
						int y = static_cast<int>(colorSpacePoint.Y);
						if (x >= 0 && x < width && y >= 0 && y < height) {
							jointPos[count][type] = ofVec2f(x, y);
						}
					}
				} else {
					for (int type = 0; type < JOINT_COUNT; type++) {
						jointPos[count][type] = ofVec2f(-100);
					}
				}
			}
		}
		for (int count = 0; count < BODY_COUNT; count++) {
			SafeRelease(pBody[count]);
		}
	}
	SafeRelease(pColorFrame);
	SafeRelease(pBodyFrame);
}

void ofKinectV2::drawColor() {
	ofSetColor(255);
	colorImage.draw(0, 0, width, height);
}

void ofKinectV2::drawJoint() {
	ofSetColor(0, 255, 0);
	for (int count = 0; count < BODY_COUNT; count++) {
		for (int type = 0; type < JointType::JointType_Count; type++) {
			ofCircle(jointPos[count][type], 10);
		}
	}
}

ofKinectV2::~ofKinectV2() {

}
