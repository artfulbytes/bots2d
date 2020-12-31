#ifndef RESOURCES_HELPER_H_
#define RESOURCES_HELPER_H_

#include <string>

/**
 * Finds the path to the resources folder. It enables the simulator binary to
 * be executed from different locations.
 */
class ResourcesHelper
{
public:
    static void init();
    static std::string getResourcesPath();
    static std::string getTexturesPath();
    static std::string getTexturesPath(std::string textureName);
    static std::string getShadersPath();
};

#endif /* RESOURCES_HELPER_H_ */
