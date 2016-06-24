#include "ofxNativeMultitouch.h"

#ifdef TARGET_WIN32

static LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	ofxNativeMultitouch* multi_touch = ( ofxNativeMultitouch* ) GetWindowLongPtr( hwnd, GWL_USERDATA );
	if ( multi_touch ) {
		return multi_touch->processMessage( msg, wparam, lparam );
	}
	return DefWindowProc( hwnd, msg, wparam, lparam );
}

#endif

ofxNativeMultitouch::ofxNativeMultitouch() {
	m_setup = false;
}
ofxNativeMultitouch::~ofxNativeMultitouch() {

}
//
//
//
bool ofxNativeMultitouch::setup(ofxNativeMultitouchTarget* target) {
	if ( m_setup ) return false;
#ifdef TARGET_WIN32
	//
	// test for touch device		
	// 
	int value = GetSystemMetrics(SM_DIGITIZER);
	if ( value != 0 ) {
		if (value & NID_READY){ /* stack ready */}
		if (value & NID_MULTI_INPUT){ /* digitizer is multitouch */ }
		if (value & NID_INTEGRATED_TOUCH){ /* Integrated touch */}

		m_hwnd					= ofGetWin32Window();
		m_default_widow_proc	= (WNDPROC)GetWindowLongPtr(m_hwnd, GWL_WNDPROC);
		SetWindowLongPtr(m_hwnd, GWL_USERDATA, (long)this);
		SetWindowLongPtr(m_hwnd, GWL_WNDPROC, (long)winProc);
		if ( RegisterTouchWindow(m_hwnd, TWF_WANTPALM ) ) {
			printf( "ofxNativeMultitouch::setup : multitouch support enabled\n" );
			m_target = target;
			m_setup = true;
		}
	} else {
		m_setup = false;
	}
#endif
	return m_setup;
}
void ofxNativeMultitouch::close() {
	if ( m_setup ) {
#ifdef TARGET_WIN32
		UnregisterTouchWindow( m_hwnd );
		SetWindowLongPtr(m_hwnd, GWL_WNDPROC, (long)m_default_widow_proc);
		SetWindowLongPtr(m_hwnd, GWL_USERDATA, 0);
		m_setup = false;
#endif
	}
}
#ifdef TARGET_WIN32
LRESULT ofxNativeMultitouch::processMessage( UINT msg, WPARAM wparam, LPARAM lparam ) {
    switch(msg){
        case WM_GESTURE:
            //return gestureEnginePtr->WndProc(hwnd, wParam, lParam);
            break;
        case WM_TOUCH:
            return processTouch(wparam, lparam);
            break;
        default:
            return CallWindowProc(m_default_widow_proc, m_hwnd, msg, wparam, lparam);
            break;
    }
    return 0;
}

LRESULT ofxNativeMultitouch::processTouch(WPARAM wparam, LPARAM lparam) {
	BOOL bHandled = FALSE;
	UINT cInputs = LOWORD(wparam);
	PTOUCHINPUT pInputs = new TOUCHINPUT[cInputs];
	if ( pInputs ) {
		if (GetTouchInputInfo((HTOUCHINPUT)lparam, cInputs, pInputs, sizeof(TOUCHINPUT))) {
			for (UINT i=0; i < cInputs; i++ ) {
				TOUCHINPUT ti = pInputs[i];
				//do something with each touch input entry
				if (ti.dwFlags & TOUCHEVENTF_DOWN) {
					onTouchDown(ti.dwID, TOUCH_COORD_TO_PIXEL(ti.x)-ofGetWindowPositionX(), TOUCH_COORD_TO_PIXEL(ti.y)-ofGetWindowPositionY());
				} else if (ti.dwFlags & TOUCHEVENTF_MOVE) {
					onTouchMove(ti.dwID, TOUCH_COORD_TO_PIXEL(ti.x)-ofGetWindowPositionX(), TOUCH_COORD_TO_PIXEL(ti.y)-ofGetWindowPositionY());
				} else if (ti.dwFlags & TOUCHEVENTF_UP) {
					onTouchUp(ti.dwID, TOUCH_COORD_TO_PIXEL(ti.x)-ofGetWindowPositionX(), TOUCH_COORD_TO_PIXEL(ti.y)-ofGetWindowPositionY());
				}
			}
			bHandled = TRUE;
		} else {
			/* handle the error here */
		}
		delete [] pInputs;
	} else {
		/* handle the error here, probably out of memory */
	}
	if ( bHandled ) {
		// if you handled the message, close the touch input handle and return
		CloseTouchInputHandle((HTOUCHINPUT)lparam);
		return 0;
	} else {
		// if you didn't handle the message, let DefWindowProc handle it
		return DefWindowProc(m_hwnd, WM_TOUCH, wparam, lparam);
	}
}
#endif

void ofxNativeMultitouch::onTouchDown(int id, int x, int y) {
	if ( m_target ) m_target->touchDown( x, y, id );
	static ofTouchEventArgs touch;
    touch.x = x;
    touch.y = y;
    touch.id = id;
	touch.type	= ofTouchEventArgs::Type::down;
    ofNotifyEvent( ofEvents().touchDown, touch );
}

void ofxNativeMultitouch::onTouchMove(int id, int x, int y) {
	if ( m_target ) m_target->touchMove( x, y, id );
	static ofTouchEventArgs touch;  
	touch.x = x;
	touch.y = y;
	touch.id = id;
	touch.type	= ofTouchEventArgs::Type::move;
	ofNotifyEvent( ofEvents().touchMoved, touch );
}

void ofxNativeMultitouch::onTouchUp(int id, int x, int y) {
	if ( m_target ) m_target->touchUp( x, y, id );
	static ofTouchEventArgs touch;
	touch.x = x;
	touch.y = y;
	touch.id = id;
	touch.type	= ofTouchEventArgs::Type::up;
	ofNotifyEvent( ofEvents().touchUp, touch );
}
