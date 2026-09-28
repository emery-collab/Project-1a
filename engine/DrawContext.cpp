#include "DrawContext.h"

namespace CMPUT350 {

DrawContext::DrawContext(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Font> font)
    : mWindow(window), mFont(font) {}

void DrawContext::DrawCenteredText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {
    // Draw text logic: declare(font) -> sfml::text/size/pos/color -> draw

    // Declare text object using our private shared pointer within DrawContext which stores the sfml font 
    sf::Text textObject(*mFont);
    
    // Set text/size/color
    textObject.setString(text);
    textObject.setCharacterSize(pixelSize);
    textObject.setFillColor(sf::Color(c.r, c.g, c.b));

    // Get a rect the size of textObject, set textObjects origin to the bounds pos(topleft) + half size of bounds for center
    sf::FloatRect bounds = textObject.getLocalBounds();
    textObject.setOrigin(bounds.position + bounds.size / 2.0f);

    // Set pos
    textObject.setPosition(sf::Vector2f(p.x, p.y));

    // Draw
    mWindow->draw(textObject);

}

void DrawContext::DrawText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {
    // Draw text logic: declare(font) -> sfml::text/size/pos/color -> draw

    // Declare text object using our private shared pointer within DrawContext which stores the sfml font 
    sf::Text textObject(*mFont);
    
    // Set text/size/pos/color
    textObject.setString(text);
    textObject.setCharacterSize(pixelSize);
    textObject.setPosition(sf::Vector2f(p.x, p.y));
    textObject.setFillColor(sf::Color(c.r, c.g, c.b));

    // Draw
    mWindow->draw(textObject);

}

void DrawContext::DrawCircle(Point2D p, float radius, RGBColor c) {
    // Draw circle logic: declare(radius) -> sfml::(pos-radius)/color -> draw
    
    // Declare circle with radius
    sf::CircleShape circle(radius);

    // Set pos, need to subtract radius since the SFML default circle implementation refers to its top left, 
    // whereas our point2d represents center
    // ie. if center = (400, 300) and radius = (20) we want to draw at (380, 280)
    circle.setPosition(sf::Vector2f(p.x - radius, p.y - radius));

    // Set color
    circle.setFillColor(sf::Color(c.r, c.g, c.b));

    // Draw
    mWindow->draw(circle);

}

void DrawContext::DrawRect(Rect r, RGBColor c) {
    // Draw rect logic: declare -> sfml::pos/size/color -> draw

    // Declare rect
    sf::RectangleShape rectangle;

    // Set pos
    rectangle.setPosition(sf::Vector2f(r.topLeft.x, r.topLeft.y));

    // Set size
    rectangle.setSize(sf::Vector2f(r.width, r.height));

    // Set Color
    rectangle.setFillColor(sf::Color(c.r, c.g, c.b));

    // Draw
    mWindow->draw(rectangle);

}

void DrawContext::FrameRect(Rect r, float width, RGBColor c) {}

/**
 * @brief Draws a line between two points with a specified width and color.
 *
 * @param from The starting point of the line (Point2D).
 * @param to The ending point of the line (Point2D).
 * @param width The width of the line in pixels.
 * @param c The color of the line, specified as an RGBColor object.
 *
 * This function calculates the distance and angle between the two points
 * and uses a polygone shape to represent the line. The line is drawn
 * relative to the world offset and rendered onto the associated window.
 */
void DrawContext::DrawLine(Point2D from, Point2D to, float width, RGBColor c) {
    // Draw line logic: direction vector(start->end) -> normalize -> perpendicular for width -> half thickness -> calculate points -> set/color/draw

    // Get direction pointing from -> to
    Point2D direction = to - from;

    // Normalize direction into unit vector 
    direction.Normalize();

    // Get perpendic vector for width using (-y, x)
    Point2D perpendicular(-direction.y, direction.x);

    // Get half width
    float halfWidth = width / 2.0f;

    // Calculate the four points by adding and subtracting width in a perpendicular direction to both the start and end points of our line.
    // This will essentially create a rectangle polygon we can draw
    Point2D p1 = from + perpendicular * halfWidth;
    Point2D p2 = to   + perpendicular * halfWidth;
    Point2D p3 = to   - perpendicular * halfWidth;
    Point2D p4 = from - perpendicular * halfWidth;

    // Now we have our four points to put into convex shape
    sf:: ConvexShape line;
    line.setPointCount(4);

    line.setPoint(0, sf::Vector2f(p1.x, p1.y));
    line.setPoint(1, sf::Vector2f(p2.x, p2.y));
    line.setPoint(2, sf::Vector2f(p3.x, p3.y));
    line.setPoint(3, sf::Vector2f(p4.x, p4.y));

    // Set color
    line.setFillColor(sf::Color(c.r, c.g, c.b));

    // Draw
    mWindow->draw(line);

}

int DrawContext::GetWindowWidth() { return mWindow->getSize().x; }

int DrawContext::GetWindowHeight() { return mWindow->getSize().y; }

}  // namespace CMPUT350
