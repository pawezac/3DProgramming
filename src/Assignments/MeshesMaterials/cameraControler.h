#include "camera.h"

class CameraControler {
public:
    CameraControler() : _camera(nullptr), _scale(0.01), LMB_pressed_(false) {}

    CameraControler(Camera* camera) : _camera(camera), _scale(0.01), LMB_pressed_(false) {}

    void set_camera(Camera* camera) { _camera = camera; }

    void rotate_camera(float dx, float dy) {
        _camera->rotate_around_center(-_scale * dy, _camera->x());
        _camera->rotate_around_center(-_scale * dx, _camera->y());
    }

    void mouse_moved(float x, float y) {
        if (LMB_pressed_) {
            auto dx = x - _x;
            auto dy = y - _y;
            _x = x;
            _y = y;

            rotate_camera(dx, dy);
        }
    }

    void LMB_pressed(float x, float y) {
        LMB_pressed_ = true;
        _x = x;
        _y = y;
    }

    void LMB_released(float x, float y) {
        LMB_pressed_ = false;
        auto dx = x - _x;
        auto dy = y - _y;

        rotate_camera(dx, dy);
    }

private:
    Camera* _camera;
    bool LMB_pressed_;
    float _x;
    float _y;
    float _scale;
};