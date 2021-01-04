#include "AssetsHelper.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <array>
#include <cassert>

namespace {
    std::string assetsPath;
    const std::array<std::string, 5> allowedParentDirs {"",
                                                        "external"
                                                        "bots2d"
                                                        "libs/bots2d"
                                                        "external/bots2d"};
}

void AssetsHelper::init()
{
    struct stat info;
    const auto nestedLevelsToCheck = 5;

    /* Give some leeway for where the program is executed from. This
     * should work for both Linux and Windows */
    std::string prefixPath = "";
    for (int level = 0; level <= nestedLevelsToCheck; level++) {
        for (const auto &parentDir : allowedParentDirs) {
            const auto path = prefixPath + parentDir + "/assets/";
            if (stat(path.c_str(), &info) == 0) {
                assetsPath = path;
                return;
            }
        }
        prefixPath = "../" + prefixPath;
    }

    std::cout << "Can't find the assets/ folder"
                 "Checked for common parent dirs and several folder levels up"
                 "You can include AssetsHelper in your application and "
                 "set the path manually with AssetsHelper::setAssetsPath(<relativePath>)" << std::endl;
    assert(false);
}

void AssetsHelper::setAssetsPath(const std::string relativePath)
{
    assetsPath = relativePath;
}

std::string AssetsHelper::getAssetsPath()
{
    assert(!assetsPath.empty());
    return assetsPath;
}

std::string AssetsHelper::getTexturesPath()
{
    return getAssetsPath() + "textures/";
}

std::string AssetsHelper::getTexturePath(std::string textureName)
{
    return getTexturesPath() + textureName;
}
