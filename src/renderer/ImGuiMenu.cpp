#include "ImGuiMenu.h"
#include "ImGuiOverlay.h"
#include "Scene.h"
#include <cassert>


namespace {
struct MenuLabel : ImGuiMenu::Item
{
    MenuLabel(std::string text) :
        Item(Item::Type::Label), text(text) {}
    ~MenuLabel() = default;
    void setText(std::string newText) { text = newText; }
    std::string text;
};

struct MenuButton : ImGuiMenu::Item
{
    MenuButton(std::string text, std::function<void()> onClick) :
        Item(Item::Type::Button), text(text), onClick(onClick) {}
    ~MenuButton() = default;
    const std::string text;
    const std::function<void()> onClick;
};

struct MenuSlider : ImGuiMenu::Item
{
    MenuSlider(std::string name, float min, float max, float startValue, std::function<void(float)> onChanged) :
        Item(Item::Type::Slider), name(name), min(min),
        max(max), currentValue(startValue), onChanged(onChanged) {}
    ~MenuSlider() = default;
    bool hasValueChanged() const { return currentValue != oldValue; }
    const std::string name;
    float min = 0.0f;
    float max = 1.0f;
    float currentValue = 0.0f;
    float oldValue = 0.0f;
    const std::function<void(float)> onChanged;
};
}

ImGuiMenu::ImGuiMenu(Scene *scene, std::string name, float x, float y, float width, float height) :
    m_name(name), m_posX(x), m_posY(y), m_width(width), m_height(height)
{
    assert(scene != nullptr);
    scene->addMenu(this);
}

void ImGuiMenu::render()
{
    ImGuiOverlay::begin(m_name.c_str(), m_posX, m_posY, m_width, m_height);
    for (const auto &item : m_menuItems) {
        switch (item->type) {
        case Item::Type::Label:
        {
            const auto label = static_cast<const MenuLabel *>(item.get());
            ImGuiOverlay::text(label->text);
            break;
        }
        case Item::Type::Button:
        {
            const auto button = static_cast<const MenuButton *>(item.get());
            if (ImGuiOverlay::button(button->text)) {
                if (button->onClick) {
                    button->onClick();
                }
            }
            break;
        }
        case Item::Type::Slider:
        {
            const auto slider = static_cast<MenuSlider *>(item.get());
            ImGuiOverlay::sliderFloat(slider->name, &slider->currentValue, slider->min, slider->max);
            if (slider->hasValueChanged()) {
                if (slider->onChanged) {
                    slider->onChanged(slider->currentValue);
                }
                slider->oldValue = slider->currentValue;
            }
            break;
        }
        }
    }

    ImGuiOverlay::end();
}

void ImGuiMenu::addButton(std::string text, std::function<void()> onClicked)
{
    m_menuItems.push_back(std::make_unique<MenuButton>(text, onClicked));
}

void ImGuiMenu::addLabel(std::string text, std::function<void(std::string)> *setText)
{
    auto menuLabel = std::make_unique<MenuLabel>(text);
    if (setText) {
        auto menuLabelPtr = menuLabel.get();
        *setText = [menuLabelPtr](std::string text)
        {
            menuLabelPtr->setText(text);
        };
    }
    m_menuItems.push_back(std::move(menuLabel));
}

void ImGuiMenu::addSlider(std::string name, float min, float max, float startValue, std::function<void(float)> onChanged)
{
    assert(min <= startValue && startValue <= max);
    m_menuItems.push_back(std::make_unique<MenuSlider>(name, min, max, startValue, onChanged));
}
