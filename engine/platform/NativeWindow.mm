#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#include "NativeWindow.h"

void* getMetalLayer(void* nsWindow) {

    NSWindow* window = (__bridge NSWindow*)nsWindow;
    NSView* view = window.contentView;

    [view setWantsLayer:YES];

    if (![view.layer isKindOfClass:[CAMetalLayer class]]) {
        CAMetalLayer* metalLayer = [CAMetalLayer layer];
        metalLayer.frame = view.bounds;
        metalLayer.opaque = YES;
        view.layer = metalLayer;
    }

    view.layer.frame = view.bounds;

    return (__bridge void*)view.layer;
}