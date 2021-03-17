#ifndef IMGUI_MENU_H_
#define IMGUI_MENU_H_

#include <string>
#include <functional>
#include <vector>
#include <memory>

class Scene;

/**
 * A custom GUI menu, which can be added to a scene. Useful for tweaking
 * parameters of scene objects or showing information about a simulated scene.
 *
 * The menu items are placed in the order they are added (from top to bottom).
 */
class ImGuiMenu
{
public:
    struct Item
    {
        enum class Type { Label, Button, Slider };
        Item(Type type) : type(type) {};
        const Type type = Type::Label;
    };

    ImGuiMenu(Scene *scene, std::string name, float x, float y, float width, float height);
    virtual ~ImGuiMenu() = default;
    void render();
    /**
     * \param setText gives a function pointer for changing the text after the
     * label has been created.
     */
    void addLabel(std::string text, std::function<void(std::string)> *setText = nullptr);
    void addButton(std::string text, std::function<void()> onClicked);
    void addSlider(std::string name, float min, float max, float startValue, std::function<void(float)> onChanged);

private:
    std::vector<std::unique_ptr<Item>> m_menuItems;
    const std::string m_name = "";
    const float m_posX = 0.0f;
    const float m_posY = 0.0f;
    const float m_width = 100.0f;
    const float m_height = 100.0f;
};

#endif /* IMGUI_MENU_H_ */
