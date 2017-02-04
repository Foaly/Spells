

#ifndef RESOLVE_PATH_HPP
#define RESOLVE_PATH_HPP

#include <string>

////////////////////////////////////////////////////////////
/// \brief This function takes a path to a resource (like: data/images/image.png)
///        On OSX the folder structure is removed and the path to the resource folder
///        associated with the main bundle is prepended.
///        On windows the input path is simply returned.
////////////////////////////////////////////////////////////
std::string resolvePath(std::string);

#endif // RESOLVE_PATH_HPP
