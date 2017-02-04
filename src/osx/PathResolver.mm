#include "../PathResolver.hpp"
#import <Foundation/Foundation.h>

// On OSX take the path and extract the file name
// then find the path the bundle resources
// and append the filename to it

std::string resolvePath(std::string originalPath)
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    std::string result = originalPath;
    NSBundle* bundle = [NSBundle mainBundle];

    if (bundle == nil) {
#ifdef DEBUG
        NSLog(@"bundle is nil. Path to bundle resources can not be found. Returning input string.");
#endif
    } else {
        NSString* originalPathNS = [NSString stringWithUTF8String:originalPath.c_str()];
        NSString* fileName = [originalPathNS lastPathComponent];
        
        NSString* bundleResourcePath = [bundle resourcePath];
        result = [bundleResourcePath UTF8String] + std::string("/") + [fileName UTF8String];
    }

    [pool drain];

    return result;
}
