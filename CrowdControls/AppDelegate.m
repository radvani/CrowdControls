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
    self.ledWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(padding, padding, windowLength, windowHeight)
                                                 styleMask:masks backing:NSBackingStoreBuffered
                                                     defer:NO];
    self.ledScreen = [[CCAnimationScreen alloc] initWithName:@"LEDScreen"];
    self.ledViewController = [[CCViewController alloc] init];
    self.ledViewController.renderDelegate = self.ledScreen;
    
    self.ledWindow.contentView = self.ledViewController.view;
    [self.ledWindow orderBack:self];
    self.ledWindow.title = @"LED Screen";
    self.ledWindowController = [[NSWindowController alloc] initWithWindow:self.ledWindow];
    
    /*
     Projector 1.
     */
    self.p1Window = [[NSWindow alloc] initWithContentRect:NSMakeRect(windowLength + 2 * padding, padding, windowLength, windowHeight)
                                                styleMask:masks backing:NSBackingStoreBuffered
                                                    defer:NO];
    self.p1Screen = [[CCAnimationScreen alloc] initWithName:@"Projector_1"];
    self.p1ViewController = [[CCViewController alloc] init];
    self.p1ViewController.renderDelegate = self.p1Screen;
    
    self.p1Window.contentView = self.p1ViewController.view;
    [self.p1Window orderBack:self];
    self.p1Window.title = @"Projector 1";
    self.p1WindowController = [[NSWindowController alloc] initWithWindow:self.p1Window];
    
    /*
     Projector 2.
     */
    self.p2Window = [[NSWindow alloc] initWithContentRect:NSMakeRect(padding, windowHeight + 2 * padding, windowLength, windowHeight)
                                                styleMask:masks backing:NSBackingStoreBuffered
                                                    defer:NO];
    self.p2Screen = [[CCAnimationScreen alloc] initWithName:@"Projector_2"];
    self.p2ViewController = [[CCViewController alloc] init];
    self.p2ViewController.renderDelegate = self.p2Screen;
    
    self.p2Window.contentView = self.p2ViewController.view;
    [self.p2Window orderBack:self];
    self.p2Window.title = @"Projector 2";
    self.p2WindowController = [[NSWindowController alloc] initWithWindow:self.p2Window];
    
    /*
     Projector 3.
     */
    self.p3Window = [[NSWindow alloc] initWithContentRect:NSMakeRect(windowLength + 2 * padding, windowHeight + 2 * padding, windowLength, windowHeight)
                                                styleMask:masks backing:NSBackingStoreBuffered
                                                    defer:NO];
    self.p3Screen = [[CCAnimationScreen alloc] initWithName:@"Projector_3"];
    self.p3ViewController = [[CCViewController alloc] init];
    self.p3ViewController.renderDelegate = self.p3Screen;
    
    self.p3Window.contentView = self.p3ViewController.view;
    [self.p3Window orderBack:self];
    self.p3Window.title = @"Projector 3";
    self.p3WindowController = [[NSWindowController alloc] initWithWindow:self.p3Window];
    
    CCSignalReader *reader = [[CCSignalReader alloc] init];
    [reader listPorts];
    [reader connect:"/dev/cu.usbmodem14411"];
    
    NSArray *animationScreens = [NSArray arrayWithObjects:self.ledScreen, self.p1Screen,
                                 self.p2Screen, self.p3Screen, nil];
    
    self.danceController = [[CCDanceController alloc] initWithAnimationScreens:animationScreens];
    reader.delegate = self.danceController;
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
