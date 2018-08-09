//
//  CCColumnSimulator.h
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
#import <AppKit/AppKit.h>
#import "CCSignalReader.h"

@interface CCColumnSimulator : NSViewController

@property (readwrite, nonatomic, strong) IBOutlet NSButton *headWhite, *headWhiteBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftArmWhite, *leftArmWhiteBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightArmWhite, *rightArmWhiteBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftLegWhite, *leftLegWhiteBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightLegWhite, *rightLegWhiteBottom;

@property (readwrite, nonatomic, strong) IBOutlet NSButton *headBlue, *headBlueBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftArmBlue, *leftArmBlueBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightArmBlue, *rightArmBlueBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftLegBlue, *leftLegBlueBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightLegBlue, *rightLegBlueBottom;

@property (readwrite, nonatomic, strong) IBOutlet NSButton *headRed, *headRedBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftArmRed, *leftArmRedBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightArmRed, *rightArmRedBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftLegRed, *leftLegRedBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightLegRed, *rightLegRedBottom;

@property (readwrite, nonatomic, strong) IBOutlet NSButton *headGreen, *headGreenBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftArmGreen, *leftArmGreenBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightArmGreen, *rightArmGreenBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftLegGreen, *leftLegGreenBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightLegGreen, *rightLegGreenBottom;

@property (readwrite, nonatomic, strong) IBOutlet NSButton *headYellow, *headYellowBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftArmYellow, *leftArmYellowBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightArmYellow, *rightArmYellowBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftLegYellow, *leftLegYellowBottom;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightLegYellow, *rightLegYellowBottom;

@property (readwrite, nonatomic) NSObject<CCSignalDelegate> *signalDelegate;

- (IBAction)onHeadChange:(id)sender;
- (IBAction)onLeftArmChange:(id)sender;
- (IBAction)onRightArmChange:(id)sender;
- (IBAction)onLeftLegChange:(id)sender;
- (IBAction)onRightLegChange:(id)sender;

@end

@interface CCButtonPin : NSObject
@property (readonly, nonatomic, strong) NSButton *button;
@property (readonly, nonatomic) CCSignalPin pin;
- (id)initWithButton:(NSButton *)button pin:(CCSignalPin)pin;
@end
