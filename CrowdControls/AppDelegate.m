//
//  AppDelegate.m
//  CrowdControls
//
//  Created by Raj Advani on 5/13/18.
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

#import "AppDelegate.h"
#import "CCSignalReader.h"
#import "CCDanceController.h"
#import "CCDanceScene.h"
#import "CCCountdownScene.h"

@interface AppDelegate ()

@property (nonatomic, strong) NSWindowController *ledWindowController;
@property (nonatomic, strong) NSWindow *ledWindow;
@property (nonatomic, strong) CCViewController *ledViewController;
@property (nonatomic, strong) CCAnimationScreen *ledScreen;

@property (nonatomic, strong) NSWindowController *p1WindowController;
@property (nonatomic, strong) NSWindow *p1Window;
@property (nonatomic, strong) CCViewController *p1ViewController;
@property (nonatomic, strong) CCAnimationScreen *p1Screen;

@property (nonatomic, strong) NSWindowController *p2WindowController;
@property (nonatomic, strong) NSWindow *p2Window;
@property (nonatomic, strong) CCViewController *p2ViewController;
@property (nonatomic, strong) CCAnimationScreen *p2Screen;

@property (nonatomic, strong) NSWindowController *p3WindowController;
@property (nonatomic, strong) NSWindow *p3Window;
@property (nonatomic, strong) CCViewController *p3ViewController;
@property (nonatomic, strong) CCAnimationScreen *p3Screen;

@property (nonatomic, strong) NSWindowController *countdownWindowController;
@property (nonatomic, strong) NSWindow *countdownWindow;
@property (nonatomic, strong) CCViewController *countdownViewController;
@property (nonatomic, strong) CCAnimationScreen *countdownScreen;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSUInteger masks = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable |
                       NSWindowStyleMaskUnifiedTitleAndToolbar | NSWindowStyleMaskTexturedBackground;
    
    int windowLength = 800;
    int windowHeight = 500;
    int padding = 15;
    
    /*
     LED Screen.
     */
    std::shared_ptr<CCDanceScene> ledScene = std::make_shared<CCDanceScene>();
    self.ledWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(padding, padding, windowLength, windowHeight)
                                                 styleMask:masks backing:NSBackingStoreBuffered
                                                     defer:NO];
    self.ledScreen = [[CCAnimationScreen alloc] initWithName:@"LEDScreen" scene:ledScene];
    self.ledViewController = [[CCViewController alloc] init];
    self.ledViewController.renderDelegate = self.ledScreen;
    
    self.ledWindow.contentView = self.ledViewController.view;
    [self.ledWindow orderBack:self];
    self.ledWindow.title = @"LED Screen";
    self.ledWindowController = [[NSWindowController alloc] initWithWindow:self.ledWindow];
    
    /*
     Projector 1.
     */
    std::shared_ptr<CCDanceScene> proj1Scene = std::make_shared<CCDanceScene>();
    self.p1Window = [[NSWindow alloc] initWithContentRect:NSMakeRect(windowLength + 2 * padding, padding, windowLength, windowHeight)
                                                styleMask:masks backing:NSBackingStoreBuffered
                                                    defer:NO];
    self.p1Screen = [[CCAnimationScreen alloc] initWithName:@"Projector_1" scene:proj1Scene];
    self.p1ViewController = [[CCViewController alloc] init];
    self.p1ViewController.renderDelegate = self.p1Screen;
    
    self.p1Window.contentView = self.p1ViewController.view;
    [self.p1Window orderBack:self];
    self.p1Window.title = @"Projector 1";
    self.p1WindowController = [[NSWindowController alloc] initWithWindow:self.p1Window];
    
    /*
     Projector 2.
     */
    std::shared_ptr<CCDanceScene> proj2Scene = std::make_shared<CCDanceScene>();
    self.p2Window = [[NSWindow alloc] initWithContentRect:NSMakeRect(padding, windowHeight + 2 * padding, windowLength, windowHeight)
                                                styleMask:masks backing:NSBackingStoreBuffered
                                                    defer:NO];
    self.p2Screen = [[CCAnimationScreen alloc] initWithName:@"Projector_2" scene:proj2Scene];
    self.p2ViewController = [[CCViewController alloc] init];
    self.p2ViewController.renderDelegate = self.p2Screen;
    
    self.p2Window.contentView = self.p2ViewController.view;
    [self.p2Window orderBack:self];
    self.p2Window.title = @"Projector 2";
    self.p2WindowController = [[NSWindowController alloc] initWithWindow:self.p2Window];
    
    /*
     Projector 3.
     */
    std::shared_ptr<CCDanceScene> proj3Scene = std::make_shared<CCDanceScene>();
    self.p3Window = [[NSWindow alloc] initWithContentRect:NSMakeRect(windowLength + 2 * padding, windowHeight + 2 * padding, windowLength, windowHeight)
                                                styleMask:masks backing:NSBackingStoreBuffered
                                                    defer:NO];
    self.p3Screen = [[CCAnimationScreen alloc] initWithName:@"Projector_3" scene:proj3Scene];
    self.p3ViewController = [[CCViewController alloc] init];
    self.p3ViewController.renderDelegate = self.p3Screen;
    
    self.p3Window.contentView = self.p3ViewController.view;
    [self.p3Window orderFront:self];
    self.p3Window.title = @"Projector 3";
    self.p3WindowController = [[NSWindowController alloc] initWithWindow:self.p3Window];
    
    /*
     Countdown.
     */
    std::shared_ptr<CCCountdownScene> countdownScene = std::make_shared<CCCountdownScene>();
    self.countdownWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(windowLength / 2.0 + padding, 0, windowLength, windowHeight)
                                                styleMask:masks backing:NSBackingStoreBuffered
                                                    defer:NO];
    self.countdownScreen = [[CCAnimationScreen alloc] initWithName:@"Countdown" scene:countdownScene];
    self.countdownViewController = [[CCViewController alloc] init];
    self.countdownViewController.renderDelegate = self.countdownScreen;
    
    self.countdownWindow.contentView = self.countdownViewController.view;
    [self.countdownWindow orderFront:self];
    self.countdownWindow.title = @"Countdown";
    self.countdownWindowController = [[NSWindowController alloc] initWithWindow:self.countdownWindow];
    
    CCSignalReader *reader = [[CCSignalReader alloc] init];
    [reader listPorts];
    [reader connect:"/dev/cu.usbmodem14411"];
    
    NSArray *animationScreens = [NSArray arrayWithObjects:self.ledScreen, self.p1Screen,
                                 self.p2Screen, self.p3Screen, self.countdownScreen, nil];
    
    self.danceController = [[CCDanceController alloc] initWithAnimationScreens:animationScreens];
    reader.delegate = self.danceController;
    
    // Give the sound engine some time to buffer. Note that start animation sequence
    // (which drives the loop) is operated on the countdown scene's rendering thread
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(6 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [(VROViewScene *) self.countdownScreen.view queueRendererTask:[self] {
            [self.danceController startAnimationSequence];
        }];
    });
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
