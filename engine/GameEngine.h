
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

namespace CMPUT350 {
class GameEngine;
}

#include "GameContext.h"
#include "EngineView.h"
#include "GameObject.h"
#include "CollisionObject.h"
#include "MathUtil.h"
#include <SFML/Graphics.hpp>
#include "NotificationManager.h"

namespace CMPUT350 {

class DrawContext;

class GameEngine : public EngineView {
public:
    GameEngine(unsigned int width, unsigned int height, const std::string& name);
    ~GameEngine();

    GameEngine(const GameEngine&) = delete;             // Prevent copy-construction
    GameEngine(GameEngine&&) = delete;                  // Prevent move-construction
    GameEngine& operator=(const GameEngine&) = delete;  // Prevent assignment
    GameEngine& operator=(GameEngine&&) = delete;       // Prevent move-assignment

    void AddGameObject(std::shared_ptr<GameObject> gameObject) override;
    void InstallKeyDownNotification(int, const std::string &notification);
    void InstallKeyUpNotification(int, const std::string &notification);

    void Run();

private:
    std::shared_ptr<sf::RenderWindow> mWindow;
    std::shared_ptr<sf::Font> mFont;

    std::vector<std::shared_ptr<GameObject>> mAliveObjects;
    std::vector<std::shared_ptr<GameObject>> mObjectsToAdd;

    std::shared_ptr<DrawContext> mDrawContext;
    std::shared_ptr<DrawContext> mDrawContextGUI;
    GameContext mGameContext;
    NotificationManager mNotificationManager;
    std::unordered_map< int, std::vector<std::string>> mKeyDownNotification;
    std::unordered_map< int, std::vector<std::string>> mKeyUpNotification;


};

}  // namespace CMPUT350

#endif  // GAMEENGINE_H
