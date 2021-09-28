#include "engine/Alien.h"
#include "engine/GameObject.h"
// #include "engine/Minion.h"
#include "engine/Sprite.h"
#include "engine/InputManager.h"
#include "engine/Game.h"
#include "engine/Mat.h"

Alien::Alien(GameObject& associated, int nMinions) : Component(associated) {
    hp = 30;

    auto* sprite = new Sprite(associated, "assets/img/alien.png");

    associated.AddComponent(*sprite);

    // ?
    // auto* pivotGO = new GameObject();
    // pivotGO->box.x = 512;
    // pivotGO->box.y = 300;
    // for(int i = 0; i < nMinions; i++) {
    //     auto* minion = new Minion(associated, pivotGO, (float)(-180 + std::rand() % 360));
    //     associated.AddComponent(*minion);
    // }
}

Alien::~Alien() {} // ? Do I really need to manually free minionArray?

void Alien::Start() {
    // * Populate minionArray with "these objects" equally spaced.
}

void Alien::Update(float dt) {
    auto& inputManager = InputManager::GetInstance();
    auto& camera = Game::GetState().GetCamera();
    
    mat::Vec2 clickPos(inputManager.GetMouseX(), inputManager.GetMouseY());
    clickPos -= camera.GetPos();
    clickPos -= mat::Vec2(associated.box.w/2, associated.box.h/2);  // * Offsets mouse click to the box's center.

    if (inputManager.KeyPress(KEYS::LEFT_MOUSE_BUTTON)) {  // Shoot
        taskQueue.emplace(Action::SHOOT, clickPos.x, clickPos.y);
    }
    if (inputManager.KeyPress(KEYS::RIGHT_MOUSE_BUTTON)) {    // Move
        taskQueue.emplace(Action::MOVE, clickPos.x, clickPos.y);
    }

    if (taskQueue.size()) {
        auto& task = taskQueue.front();

        switch (task.type) {
            case task.MOVE:
            {   // Creates scope for local variables.
                const float maxSpeed = 20.0f;
                const float acc = 5.0f;
                // auto posI = associated.box.Center();
                auto posI = mat::Vec2(associated.box.x, associated.box.y);
                auto posdt = task.pos - posI;

                speed += posdt.Normalized() * acc * dt;
                speed.x = std::max(std::min(maxSpeed, speed.x), -maxSpeed);
                speed.y = std::max(std::min(maxSpeed, speed.y), -maxSpeed);
                
                auto posF = posI + speed;
                
                if (posI.DistanceTo(posF) >= posI.DistanceTo(task.pos)) {   // arrived
                    speed.x = speed.y = 0.0f;

                    associated.box.x = task.pos.x;
                    associated.box.y = task.pos.y;

                    taskQueue.pop();
                }
                else {  // add speed
                    associated.box.x += speed.x;
                    associated.box.y += speed.y; 
                }
                break;
            } 
            case task.SHOOT:
                // ! Temporary
                taskQueue.pop();
                break;
            default:
                break;
        }
    }

    if (hp <= 0) {
        associated.RequestDelete();
    }
}

void Alien::Render() {}

bool Alien::Is(std::string type) { return type == "Alien"; }

/* Inner class Action */
Alien::Action::Action(ActionType type, float x, float y) {
    this->type = type;
    this->pos = mat::Vec2(x, y);
}
