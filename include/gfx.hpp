#ifndef INCLUDE_GFX_HPP_
#define INCLUDE_GFX_HPP_
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <array>
using std::string;
using std::vector;

namespace game {
namespace gfx {

// Colors for debug display
static constexpr std::array<int, 10> COLORS_DIST = { 0xFFFFFF, 0xFFFF00,
        0xFFAA00, 0xFF8800, 0xFF4400, 0xFF0000, 0xFF0044, 0xFF0088, 0xFF00AA,
        0xFF00FF };

// Game constants
const int TICK_INTERVAL = 100;

const int SCREEN_WIDTH = 120, SCREEN_HEIGHT = 48;
const int CHAR_WIDTH = 12, CHAR_HEIGHT = 18;
const int TILESET_CHAR_WIDTH = 8, TILESET_CHAR_HEIGHT = 12;
const int SCREEN_WIDTH_PIXELS = SCREEN_WIDTH * CHAR_WIDTH,
        SCREEN_HEIGHT_PIXELS = CHAR_HEIGHT * SCREEN_HEIGHT;

// Internal character representation struct
struct CharacterWrapper {
    int characterIndex;
    int backgroundColor;
    int foregroundColor;
};

// Sprite sheet for holding matrix of sprites
class SpriteSheet {
public:
    // Constructor
    SpriteSheet(const string& filename, int wc, int hc);
    // Destructor
    ~SpriteSheet();

    // Main draw function
    void render(int i, int x, int y, int w, int h);
    void render(int i, SDL_Rect* r);

    void applyColor(int h);
private:
    // Private load method
    void load();

    // Texture filename
    const string mFilename;
    // Size in cells
    int mWidthCells, mHeightCells;
    // Calculated cell size
    int mCellWidth, mCellHeight;
    // Real image size
    int mWidth, mHeight;
    // Pointer to real texture
    SDL_Texture* mTexture;
};

class RendererBuffer {
public:
    // Constructor
    RendererBuffer(int w, int h);
    // Destructor
    ~RendererBuffer();

    // Puts character (c, b, f) at position (x, y)
    void put(int x, int y, int c, int f, int b);

    // Returns charcter at (x, y)
    CharacterWrapper& operator()(int x, int y);

    // Clears buffer
    void clear();
private:
    // Internal array for holding characters
    vector<CharacterWrapper> mInternalBuffer;

    // Size
    int mWidth, mHeight;
};

class Renderer {
public:
    // Constructor
    Renderer();
    // Destructor
    ~Renderer();

    // Polls SDL events
    void pollEvents();

    void nextRenderMode();

    // Main render method
    void renderAll();

    // Drawing functions
    void renderLevel(int sx, int sy, int dx, int dy);
    void renderHud();
    void renderEntitiesAndAdjacent(int dx);
    void renderEntities();
    void renderGui();
    void renderLoot();

    // Character drawing functions
    int renderText(int x, int y, const string& str, int fg, int bg);
    void put(int x, int y, int c, int fg, int bg);
    void fill(int sx, int sy, int dx, int dy, int c, int fg, int bg);

    // Renders internal buffer item to screen
    void renderBufferItem(int x, int y, const CharacterWrapper& c);

    // Clears internal buffer
    void clear();

    // Auto-render
    void disableRenderTicking();
    void enableRenderTicking();

    // Returns SDL renderer object
    SDL_Renderer* sdlRenderer();

    // Loads all resources
    void loadResources();

private:
    // Tiles sprite sheet
    SpriteSheet* mTilesetTexture;
    // Internal buffer
    RendererBuffer mBuffer;
    // SDL renderer object
    SDL_Renderer* mRenderer;
    // SDL window object
    SDL_Window* mWindow;
    // SDL event object
    SDL_Event mEvent;
    // Last tick time
    long mLastTick;
    // Color filter
    int mLastColor;
    int mRenderMode;
    bool mRenderTick;
};

}
}

#endif
