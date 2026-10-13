#include "Bezier.h"
#include <cassert>
#include <cmath>

namespace CMPUT350 {

Bezier::Bezier(const std::vector<Point2D>& pts) : mPoints(pts) {

    // Check that there are the correct amount of points (>=4 and 3k + 1 control points)
    // since each new section shares previous endpoint 4,7,10,etc
    assert(pts.size() >= 4 && (pts.size() - 1) % 3 == 0); 





}

}