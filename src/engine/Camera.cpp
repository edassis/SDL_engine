#include "engine/Camera.h"

#include "engine/Game.h"
#include "engine/GameObject.h"
#include "engine/InputManager.h"

Camera::Camera() {}

Camera::~Camera() {}

void Camera::Follow(std::weak_ptr<GameObject> newFocus) {
    auto spNewFocus = newFocus.lock();
    
    if (spNewFocus == nullptr) {
        std::cout << "Warning! Camera::Follow() has nullptr in newFocus." << std::endl;
        return;
    }

    pos = mat::Vec2(spNewFocus->box.x, spNewFocus->box.y);
    
    focus = std::move(newFocus);
}

void Camera::Unfollow() {
    focus.reset();
}

void Camera::Update(float dt) {
    mat::Rect screenArea(0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

    if (auto spFocus = focus.lock()) {    // Camera attached to some GameObject
        auto objCenter = spFocus->box.Center();
        auto screenCenter = screenArea.Center();

        pos = mat::Vec2( -(screenCenter.x - objCenter.x), -(screenCenter.y - objCenter.y) ); 
    }
    else {  // Camera on free mode
        const float velocity = 210.0f;

        if (InputManager::GetInstance().IsKeyDown(KEYS::UP_ARROW_KEY)) {
            pos.y -= velocity * dt;
        }
        if (InputManager::GetInstance().IsKeyDown(KEYS::LEFT_ARROW_KEY)) {
            pos.x -= velocity * dt;
        }
        if (InputManager::GetInstance().IsKeyDown(KEYS::DOWN_ARROW_KEY)) {
            pos.y += velocity * dt;
        }
        if (InputManager::GetInstance().IsKeyDown(KEYS::RIGHT_ARROW_KEY)) {
            pos.x += velocity * dt;
        }
    }
}

mat::Vec2 Camera::GetPos() {
    return pos;
}

mat::Vec2 Camera::GetSpeed() {
    return speed;
}