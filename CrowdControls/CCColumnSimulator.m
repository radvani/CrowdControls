//
//  CCColumnSimulator.m
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

#import "CCColumnSimulator.h"
#import "AppDelegate.h"
#import "CCDanceController.h"

@implementation CCButtonPin

- (id)initWithButton:(NSButton *)button pin:(CCSignalPin)pin {
    self = [super init];
    if (self) {
        _button = button;
        _pin = pin;
    }
    return self;
}

@end

@interface CCColumnSimulator ()

@property (readwrite, nonatomic) NSArray *headGroup;
@property (readwrite, nonatomic) NSArray *leftArmGroup;
@property (readwrite, nonatomic) NSArray *rightArmGroup;
@property (readwrite, nonatomic) NSArray *leftLegGroup;
@property (readwrite, nonatomic) NSArray *rightLegGroup;

@end

@implementation CCColumnSimulator

- (void)awakeFromNib {
    self.headGroup     = [NSArray arrayWithObjects:[[CCButtonPin alloc] initWithButton:self.headWhite pin:CCSignalPinHeadWhite],
                                                   [[CCButtonPin alloc] initWithButton:self.headBlue pin:CCSignalPinHeadBlue],
                                                   [[CCButtonPin alloc] initWithButton:self.headYellow pin:CCSignalPinHeadYellow],
                                                   [[CCButtonPin alloc] initWithButton:self.headGreen pin:CCSignalPinHeadGreen],
                                                   [[CCButtonPin alloc] initWithButton:self.headRed pin:CCSignalPinHeadRed],
                                                   [[CCButtonPin alloc] initWithButton:self.headWhiteBottom pin:CCSignalPinHeadWhite],
                                                   [[CCButtonPin alloc] initWithButton:self.headBlueBottom pin:CCSignalPinHeadBlue],
                                                   [[CCButtonPin alloc] initWithButton:self.headYellowBottom pin:CCSignalPinHeadYellow],
                                                   [[CCButtonPin alloc] initWithButton:self.headGreenBottom pin:CCSignalPinHeadGreen],
                                                   [[CCButtonPin alloc] initWithButton:self.headRedBottom pin:CCSignalPinHeadRed], nil];
    
    self.leftArmGroup  = [NSArray arrayWithObjects:[[CCButtonPin alloc] initWithButton:self.leftArmWhite pin:CCSignalPinLeftArmWhite],
                                                   [[CCButtonPin alloc] initWithButton:self.leftArmBlue pin:CCSignalPinLeftArmBlue],
                                                   [[CCButtonPin alloc] initWithButton:self.leftArmYellow pin:CCSignalPinLeftArmYellow],
                                                   [[CCButtonPin alloc] initWithButton:self.leftArmGreen pin:CCSignalPinLeftArmGreen],
                                                   [[CCButtonPin alloc] initWithButton:self.leftArmRed pin:CCSignalPinLeftArmRed],
                                                   [[CCButtonPin alloc] initWithButton:self.leftArmWhiteBottom pin:CCSignalPinLeftArmWhite],
                                                   [[CCButtonPin alloc] initWithButton:self.leftArmBlueBottom pin:CCSignalPinLeftArmBlue],
                                                   [[CCButtonPin alloc] initWithButton:self.leftArmYellowBottom pin:CCSignalPinLeftArmYellow],
                                                   [[CCButtonPin alloc] initWithButton:self.leftArmGreenBottom pin:CCSignalPinLeftArmGreen],
                                                   [[CCButtonPin alloc] initWithButton:self.leftArmRedBottom pin:CCSignalPinLeftArmRed], nil];
    
    self.rightArmGroup = [NSArray arrayWithObjects:[[CCButtonPin alloc] initWithButton:self.rightArmWhite pin:CCSignalPinRightArmWhite],
                                                   [[CCButtonPin alloc] initWithButton:self.rightArmBlue pin:CCSignalPinRightArmBlue],
                                                   [[CCButtonPin alloc] initWithButton:self.rightArmYellow pin:CCSignalPinRightArmYellow],
                                                   [[CCButtonPin alloc] initWithButton:self.rightArmGreen pin:CCSignalPinRightArmGreen],
                                                   [[CCButtonPin alloc] initWithButton:self.rightArmRed pin:CCSignalPinRightArmRed],
                                                   [[CCButtonPin alloc] initWithButton:self.rightArmWhiteBottom pin:CCSignalPinRightArmWhite],
                                                   [[CCButtonPin alloc] initWithButton:self.rightArmBlueBottom pin:CCSignalPinRightArmBlue],
                                                   [[CCButtonPin alloc] initWithButton:self.rightArmYellowBottom pin:CCSignalPinRightArmYellow],
                                                   [[CCButtonPin alloc] initWithButton:self.rightArmGreenBottom pin:CCSignalPinRightArmGreen],
                                                   [[CCButtonPin alloc] initWithButton:self.rightArmRedBottom pin:CCSignalPinRightArmRed], nil];
    
    self.leftLegGroup  = [NSArray arrayWithObjects:[[CCButtonPin alloc] initWithButton:self.leftLegWhite pin:CCSignalPinLeftLegWhite],
                                                   [[CCButtonPin alloc] initWithButton:self.leftLegBlue pin:CCSignalPinLeftLegBlue],
                                                   [[CCButtonPin alloc] initWithButton:self.leftLegYellow pin:CCSignalPinLeftLegYellow],
                                                   [[CCButtonPin alloc] initWithButton:self.leftLegGreen pin:CCSignalPinLeftLegGreen],
                                                   [[CCButtonPin alloc] initWithButton:self.leftLegRed pin:CCSignalPinLeftLegRed],
                                                   [[CCButtonPin alloc] initWithButton:self.leftLegWhiteBottom pin:CCSignalPinLeftLegWhite],
                                                   [[CCButtonPin alloc] initWithButton:self.leftLegBlueBottom pin:CCSignalPinLeftLegBlue],
                                                   [[CCButtonPin alloc] initWithButton:self.leftLegYellowBottom pin:CCSignalPinLeftLegYellow],
                                                   [[CCButtonPin alloc] initWithButton:self.leftLegGreenBottom pin:CCSignalPinLeftLegGreen],
                                                   [[CCButtonPin alloc] initWithButton:self.leftLegRedBottom pin:CCSignalPinLeftLegRed], nil];
    
    self.rightLegGroup = [NSArray arrayWithObjects:[[CCButtonPin alloc] initWithButton:self.rightLegWhite pin:CCSignalPinRightLegWhite],
                                                   [[CCButtonPin alloc] initWithButton:self.rightLegBlue pin:CCSignalPinRightLegBlue],
                                                   [[CCButtonPin alloc] initWithButton:self.rightLegYellow pin:CCSignalPinRightLegYellow],
                                                   [[CCButtonPin alloc] initWithButton:self.rightLegGreen pin:CCSignalPinRightLegGreen],
                                                   [[CCButtonPin alloc] initWithButton:self.rightLegRed pin:CCSignalPinRightLegRed],
                                                   [[CCButtonPin alloc] initWithButton:self.rightLegWhiteBottom pin:CCSignalPinRightLegWhite],
                                                   [[CCButtonPin alloc] initWithButton:self.rightLegBlueBottom pin:CCSignalPinRightLegBlue],
                                                   [[CCButtonPin alloc] initWithButton:self.rightLegYellowBottom pin:CCSignalPinRightLegYellow],
                                                   [[CCButtonPin alloc] initWithButton:self.rightLegGreenBottom pin:CCSignalPinRightLegGreen],
                                                   [[CCButtonPin alloc] initWithButton:self.rightLegRedBottom pin:CCSignalPinRightLegRed], nil];
}

- (void)onButtonChange:(id)sender group:(NSArray *)group {
    AppDelegate *appDelegate = (AppDelegate *) [[NSApplication sharedApplication] delegate];
    if (!appDelegate.danceController) {
        return;
    }
    
    for (CCButtonPin *buttonPin in group) {
        if (buttonPin.button == sender) {
            [appDelegate.danceController pin:buttonPin.pin didChangeSignal:kSignalOn];
        } else {
            [appDelegate.danceController pin:buttonPin.pin didChangeSignal:kSignalOff];
        }
    }
}

- (IBAction)onHeadChange:(id)sender {
    [self onButtonChange:sender group:self.headGroup];
}

- (IBAction)onLeftArmChange:(id)sender {
    [self onButtonChange:sender group:self.leftArmGroup];
}

- (IBAction)onRightArmChange:(id)sender {
    [self onButtonChange:sender group:self.rightArmGroup];
}

- (IBAction)onLeftLegChange:(id)sender {
    [self onButtonChange:sender group:self.leftLegGroup];
}

- (IBAction)onRightLegChange:(id)sender {
    [self onButtonChange:sender group:self.rightLegGroup];
}

@end
