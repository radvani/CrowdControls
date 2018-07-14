//
//  CCSignalReader.h
//  CrowdControls
//
//  Copyright © 2018 Raj Advani. All rights reserved.
//  Created by Raj Advani on 7/14/18.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:

//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#ifndef CCSignalReader_h
#define CCSignalReader_h

#include <Foundation/Foundation.h>
#include <string>

static const int kSignalOff = 0;
static const int kSignalOn = 1;

typedef NS_ENUM(NSInteger, CCSignalPin) {
    CCSignalPinHeadWhite,
    CCSignalPinLeftArmWhite,
    CCSignalPinRightArmWhite,
    CCSignalPinLeftLegWhite,
    CCSignalPinRightLegWhite,
    CCSignalPinHeadBlue,
    CCSignalPinLeftArmBlue,
    CCSignalPinRightArmBlue,
    CCSignalPinLeftLegBlue,
    CCSignalPinRightLegBlue,
    CCSignalPinHeadRed,
    CCSignalPinLeftArmRed,
    CCSignalPinRightArmRed,
    CCSignalPinLeftLegRed,
    CCSignalPinRightLegRed,
    CCSignalPinHeadGreen,
    CCSignalPinLeftArmGreen,
    CCSignalPinRightArmGreen,
    CCSignalPinLeftLegGreen,
    CCSignalPinRightLegGreen,
    CCSignalPinHeadYellow,
    CCSignalPinLeftArmYellow,
    CCSignalPinRightArmYellow,
    CCSignalPinLeftLegYellow,
    CCSignalPinRightLegYellow
};

@protocol CCSignalDelegate
@required

- (void)pin:(CCSignalPin)pin didChangeSignal:(int)signal;

@end

@interface CCSignalReader : NSObject

@property (readwrite, nonatomic) id<CCSignalDelegate> delegate;

- (id)init;
- (void)listPorts;
- (void)connect:(std::string)port;
    
@end

#endif /* CCSignalReader_h */
