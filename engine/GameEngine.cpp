#include "GameEngine.h"

/// @brief
namespace CMPUT350 {
#include "FontData.h"

GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& name) {

    // Create font
    mFont = std::make_shared<sf::Font>();

    // Sample font loading code
    if (!mFont->openFromMemory(&_font, _font_len))
    {
    	fprintf(stderr, "WARNING: Font did not load.\n");
    }

    // Create window
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

    while (mWindow->isOpen())  // window is open
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
        // Trim the end of aliveObject vector to write - 1 size and destroy the unused shared_ptrs
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

            // Is window being resized?
            } else if (event->is<sf::Event::Resized>()) {
            // No implementation as of project1a

            // Is key press a within the text unicode range
            } else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>()) {
                if (keyPressed->unicode < 128) {

                    // Convert to c++ char
                    char key = static_cast<char>(keyPressed->unicode);
                    
                    // Each alive object handles the event accordingly with their own logic
                    for (auto& object : mAliveObjects) {
                        object->HandleKeyEvent(&mGameContext, key);
                    }
                }
            }
        }

        // 3. Update game objects
        for (auto& object : mAliveObjects) {
            object->Update(&mGameContext);
        }

        // 4. Process collision events

        // Compare each alive object with each other excactly once
        for (size_t a = 0; a < mAliveObjects.size(); a++) {
            auto objA = std::dynamic_pointer_cast<CollisionObject>(mAliveObjects[a]);

            // Not Collision Object, skip to next loop iteration
            if (objA == nullptr) 
                continue;
         
            // This for loop specifies only comparing each object once since b starts at a + 1
            for (size_t b = a + 1; b < mAliveObjects.size(); b++) {
                auto objB = std::dynamic_pointer_cast<CollisionObject>(mAliveObjects[b]);

                // Again, if not a Collision Object, skip to next loop iteration 
                if (objB == nullptr)
                continue;

                // First, we get the bounding box for the first object, we then &= it with the bounding box for the other second object to be compared
                Rect overlap = objA->GetBounds();

                // Calculate overlap, since &= we assigned in mathutil.h returns 0 for width and height if there is no overlap
                overlap &= objB->GetBounds();

                // If there is overlap, call both objects respective collision implementation, with each other as arguments
                if (overlap.width > 0 && overlap.height > 0) {
                    objA->CollisionEnter(objB);
                    objB->CollisionEnter(objA);

                }
            }    
        }


        // 5. Late updates
        for (auto& object : mAliveObjects) {
            object->LateUpdate(&mGameContext);
        }

        // Clear window
        mWindow->clear();

        // 6. Render background
        
        // For alive objects, dynamic_pointer_cast again to get if its a graphics object or not
        for (auto& object : mAliveObjects) {
            auto graphics = std::dynamic_pointer_cast<GraphicsObject>(object);

            // If it is a graphics object, call its render BACKGROUND logic FIRST
            if (graphics != nullptr)
                graphics->RenderBackground(&mGameContext);
        }

        // 7. Render foreground

        // For alive objects, dynamic_pointer_cast again to get if its a graphics object or not
        for (auto& object : mAliveObjects) {
            auto graphics = std::dynamic_pointer_cast<GraphicsObject>(object);

            // If it is a graphics object, call its render FOREGROUND logic SECOND
            if (graphics != nullptr)
                graphics->RenderForeground(&mGameContext);
        }

        // Actually render to window
        mWindow->display();

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
