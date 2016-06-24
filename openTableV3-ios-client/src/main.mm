#include "ofMain.h"
#include "ofApp.h"
#include <Fabric/Fabric.h>
#include <Crashlytics/Crashlytics.h>

int main(){
    [Fabric with:@[[Crashlytics class]]];
    ofSetupOpenGL(1024,768,OF_FULLSCREEN);			// <-------- setup the GL context
    ofAppiOSWindow* window = ( ofAppiOSWindow* ) ofGetWindowPtr();
    if ( window ) {
        window->enableHardwareOrientation();
    }
	ofRunApp(new ofApp());
}
