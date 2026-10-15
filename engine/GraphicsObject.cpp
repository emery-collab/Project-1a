#include "GraphicsObject.h"

namespace CMPUT350 {

    void GraphicsObject::RenderBackground(GameContext *contextrender) { return; }
    void GraphicsObject::RenderForeground(GameContext *contextrender) { return; }
    //center of object 
    Point2D GraphicsObject::GetLocation() {return 0;};
    //rotation of object in radians
    float GraphicsObject::GetRotation() {return 0;};

}  // namespace CMPUT350
