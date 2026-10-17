#include "GameEngine.h"
#include "NotificationManager.h"
#include <string>
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


    mDrawContextGUI = std::make_shared<DrawContext>(mWindow, mFont);

    // Set up the GameContext
    mGameContext.mEngineView = this;
    mGameContext.ScreenContext = mDrawContext.get();
    mGameContext.GUIContext = mDrawContextGUI.get();
    mGameContext.mNotificationManager =  &mNotificationManager;
    mGameContext.currentFrame = 0;


}

GameEngine::~GameEngine() {
    // Cleanup resources
    if (mWindow) {
        mWindow->close();
    }

}

void GameEngine::InstallKeyDownNotification(int key, const std::string& notification){
    mKeyDownNotification[key].push_back(notification);


}
void GameEngine::InstallKeyUpNotification(int key, const std::string& notification){
    mKeyUpNotification[key].push_back(notification);

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
        mGameContext.currentFrame++;

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
            mGameContext.CurrObject = object;
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
            
            // Is key is pressed
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

                auto key = static_cast<int>(keyPressed->scancode);
                auto validKey = mKeyDownNotification.find(key);
                if(validKey != mKeyDownNotification.end()){
                    for (const std::string& notification : validKey->second) {
                        mNotificationManager.Notify(notification);
                    }
                    
                }
                
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>()) {

                auto key = static_cast<int>(keyPressed->scancode);
                auto validKey = mKeyUpNotification.find(key);

                if(validKey != mKeyUpNotification.end()){
                    for (const std::string& notification : validKey->second) {
                        mNotificationManager.Notify(notification);
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

                // Object locations and rotations.
                Point2D posA = objA->GetLocation();
                Point2D posB = objB->GetLocation();

                float angleA = objA->GetRotation();
                float angleB = objB->GetRotation();

                // Retrieve the collision shapes in local coordinates.
                const auto& shapesA = objA->GetShapes();
                const auto& shapesB = objB->GetShapes();

                // Actual shape comparisons go here.

                constexpr float EPSILON = 1e-6f;

                auto transformPoint = [](Point2D p, Point2D pos, float angle) {
                    float c = std::cos(angle), s = std::sin(angle);
                    return Point2D(p.x * c - p.y * s + pos.x, p.x * s + p.y * c + pos.y);
                };

                auto inverseTransformPoint = [](Point2D p, Point2D pos, float angle) {
                    Point2D translated = p - pos;
                    float c = std::cos(angle), s = std::sin(angle);
                    return Point2D(translated.x * c + translated.y * s, -translated.x * s + translated.y * c);
                };

                auto intersectLines = [&](Line l1, Line l2, Point2D& hit) {
                    return l1.Crosses(l2, hit);
                };

                auto pointInPolygon = [](Point2D p, const std::vector<Point2D>& polygon) {
                    bool inside = false;
                    for (size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
                        const Point2D& a = polygon[i];
                        const Point2D& b = polygon[j];
                        bool crosses = ((a.y > p.y) != (b.y > p.y)) &&
                            (p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x);
                        if (crosses) inside = !inside;
                    }
                    return inside;
                };

                auto lineCircleHit = [&](Line line, Circle circle, Point2D& hit) {
                    Point2D d = line.p2 - line.p1, f = line.p1 - circle.center;
                    float a = d.Dot(d);
                    if (a <= EPSILON) return false;
                    float b = 2.0f * f.Dot(d), c = f.Dot(f) - circle.radius * circle.radius;
                    float discriminant = b * b - 4.0f * a * c;
                    if (discriminant < 0.0f) return false;
                    float root = std::sqrt(discriminant);
                    float t1 = (-b - root) / (2.0f * a), t2 = (-b + root) / (2.0f * a);
                    if (c <= 0.0f) { hit = line.p1; return true; }
                    if (t1 >= 0.0f && t1 <= 1.0f) { hit = line.p1 + d * t1; return true; }
                    if (t2 >= 0.0f && t2 <= 1.0f) { hit = line.p1 + d * t2; return true; }
                    return false;
                };

                auto linePolygonHit = [&](Line line, const std::vector<Point2D>& polygon, Point2D& hit) {
                    bool found = false;
                    float bestDistance = std::numeric_limits<float>::max();
                    if (pointInPolygon(line.p1, polygon)) { hit = line.p1; return true; }
                    for (size_t i = 0; i < polygon.size(); ++i) {
                        Line edge(polygon[i], polygon[(i + 1) % polygon.size()]);
                        Point2D candidate;
                        if (intersectLines(line, edge, candidate)) {
                            float distance = (candidate - line.p1).Dot(candidate - line.p1);
                            if (distance < bestDistance) { bestDistance = distance; hit = candidate; found = true; }
                        }
                    }
                    return found;
                };

                bool collided = false, lineCollision = false, lineBelongsToA = false;
                Point2D worldContact;

                for (const auto& shapeA : shapesA) {
                    for (const auto& shapeB : shapesB) {
                        bool pairHit = false, pairHasLine = false, pairLineIsA = false;
                        Point2D pairContact;

                        auto rectCorners = [&](const Rect& r, Point2D pos, float angle) {
                            std::vector<Point2D> corners = {
                                r.topLeft,
                                Point2D(r.topLeft.x + r.width, r.topLeft.y),
                                Point2D(r.topLeft.x + r.width, r.topLeft.y + r.height),
                                Point2D(r.topLeft.x, r.topLeft.y + r.height)
                            };
                            for (auto& p : corners) p = transformPoint(p, pos, angle);
                            return corners;
                        };

                        auto worldLine = [&](const Line& l, Point2D pos, float angle) {
                            return Line(transformPoint(l.p1, pos, angle), transformPoint(l.p2, pos, angle));
                        };

                        auto worldCircle = [&](const Circle& c, Point2D pos, float angle) {
                            return Circle(transformPoint(c.center, pos, angle), c.radius);
                        };

                        bool aLine = shapeA.t == ShapeType::kLine, bLine = shapeB.t == ShapeType::kLine;
                        bool aCircle = shapeA.t == ShapeType::kCircle, bCircle = shapeB.t == ShapeType::kCircle;

                        if (aLine && bLine) {
                            Line la = worldLine(shapeA.shape.line, posA, angleA), lb = worldLine(shapeB.shape.line, posB, angleB);
                            pairHit = intersectLines(la, lb, pairContact);
                            pairHasLine = pairHit;
                            pairLineIsA = true;
                        }
                        else if (aLine && bCircle) {
                            Line la = worldLine(shapeA.shape.line, posA, angleA);
                            Circle cb = worldCircle(shapeB.shape.circle, posB, angleB);
                            pairHit = lineCircleHit(la, cb, pairContact);
                            pairHasLine = pairHit;
                            pairLineIsA = true;
                        }
                        else if (aCircle && bLine) {
                            Circle ca = worldCircle(shapeA.shape.circle, posA, angleA);
                            Line lb = worldLine(shapeB.shape.line, posB, angleB);
                            pairHit = lineCircleHit(lb, ca, pairContact);
                            pairHasLine = pairHit;
                            pairLineIsA = false;
                        }
                        else if (aLine && shapeB.t == ShapeType::kRect) {
                            Line la = worldLine(shapeA.shape.line, posA, angleA);
                            auto polygon = rectCorners(shapeB.shape.rect, posB, angleB);
                            pairHit = linePolygonHit(la, polygon, pairContact);
                            pairHasLine = pairHit;
                            pairLineIsA = true;
                        }
                        else if (shapeA.t == ShapeType::kRect && bLine) {
                            auto polygon = rectCorners(shapeA.shape.rect, posA, angleA);
                            Line lb = worldLine(shapeB.shape.line, posB, angleB);
                            pairHit = linePolygonHit(lb, polygon, pairContact);
                            pairHasLine = pairHit;
                            pairLineIsA = false;
                        }
                        else if (aCircle && bCircle) {
                            Circle ca = worldCircle(shapeA.shape.circle, posA, angleA);
                            Circle cb = worldCircle(shapeB.shape.circle, posB, angleB);
                            float radiusSum = ca.radius + cb.radius;
                            Point2D difference = ca.center - cb.center;
                            pairHit = difference.Dot(difference) <= radiusSum * radiusSum;
                        }
                        else if (aCircle && shapeB.t == ShapeType::kRect) {
                            Circle ca = worldCircle(shapeA.shape.circle, posA, angleA);
                            auto polygon = rectCorners(shapeB.shape.rect, posB, angleB);
                            if (pointInPolygon(ca.center, polygon)) pairHit = true;
                            else {
                                for (size_t i = 0; i < polygon.size(); ++i) {
                                    Line edge(polygon[i], polygon[(i + 1) % polygon.size()]);
                                    Point2D delta = edge.ClosestPoint(ca.center) - ca.center;
                                    if (delta.Dot(delta) <= ca.radius * ca.radius) { pairHit = true; break; }
                                }
                            }
                        }
                        else if (shapeA.t == ShapeType::kRect && bCircle) {
                            auto polygon = rectCorners(shapeA.shape.rect, posA, angleA);
                            Circle cb = worldCircle(shapeB.shape.circle, posB, angleB);
                            if (pointInPolygon(cb.center, polygon)) pairHit = true;
                            else {
                                for (size_t i = 0; i < polygon.size(); ++i) {
                                    Line edge(polygon[i], polygon[(i + 1) % polygon.size()]);
                                    Point2D delta = edge.ClosestPoint(cb.center) - cb.center;
                                    if (delta.Dot(delta) <= cb.radius * cb.radius) { pairHit = true; break; }
                                }
                            }
                        }
                        else {
                            auto polygonA = rectCorners(shapeA.shape.rect, posA, angleA);
                            auto polygonB = rectCorners(shapeB.shape.rect, posB, angleB);
                            for (size_t i = 0; i < polygonA.size() && !pairHit; ++i) {
                                Line edgeA(polygonA[i], polygonA[(i + 1) % polygonA.size()]);
                                for (size_t j = 0; j < polygonB.size(); ++j) {
                                    Line edgeB(polygonB[j], polygonB[(j + 1) % polygonB.size()]);
                                    Point2D ignored;
                                    if (intersectLines(edgeA, edgeB, ignored)) { pairHit = true; break; }
                                }
                            }
                            if (!pairHit) pairHit = pointInPolygon(polygonA[0], polygonB) || pointInPolygon(polygonB[0], polygonA);
                        }

                        if (pairHit) {
                            collided = true;
                            if (pairHasLine) { lineCollision = true; lineBelongsToA = pairLineIsA; worldContact = pairContact; }
                            break;
                        }
                    }
                    if (collided) break;
                }

                if (collided) {
                    if (lineCollision) {
                        Point2D localContact = lineBelongsToA
                            ? inverseTransformPoint(worldContact, posA, angleA)
                            : inverseTransformPoint(worldContact, posB, angleB);
                        if (lineBelongsToA) { objA->CollisionEnter(objB, localContact); objB->CollisionEnter(objA); }
                        else { objB->CollisionEnter(objA, localContact); objA->CollisionEnter(objB); }
                    }
                    else { objA->CollisionEnter(objB); objB->CollisionEnter(objA); }
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
            if (graphics == nullptr)
                continue;


            mDrawContext->SetContextOffset(graphics->GetLocation());

            // GraphicsObject returns radians; DrawContext expects degrees.
            // std::acos(-1.0f) is how you calculate PI, so we are dividing by PI
            mDrawContext->SetContextRotation(graphics->GetRotation() * 180.0f / std::acos(-1.0f));
            graphics->RenderBackground(&mGameContext);


        }

        // 7. Render foreground

        // For alive objects, dynamic_pointer_cast again to get if its a graphics object or not
        for (auto& object : mAliveObjects) {
            auto graphics = std::dynamic_pointer_cast<GraphicsObject>(object);

            // If it is a graphics object, call its render FOREGROUND logic SECOND
            if (graphics == nullptr)
                continue;

            mDrawContext->SetContextOffset(graphics->GetLocation());
            // Again, we are diving by std::acos(-1.0f) which is how you calculate PI
            mDrawContext->SetContextRotation(graphics->GetRotation() * 180.0f / std::acos(-1.0f));
            graphics->RenderForeground(&mGameContext);

        }

        // Render UI without any object transformation.
        mDrawContext->SetContextOffset(Point2D(0.0f, 0.0f));
        mDrawContext->SetContextRotation(0.0f);
        for (auto& object : mAliveObjects)
        {
            mGameContext.CurrObject = object;
            object->RenderUI(&mGameContext);
        }

        // Actually render to window
        mWindow->display();

    }
}

GameEngine::tGameObject::const_iterator GameEngine::cbegin() const {
    return mAliveObjects.cbegin();

}
GameEngine::tGameObject::const_iterator GameEngine::begin() const {
    return mAliveObjects.begin();
}
GameEngine::tGameObject::const_iterator GameEngine::cend() const {
    return mAliveObjects.cend();
}
GameEngine::tGameObject::const_iterator GameEngine::end() const {
    return mAliveObjects.end();
}
    
}  // namespace CMPUT350
