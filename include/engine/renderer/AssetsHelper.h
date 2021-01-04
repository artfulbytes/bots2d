#ifndef ASSETS_HELPER_H_
#define ASSETS_HELPER_H_

#include <string>

/**
 * Finds the path to the resources folder. It enables the simulator binary to
 * be executed from different locations.
 */
class AssetsHelper
{
public:
    static void init();
    void setAssetsPath(const std::string relativePath);
    static std::string getAssetsPath();
    static std::string getTexturesPath();
    static std::string getTexturePath(std::string textureName);
};

#endif /* ASSETS__HELPER_H_ */
