/// Spells - a game about magic spells
/// Copyright (C) 2015 - 2017  Foaly

/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.

/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.

/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
