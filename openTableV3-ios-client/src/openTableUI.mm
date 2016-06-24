//
//  openTableUI.cpp
//  openTable
//
//  Created by Jonathan Jones-morris on 09/11/2014.
//
//

#include "openTableUI.h"
#include "ofApp.h"

#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

id m_gesture_processor;

@interface GestureProcessor : NSObject<UIGestureRecognizerDelegate> {
    UIPanGestureRecognizer*    _recogniser;
}

-(id)init;
-(void)handleGesture:(UIGestureRecognizer *)gestureRecognizer;

@end

@implementation GestureProcessor

-(id)init {
    self = [super init];
    if ( self ) {
        _recogniser = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handleGesture:)];
        [_recogniser setCancelsTouchesInView:NO];
        UIView * view = ofxiOSGetGLParentView();
        [view addGestureRecognizer:_recogniser];
    }
    return self;
}

-(void)handleGesture:(UIGestureRecognizer *)gestureRecognizer {
    if ( [gestureRecognizer state] == UIGestureRecognizerStateEnded ) {
        UIView * view = ofxiOSGetGLParentView();
        CGPoint v = [_recogniser velocityInView:view];
        ofApp* app = ( ofApp* ) ofGetAppPtr();
        //
        // TODO: allow for OF orientation
        //
        app->panGesture(ofVec2f( v.x, v.y ));
    }
}

-(void) dealloc {
    UIView * view = ofxiOSGetGLParentView();
    [view removeGestureRecognizer:_recogniser];
    [super dealloc];
}

@end

openTableUI openTableUI::defaultUI;

openTableUI::openTableUI() {
    m_setup = false;
}
openTableUI::~openTableUI() {
    
}
//
//
//
void openTableUI::setup() {
    //
    // setup UI
    //
    m_ui_root               = new mui::Root();
    //
    // main menu
    //
    /*
    vector<mui::ImageButton*> menu_buttons(3);
    const string icons[] = { "ui/Settings.png", "ui/NewPhotoCollection.png", "ui/NewBusinessModel.png" };
    float menu_top = 8;
    for ( int i = 0; i < 3; i++ ) {
        menu_buttons[ i ] = new mui::ImageButton(icons[i], 8, menu_top, 64, 64 );
        m_ui_root->add(menu_buttons[ i ]);
        switch( i ) {
            case 0:
                menu_buttons[ i ]->onPress += Poco::Delegate<openTableUI, ofTouchEventArgs>( this, &openTableUI::openSettings );
                break;
            case 1:
                menu_buttons[ i ]->onPress += Poco::Delegate<openTableUI, ofTouchEventArgs>( this, &openTableUI::newPhotoCollection );
                break;
            case 2:
                menu_buttons[ i ]->onPress += Poco::Delegate<openTableUI, ofTouchEventArgs>( this, &openTableUI::newBusinessModel );
                break;
                
        }
    }
    mui::ImageButton* menu_button = new mui::ImageButton("ui/Menu.png", 8, menu_top, 64, 64 );
    m_ui_root->add(menu_button);
    m_ui_main_menu.setup("main_menu", m_ui_root, menu_button, menu_buttons);
    */
    mui::ImageButton* new_collection = new mui::ImageButton( "ui/NewPhotoCollection.png", ofGetWidth() - 64, 0, 64, 64 );
    new_collection->onPress += Poco::Delegate<openTableUI, ofTouchEventArgs>( this, &openTableUI::onNewCollection );
    m_ui_root->add(new_collection);
    //
    //
    //
    
    //m_gesture_processor = [[GestureProcessor alloc] init];
    m_setup = true;
}
//
//
//
void openTableUI::update() {
    m_ui_root->handleUpdate();
}
void openTableUI::draw() {
    m_ui_root->handleDraw();
}
//
//
//
void openTableUI::openSettings(const void* sender, ofTouchEventArgs &args ) {
    m_ui_main_menu.close();
}
void openTableUI::newPhotoCollection(const void* sender, ofTouchEventArgs &args ) {
    
    m_ui_main_menu.close();
}
void openTableUI::newBusinessModel(const void* sender, ofTouchEventArgs &args ) {
    m_ui_main_menu.close();
}
//
//
//
void openTableUI::onNewCollection( const void* sender, ofTouchEventArgs &args ) {
    ofApp* app = ( ofApp* ) ofGetAppPtr();
    app->newDocument( "image" );
}
