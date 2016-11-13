#ifndef INCLUDE_CAMERA_HPP_
#define INCLUDE_CAMERA_HPP_

namespace game {
namespace level {

// Game Camera class
// Used to translate world coordinates to screen coordinates
class Camera {
public:
    // Constructor
    Camera(int x = 0, int y = 0);
    // Destructor
    ~Camera();

    // Position
    void setPosition(int x, int y);
    void getPosition(int& x, int& y);
    void move(int x, int y);
    void center(int x, int y);

    // Translates world point (x0,y0) to on-screen coordinates (x,y)
    void translatePoint(int x0, int y0, int& x, int& y);
private:
    // Position
    int mX, mY;
};

}
}

#endif
