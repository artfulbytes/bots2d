#include "ResourcesHelper.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <array>
#include <cassert>

namespace {
    std::string resourcesPath;
}

void ResourcesHelper::init()
{
    /* Give some leeway for where the program is executed from. This
     * should work for both Linux and Windows */
    struct stat info;

    std::array<std::string, 4> pathsToCheck {"./resources/", "../resources/",
                                             "../../resources/", "../../../resources/"};
    for (const auto &path : pathsToCheck) {
        if (stat(path.c_str(), &info) == 0) {
            resourcesPath = path;
            return;
        }
    }
    std::cout << "Couldn't find resources folder" << std::endl;
    std::cout << "You must run the program from project folder or at most 3 nested levels down" << std::endl;
    assert(false);
}

void ResourcesHelper::destroy()
{
    resourcesPath.clear();
}

std::string ResourcesHelper::getResourcesPath()
{
    assert(!resourcesPath.empty());
    return resourcesPath;
}

std::string ResourcesHelper::getTexturesPath()
{
    return getResourcesPath() + "textures/";
}

std::string ResourcesHelper::getTexturesPath(std::string textureName)
{
    return getTexturesPath() + textureName;
}

std::string ResourcesHelper::getShadersPath()
{
    return getResourcesPath() + "shaders/";
}
