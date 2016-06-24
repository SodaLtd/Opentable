//
//  sdaKeyboard.cpp
//  openTable
//
//  Created by Jonathan Jones-morris on 16/02/2015.
//
//
/*
#include "sdaKeyboard.h"

const string k_keyboards[2][26] = {
    { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
        "A", "S", "D", "F", "G", "H", "J", "K", "L",
        "Z", "X", "C", "V" , "B", "N", "M" },
    { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
        "@", "#", "£", "%", "&", "-", "+", "(", ")",
        "*", "\"", "'", ":", ";", "!", "?" }
};

const float kUnitDim = 48.0;
const float kUnitMargin = 4;

sdaKeyboard::sdaKeyboard() : mui::Container(0,0) {
    
}
sdaKeyboard::~sdaKeyboard() {
    
}
//
//
//
bool sdaKeyboard::setup() {
    //
    // setup keyboard body
    //
    for ( int i = 0; i < 2; i++ ) {
        m_keyboard[ i ].resize( 26 );
        for ( int j = 0; j < 26; j++ ) {
            float y = 0.0;
            if ( j < 10 ) {
                x = kUnitMargin + ( ( kUnitDim + kUnitMargin ) * ( j ));
                y = kUnitMargin;
            } else if ( y < 19 ) {
                y = kUnitMargin + kUnitDim + kUnitMargin;
            } else {
                y = kUnitMargin + kUnitDim + kUnitMargin + kUnitDim + kUnitMargin;
            }
            x = kUnitMargin + ( ( kUnitDim + kUnitMargin ) * ( j ));
            m_keyboard[ i ][ i ].init(k_keyboards[i][j]);
        }
    }
    //
    // setup common keys
    //
    
}
void sdaKeyboard::update() {
    
}
void sdaKeyboard::draw() {
    
}
//
//
//
void sdaKeyboard::onKeyPress( const void* sender, ofTouchEventArgs &args ) {
    
}
*/