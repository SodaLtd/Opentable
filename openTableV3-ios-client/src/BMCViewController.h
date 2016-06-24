//
//  BMCViewController.h
//  openTable
//
//  Created by Jonathan Jones-morris on 08/05/2016.
//
//

#import <UIKit/UIKit.h>

@interface BMCViewController : UIViewController< UITextFieldDelegate, UITextViewDelegate >

@property (retain, nonatomic) IBOutlet UITextField*     element_title;
@property (retain, nonatomic) IBOutlet UITextView*      element_content;
@property (retain, nonatomic) IBOutlet UIView*          swipe_strip;

-(void)setEditor:(void*)e;

@end
