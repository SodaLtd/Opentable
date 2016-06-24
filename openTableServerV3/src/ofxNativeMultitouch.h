/*
	Created by Jonathan Jones-Morris 08/05/2015
*/
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#include "ofMain.h"

#ifndef LONG
#define LONG long
#endif

class ofxNativeMultitouchTarget {
public:
    virtual void twoFingerTap(){}
    virtual void twoFingerZoom(double dZoomFactor,const LONG lZx,const LONG lZy) {}

	virtual void touchDown(int x, int y, int id) {};
	virtual void touchMove(int x, int y, int id) {};
	virtual void touchUp(int x, int y, int id) {};
};

class ofxNativeMultitouch {
public:
	ofxNativeMultitouch();
	~ofxNativeMultitouch();
	//
	//
	//
	bool setup( ofxNativeMultitouchTarget* target = 0);
	void close();
	bool isSetup() { return m_setup; };
	//
	//
	//
#ifdef TARGET_WIN32
	LRESULT processMessage( UINT msg, WPARAM wparam, LPARAM lparam );
	LRESULT processTouch(WPARAM wparam, LPARAM lparam);
#endif
	//
	//
	//
	void onTouchDown(int id, int x, int y);
	void onTouchMove(int id, int x, int y);
	void onTouchUp(int id, int x, int y);
protected:
#ifdef TARGET_WIN32
	HWND	m_hwnd;
	WNDPROC m_default_widow_proc;
#endif
	ofxNativeMultitouchTarget*	m_target;
	bool						m_setup;
};