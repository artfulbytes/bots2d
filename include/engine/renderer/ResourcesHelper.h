#ifndef RESOURCES_HELPER_H_
#define RESOURCES_HELPER_H_

#include <string>

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
