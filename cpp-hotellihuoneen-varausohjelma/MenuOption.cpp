#include "MenuOption.h"

MenuOption::MenuOption(const std::string& title, std::function<void()> action) : name{ title }, action{ action } {}
MenuOption::MenuOption(const std::string& title, const std::vector<MenuOption>& sub_menu) : name{ title }, sub_menu{ sub_menu }, action{ nullptr } {}

bool MenuOption::has_sub_menu() const { return !sub_menu.empty(); }
bool MenuOption::has_action() const { return action != nullptr; }
