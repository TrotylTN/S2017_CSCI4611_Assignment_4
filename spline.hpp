#ifndef SPLINE_HPP
#define SPLINE_HPP

#include <glm/glm.hpp>
#include <vector>
using glm::vec3;

class SplinePoint3 {
public:
    float t;    // time
    vec3 p, dp; // value and derivative
    SplinePoint3(float t, vec3 p, vec3 dp): t(t), p(p), dp(dp) {}
};

class Spline3 {
public:
    // list of spline control points. assumed to be in increasing order of t
    std::vector<SplinePoint3> points;

    // value of t for first and last control points
    float minTime() {return points.front().t;}
    float maxTime() {return points.back().t;}

    // returns i such that t lies between points[i].t and points[i+1].t.
    // also modifies t to lie in [minTime(), maxTime()] if necessary.
    int findSegment(float &t);

    // returns value of spline function at time t
    vec3 getValue(float t);

    // returns d/dt of spline function at time t
    vec3 getDerivative(float t);
};

inline int Spline3::findSegment(float &t) {

    // TODO: If t is outside the range [minTime(), maxTime()], replace
    // it with the closest time in that range. Then, find the segment
    // that contains t, so that you can perform cubic Hermite
    // interpolation within it.
    if (t < minTime())  t = minTime();
    if (t > maxTime())  t = maxTime();
    for (int i = 0 ; i < this->points.size() - 1; i++)
    if (this->points[i].t <= t && t <= this->points[i + 1].t) {
      return i;
    }
    return 0;

}

inline vec3 Spline3::getValue(float t) {

    // TODO: Find the segment that contains t, and use the cubic
    // Hermite interpolation formula to find the interpolated value
    // within it. Note that the formula discussed in class is only
    // valid for t0 = 0, t1 = 1, so you will have to use a modified
    // formula.
    int seg = findSegment(t);
    // int seg = 0;
    vec3 p0 = this->points[seg].p;
    vec3 p0_p = this->points[seg].dp;
    vec3 p1 = this->points[seg + 1].p;
    vec3 p1_p = this->points[seg + 1].dp;
    float tp = (t - this->points[seg].t) /
               (this->points[seg + 1].t - this->points[seg].t);
    vec3 ft = (2 * tp * tp * tp - 3 * tp * tp + 1) * p0 +
              (tp * tp * tp - 2 * tp * tp + tp) * p0_p * (this->points[seg + 1].t - this->points[seg].t) +
              (-2 * tp * tp * tp + 3 * tp * tp) * p1 +
              (tp * tp * tp - tp * tp) * p1_p * (this->points[seg + 1].t - this->points[seg].t);
    return ft;
    // return vec3(0,0,0);

}

inline vec3 Spline3::getDerivative(float t) {

    // TODO: Find the segment that contains t. Differentiate the cubic
    // Hermite interpolation formula to find the derivative of the
    // spline function. Be careful about how rescaling affects
    // derivatives.

    int seg = findSegment(t);

    // int seg = 0;
    vec3 p0 = this->points[seg].p;
    vec3 p0_p = this->points[seg].dp;
    vec3 p1 = this->points[seg + 1].p;
    vec3 p1_p = this->points[seg + 1].dp;
    float tp = (t - this->points[seg].t) /
               (this->points[seg + 1].t - this->points[seg].t);
    vec3 ft = (6 * tp * tp - 6 * tp) * p0 / (this->points[seg + 1].t - this->points[seg].t) +
              (3 * tp * tp - 4 * tp + 1) * p0_p +
              (-6 * tp * tp + 6 * tp) * p1 / (this->points[seg + 1].t - this->points[seg].t) +
              (3 * tp * tp - 2 * tp) * p1_p;
    return ft;
}

#endif
