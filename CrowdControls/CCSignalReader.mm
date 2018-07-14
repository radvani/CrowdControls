//
//  CCSignalReader.cpp
//  CrowdControls
//
//  Created by Raj Advani on 7/14/18.
//  Copyright © 2018 Raj Advani. All rights reserved.
//

#include "CCSignalReader.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/serial/ioss.h>
#include <IOKit/IOBSD.h>
#include <ViroKit/ViroKit.h>

static const int kPinCapacity = 50;

@implementation CCSignalReader {
    int _serialFileDescriptor;
    int pins[kPinCapacity];
}

- (id)init {
    self = [super init];
    if (self) {
        for (int i = 0; i < kPinCapacity; i++) {
            pins[i] = -1;
        }
    }
    return self;
}

- (void)listPorts {
    NSLog(@"Listing available ports");
    io_object_t serialPort;
    io_iterator_t serialPortIterator;
    
    // ask for all the serial ports
    IOServiceGetMatchingServices(kIOMasterPortDefault,
                                 IOServiceMatching(kIOSerialBSDServiceValue),
                                 &serialPortIterator);
    
    // loop through all the serial ports
    while ((serialPort = IOIteratorNext(serialPortIterator))) {
        // you want to do something useful here
        NSLog(@"Found available port %@", (__bridge NSString *) IORegistryEntryCreateCFProperty(serialPort, CFSTR(kIOCalloutDeviceKey),
                                                                                                kCFAllocatorDefault, 0));
        IOObjectRelease(serialPort);
    }
    IOObjectRelease(serialPortIterator);
}

- (void)connect:(std::string)port {
    NSLog(@"Connecting to port %s", port.c_str());
    
    _serialFileDescriptor = open(port.c_str(), O_RDWR |O_NOCTTY | O_NONBLOCK );
    if (_serialFileDescriptor < 0) {
        NSLog(@"Failed to connect to port");
        return;
    }
    
    // Block non-root users from using this port
    ioctl(_serialFileDescriptor, TIOCEXCL);
    
    // clear the O_NONBLOCK flag, so that read() will
    //   block and wait for data.
    fcntl(_serialFileDescriptor, F_SETFL, 0);
    
    // grab the options for the serial port
    struct termios options;
    tcgetattr(_serialFileDescriptor, &options);
    
    // Setting raw-mode allows the use of tcsetattr() and ioctl()
    cfmakeraw(&options);
    
    // Specify any arbitrary baud rate
    int baudRate = 9600;
    ioctl(_serialFileDescriptor, IOSSIOSPEED, &baudRate);
    
    // Start a thread to read the data coming in
    [self performSelectorInBackground:@selector(incomingTextUpdateThread:) withObject:[NSThread currentThread]];
}

- (void)incomingTextUpdateThread:(NSThread *)parentThread {
    VROByteBuffer buffer(100);
    ssize_t numBytes = 1;
    
    bool readingPin = true;
    int pin = 0;
    int signal = 0;
    
    // Loops until the serial port closes
    while (numBytes > 0) {
        // Blocks until data is read or the port is closed
        numBytes = read(_serialFileDescriptor, buffer.getData(), 100);
        
        for (int i = 0; i < numBytes; i++) {
            if (readingPin) {
                pin = buffer.readByte();
            } else {
                signal = buffer.readByte();
                
                if (pins[pin] != signal) {
                    pins[pin] = signal;
                    if (self.delegate) {
                        [self.delegate pin:(CCSignalPin)pin didChangeSignal:signal];
                    }
                }
            }
            readingPin = !readingPin;
        }
        buffer.rewind();
    }
}

@end
