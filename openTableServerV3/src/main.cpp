#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
#ifdef _DEBUG
	ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context
#else
	ofSetupOpenGL(1920, 1080, OF_WINDOW);			// <-------- setup the GL context
#ifdef TARGET_WIN32
	ofSetWindowPosition(0, -16);
	HWND hwnd = ofGetWin32Window();
	SetWindowLong(hwnd,GWL_STYLE, WS_POPUP);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 1920, 1080, SWP_NOSIZE);
#endif
#endif

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
