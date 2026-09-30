#include "GameEngine.h"

/// @brief
namespace CMPUT350 {
#include "FontData.h"

GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& name) {


    // mFont = std::make_shared<sf::Font>();

    // Sample font loading code
    if (!mFont->openFromMemory(&_font, _font_len))
    {
    	fprintf(stderr, "WARNING: Font did not load.\n");
    }



    mWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode({width, height}), name);

    mWindow->setFramerateLimit(30);

    

    // Make a shared pointer to mDrawContext
    mDrawContext = std::make_shared<DrawContext>(mWindow, mFont);

    // Set up the GameContext
    mGameContext.mEngineView = this;
    mGameContext.ScreenContext = mDrawContext.get();


}

GameEngine::~GameEngine() {
    // Cleanup resources
    if (mWindow) {
        mWindow->close();
    }

}

void GameEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    // Will be first added to our buffer vector of objects to be added so that we dont change mAliveObjects while its being used
    mObjectsToAdd.push_back(gameObject);
}

/**
 * @method Run
 * @arguments None
 * @description Gives control to the game engine. Will not return until the game window is closed or
 * all objects have been destroyed.
 */
void GameEngine::Run() {

    GameContext context;

    context.mEngineView = this;
    context.ScreenContext = mDrawContext.get();

    while (true)  // window is open
    {
        // 0. Remove any objects that are now dead

        size_t write = 0;
        // O(n) time
        for (size_t read = 0; read < mAliveObjects.size(); ++read) {
            // This will only copy in alive objects, skipping dead ones
            if (mAliveObjects[read]->IsAlive()) {
                mAliveObjects[write] = mAliveObjects[read];
                ++write;
            }
        }
        // Trim the end of aliveObject vector to write - 1 size
        // O(1) time
        mAliveObjects.resize(write);

        // 1. Activate and initialize any objects added during the last frame

        // Go through vector of objects, append to aliveObjects, and initialize with game context
        for (auto& object : mObjectsToAdd) {
            mAliveObjects.push_back(object);
            object->Initialize(&mGameContext);
        }
        mObjectsToAdd.clear();

        // 2. Process events
        while (const std::optional event = mWindow->pollEvent()) {

            // Is window being closed?
            if (event->is<sf::Event::Closed>()){
                mWindow->close();
            
            // Is key press a within the text unicode range
            } else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>()) {
                if (keyPressed->unicode < 128) {
                    // Convert to c++ char
                    char key = static_cast<char>(keyPressed->unicode);
                    
                    // Each alive object handles the event accordingly with their own logic
                    for (auto& object : mAliveObjects) {
                        object->HandleKeyEvent(&context, key);
                    }
                }
            }
        }

        /*
        if (event->is<sf::Event::Closed>()) {
            mWindow->close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>()) {
            if (keyPressed->unicode == 'p'){
                // do something here
            }
            
        }
        */

        // 3. Update game objects
        for (auto& object : mAliveObjects) {
            object->Update(&context);
        }

        // 4. Process collision events
        


        // 5. Late updates
        for (auto& object : mAliveObjects) {
            object->LateUpdate(&context);
        }


        // Clear window

        // 6. Render background

        // 7. Render foreground

        // Actually render to window
    }
}

// Sample code for processing events

// bool GameEngine::ProcessEvents(GameContext *context)
//{
//	while (const std::optional event = mWindow->pollEvent())
//	{
//		if (event->is<sf::Event::Closed>())
//		{
//		}
//		else if (event->is<sf::Event::Resized>())
//		{
//		}
//		else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
//		{
//			// use keyPressed->unicode to get character
//		}
//	}
// }

}  // namespace CMPUT350
