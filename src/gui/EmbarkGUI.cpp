#include "gui.hpp"
#include "game.hpp"

#include <easylogging++.h>

game::gui::EmbarkGUI::EmbarkGUI() {
    mAttributes = {
        {   "STR", 8},
        {   "CNS", 8},
        {   "DEX", 8},
        {   "INT", 8},
        {   "WSD", 8},
        {   "CHR", 8}
    };
    mPointsLeft = 12;
    mSelectedItem = 0;
}

game::gui::EmbarkGUI::~EmbarkGUI() {
}

bool game::gui::EmbarkGUI::capturesKeyboard() {
    return true;
}

bool game::gui::EmbarkGUI::onKeyDown(int sc, int ch) {
    if (AbstractGUI::onKeyDown(sc, ch)) {
        return true;
    }

    if (ch == SDLK_KP_2 && mSelectedItem < 5) {
        mSelectedItem++;
        G->fullRender();
        return true;
    }
    if (ch == SDLK_KP_8 && mSelectedItem > 0) {
        mSelectedItem--;
        G->fullRender();
        return true;
    }
    if ((ch == SDLK_KP_PLUS || ch == SDLK_PLUS) && mPointsLeft != 0) {
        mAttributes[mSelectedItem] = {mAttributes[mSelectedItem].first, mAttributes[mSelectedItem].second+1};
        mPointsLeft--;
        G->fullRender();
        return true;
    }
    if ((ch == SDLK_KP_MINUS || ch == SDLK_MINUS)) {
        auto a = mAttributes[mSelectedItem];
        if (a.second > 0) {
            a.second--;
            mAttributes[mSelectedItem] = a;
            mPointsLeft++;
            G->fullRender();
        }
        return true;
    }
    if (ch == SDLK_KP_ENTER || ch == SDLK_RETURN) {
        G->generatePlayerAndStart(mAttributes);
        return true;
    }

    return false;
}

void game::gui::EmbarkGUI::render() {
    auto r = GR;
    r->disableRenderTicking();

    r->fill(1, 1, gfx::SCREEN_WIDTH - 1, gfx::SCREEN_HEIGHT - 1, ' ', 0, 0xCFCFCF);

    r->renderText(2, 1, "Embark menu", 0, 0xCFCFCF);
    string pts_label = "Points left: " + std::to_string(mPointsLeft);

    r->renderText(2, gfx::SCREEN_HEIGHT-2, "[Enter] - Embark!", 0, 0xCFCFCF);

    r->renderText(2, 3, pts_label, 0, 0xCFCFCF);
    if (mPointsLeft != 0)
        r->renderText(2, 4, "(Use +/- to allocate points)", 0, 0xCFCFCF);

    char x = 'a';
    int yoff = 0;

    string n;
    for (auto it: mAttributes){
        n = "[" + std::string(1, x) + "] " + it.first + " - " + std::to_string(it.second);
        r->renderText(2, 6+yoff, n, 0, mSelectedItem == yoff ? 0xFFFFFF: 0xCFCFCF);
        yoff++;
        x++;
    }
}
