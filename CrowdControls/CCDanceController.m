//
//  CCDanceController.m
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

#import "CCDanceController.h"
#import "CCAnimationScreen.h"
#import "CCDanceScene.h"
#import "CCCountdownScene.h"
#import <ViroKit/ViroKit.h>
#import <functional>
#import <atomic>

static const int kNumDanceScreens = 4;

static const VROVector4f kWhiteColor = VROVector4f(1, 1, 1, 1);
static const VROVector4f kBlueColor = VROVector4f(30.0 / 255.0, 145.0 / 255.0, 225.0 / 255.0, 1);
static const VROVector4f kRedColor = VROVector4f(222.0 / 255.0, 27.0 / 255.0,  55.0 / 255.0, 1);
static const VROVector4f kGreenColor = VROVector4f(106.0 / 255.0, 237.0 / 255.0, 109.0 / 255.0, 1);
static const VROVector4f kYellowColor = VROVector4f(255.0 / 255.0, 217.0 / 255.0, 86.0 / 255.0, 1);

@interface CCDanceController ()

@property (readonly, nonatomic) NSArray *screens;

@end

@implementation CCDanceController {

}

- (id)initWithAnimationScreens:(NSArray *)animationScreens {
    self = [super init];
    if (self) {
        _screens = animationScreens;
    }
    return self;
}

+ (CCBodyPart)bodyPartForPin:(CCSignalPin)pin {
    switch (pin) {
        case CCSignalPinHeadWhite:
        case CCSignalPinHeadBlue:
        case CCSignalPinHeadRed:
        case CCSignalPinHeadGreen:
        case CCSignalPinHeadYellow:
            return CCBodyPartHead;

        case CCSignalPinLeftArmWhite:
        case CCSignalPinLeftArmBlue:
        case CCSignalPinLeftArmRed:
        case CCSignalPinLeftArmGreen:
        case CCSignalPinLeftArmYellow:
            return CCBodyPartLeftArm;

        case CCSignalPinRightArmWhite:
        case CCSignalPinRightArmBlue:
        case CCSignalPinRightArmRed:
        case CCSignalPinRightArmGreen:
        case CCSignalPinRightArmYellow:
            return CCBodyPartRightArm;

        case CCSignalPinLeftLegWhite:
        case CCSignalPinLeftLegBlue:
        case CCSignalPinLeftLegRed:
        case CCSignalPinLeftLegGreen:
        case CCSignalPinLeftLegYellow:
            return CCBodyPartLeftLeg;
            
        case CCSignalPinRightLegWhite:
        case CCSignalPinRightLegBlue:
        case CCSignalPinRightLegRed:
        case CCSignalPinRightLegGreen:
        case CCSignalPinRightLegYellow:
            return CCBodyPartRightLeg;
            
        default:
            return CCBodyPartHead;
    }
}

+ (VROVector4f)colorForPin:(CCSignalPin)pin {
    switch (pin) {
        case CCSignalPinHeadWhite:
        case CCSignalPinLeftArmWhite:
        case CCSignalPinRightArmWhite:
        case CCSignalPinLeftLegWhite:
        case CCSignalPinRightLegWhite:
            return kWhiteColor;
            
        case CCSignalPinHeadBlue:
        case CCSignalPinLeftArmBlue:
        case CCSignalPinRightArmBlue:
        case CCSignalPinLeftLegBlue:
        case CCSignalPinRightLegBlue:
            return kBlueColor;
            
        case CCSignalPinHeadRed:
        case CCSignalPinLeftArmRed:
        case CCSignalPinRightArmRed:
        case CCSignalPinLeftLegRed:
        case CCSignalPinRightLegRed:
            return kRedColor;
            
        case CCSignalPinHeadGreen:
        case CCSignalPinLeftArmGreen:
        case CCSignalPinRightArmGreen:
        case CCSignalPinLeftLegGreen:
        case CCSignalPinRightLegGreen:
            return kGreenColor;
            
        case CCSignalPinHeadYellow:
        case CCSignalPinLeftArmYellow:
        case CCSignalPinRightArmYellow:
        case CCSignalPinLeftLegYellow:
        case CCSignalPinRightLegYellow:
            return kYellowColor;
            
        default:
            return kWhiteColor;
    }
}

- (void)startAnimationSequence {
    for (CCAnimationScreen *screen in self.screens) {
        VROViewScene *view = (VROViewScene *) screen.view;
        std::function<void()> task = [screen, self] {
            screen.scene->startSequence(9.583f, [self] (CCScene *finishedScene) {
                CCCountdownScene *finishedCountdownScene = dynamic_cast<CCCountdownScene *>(finishedScene);
                if (finishedCountdownScene) {
                    dispatch_async(dispatch_get_main_queue(), [self] {
                        [self startAnimationSequence];
                    });
                }
            });
        };
        [view queueRendererTask:task];
    }
}

- (void)pin:(CCSignalPin)pin didChangeSignal:(int)signal {
    if (signal == kSignalOff) {
        return;
    }
    
    NSLog(@"Pin %d did change signal to %d", (int) pin, signal);
    
    for (CCAnimationScreen *screen in self.screens) {
        VROViewScene *view = (VROViewScene *) screen.view;
        std::function<void()> task = [pin, screen] {
            [screen setBodyPart:[CCDanceController bodyPartForPin:pin] toColor:[CCDanceController colorForPin:pin]];
        };
        [view queueRendererTask:task];
    }
}

@end
