#pragma once

#include "ofMain.h"
#include "Kinect.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
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
};

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