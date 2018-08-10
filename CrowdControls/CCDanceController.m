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
#import <AVFoundation/AVFoundation.h>
#import <functional>
#import <atomic>

static const int kNumDanceScreens = 4;

static const VROVector4f kWhiteColor = VROVector4f(1, 1, 1, 1);
static const VROVector4f kBlueColor = VROVector4f(30.0 / 255.0, 145.0 / 255.0, 225.0 / 255.0, 1);
static const VROVector4f kRedColor = VROVector4f(222.0 / 255.0, 27.0 / 255.0,  55.0 / 255.0, 1);
static const VROVector4f kGreenColor = VROVector4f(106.0 / 255.0, 237.0 / 255.0, 109.0 / 255.0, 1);
static const VROVector4f kYellowColor = VROVector4f(255.0 / 255.0, 217.0 / 255.0, 86.0 / 255.0, 1);

@interface CCDanceController ()

@property (readonly, nonatomic) AVAudioEngine *audioEngine;
@property (readonly, nonatomic) AVAudioMixerNode *audioMixer;
@property (readonly, nonatomic) NSArray *screens;
@property (readonly, nonatomic) std::map<CCSignalPin, std::pair<AVAudioPlayerNode *, AVAudioPCMBuffer *>> audioStems;
@property (readonly, nonatomic) std::map<CCColor, std::pair<AVAudioPlayerNode *, AVAudioPCMBuffer *>> drumStems;
@property (readonly, nonatomic) std::map<CCBodyPart, CCSignalPin> activePins;
@property (readonly, nonatomic) std::pair<AVAudioPlayerNode *, AVAudioPCMBuffer *> activeDrums;

@end

@implementation CCDanceController {

}

- (id)initWithAnimationScreens:(NSArray *)animationScreens {
    self = [super init];
    if (self) {
        _screens = animationScreens;
        _activeDrums.first = nil;
        _audioEngine = [[AVAudioEngine alloc] init];
        _audioMixer = [[AVAudioMixerNode alloc] init];
        
        [self.audioEngine attachNode:self.audioMixer];
        [self.audioEngine connect:self.audioMixer to:self.audioEngine.outputNode format:nil];
        
        NSError *error;
        [self.audioEngine prepare];
        [self.audioEngine startAndReturnError:&error];
        if (error) {
            NSLog(@"Error starting audio engine: %@", error.localizedDescription);
            error = nil;
        }
        
        [self loadAudioStems];
        
        _activePins[CCBodyPartHead] = CCSignalPinHeadBlue;
        _activePins[CCBodyPartLeftArm] = CCSignalPinLeftArmBlue;
        _activePins[CCBodyPartRightArm] = CCSignalPinRightArmBlue;
        _activePins[CCBodyPartLeftLeg] = CCSignalPinLeftLegBlue;
        _activePins[CCBodyPartRightLeg] = CCSignalPinRightLegBlue;
    }
    return self;
}

- (std::string)urlForResource:(std::string)resource type:(std::string)type {
    NSString *objPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:resource.c_str()]
                                                        ofType:[NSString stringWithUTF8String:type.c_str()]];
    NSURL *objURL = [NSURL fileURLWithPath:objPath];
    return std::string([[objURL description] UTF8String]);
}

- (AVAudioPlayerNode *)loadAudioPlayerFromURL:(NSURL *)url outBuffer:(AVAudioPCMBuffer **)outBuffer {
    NSError *error;
    AVAudioFile *file = [[AVAudioFile alloc] initForReading:url error:&error];
    if (error) {
        NSLog(@"Error loading audio file %@", error.localizedDescription);
        error = nil;
    }
    
    AVAudioPCMBuffer *buffer = [[AVAudioPCMBuffer alloc] initWithPCMFormat:file.processingFormat frameCapacity:(int) file.length];
    [file readIntoBuffer:buffer error:&error];
    if (error) {
        NSLog(@"Error reading audio file into buffer %@", error.localizedDescription);
        error = nil;
    }
    
    AVAudioPlayerNode *audioPlayer = [[AVAudioPlayerNode alloc] init];
    [self.audioEngine attachNode:audioPlayer];
    [self.audioEngine connect:audioPlayer to:self.audioMixer format:nil];

    [audioPlayer scheduleBuffer:buffer atTime:0 options:AVAudioPlayerNodeBufferLoops completionHandler:nil];
    [audioPlayer prepareWithFrameCount:buffer.frameLength];
    
    *outBuffer = buffer;
    return audioPlayer;
}

- (void)loadAudioStem:(CCSignalPin)pin resource:(NSString *)name {
    NSURL *url = [[NSBundle mainBundle] URLForResource:name withExtension:@"mp3"];
    AVAudioPCMBuffer *buffer;
    AVAudioPlayerNode *audioPlayer = [self loadAudioPlayerFromURL:url outBuffer:&buffer];
    _audioStems.insert({ pin, { audioPlayer, buffer } });
}

- (void)loadDrumStem:(CCColor)color resource:(NSString *)name {
    NSURL *url = [[NSBundle mainBundle] URLForResource:name withExtension:@"mp3"];
    AVAudioPCMBuffer *buffer;
    AVAudioPlayerNode *audioPlayer = [self loadAudioPlayerFromURL:url outBuffer:&buffer];
    _drumStems.insert({ color, { audioPlayer, buffer } });
}

- (void)loadAudioStems {
    [self loadAudioStem:CCSignalPinHeadWhite resource:@"Body_W"];
    [self loadAudioStem:CCSignalPinLeftArmWhite resource:@"Lhand_W"];
    [self loadAudioStem:CCSignalPinRightArmWhite resource:@"Rhand_W"];
    [self loadAudioStem:CCSignalPinLeftLegWhite resource:@"Lfoot_W"];
    [self loadAudioStem:CCSignalPinRightLegWhite resource:@"Rfoot_W"];
    [self loadAudioStem:CCSignalPinHeadRed resource:@"Body_R"];
    [self loadAudioStem:CCSignalPinLeftArmRed resource:@"Lhand_R"];
    [self loadAudioStem:CCSignalPinRightArmRed resource:@"Rhand_R"];
    [self loadAudioStem:CCSignalPinLeftLegRed resource:@"Lfoot_R"];
    [self loadAudioStem:CCSignalPinRightLegRed resource:@"Rfoot_R"];
    [self loadAudioStem:CCSignalPinHeadGreen resource:@"Body_G"];
    [self loadAudioStem:CCSignalPinLeftArmGreen resource:@"Lhand_G"];
    [self loadAudioStem:CCSignalPinRightArmGreen resource:@"Rhand_G"];
    [self loadAudioStem:CCSignalPinLeftLegGreen resource:@"Lfoot_G"];
    [self loadAudioStem:CCSignalPinRightLegGreen resource:@"Rfoot_G"];
    [self loadAudioStem:CCSignalPinHeadYellow resource:@"Body_Y"];
    [self loadAudioStem:CCSignalPinLeftArmYellow resource:@"Lhand_Y"];
    [self loadAudioStem:CCSignalPinRightArmYellow resource:@"Rhand_Y"];
    [self loadAudioStem:CCSignalPinLeftLegYellow resource:@"Lfoot_Y"];
    [self loadAudioStem:CCSignalPinRightLegYellow resource:@"Rfoot_Y"];
    
    [self loadDrumStem:CCColorWhite resource:@"Drums_W"];
    [self loadDrumStem:CCColorYellow resource:@"Drums_Y"];
    [self loadDrumStem:CCColorGreen resource:@"Drums_G"];
    [self loadDrumStem:CCColorRed resource:@"Drums_R"];
}

+ (CCColor)colorForPin:(CCSignalPin)pin {
    switch (pin) {
        case CCSignalPinHeadWhite:
        case CCSignalPinLeftArmWhite:
        case CCSignalPinRightArmWhite:
        case CCSignalPinLeftLegWhite:
        case CCSignalPinRightLegWhite:
            return CCColorWhite;
            
        case CCSignalPinHeadRed:
        case CCSignalPinLeftArmRed:
        case CCSignalPinRightArmRed:
        case CCSignalPinLeftLegRed:
        case CCSignalPinRightLegRed:
            return CCColorRed;

        case CCSignalPinHeadGreen:
        case CCSignalPinLeftArmGreen:
        case CCSignalPinRightArmGreen:
        case CCSignalPinLeftLegGreen:
        case CCSignalPinRightLegGreen:
            return CCColorGreen;

        case CCSignalPinHeadYellow:
        case CCSignalPinLeftArmYellow:
        case CCSignalPinRightArmYellow:
        case CCSignalPinLeftLegYellow:
        case CCSignalPinRightLegYellow:
            return CCColorYellow;
            
        case CCSignalPinHeadBlue:
        case CCSignalPinLeftArmBlue:
        case CCSignalPinRightArmBlue:
        case CCSignalPinLeftLegBlue:
        case CCSignalPinRightLegBlue:
            return CCColorBlue;
            
        default:
            return CCColorBlue;
    }
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

+ (VROVector4f)rgbForPin:(CCSignalPin)pin {
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

- (CCColor)isSynchronized:(BOOL *)outSynchronized {
    int i = 0;
    *outSynchronized = YES;
    CCColor lastColor = CCColorBlue;
    
    for (auto kv : _activePins) {
        CCColor colorForBodyPart = [CCDanceController colorForPin:kv.second];
        if (i == 0) {
            lastColor = colorForBodyPart;
            ++i;
            continue;
        }
        if (colorForBodyPart != lastColor) {
            *outSynchronized = NO;
            break;
        }
        ++i;
    }
    return lastColor;
}

- (void)startAnimationSequence {
    bool isFirst = true;
    AVAudioFramePosition masterStartTime = 0;
    
    // Iterate through all sounds: turn off those that are no longer selected, and
    // turn on those that are selected for the first time
    for (auto kv : _audioStems) {
        CCSignalPin pin = kv.first;
        CCBodyPart bodyPart = [CCDanceController bodyPartForPin:pin];
        
        // Use the render time of the first player as the master start time for all
        // players. This is how we ensure sync: by using playAtTime: with a common
        // time for all players (and by scheduling and preparing beforehand during
        // initialization).
        AVAudioPlayerNode *player = kv.second.first;
        AVAudioPCMBuffer *buffer = kv.second.second;
        
        // Note that the 'time' used in playAtTime: is a system-wide time, not the
        // time inside the audio track at which to start. In other words, playAtTime
        // means "play the audio, starting from the beginning, at the given time."
        if (isFirst) {
            masterStartTime = player.lastRenderTime.sampleTime;
            isFirst = false;
        }
        
        if (_activePins[bodyPart] != pin) {
            [player stop];
            [player scheduleBuffer:buffer atTime:0 options:AVAudioPlayerNodeBufferLoops completionHandler:nil];
        } else if (![player isPlaying]) {
            AVAudioFormat *outputFormat = [player outputFormatForBus:0];
            AVAudioTime *startTime = [AVAudioTime timeWithSampleTime:masterStartTime atRate:outputFormat.sampleRate];
            [player playAtTime:startTime];
        }
    }
    
    /*
     Check if all the body parts are of the same color. If so, drums will kick in and
     a synchronized animation will start.
     */
    BOOL synchronized = YES;
    CCColor synchronizedColor = [self isSynchronized:&synchronized];
    
    if (synchronized && synchronizedColor != CCColorBlue) {
        auto kv = _drumStems.find(synchronizedColor);
        AVAudioPlayerNode *drumPlayer = kv->second.first;
        AVAudioPCMBuffer *drumBuffer = kv->second.second;
        
        if (_activeDrums.first && _activeDrums.first != drumPlayer) {
            [_activeDrums.first stop];
            [_activeDrums.first scheduleBuffer:_activeDrums.second atTime:0 options:AVAudioPlayerNodeBufferLoops completionHandler:nil];
        }
        
        if (![drumPlayer isPlaying]) {
            AVAudioFormat *outputFormat = [drumPlayer outputFormatForBus:0];
            AVAudioTime *startTime = [AVAudioTime timeWithSampleTime:masterStartTime atRate:outputFormat.sampleRate];
            [drumPlayer playAtTime:startTime];
            _activeDrums.first = drumPlayer;
            _activeDrums.second = drumBuffer;
        }
    } else if (_activeDrums.first) {
        [_activeDrums.first stop];
        [_activeDrums.first scheduleBuffer:_activeDrums.second atTime:0 options:AVAudioPlayerNodeBufferLoops completionHandler:nil];

        _activeDrums.first = nil;
    }
    
    // Begin the next queued animation for each screen
    for (CCAnimationScreen *screen in self.screens) {
        VROViewScene *view = (VROViewScene *) screen.view;
        std::function<void()> task = [screen, self] {
            
            // The callback is only invoked by the countdown screen, which serves as the
            // master timer
            screen.scene->startSequence(9.583, [self] (CCScene *finishedScene) {
                // The callback executes on the rendering thread of the countdown scene, which is
                // precisely timed by VROFrameListener (backed by a CADisplayLink). We do not dispatch
                // to the main thread; instead we start the next animation sequence directly from this
                // rendering thread, otherwise the dispatch call will throw any new sounds off-sync
                CCCountdownScene *finishedCountdownScene = dynamic_cast<CCCountdownScene *>(finishedScene);
                if (finishedCountdownScene) {
                    [self startAnimationSequence];
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
    
    CCBodyPart bodyPart = [CCDanceController bodyPartForPin:pin];
    _activePins[bodyPart] = pin;
    
    for (CCAnimationScreen *screen in self.screens) {
        VROViewScene *view = (VROViewScene *) screen.view;
        std::function<void()> task = [pin, bodyPart, screen] {
            [screen setBodyPart:bodyPart toColor:[CCDanceController rgbForPin:pin]];
        };
        [view queueRendererTask:task];
    }
}

@end
