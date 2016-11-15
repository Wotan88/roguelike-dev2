#include "gui.hpp"
#include "game.hpp"
#include "registry.hpp"
#include "general_utils.hpp"
#include "messages.hpp"

game::gui::InventoryGUI::InventoryGUI() {
    mPlayer = GP;
    mCheckedItem = -1;
}

game::gui::InventoryGUI::~InventoryGUI() {
}

bool game::gui::InventoryGUI::capturesKeyboard() {
    return true;
}

void game::gui::InventoryGUI::performAction(int i, int name) {
    auto it = mPlayer->getItem(i);
    if (!it.item)
        return;
    game::item::AbstractItem* item = (it.item);

    switch (name) {
    case ACTION_USE:
        if (item->onUsed(mPlayer)) {
            mPlayer->updateCount(mCheckedItem, -1);
            G->setGui(nullptr);
        }
        return;
        case ACTION_EQUIP:
        if (item->isEquipable()) {
            if (item->getProperty<string>("class", "") == "MissileAmmo") {
                int oldCount = mPlayer->missileAmmoCount;
                item::AbstractItem* oldItem = mPlayer->missileAmmoItem;

                mPlayer->missileAmmoCount = it.count;
                mPlayer->missileAmmoItem = item;

                mPlayer->setSlot(mCheckedItem, oldItem, oldCount);
                messages::push("You equip " + item->getProperty<string>("name", "") + " in your missile slot");
                G->setGui(nullptr);
                return;
            }

            int slot = item->getProperty<int>("equipSlot", -1);
            if (slot >= 0) {
                auto sl = mPlayer->getEquipedItem(slot);
                if (sl) {
                    messages::push("Unequip item first.");
                } else {
                    messages::push("You equip " + item->getProperty<string>("name", "item"));
                    mPlayer->setEquipmentItem(slot, item);
                    mPlayer->setSlot(mCheckedItem, nullptr, 0);
                }
                G->setGui(nullptr);
            }
        }
        return;
    }
}

bool game::gui::InventoryGUI::onKeyDown(int sc, int ch) {
    if (sc == SDL_SCANCODE_ESCAPE) {
        if (mCheckedItem == -1) {
            G->setGui(nullptr);
            G->fullRender();
        } else {
            mCheckedItem = -1;
            G->fullRender();
        }
        return true;
    }

    if (mCheckedItem < 0 && sc == SDL_SCANCODE_MINUS) {
        if (mPlayer->hasMissileAmmo()) {
            int cnt = mPlayer->missileAmmoCount;
            auto it = mPlayer->missileAmmoItem;
            if (mPlayer->addItemI(it, cnt)) {
                mPlayer->missileAmmoCount = 0;
                mPlayer->missileAmmoItem = nullptr;
            }
            G->fullRender();
        }
        return true;
    }

    if (mCheckedItem < 0 && sc >= SDL_SCANCODE_0 && sc <= SDL_SCANCODE_0 + mPlayer->getEquipmentInventorySize() - 1) {
        int i = sc - SDL_SCANCODE_0;
        if (mPlayer->hasItemEquiped(i)) {
            item::AbstractItem* it = mPlayer->getEquipedItem(i);
            if (mPlayer->addItemI(it, 1)) {
                LOG(DEBUG)<< "Successfully unequiped";
                mPlayer->setEquipmentItem(i, 0);
                G->fullRender();
            }
        }
        return true;
    }

    if (sc >= SDL_SCANCODE_A && sc <= SDL_SCANCODE_Z && mCheckedItem >= 0) {
        int i = (sc - SDL_SCANCODE_A) + 'a';
        auto it = std::find_if(mActions.begin(), mActions.end(), [i](std::tuple<char, int, string>& t)-> bool {
                    return i == std::get<0>(t);
                });

        if (it != mActions.end()) {
            int actionIndex = std::get<1>(*it);
            performAction(mCheckedItem, actionIndex);
        }
        return true;
    }

    if (sc >= SDL_SCANCODE_A && sc <= SDL_SCANCODE_A + mPlayer->getInventorySize() - 1) {
        int i = sc - SDL_SCANCODE_A;
        item::InventoryItem it = mPlayer->getItem(i);
        if (it.item && it.count) {
            game::item::AbstractItem* g = (it.item);
            mActions.clear();

            if (g->isUsable()) {
                char n = g->getProperty<string>("useKey", "u")[0];
                mActions.push_back(std::tuple<char, int, string> {n, ACTION_USE, g->getProperty<string>("useAction", "Use")});
            }
            if (g->isEquipable()) {
                char n = g->getProperty<string>("equipKey", "e")[0];
                mActions.push_back(std::tuple<char, int, string> {n, ACTION_EQUIP, g->getProperty<string>("equipAction", "Equip")});
            }

            mCheckedItem = i;
            G->fullRender();
        }
        return true;
    }

    return true;
}

void game::gui::InventoryGUI::render() {
    static string lipsum =
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    auto r = GR;
    r->disableRenderTicking();

    r->fill(4, 3, gfx::SCREEN_WIDTH - 4, gfx::SCREEN_HEIGHT - 3, ' ', 0,
            0xABABAB);
    r->fill(5, 4, gfx::SCREEN_WIDTH - 5, gfx::SCREEN_HEIGHT - 4, ' ', 0,
            0xCFCFCF);

    r->renderText(5, 3, "Inventory", 0, 0xABABAB);

    r->fill(4, 16, gfx::SCREEN_WIDTH - 4, 17, ' ', 0, 0xABABAB);

    r->renderText(5, 16, "Equipment", 0, 0xABABAB);

    int c = mPlayer->getInventorySize();
    item::InventoryItem item;
    string s;
    string n;

    for (int i = 0; i < c; i++) {
        item = mPlayer->getItem(i);
        s = "[" + std::string(1, 'a' + i) + "] ";
        if (!item.item) {
            s += "Empty slot";
        } else {
            game::item::AbstractItem* i = (item.item);

            n = i->getProperty<string>("name", "noname item");
            if (item.count > 1) {
                s += std::to_string(item.count) + " "
                        + i->getProperty<string>("namePlural", n);
            } else {
                s += n;
            }
        }

        r->renderText(6, i + 5, s, 0, 0xCFCFCF);
    }

    string missileSlot = "[-] Missile slot: ";

    if (mPlayer->missileAmmoCount <= 0 && !mPlayer->missileAmmoItem) {
        missileSlot += "Nothing";
    } else {
        n = mPlayer->missileAmmoItem->getProperty<string>("name",
                "noname item");
        if (mPlayer->missileAmmoCount > 1) {
            missileSlot += std::to_string(mPlayer->missileAmmoCount) + " "
                    + mPlayer->missileAmmoItem->getProperty<string>(
                            "namePlural", n);
        } else {
            missileSlot += n;
        }
    }

    r->renderText(6, 28, missileSlot, 0, 0xCFCFCF);

    c = mPlayer->getEquipmentInventorySize();

    for (int i = 0; i < c; i++) {
        item::AbstractItem* it = mPlayer->getEquipedItem(i);
        s = "[" + std::string(1, '0' + i) + "] "
                + mPlayer->getEquipmentSlotName(i) + ": ";
        if (!it) {
            s += "Nothing";
        } else {
            n = it->getProperty<string>("name", "noname item");
            if (item.count > 1) {
                s += std::to_string(item.count) + " "
                        + it->getProperty<string>("namePlural", n);
            } else {
                s += n;
            }
        }

        r->renderText(6, i + 18, s, 0, 0xCFCFCF);
    }

    string usageNotes = game::util::wrap(
            "[" + string(1, 'a') + "-"
                    + string(1, 'a' + mPlayer->getInventorySize())
                    + "] - Select item, [Esc] - Close inventory", 50);
    int nn = usageNotes.find('\n');
    r->renderText(
            (gfx::SCREEN_WIDTH - (nn == -1 ? usageNotes.length() : nn)) / 2,
            gfx::SCREEN_HEIGHT - 6, usageNotes, 0, 0xCFCFCF);

    // TODO: remove this
    // Just use free space, LOL
    r->renderText((gfx::SCREEN_WIDTH - 80) / 2, gfx::SCREEN_HEIGHT - 13, game::util::wrap(lipsum, 80), 0, 0xCFCFCF);

    if (mCheckedItem != -1) {
        game::item::InventoryItem item = mPlayer->getItem(mCheckedItem);
        game::item::AbstractItem* itemI = (item.item);

        if (!itemI) {
            LOG(FATAL)<< "itemI == null";
        }
        int sx = gfx::SCREEN_WIDTH / 2 - 18;
        int sy = gfx::SCREEN_HEIGHT / 2 - 8;
        int ex = gfx::SCREEN_WIDTH / 2 + 18;
        int ey = gfx::SCREEN_HEIGHT / 2 + 8;

        string wrapped = game::util::wrap(
                itemI->getProperty<string>("desc", "No description"), ex - sx);

        r->fill(sx, sy, ex, ey, ' ', 0, 0xABABAB);
        r->renderText(sx + 1, sy, "Choose action", 0, 0xABABAB);
        r->renderText(sx, sy + 2, wrapped, 0, 0xABABAB);
        string txt;
        int xoff = 0;
        for (auto a : mActions) {
            txt = "[" + string(1, std::get<0>(a)) + "] " + std::get<2>(a);
            r->renderText(xoff + sx, ey - 1, txt, 0, 0xABABAB);
            xoff += txt.length();
        }
    }
}
