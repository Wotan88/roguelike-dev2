#ifndef INCLUDE_CAMERA_HPP_
#define INCLUDE_CAMERA_HPP_

namespace game {
namespace level {

class Camera {
public:
    Camera(int x = 0, int y = 0);
    ~Camera();

    void setPosition(int x, int y);
    void getPosition(int& x, int& y);

    void translatePoint(int x0, int y0, int& x, int& y);
    void move(int x, int y);
    void center(int x, int y);
private:
    int mX, mY;
};

}
}

#endif
