//
//  ImageCollectionViewCell.m
//  openTable
//
//  Created by Jonathan Jones-morris on 08/05/2016.
//
//

#import "ImageCollectionViewCell.h"

@implementation ImageCollectionViewCell

-(id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if ( self ) {
        _imageView = [[UIImageView alloc] initWithFrame:self.contentView.bounds];
        [_imageView setContentMode:UIViewContentModeScaleAspectFit];
        [self.contentView addSubview:_imageView];
    }
    return self;
}
-(void)dealloc {
    [_imageView release];
    [super dealloc];
}
-(void)prepareForReuse {
    [super prepareForReuse];
    [_imageView setImage:nil];
    [_imageView setFrame:self.contentView.bounds];
}
@end
