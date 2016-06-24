//
//  SessionViewController.h
//  openTable
//
//  Created by Jonathan Jones-morris on 08/05/2016.
//
//

#import <UIKit/UIKit.h>

@interface SessionViewController : UIViewController< UITextViewDelegate, UITableViewDelegate, UITableViewDataSource >

@property (retain, nonatomic) IBOutlet UITextField*     session_title;
@property (retain, nonatomic) IBOutlet UITableView*     session_list;
@property (retain, nonatomic) IBOutlet UIView*          swipe_strip;

-(void)setEditor:(void*)e;
-(IBAction)newSession:(id)sender;

@end
