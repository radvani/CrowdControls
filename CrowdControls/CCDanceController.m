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
static bool kMuteAudio = true;
static bool kDancesEnabled = true;

static const VROVector4f kWhiteColor = VROVector4f(1, 1, 1, 1);
static const VROVector4f kBlueColor = VROVector4f(30.0 / 255.0, 145.0 / 255.0, 225.0 / 255.0, 1);
static const VROVector4f kRedColor = VROVector4f(222.0 / 255.0, 27.0 / 255.0,  55.0 / 255.0, 1);
static const VROVector4f kGreenColor = VROVector4f(106.0 / 255.0, 237.0 / 255.0, 109.0 / 255.0, 1);
static const VROVector4f kYellowColor = VROVector4f(255.0 / 255.0, 217.0 / 255.0, 86.0 / 255.0, 1);

@interface CCDanceController ()

@property (readonly, nonatomic) AVAudioEngine *audioEngine;
@property (readonly, nonatomic) AVAudioMixerNode *audioMixer;
@property (readonly, nonatomic) NSArray *screens;
@property (readonly, nonatomic) std::map<CCBodyPart, std::map<CCColor, std::pair<AVAudioPlayerNode *, AVAudioPCMBuffer *>>> audioStems;
@property (readonly, nonatomic) std::map<CCColor, std::pair<AVAudioPlayerNode *, AVAudioPCMBuffer *>> drumStems;
@property (readonly, nonatomic) std::map<CCBodyPart, CCColor> activeColors;
@property (readonly, nonatomic) std::pair<AVAudioPlayerNode *, AVAudioPCMBuffer *> activeDrums;
@property (readonly, nonatomic) std::map<CCBodyPart, std::vector<CCColor>> activatedPins;

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
        
        [self loadAnimationAndAudioStemss];
        
        _activeColors[CCBodyPartHead] = CCColorBlue;
        _activeColors[CCBodyPartLeftArm] = CCColorBlue;
        _activeColors[CCBodyPartRightArm] = CCColorBlue;
        _activeColors[CCBodyPartLeftLeg] = CCColorBlue;
        _activeColors[CCBodyPartRightLeg] = CCColorBlue;
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
    if (kMuteAudio) {
        [audioPlayer setVolume:0];
    }
    
    *outBuffer = buffer;
    return audioPlayer;
}

- (void)loadAnimationAndAudioStems:(CCSignalPin)pin resource:(NSString *)name {
    NSURL *url = [[NSBundle mainBundle] URLForResource:name withExtension:@"mp3"];
    AVAudioPCMBuffer *buffer;
    AVAudioPlayerNode *audioPlayer = [self loadAudioPlayerFromURL:url outBuffer:&buffer];
    
    CCColor color = [CCDanceController colorForPin:pin];
    CCBodyPart bodyPart = [CCDanceController bodyPartForPin:pin];
    
    std::map<CCColor, std::pair<AVAudioPlayerNode *, AVAudioPCMBuffer *>> &bodyPartColorToStem = _audioStems[bodyPart];
    bodyPartColorToStem.insert({ color, { audioPlayer, buffer } });
}

- (void)loadDrumStem:(CCColor)color resource:(NSString *)name {
    NSURL *url = [[NSBundle mainBundle] URLForResource:name withExtension:@"mp3"];
    AVAudioPCMBuffer *buffer;
    AVAudioPlayerNode *audioPlayer = [self loadAudioPlayerFromURL:url outBuffer:&buffer];
    _drumStems.insert({ color, { audioPlayer, buffer } });
}

- (void)loadAnimationAndAudioStemss {
    [self loadAnimationAndAudioStems:CCSignalPinHeadWhite resource:@"Body_W"];
    [self loadAnimationAndAudioStems:CCSignalPinLeftArmWhite resource:@"Lhand_W"];
    [self loadAnimationAndAudioStems:CCSignalPinRightArmWhite resource:@"Rhand_W"];
    [self loadAnimationAndAudioStems:CCSignalPinLeftLegWhite resource:@"Lfoot_W"];
    [self loadAnimationAndAudioStems:CCSignalPinRightLegWhite resource:@"Rfoot_W"];
    [self loadAnimationAndAudioStems:CCSignalPinHeadRed resource:@"Body_R"];
    [self loadAnimationAndAudioStems:CCSignalPinLeftArmRed resource:@"Lhand_R"];
    [self loadAnimationAndAudioStems:CCSignalPinRightArmRed resource:@"Rhand_R"];
    [self loadAnimationAndAudioStems:CCSignalPinLeftLegRed resource:@"Lfoot_R"];
    [self loadAnimationAndAudioStems:CCSignalPinRightLegRed resource:@"Rfoot_R"];
    [self loadAnimationAndAudioStems:CCSignalPinHeadGreen resource:@"Body_G"];
    [self loadAnimationAndAudioStems:CCSignalPinLeftArmGreen resource:@"Lhand_G"];
    [self loadAnimationAndAudioStems:CCSignalPinRightArmGreen resource:@"Rhand_G"];
    [self loadAnimationAndAudioStems:CCSignalPinLeftLegGreen resource:@"Lfoot_G"];
    [self loadAnimationAndAudioStems:CCSignalPinRightLegGreen resource:@"Rfoot_G"];
    [self loadAnimationAndAudioStems:CCSignalPinHeadYellow resource:@"Body_Y"];
    [self loadAnimationAndAudioStems:CCSignalPinLeftArmYellow resource:@"Lhand_Y"];
    [self loadAnimationAndAudioStems:CCSignalPinRightArmYellow resource:@"Rhand_Y"];
    [self loadAnimationAndAudioStems:CCSignalPinLeftLegYellow resource:@"Lfoot_Y"];
    [self loadAnimationAndAudioStems:CCSignalPinRightLegYellow resource:@"Rfoot_Y"];
    
    [self loadDrumStem:CCColorWhite resource:@"Drums_W"];
    [self loadDrumStem:CCColorYellow resource:@"Drums_Y"];
    [self loadDrumStem:CCColorGreen resource:@"Drums_G"];
    [self loadDrumStem:CCColorRed resource:@"Drums_R"];
}

- (CCColor)isAllSameColor:(BOOL *)outSameColor {
    CCColor color = CCColorBlue;
    BOOL setFirst = NO;
    
    for (auto body_color : _activeColors) {
        if (!setFirst) {
            color = body_color.second;
            setFirst = YES;
            continue;
        }
        
        if (color != body_color.second) {
            *outSameColor = NO;
            return color;
        }
    }
    
    *outSameColor = YES;
    return color;
}

- (NSString *)letterForColor:(CCColor) color {
    if (color == CCColorBlue) {
        return @"B";
    } else if (color == CCColorWhite) {
        return @"W";
    } else if (color == CCColorYellow) {
        return @"Y";
    } else if (color == CCColorGreen) {
        return @"G";
    } else {
        return @"R";
    }
}

- (std::map<CCSkeletonWeights, std::vector<std::string>>)animationsForActiveColors {
    std::map<CCSkeletonWeights, std::vector<std::string>> animations;
    
    // First check for rest or dance state
    BOOL allSameColor = NO;
    CCColor unifiedColor = [self isAllSameColor:&allSameColor];
    if (allSameColor) {
        if (unifiedColor == CCColorBlue) {
            animations.insert({ CCSkeletonAll, { "Dance_B_rest" } });
            return animations;
        }
        
        if (kDancesEnabled) {
            NSString *dance = [NSString stringWithFormat:@"Dance_%@", [self letterForColor:unifiedColor]];
            animations.insert({ CCSkeletonAll, { std::string([dance UTF8String]) } });
            return animations;
        }
    }
    
    // If we're not in rest state, then the character is "bouncing"
    
    // Activate the correct head
    NSString *head = [NSString stringWithFormat:@"Head_%@", [self letterForColor:_activeColors[CCBodyPartHead]]];
    animations[CCSkeletonHead].push_back(std::string([head UTF8String]));
    
    // Activate the correct arms
    NSString *leftArm = [NSString stringWithFormat:@"Lhand_%@", [self letterForColor:_activeColors[CCBodyPartLeftArm]]];
    animations[CCSkeletonLeftArm].push_back(std::string([leftArm UTF8String]));
    
    NSString *rightArm = [NSString stringWithFormat:@"Rhand_%@", [self letterForColor:_activeColors[CCBodyPartRightArm]]];
    animations[CCSkeletonRightArm].push_back(std::string([rightArm UTF8String]));
    
    // Activate the correct legs (the body bouncing legs)
    NSString *leftLeg = [NSString stringWithFormat:@"Lfoot_%@_withBody", [self letterForColor:_activeColors[CCBodyPartLeftLeg]]];
    animations[CCSkeletonLeftLeg].push_back(std::string([leftLeg UTF8String]));
    
    NSString *rightLeg = [NSString stringWithFormat:@"Rfoot_%@_withBody01", [self letterForColor:_activeColors[CCBodyPartRightLeg]]];
    animations[CCSkeletonRightLeg].push_back(std::string([rightLeg UTF8String]));
    
    return animations;
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

+ (VROVector4f)rgbForColor:(CCColor)color {
    switch (color) {
        case CCColorWhite:
            return kWhiteColor;
        case CCColorBlue:
            return kBlueColor;
        case CCColorRed:
            return kRedColor;
        case CCColorGreen:
            return kGreenColor;
        case CCColorYellow:
            return kYellowColor;
        default:
            return kWhiteColor;
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
    
    for (auto kv : _activeColors) {
        CCColor colorForBodyPart = kv.second;
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
    
    // Update the color of each body part
    for (auto kv : _activeColors) {
        CCBodyPart bodyPart = kv.first;
        CCColor color = kv.second;
        
        for (CCAnimationScreen *screen in self.screens) {
            VROViewScene *view = (VROViewScene *) screen.view;
            std::function<void()> task = [self, color, bodyPart, screen] {
                std::shared_ptr<CCDanceScene> danceScene = std::dynamic_pointer_cast<CCDanceScene>(screen.scene);
                [screen setBodyPart:bodyPart toColor:[CCDanceController rgbForColor:color]];
            };
            [view queueRendererTask:task];
        }
    }
    
    // Queue the updated animations
    std::map<CCSkeletonWeights, std::vector<std::string>> animations = [self animationsForActiveColors];
    for (CCAnimationScreen *screen in self.screens) {
        VROViewScene *view = (VROViewScene *) screen.view;
        std::function<void()> task = [self, screen, animations] {
            std::shared_ptr<CCDanceScene> danceScene = std::dynamic_pointer_cast<CCDanceScene>(screen.scene);
            if (danceScene) {
                danceScene->queueAnimations(animations);
            }
        };
        [view queueRendererTask:task];
    }
    
    // Iterate through all sounds: turn off those that are no longer selected, and
    // turn on those that are selected for the first time
    for (auto kv : _audioStems) {
        CCBodyPart bodyPart = kv.first;
        
        for (auto colorToAudio : kv.second) {
            // Use the render time of the first player as the master start time for all
            // players. This is how we ensure sync: by using playAtTime: with a common
            // time for all players (and by scheduling and preparing beforehand during
            // initialization).
            AVAudioPlayerNode *player = colorToAudio.second.first;
            AVAudioPCMBuffer *buffer = colorToAudio.second.second;
            
            // Note that the 'time' used in playAtTime: is a system-wide time, not the
            // time inside the audio track at which to start. In other words, playAtTime
            // means "play the audio, starting from the beginning, at the given time."
            if (isFirst) {
                masterStartTime = player.lastRenderTime.sampleTime;
                isFirst = false;
            }
            
            if (_activeColors[bodyPart] != colorToAudio.first) {
                [player stop];
                [player scheduleBuffer:buffer atTime:0 options:AVAudioPlayerNodeBufferLoops completionHandler:nil];
            } else if (![player isPlaying]) {
                AVAudioFormat *outputFormat = [player outputFormatForBus:0];
                AVAudioTime *startTime = [AVAudioTime timeWithSampleTime:masterStartTime atRate:outputFormat.sampleRate];
                [player playAtTime:startTime];
            }
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
            screen.scene->startSequence(4.583, [self] (CCScene *finishedScene) { // 9.583
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
    _activeColors[bodyPart] = [CCDanceController colorForPin:pin];
}

@end
