#include "Bezier.h"
#include <cassert>
#include <cmath>

namespace CMPUT350 {

Bezier::Bezier(const std::vector<Point2D>& pts) : mPoints(pts) {

    // Check that there are the correct amount of points (>=4 and 3k + 1 control points)
    // since each new section shares previous endpoint 4,7,10,etc
    assert(pts.size() >= 4 && (pts.size() - 1) % 3 == 0); 

}
Point2D Bezier::GetPoint(const std::vector<Point2D>& pts, float t){
    //cubic bezier. Returns the point on the curve at t.
    assert(pts.size() >= 4 && (pts.size() - 1) % 3 == 0 && t >= 0);
    auto totalSegments = (pts.size() - 1) / 3;
    assert(totalSegments >= t);
    auto seg = static_cast<size_t>(std::floor(t));
    //consider 4 points and t = 1. segment = 1, t =1 still refers segment 0, not segment 1.
    //so we need to reduce seg by 1 to ensure it is in the right segment.
    if(seg > totalSegments - 1){
        seg = totalSegments - 1;
    }
    float localT = t - seg;
    //b4(t) = (1 − t)³ p1 + 3(1 − t)² t p2 + 3(1 − t) t² p3 + t³ p4
    return ((1-localT)*(1-localT)*(1-localT)*pts[seg*3+0] + 
        3*(1-localT)*(1-localT)*pts[seg*3+1]*localT +
        3*(1-localT)*pts[seg*3+2]*localT*localT + 
        localT*localT*localT*pts[seg*3+3]);
    

}

Point2D Bezier::GetSlope(const std::vector<Point2D> &pts, float t){
    //cubic bezier. Returns the slope on the curve at t.
    assert(pts.size() >= 4 && (pts.size() - 1) % 3 == 0 && t >= 0);
    auto totalSegments = (pts.size() - 1) / 3;
    assert(totalSegments >= t);
    auto seg = static_cast<size_t>(std::floor(t));
    //Same logic as GetPoint
    if(seg > totalSegments - 1){
        seg = totalSegments - 1;
    }
    float localT = t - seg;
    //b4'(t) = 3(1 - t)^2 (p2 - p1) + 6(1 - t) t (p3 - p2) + 3 t^2 (p4 - p3)
    return 3*(1-localT)*(1-localT)*(pts[seg*3+1]-pts[seg*3+0]) + 
        6*(1-localT)*localT*(pts[seg*3+2]-pts[seg*3+1]) +
        3*localT*localT*(pts[seg*3+3]-pts[seg*3+2]);
}

Point2D Bezier::GetPoint(float t) const{
    return GetPoint(mPoints,t);
}
Point2D Bezier::GetSlope(float t) const{
    return GetSlope(mPoints,t);
}
}