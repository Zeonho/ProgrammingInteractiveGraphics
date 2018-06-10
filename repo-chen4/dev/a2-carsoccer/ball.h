/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef BALL_H_
#define BALL_H_

#include <mingfx.h>

/// Small data structure for a ball
class Ball {
public:

    /// The constructor sets the radius and calls Reset() to start the ball at
    /// the center of the field
    Ball() : radius_(2.0) {
        Reset();
    }

    /// Nothing special needed in the constructor
    virtual ~Ball() {}


    void Reset() {
        position_ = Point3(0, rand() % 33 + radius_, 0);
        velocity_ = Point3(rand() % 20 - 10, 0, rand() % 20 - 10);
    }

    float radius() { return radius_; }

    Point3 position() { return position_; }
    void set_position(const Point3 &p) { position_ = p; }

    Point3 velocity() { return velocity_; }
    void set_velocity(const Point3 &v) { velocity_ = v; }

private:
    // You will probably need to store some additional data here, e.g., velocity

    Point3 position_;
    float radius_;
    Point3 velocity_;
};

#endif
