#include "DrawContext.h"
#include <cmath>
#include <algorithm>

namespace CMPUT350 {

DrawContext::DrawContext(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Font> font)
    : mWindow(window), mFont(font) {}


// setter functions for transformation member variables added in partB
void DrawContext::SetContextOffset(Point2D p)
{
    mContextOffset = p;
}

void DrawContext::SetContextRotation(float rotation)
{
    mContextRotation = rotation;
}

// transform member functions for game engine to use
Point2D DrawContext::Transform(Point2D p)
{
    // calculate PI here
    constexpr float PI = std::acos(-1.0f);

    // Convert degrees to radians.
    float radians = mContextRotation * PI / 180.0f;

    float cosTheta = std::cos(radians);
    float sinTheta = std::sin(radians);

    // Rotate the local point.
    Point2D rotated(p.x * cosTheta - p.y * sinTheta,p.x * sinTheta + p.y * cosTheta);

    // Move the point into world/window coordinates.
    return rotated + mContextOffset;
}

Point2D DrawContext::ReverseTransform(Point2D p)
{
    // calculate PI here
    constexpr float PI = std::acos(-1.0f);

    // Undo the translation first.
    Point2D translated = p - mContextOffset;

    // The inverse of a rotation is rotation by the negative angle.
    float radians = -mContextRotation * PI / 180.0f;

    float cosTheta = std::cos(radians);
    float sinTheta = std::sin(radians);

    return Point2D(
        translated.x * cosTheta - translated.y * sinTheta,
        translated.x * sinTheta + translated.y * cosTheta
    );
}

Rect DrawContext::Transform(Rect r)
{
    // Find the four corners in local coordinates.
    Point2D topLeft = r.topLeft;
    Point2D topRight(r.topLeft.x + r.width, r.topLeft.y);
    Point2D bottomLeft(r.topLeft.x, r.topLeft.y + r.height);
    Point2D bottomRight(r.topLeft.x + r.width, r.topLeft.y + r.height);

    // Transform every corner into world coordinates.
    topLeft = Transform(topLeft);
    topRight = Transform(topRight);
    bottomLeft = Transform(bottomLeft);
    bottomRight = Transform(bottomRight);

    // Find the enclosing axis-aligned bounds.
    float minX = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
    float maxX = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
    float minY = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));
    float maxY = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));

    return Rect(Point2D(minX, minY), maxX - minX, maxY - minY);
}


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
    Point2D worldPosition = Transform(p);
    textObject.setPosition(sf::Vector2f(worldPosition.x, worldPosition.y));
    textObject.setRotation(sf::degrees(mContextRotation));

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
    textObject.setFillColor(sf::Color(c.r, c.g, c.b));

    // Transform the local position.
    Point2D worldPosition = Transform(p);
    textObject.setPosition(sf::Vector2f(worldPosition.x, worldPosition.y));

    // Rotate the text around its default origin.
    textObject.setRotation(sf::degrees(mContextRotation));

    // Draw
    mWindow->draw(textObject);

}

void DrawContext::DrawCircle(Point2D p, float radius, RGBColor c) {
    // Draw circle logic: declare(radius) -> sfml::(pos-radius)/color -> draw
    
    // Declare circle with radius
    sf::CircleShape circle(radius);

    // Use its center as the origin so it rotates around its center.
    circle.setOrigin(sf::Vector2f(radius, radius));

    // Convert the local center to world coordinates.
    Point2D center = Transform(p);

    // Set pos
    circle.setPosition(sf::Vector2f(center.x, center.y));
    circle.setRotation(sf::degrees(mContextRotation));

    // Set color
    circle.setFillColor(sf::Color(c.r, c.g, c.b));

    // Draw
    mWindow->draw(circle);

}

void DrawContext::DrawRect(Rect r, RGBColor c) {
    // Draw rect logic: declare -> sfml::pos/size/color -> draw

    // Transform all four corners
    Point2D topLeft = Transform(r.topLeft);
    Point2D topRight = Transform(Point2D(r.topLeft.x + r.width, r.topLeft.y));
    Point2D bottomRight = Transform(Point2D(r.topLeft.x + r.width, r.topLeft.y + r.height));
    Point2D bottomLeft = Transform(Point2D(r.topLeft.x, r.topLeft.y + r.height));

    // Declare rect
    sf::ConvexShape rectangle;
    rectangle.setPointCount(4);

    // Set pos of all four points
    rectangle.setPoint(0, sf::Vector2f(topLeft.x, topLeft.y));
    rectangle.setPoint(1, sf::Vector2f(topRight.x, topRight.y));
    rectangle.setPoint(2, sf::Vector2f(bottomRight.x, bottomRight.y));
    rectangle.setPoint(3, sf::Vector2f(bottomLeft.x, bottomLeft.y));

    // Set Color
    rectangle.setFillColor(sf::Color(c.r, c.g, c.b));

    // Draw
    mWindow->draw(rectangle);

}

void DrawContext::FrameRect(Rect r, float width, RGBColor c) {

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

    // Get the four corners of our rectangle to be framed
    Point2D topLeft = r.topLeft;
    Point2D topRight(r.topLeft.x + r.width, r.topLeft.y);
    Point2D bottomLeft(r.topLeft.x, r.topLeft.y + r.height);
    Point2D bottomRight(r.topLeft.x + r.width, r.topLeft.y + r.height);

    // Draw the four edges using the four corners
    DrawLine(topLeft, topRight, width, c);
    DrawLine(topRight, bottomRight, width, c);
    DrawLine(bottomRight, bottomLeft, width, c);
    DrawLine(bottomLeft, topLeft, width, c);

}

void DrawContext::DrawLine(Point2D from, Point2D to, float width, RGBColor c) {
    // Draw line logic: direction vector(start->end) -> normalize -> perpendicular for width -> half thickness -> calculate points -> set/color/draw

    // Transform both endpoints from local to world coordinates.
    from = Transform(from);
    to = Transform(to);

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
