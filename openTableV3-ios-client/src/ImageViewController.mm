//
//  ImageViewController.m
//  openTable
//
//  Created by Jonathan Jones-morris on 07/05/2016.
//
//

#import "ImageViewController.h"
#import "ImageCollectionViewCell.h"
#import "ofApp.h"
#import "openTableImageEditor.h"

NSString* reuseIdentifier = @"OpenTableCell";

@interface ImageViewController () {

    openTableImageEditor* editor;
    //
    // drag and drop
    //
    int selected_source;
    int selected_index;
}
@property (retain, nonatomic) IBOutlet UIImageView* dragged_image;

@end

@implementation ImageViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    //
    // background tile
    //
    UIImage* tile = [UIImage imageNamed:@"Background"];
    UIColor* background = [UIColor colorWithPatternImage:tile];
    [self.view setBackgroundColor:background];
    //
    //
    //
    [_collection registerClass:[ImageCollectionViewCell class] forCellWithReuseIdentifier:reuseIdentifier];
    [_library registerClass:[ImageCollectionViewCell class] forCellWithReuseIdentifier:reuseIdentifier];
    //
    // Drag and drop
    //
    selected_source = selected_index = -1;
    UILongPressGestureRecognizer* longPressGesture = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(libraryPress:)];
    longPressGesture.numberOfTouchesRequired = 1;
    longPressGesture.minimumPressDuration    = 0.1f;
    [_library addGestureRecognizer:longPressGesture];
    [longPressGesture release];
    
    longPressGesture = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(collectionPress:)];
    longPressGesture.numberOfTouchesRequired = 1;
    longPressGesture.minimumPressDuration    = 0.1f;
    [_collection addGestureRecognizer:longPressGesture];
    [longPressGesture release];
    
    UIPanGestureRecognizer *panGesture =
    [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
    [panGesture setDelegate:self];
    [self.view addGestureRecognizer:panGesture];
    [panGesture release];
    
    _dragged_image = [[UIImageView alloc] initWithFrame:CGRectMake(0,0,120,120)];
    _dragged_image.hidden = YES;
    [_dragged_image setContentMode:UIViewContentModeScaleAspectFit];
    [self.view addSubview:_dragged_image];
    
    UISwipeGestureRecognizer *swipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipe:)];
    [swipeGesture setDirection:UISwipeGestureRecognizerDirectionUp];
    [_swipe_strip addGestureRecognizer:swipeGesture];
    [swipeGesture release];
}
-(void) dealloc {
    [super dealloc];
    [_dragged_image release];
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void) setEditor:(void*) e {
    editor = (openTableImageEditor*)e;
    [_collection reloadData];
    [_library reloadData];
    [_caption setText:editor?[NSString stringWithUTF8String:editor->getAnnotation().c_str()]:@""];
}
#pragma mark UICollectionViewDataSource

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    if ( editor ) {
        if ( collectionView == self.collection) {
            return editor->imagesInCollection();
        } else {
            return editor->imagesInLibrary();
        }
    }
    return 0;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    string path;
    if ( collectionView == self.collection ) {
        path = editor->pathForCollectionImage(indexPath.row);
    } else {
        path = editor->pathForLibraryImage(indexPath.row);
    }
    
    ImageCollectionViewCell* cell = [collectionView dequeueReusableCellWithReuseIdentifier:reuseIdentifier forIndexPath:indexPath];
    NSString* ns_path = [NSString stringWithUTF8String: path.c_str()];
    UIImage* image = [UIImage imageWithContentsOfFile:ns_path];
    [cell.imageView setImage:image];
    //[image release];
    cell.alpha = 1.f;
    return cell;
    
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
    string annotation([[textField text] UTF8String]);
    editor->setAnnotation(annotation);    
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
    string annotation([[textField text] UTF8String]);
    editor->setAnnotation(annotation);
    [textField resignFirstResponder];
    return NO;
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/
#pragma mark - Drag and Drop
- (void)libraryPress:(UILongPressGestureRecognizer *)gesture {
    CGPoint point = [gesture locationInView:_library];
    if (gesture.state == UIGestureRecognizerStateBegan) {
        NSIndexPath *indexPath = [_library indexPathForItemAtPoint:point];
        if (indexPath != nil) {
            selected_source = 0;
            selected_index = indexPath.row;
            
            // calculate point in parent view
            point = [gesture locationInView:self.view];
            
            
            // hide the cell
            // hide the cell
            ImageCollectionViewCell* cell = ( ImageCollectionViewCell* )[_library cellForItemAtIndexPath:indexPath];
            if ( cell ) {
                cell.alpha = 0.0f;
                UIImage* image = cell.imageView.image;
                [_dragged_image setImage:image];
                _dragged_image.center = point;
                _dragged_image.hidden = NO;
            }
        }
    }
}
- (void)collectionPress:(UILongPressGestureRecognizer *)gesture {
    CGPoint point = [gesture locationInView:_collection];
    if (gesture.state == UIGestureRecognizerStateBegan) {
        NSIndexPath *indexPath = [_collection indexPathForItemAtPoint:point];
        if (indexPath != nil) {
            selected_source = 1;
            selected_index = indexPath.row;
            
            // calculate point in parent view
            point = [gesture locationInView:self.view];
            
            
            // hide the cell
            ImageCollectionViewCell* cell = ( ImageCollectionViewCell* )[_collection cellForItemAtIndexPath:indexPath];
            if ( cell ) {
                cell.alpha = 0.0f;
                UIImage* image = cell.imageView.image;
                [_dragged_image setImage:image];
                _dragged_image.center = point;
                _dragged_image.hidden = NO;
            }
        }
    }
}

- (void)handlePan:(UIPanGestureRecognizer *)gesture {
    if ( selected_index == -1 || selected_source == -1 ) return;
    CGPoint touchPoint = [gesture locationInView:self.view];
    if (gesture.state == UIGestureRecognizerStateChanged
        && !_dragged_image.hidden) {
        // image is dragged
        _dragged_image.center = touchPoint;
        [self updateImageViewDragState:[self isValidDragPoint:touchPoint]];
    } else if (gesture.state == UIGestureRecognizerStateRecognized) {
        _dragged_image.hidden = YES;
        
        BOOL validDropPoint = [self isValidDragPoint:touchPoint];
        if ( validDropPoint ) {
            switch ( selected_source ) {
                case 0:
                    editor->moveImageFromLibraryToCollection(selected_index);
                    break;
                case 1:
                    editor->moveImageFromCollectionToLibrary(selected_index);
                    break;
            }
        } else {
            
        }
        [_collection reloadData];
        [_library reloadData];
        selected_source = selected_index = -1;
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

- (void)updateImageViewDragState:(BOOL)validDropPoint {
    if (validDropPoint) {
        _dragged_image.alpha = 1.0f;
    } else {
        _dragged_image.alpha = 0.2f;
    }
}

- (BOOL)isValidDragPoint:(CGPoint)point {
    return !CGRectContainsPoint(selected_source==0?_library.frame:_collection.frame, point);
}

#pragma mark - UIGestureRecognizerDelegate
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
    return YES;
}
@end
