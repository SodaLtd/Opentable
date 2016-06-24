//
//  ImageViewController.h
//  openTable
//
//  Created by Jonathan Jones-morris on 07/05/2016.
//
//

#import <UIKit/UIKit.h>

@interface ImageViewController : UIViewController< UICollectionViewDataSource, UICollectionViewDelegate, UITextFieldDelegate, UIGestureRecognizerDelegate >

@property (retain, nonatomic) IBOutlet UITextField*         caption;
@property (retain, nonatomic) IBOutlet UICollectionView*    collection;
@property (retain, nonatomic) IBOutlet UICollectionView*    library;
@property (retain, nonatomic) IBOutlet UIView*              swipe_strip;

-(void) setEditor:(void*) e;

@end
