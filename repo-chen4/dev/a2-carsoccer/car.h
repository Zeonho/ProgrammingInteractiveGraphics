/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef CAR_H_
#define CAR_H_

#include <mingfx.h>

/// Small data structure for a car
class Car {
public:

    /// The constructor sets the static properties of the car, like its size,
    /// and then calls Reset() to reset the position, velocity, and any other
    /// dynamic variables that change during game play.
    Car() : size_(3, 2, 4), collision_radius_(2.5) {
        Reset();
    }

    /// Nothing special needed in the constructor
    virtual ~Car() {}

    /// Resets all the dynamic variables, so if you call this after a goal, the
    /// car will go back to its starting position.
    void Reset() {
        position_ = Point3(0, size_[1]/2, 45);
        velocity_ = Point3(0, 0, 0);
        angle_ = 0;
        speed_ = 0;
    }

    float collision_radius() { return collision_radius_; }

    Vector3 size() { return size_; }

    Point3 position() { return position_; }
    void set_position(const Point3 &p) { position_ = p; }

    Point3 velocity() { return velocity_; }
    void set_velocity(const Point3 &v) { velocity_ = v; }

    float angle() { return angle_; }
    void set_angle(const float &a) { angle_ = a; }

    float speed() { return speed_; }
    void set_speed(const float &s) { speed_ = s; }

    void update(Vector2 dir);


private:
    // You will probably need to store some additional data here, e.g., speed.

    Vector3 size_;
    float collision_radius_;
    Point3 position_;
    Point3 velocity_;
    float angle_;
    float speed_;
};

class Astar {
 public:
	double distanceBetween(int ax, int ay, int bx, int by) {
		double delta_x = bx - ax;
		double delta_y = by - ay;
		return sqrt(delta_x*delta_x + delta_y*delta_y);
	}
 private:
	
};

#endif
