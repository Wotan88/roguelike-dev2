#include "gui.hpp"
#include "game.hpp"
#include "registry.hpp"

game::gui::InventoryGUI::InventoryGUI() {
    mPlayer = GP;
}

game::gui::InventoryGUI::~InventoryGUI() {
}

bool game::gui::InventoryGUI::capturesKeyboard() {
    return true;
}

bool game::gui::InventoryGUI::onKeyDown(int sc, int ch) {
    if (sc == SDL_SCANCODE_ESCAPE) {
        G->setGui(nullptr);
        G->fullRender();
        return true;
    }

    return true;
}

void game::gui::InventoryGUI::render() {
    auto r = GR;
    r->disableRenderTicking();

    r->fill(4, 3, gfx::SCREEN_WIDTH - 4, gfx::SCREEN_HEIGHT - 3, ' ', 0, 0xABABAB);
    r->fill(5, 4, gfx::SCREEN_WIDTH - 5, gfx::SCREEN_HEIGHT - 4, ' ', 0, 0xCFCFCF);

    r->renderText(5, 3, "Inventory", 0, 0xABABAB);

    int c = mPlayer->getInventorySize();
    item::InventoryItem item;
    string s;
    string n;

    for (int i = 0; i < c; i++){
        item = mPlayer->getItem(i);
        s = "[" + std::string(1, 'a' + i) + "] ";
        if (item.itemId == 0){
            s += "Empty slot";
        } else {
            game::item::AbstractItem* i = itemregistry::byId(item.itemId);

            n = i->getProperty<string>("name", "noname item");
            if (item.count > 1){
                s += std::to_string(item.count) + " " + i->getProperty<string>("namePlural", n);
            } else {
                s += n;
            }
        }

        r->renderText(6, i + 5, s, 0, 0xCFCFCF);
    }
}
