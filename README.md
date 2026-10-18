# CMPUT 350 HW 1
MathUtils
Implemented a line-line crosses() function which calculates the variables required for the final equations we derived in class first, then checks for parallel lines, then checks for intersection using the equations for u and t. It returns true and sets the crossing point if there was a collision. I asked AI to review my function, and it suggested the check for being less than epsilon (a really small number) instead of the denominator in the final equation being equal to 0, since floating point arithmetic could let the result be almost 0, but not quite, just very small like 0.0000000001.

Implemented closest point as well as other various todos, such as ostream operators. Closest point will find the actual closest point instead of just the closest end point

CollisionObject.h
Added code in assignment description, then edited player/enemy/bullet to implement their own getshapes(), getrotation(), getlocation() which they inherit from the new virtual function in collisionobject.h. Within player.cpp, There is a section around lines 231-233 where I convert a world-coordinate rectangle into a local coordinate rectangle; this change I used AI for. I asked what was incorrect with my implementation, and that was the fix.

DrawContext.cpp
Implemented FrameRect() which simply gets the four corners and then draws lines between them
Added transform(point2d), transform(rect), and reversetransform() so the engine can call these to handle rotations. Then added transform to the other draw functions so that they respect the current transformations. I asked AI to correct my implementation again, it recommended adding the setter functions setContextOffset and setContextRotation within draw context so that the engine can use the to access private member variables which hold the current transformation.
For DrawRect(), I had to ask AI to refactor my rotation code, as simply transforming the object's local position or local center wouldn’t work. It explained how to implement it by transforming all four corners, then drawing a complex polygon with those corners.
Each of the respective draw functions have their own transformations included before the drawing is done, except for FrameRect since it just uses drawline’s logic.

GameEngine.cpp/h
Updated to support new events.
Refactored the old bounding box check to simply skip pairs whose bounds do not overlap. Then moves on to acquiring location, rotation, shape, and finally performing the correct shape comparisons.

For the collision detection during run(), we heavily utilized AI. We prompted the AI to help fill in the section for collision, and it provided an improved implementation of our own code. Next after some refactoring, we structured the code into lambda functions from lines 137-237 in our GameEngine.cpp which would reduce the amount of redundant code when checking shape pairs in the following for loops, and avoid checking things like line-circle, then circle-line. Much of the code from 253-328 also came from the AI integration, which we then restructured, after ensuring functionality.
The overall flow of the collision detection section in gameengine’s run() is:
Declare lambdas
Reusable calculations such as transforming a point or testing an intersection
shape detection
Uses those calculations to determine whether two particular shapes overlap
collision handling
Uses the result to notify the objects and also delivers a contact point in the case of a line collision

Bullet.h/cpp
Had to add three more member variables to the header, one to track if it exploded, where it exploded (also so we can pass back largest Y), and explosion frames to keep the explosion around for a few frames. Redid the collision enter that feeds in a collisionPoint so record the largest Y, and changed both versions of collision enter to check for explosion first before deciding if there was collision. Changed the render foreground function to check if exploded, and if so to render a small circle instead of a line. Changed the update to check if explosion frames have elapsed before sending the kill() signal. Finally, changed the get shapes function to not return anything if a bullet is in the exploded state since it would otherwise count as an object on an enemy, burning through its entire health bar in about 3 frames at most.

Extras
Refactored a few functions like renderforeground within all galaga files

Extra Credit
Working on Vector2D credit at the moment

Enemy1/2/3.cpp
Have their own custom bezier curves, with different locations as a vector of point2d’s, each starting and ending with their formation spot. They all track the player’s coordinates in their path calculation within these point2d’s to a lesser or greater extent.
