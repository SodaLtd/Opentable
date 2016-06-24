//
//  connectDialog.cpp
//  openTable
//
//  Created by Jonathan Jones-morris on 12/02/2015.
//
//

#include "connectDialog.h"
#include "ofApp.h"

#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

bool connection_dialog_visible = false;

void showConnectDialog() {
    connection_dialog_visible = true;
    UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Connect"
                                                                   message:@"Enter the server IP"
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert addTextFieldWithConfigurationHandler:^(UITextField *textField) {
        textField.placeholder = @"192.168.1.63";
        textField.keyboardType = UIKeyboardTypeDecimalPad;
    }];
    UIAlertAction* connect = [UIAlertAction actionWithTitle:@"Connect" style:UIAlertActionStyleDefault
        handler:^(UIAlertAction * action) {
            connection_dialog_visible = false;
           UITextField* ip_field = [[alert textFields] firstObject];
           NSString* ip_text = [ip_field text];
           string ip_string = ofxiOSNSStringToString(ip_text);
           ofApp* app = ( ofApp* ) ofGetAppPtr();
           app->connect( ip_string );
           [alert dismissViewControllerAnimated:YES completion:nil];
        }];
    UIAlertAction* cancel = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel
        handler:^(UIAlertAction * action) {
            connection_dialog_visible = false;
           [alert dismissViewControllerAnimated:YES completion:nil];
        }];
    
    [alert addAction:cancel];
    [alert addAction:connect];
    
    UIViewController* controller = [ofxiOSGetUIWindow() rootViewController];
    [controller presentViewController:alert animated:YES completion:nil];
}