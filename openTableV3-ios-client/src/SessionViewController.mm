//
//  SessionViewController.m
//  openTable
//
//  Created by Jonathan Jones-morris on 08/05/2016.
//
//

#import "SessionViewController.h"
#import "openTableSessionEditor.h"
#import "ofApp.h"

NSString* sessionReuseIdentifier = @"Session";

@interface SessionViewController () {
    openTableSessionEditor* editor;
}

@end

@implementation SessionViewController

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
    /*
    [_session_list setDelegate:self];
    [_session_list setDataSource:self];
     */
    //
    //
    //
    UISwipeGestureRecognizer *swipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipe:)];
    [swipeGesture setDirection:UISwipeGestureRecognizerDirectionUp];
    [_swipe_strip addGestureRecognizer:swipeGesture];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)setEditor:(void *)e {
    editor = (openTableSessionEditor*)e;
    if ( editor ) {
        int active_session = editor->getActiveSession();
        if ( active_session >= 0 ) {
            [_session_title setText:[NSString stringWithUTF8String:editor->getSessionName(active_session).c_str()]];
        }
    } else {
        [_session_title setText:@""];
    }
    [_session_list reloadData];
    
}

-(IBAction)newSession:(id)sender {
    if ( editor ) {
        int i = editor->addSession("Untitled");
        editor->setActiveSession(i);
        [_session_title setText:[NSString stringWithUTF8String:editor->getSessionName(i).c_str()]];
        [_session_list reloadData];
    }
}

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    if ( editor ) {
        return editor->sessionCount();
    }
    return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:sessionReuseIdentifier];
    if ( cell == nil ) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:sessionReuseIdentifier];
    }
    [cell setBackgroundColor:[UIColor colorWithRed:253.f/255.f green:185.f/255.f blue:0.f alpha:1]];
    [cell.textLabel setText:[NSString stringWithUTF8String:editor->getSessionName((int)indexPath.row).c_str()]];
    [cell.textLabel setTextColor:[UIColor colorWithWhite:1.f alpha:1.f]];
    if ( editor->getSessionActive((int)indexPath.row) ) {
        [tableView selectRowAtIndexPath:indexPath animated:NO scrollPosition:UITableViewScrollPositionNone];
    }
    return cell;
    
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if ( editor ) {
        editor->setActiveSession((int)indexPath.row);
        [_session_title setText:[NSString stringWithUTF8String:editor->getSessionName((int)indexPath.row).c_str()]];
    }
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        editor->removeSession((int)indexPath.row);
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
        [tableView reloadData];
    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view.
    }
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
    string session_name([[textField text] UTF8String]);
    editor->setSessionName(editor->getActiveSession(),session_name);
    [_session_list reloadData];
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
    string session_name([[textField text] UTF8String]);
    editor->setSessionName(editor->getActiveSession(),session_name);
    [textField resignFirstResponder];
    [_session_list reloadData];
    return NO;
}

@end
