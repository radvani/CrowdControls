//
//  CCDanceController.h
//  CrowdControls
//
//  Created by Raj Advani on 7/14/18.
//  Copyright © 2018 Raj Advani. All rights reserved.
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

#import <Foundation/Foundation.h>
#import "CCSignalReader.h"
#import "CCBeatDetection.h"
#import "CCTypes.h"

@interface CCDanceController : NSObject <CCSignalDelegate>

@property (readwrite, nonatomic, strong) CCBeatDetection *beatDetection;

- (id)initWithAnimationScreens:(NSArray *)animationScreens;
- (void)pin:(CCSignalPin)pin didChangeSignal:(int)signal;
- (void)startAnimationSequence;

+ (NSString *)letterForColor:(CCColor)color;

@end

