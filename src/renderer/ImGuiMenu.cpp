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
}

ImGuiMenu::ImGuiMenu(Scene *scene, std::string name, float x, float y) :
    m_name(name), m_posX(x), m_posY(y)
{
    assert(scene != nullptr);
    scene->addMenu(this);
}

void ImGuiMenu::render()
{
    ImGuiOverlay::begin(m_name.c_str(), m_posX, m_posY);
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
