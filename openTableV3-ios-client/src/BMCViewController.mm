//
//  BMCViewController.m
//  openTable
//
//  Created by Jonathan Jones-morris on 08/05/2016.
//
//

#import "BMCViewController.h"
#import "openTableBusinessModelEditor.h"
#import "ofApp.h"

@interface BMCViewController () {
    openTableBusinessModelEditor* editor;
}

@end

@implementation BMCViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    //
    //
    //
    UIImage* tile = [UIImage imageNamed:@"Background"];
    UIColor* background = [UIColor colorWithPatternImage:tile];
    [self.view setBackgroundColor:background];
    //
    //
    //
    UISwipeGestureRecognizer *swipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipe:)];
    [swipeGesture setDirection:UISwipeGestureRecognizerDirectionUp];
    [_swipe_strip addGestureRecognizer:swipeGesture];
}

-(void)setEditor:(void*)e {
    editor = ( openTableBusinessModelEditor*) e;
    [_element_title setText:editor?[NSString stringWithUTF8String:editor->getTitle().c_str()]:@""];
    [_element_content setText:editor?[NSString stringWithUTF8String:editor->getContent().c_str()]:@""];
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark UITextFieldDelegate

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField {        // return NO to disallow editing.
    return YES;
}
- (void)textFieldDidBeginEditing:(UITextField *)textField {           // became first responder
    
}
- (BOOL)textFieldShouldEndEditing:(UITextField *)textField {          // return YES to allow editing to stop and to resign firstresponder status. NO to disallow the editing session to end
    return YES;
}
- (void)textFieldDidEndEditing:(UITextField *)textField {            // may be called if forced even if shouldEndEditing returns NO (e.g. view removed from window) or endEditing:YES called
    string element_title([[textField text] UTF8String]);
    editor->setTitle(element_title);
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string {   // return NO to not change text
    if(range.length + range.location > textField.text.length) {
        return NO;
    }
    NSUInteger newLength = [textField.text length] + [string length] - range.length;
    return newLength <= 256;
}

- (BOOL)textFieldShouldClear:(UITextField *)textField {               // called when clear button pressed. return NO to ignore (no notifications)
    return YES;
}
- (BOOL)textFieldShouldReturn:(UITextField *)textField {              // called when 'return' key pressed. return NO to ignore.
    string element_title([[textField text] UTF8String]);
    editor->setTitle(element_title);
    [textField resignFirstResponder];
    return NO;
}

#pragma mark - UITextViewDelegate
- (void)textViewDidEndEditing:(UITextView *)textView {
    string element_content([[textView text] UTF8String]);
    editor->setContent(element_content);
}

- (void)textViewDidChange:(UITextView *)textView {
    string element_content([[textView text] UTF8String]);
    editor->setContent(element_content);
}

- (void)handleSwipe:(UISwipeGestureRecognizer *)gesture {
    if ( [gesture direction] == UISwipeGestureRecognizerDirectionUp ) {
        //
        // save
        //
        ofVec2f velocity;
        (( ofApp* ) ofGetAppPtr())->saveCurrentDocument(velocity);
    }
}

@end
