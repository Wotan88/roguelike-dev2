#include "gui.hpp"

#include <easylogging++.h>

game::gui::AbstractGUI::AbstractGUI() {
    LOG(DEBUG)<< "GUI created";
}

game::gui::AbstractGUI::~AbstractGUI() {
    LOG(DEBUG)<< "GUI destroyed";
}

bool game::gui::AbstractGUI::capturesKeyboard() {
    return true;
}

bool game::gui::AbstractGUI::onKeyDown(int sc, int ch) {
    return false;
}

void game::gui::AbstractGUI::render() {
}
