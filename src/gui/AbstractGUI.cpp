#include "gui.hpp"

game::gui::AbstractGUI::AbstractGUI() {
}

game::gui::AbstractGUI::~AbstractGUI() {
}

bool game::gui::AbstractGUI::capturesKeyboard() {
    return true;
}

bool game::gui::AbstractGUI::onKeyDown(int sc, int ch) {
    return false;
}

void game::gui::AbstractGUI::render() {
}
