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

@property (readwrite, nonatomic, strong) IBOutlet NSButton *headWhite;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftArmWhite;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightArmWhite;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftLegWhite;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightLegWhite;

@property (readwrite, nonatomic, strong) IBOutlet NSButton *headBlue;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftArmBlue;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightArmBlue;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftLegBlue;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightLegBlue;

@property (readwrite, nonatomic, strong) IBOutlet NSButton *headRed;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftArmRed;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightArmRed;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftLegRed;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightLegRed;

@property (readwrite, nonatomic, strong) IBOutlet NSButton *headGreen;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftArmGreen;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightArmGreen;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftLegGreen;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightLegGreen;

@property (readwrite, nonatomic, strong) IBOutlet NSButton *headYellow;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftArmYellow;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightArmYellow;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *leftLegYellow;
@property (readwrite, nonatomic, strong) IBOutlet NSButton *rightLegYellow;

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
