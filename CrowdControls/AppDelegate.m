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

@interface AppDelegate ()

@property (nonatomic, strong) NSWindowController *ledWindowController;
@property (nonatomic, strong) NSWindow *ledWindow;
@property (nonatomic, strong) CCViewController *ledViewController;
@property (nonatomic, strong) CCRenderDelegate *ledRenderDelegate;

@property (nonatomic, strong) NSWindowController *p1WindowController;
@property (nonatomic, strong) NSWindow *p1Window;
@property (nonatomic, strong) CCViewController *p1ViewController;
@property (nonatomic, strong) CCRenderDelegate *p1RenderDelegate;

@property (nonatomic, strong) NSWindowController *p2WindowController;
@property (nonatomic, strong) NSWindow *p2Window;
@property (nonatomic, strong) CCViewController *p2ViewController;
@property (nonatomic, strong) CCRenderDelegate *p2RenderDelegate;

@property (nonatomic, strong) NSWindowController *p3WindowController;
@property (nonatomic, strong) NSWindow *p3Window;
@property (nonatomic, strong) CCViewController *p3ViewController;
@property (nonatomic, strong) CCRenderDelegate *p3RenderDelegate;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSUInteger masks = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable |
                       NSWindowStyleMaskUnifiedTitleAndToolbar | NSWindowStyleMaskTexturedBackground;
    
    NSRect windowSize = NSMakeRect(20, 20, 600, 600);
    
    
    /*
     LED Screen.
     */
    self.ledWindow = [[NSWindow alloc] initWithContentRect:windowSize styleMask:masks backing:NSBackingStoreBuffered
                                                     defer:NO];
    self.ledRenderDelegate = [[CCRenderDelegate alloc] init];
    self.ledViewController = [[CCViewController alloc] init];
    self.ledViewController.renderDelegate = self.ledRenderDelegate;
    
    self.ledWindow.contentView = self.ledViewController.view;
    [self.ledWindow orderFront:self];
    self.ledWindow.title = @"LED Screen";
    self.ledWindowController = [[NSWindowController alloc] initWithWindow:self.ledWindow];
    
    /*
     Projector 1.
     */
    self.p1Window = [[NSWindow alloc] initWithContentRect:windowSize styleMask:masks backing:NSBackingStoreBuffered
                                                    defer:NO];
    self.p1RenderDelegate = [[CCRenderDelegate alloc] init];
    self.p1ViewController = [[CCViewController alloc] init];
    self.p1ViewController.renderDelegate = self.p1RenderDelegate;
    
    self.p1Window.contentView = self.p1ViewController.view;
    [self.p1Window orderFront:self];
    self.p1Window.title = @"Projector 1";
    self.p1WindowController = [[NSWindowController alloc] initWithWindow:self.p1Window];
    
    /*
     Projector 2.
     */
    self.p2Window = [[NSWindow alloc] initWithContentRect:windowSize styleMask:masks backing:NSBackingStoreBuffered
                                                    defer:NO];
    self.p2RenderDelegate = [[CCRenderDelegate alloc] init];
    self.p2ViewController = [[CCViewController alloc] init];
    self.p2ViewController.renderDelegate = self.p2RenderDelegate;
    
    self.p2Window.contentView = self.p2ViewController.view;
    [self.p2Window orderFront:self];
    self.p2Window.title = @"Projector 2";
    self.p2WindowController = [[NSWindowController alloc] initWithWindow:self.p2Window];
    
    /*
     Projector 3.
     */
    self.p3Window = [[NSWindow alloc] initWithContentRect:windowSize styleMask:masks backing:NSBackingStoreBuffered
                                                    defer:NO];
    self.p3RenderDelegate = [[CCRenderDelegate alloc] init];
    self.p3ViewController = [[CCViewController alloc] init];
    self.p3ViewController.renderDelegate = self.p3RenderDelegate;
    
    self.p3Window.contentView = self.p3ViewController.view;
    [self.p3Window orderFront:self];
    self.p3Window.title = @"Projector 3";
    self.p3WindowController = [[NSWindowController alloc] initWithWindow:self.p3Window];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
