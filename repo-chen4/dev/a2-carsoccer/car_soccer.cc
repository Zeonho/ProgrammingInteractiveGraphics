/** CSci-4611 Assignment 2:  Car Soccer
 */

#include "car_soccer.h"
#include "config.h"

#define gravity 0.98;
int win[2];

CarSoccer::CarSoccer() : GraphicsApp(1024, 768, "Car Soccer") {
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
    // Create a menu with 2 buttons for pausing and restarting the simulation.

      nanogui::FormHelper *gui = new nanogui::FormHelper(screen());
      nanogui::ref<nanogui::Window> window =
          gui->addWindow(Eigen::Vector2i(10, 10), "Simulation Controls");
      pause_btn_ =
          // gui->addButton("Pause", std::bind(&CarSoccer::OnPauseBtnPressed, this));
      gui->addButton("Restart", std::bind(&CarSoccer::OnRestartBtnPressed, this));

      screen()->performLayout();

  // paused_ = false;

}

CarSoccer::~CarSoccer() {
}

void CarSoccer::OnRestartBtnPressed() {
    car_.Reset();
    car_2.Reset();
    ball_.Reset();
    car_2.set_position(Point3(0, 1, -45));
    for (int i = 0; i < 2; i++){
        win[i] = 0;
    }
}


Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;
    if (IsKeyDown(GLFW_KEY_LEFT))
        dir[0]--;
    if (IsKeyDown(GLFW_KEY_RIGHT))
        dir[0]++;
    if (IsKeyDown(GLFW_KEY_UP))
        dir[1]++;
    if (IsKeyDown(GLFW_KEY_DOWN))
        dir[1]--;
    return dir;
}
Vector2 CarSoccer::joystick_direction2() {
    Vector2 dir;
    if (IsKeyDown(GLFW_KEY_A))
        dir[0]--;
    if (IsKeyDown(GLFW_KEY_D))
        dir[0]++;
    if (IsKeyDown(GLFW_KEY_W))
        dir[1]--;
    if (IsKeyDown(GLFW_KEY_S))
        dir[1]++;
    return dir;
}

void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        // Here's where you could call some form of launch_ball();
        car_.Reset();
        car_2.Reset();
        ball_.Reset();
        car_2.set_position(Point3(0, 1, -45));
    }
}



void CarSoccer::UpdateSimulation(double timeStep) {
    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball as needed and checking for collisions.  Filling this routine
    // in is the main part of the assignment.
    // Field Size 100 * 80
    updateCar(0, timeStep);
    updateCar(1, timeStep);
    updateBall(0, timeStep);
    updateBall(1, timeStep);
}


void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    modelMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));

    // Set a background color for the screen
    glClearColor(0.8,0.8,0.8, 1);

    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));

    for (int i = 0; i < 2; i++){
        win[i] = 0;
    }
    car_2.set_position(Point3(0, 1, -45));
}


void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);

    // Draw the field with the field texture on it.
    Color col(16.0/255.0, 46.0/255.0, 9.0/255.0);
    Matrix4 M = Matrix4::Translation(Vector3(0,-0.201,0)) * Matrix4::Scale(Vector3(50, 1, 60));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0,-0.2,0)) * Matrix4::Scale(Vector3(40, 1, 50));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);


    // Draw the car
    Color carcol(0.8, 0.2, 0.2);
    Matrix4 Mcar =
        Matrix4::Translation(car_.position() - Point3(0,0,0)) *
        Matrix4::RotationY(car_.angle()) *
        Matrix4::Scale(car_.size()) *
        Matrix4::Scale(Vector3(0.5,0.5,0.5));
    quickShapes_.DrawCube(modelMatrix_ * Mcar, viewMatrix_, projMatrix_, carcol);

    Color carcol2(0.2, 0.2, 0.8);
    Matrix4 Mcar2 =
        Matrix4::Translation(car_2.position() - Point3(0,0,0)) *
        Matrix4::RotationY(car_2.angle()) *
        Matrix4::Scale(car_2.size()) *
        Matrix4::Scale(Vector3(0.5,0.5,0.5));
    quickShapes_.DrawCube(modelMatrix_ * Mcar2, viewMatrix_, projMatrix_, carcol2);
    // //Debug
    // Matrix4 car_collision =
    //     Matrix4::Translation(car_.position() - Point3(0,0,0)) *
    //     Matrix4::Scale(Vector3(car_.collision_radius(),
    //                      car_.collision_radius(), car_.collision_radius() ));
    // quickShapes_.DrawSphere(modelMatrix_ * car_collision, viewMatrix_,
    //                                                     projMatrix_, carcol);


    // Draw the ball
    Color ballcol(1,1,1);
    Matrix4 Mball =
        Matrix4::Translation(ball_.position() - Point3(0,0,0)) *
        Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
    quickShapes_.DrawSphere(modelMatrix_ * Mball, viewMatrix_, projMatrix_, ballcol);

    //    win[0]
    ballcol = Color(0.2,0.2,0.8);
    for (int i = 0; i < win[0]; i++){
        Matrix4 Goalball =
        Matrix4::Translation(Point3(35-i*4, ball_.radius(), -52) - Point3(0,0,0) ) *
        Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
        quickShapes_.DrawSphere(modelMatrix_ * Goalball, viewMatrix_, projMatrix_, ballcol);
    }
    ballcol = Color(0.8,0.2,0.2);
    for (int i = 0; i < win[1]; i++){
        Matrix4 Goalball =
        Matrix4::Translation(Point3(-35+i*4, ball_.radius(), 52) - Point3(0,0,0) ) *
        Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
        quickShapes_.DrawSphere(modelMatrix_ * Goalball, viewMatrix_, projMatrix_, ballcol);
    }
    // Draw the ball's shadow -- this is a bit of a hack, scaling Y by zero
    // flattens the sphere into a pancake, which we then draw just a bit
    // above the ground plane.
    Color shadowcol(0.2,0.4,0.15);
    Matrix4 Mshadow =
        Matrix4::Translation(Vector3(ball_.position()[0], -0.1, ball_.position()[2])) *
        Matrix4::Scale(Vector3(ball_.radius(), 0, ball_.radius())) *
        Matrix4::RotationX(90);
    quickShapes_.DrawSphere(modelMatrix_ * Mshadow, viewMatrix_, projMatrix_, shadowcol);


    // You should add drawing the goals and the boundary of the playing area
    // using quickShapes_.DrawLines()
    Color goalcol(1, 1, 1);
    std::vector<Point3> goal_1;
    goal_1.push_back(Point3(10.0, -0.1, -49.0));
    goal_1.push_back(Point3(-10.0, -0.1, -49.0));
    goal_1.push_back(Point3(-10.0, 10,  -49.0));
    goal_1.push_back(Point3(10.0, 10,  -49.0));
    goal_1.push_back(Point3(10.0, -0.1, -49.0));

    goal_1.push_back(Point3(10.0, 0, -53.0));
    goal_1.push_back(Point3(10.0, 10,  -49.0));
    goal_1.push_back(Point3(10.0, 0, -53.0));

    goal_1.push_back(Point3(-10.0, 0, -53.0));
    goal_1.push_back(Point3(-10.0, 10, -49.0));
    goal_1.push_back(Point3(-10.0, -0.1, -49.0));
    quickShapes_.DrawLines(modelMatrix_, viewMatrix_, projMatrix_, goalcol,
                            goal_1, QuickShapes::LinesType::LINE_LOOP, 0.2);
    std::vector<Point3> goal_2;
     goal_2.push_back(Point3(10.0, -0.1, 49.0));
     goal_2.push_back(Point3(-10.0, -0.1, 49.0));
     goal_2.push_back(Point3(-10.0, 10,  49.0));
     goal_2.push_back(Point3(10.0, 10,  49.0));
     goal_2.push_back(Point3(10.0, -0.1, 49.0));

     goal_2.push_back(Point3(10.0, 0, 53.0));
     goal_2.push_back(Point3(10.0, 10,  49.0));
     goal_2.push_back(Point3(10.0, 0, 53.0));

     goal_2.push_back(Point3(-10.0, 0, 53.0));
     goal_2.push_back(Point3(-10.0, 10, 49.0));
     goal_2.push_back(Point3(-10.0, -0.1, 49.0));

    quickShapes_.DrawLines(modelMatrix_,
                            viewMatrix_, projMatrix_, goalcol,
                            goal_2, QuickShapes::LinesType::LINE_LOOP, 0.2);

    std::vector<Point3> air_frame;
    air_frame.push_back(Point3(40.0, 0, -50.0));
    air_frame.push_back(Point3(40.0, 35, -50.0));
    air_frame.push_back(Point3(40.0, 35, 50.0));
    air_frame.push_back(Point3(40.0, 0, 50.0));
    air_frame.push_back(Point3(40.0, 35, 50.0));
    air_frame.push_back(Point3(-40.0, 35, 50.0));
    air_frame.push_back(Point3(-40.0, 0, 50.0));
    air_frame.push_back(Point3(-40.0, 35, 50.0));
    air_frame.push_back(Point3(-40.0, 35, -50.0));
    air_frame.push_back(Point3(-40.0, 0, -50.0));
    air_frame.push_back(Point3(-40.0, 35, -50.0));
    air_frame.push_back(Point3(40.0, 35, -50.0));
    quickShapes_.DrawLines(modelMatrix_,
                           viewMatrix_, projMatrix_, goalcol,
                           air_frame, QuickShapes::LinesType::LINE_LOOP, 0.1);



}

float absoluteDistance(Point3 p1, Point3 p2) {
    return sqrt(pow(p1[0]-p2[0], 2) +
           pow(p1[1]-p2[1], 2) +
           pow(p1[2]-p2[2], 2) );
}


void CarSoccer::updateBall(int id, double timeStep) {
    float new_x_pos, new_y_pos, new_z_pos;
    float new_x_velocity, new_y_velocity, new_z_velocity;
    Point3 ball_velocity = ball_.velocity();
    Point3 ball_pos = ball_.position();
    Point3 car_pos, car_velocity;
    if (id == 0) {
        car_pos = car_.position();
        car_velocity = car_.velocity();
    } else {
        car_pos = car_2.position();
        car_velocity = car_2.velocity();
    }
    // initialize ball position
    new_x_pos = ball_pos[0];
    new_y_pos = ball_pos[1];
    new_z_pos = ball_pos[2];
    new_x_velocity = ball_velocity[0];
    new_y_velocity = ball_velocity[1];
    new_z_velocity = ball_velocity[2];

    // Gravity
    // Y axis
    new_y_velocity -= gravity;
     if (new_y_pos <= ball_.radius()) {
         // Bound of the ground
         // Soft grass
         // V reduce by 0.5.
         new_y_velocity = -new_y_velocity * 0.5;
         new_y_pos = ball_.radius();
     }



    // Collisions
    float critical_distance = car_.collision_radius() + ball_.radius();

    // car/ball
    if (absoluteDistance(ball_pos, car_pos) <=  critical_distance) {
        // p' Correction

        if (abs(ball_pos[0] - car_pos[0]) <= critical_distance) {
            // Horizontal x
            // On the right
            if (ball_pos[0] - car_pos[0] > 0) {
                new_x_pos += (critical_distance -
                                        absoluteDistance(ball_pos, car_pos));
            // left
            } else {
                new_x_pos -= (critical_distance -
                                        absoluteDistance(ball_pos, car_pos));
            }
        } else if (abs(ball_pos[3] - car_pos[3]) <= critical_distance) {
            // Horizontal z
            // On the up
            if (ball_pos[0] - car_pos[0] > 0) {
                new_z_pos += (critical_distance -
                                        absoluteDistance(ball_pos, car_pos));
            // down
            } else {
                new_z_pos -= (critical_distance -
                                        absoluteDistance(ball_pos, car_pos));
            }
        } else if (abs(ball_pos[1] - car_pos[1]) <= critical_distance) {
            new_y_pos += (critical_distance -
                                        absoluteDistance(ball_pos, car_pos));
        }
        new_x_velocity += car_velocity[0];
        new_z_velocity += car_velocity[2];
    }

    new_x_pos += new_x_velocity * timeStep;
    new_y_pos += new_y_velocity * timeStep;
    new_z_pos += new_z_velocity * timeStep;


    // Wall/Ball
    // p' correction for x axis
    // ball radius
    // Goal
    if (new_x_pos - ball_.radius() >= -10 && new_x_pos - ball_.radius() <= 10 &&
        (new_z_pos - ball_.radius() >= 45 || new_z_pos + ball_.radius() <= -45) ) {
        car_.Reset();
        car_2.Reset();
        ball_.Reset();
        car_2.set_position(Point3(0, 1, -45));
        if (new_z_pos - ball_.radius() >= 45){
            win[0]++;
        }else{
            win[1]++;
        }
    } else {
        if (new_x_pos - ball_.radius() >= 36) {
            new_x_pos = 36 - ball_.radius();
            new_x_velocity = -0.8 * new_x_velocity;
            new_y_velocity = -0.8 * new_y_velocity;
            new_z_velocity = -0.8 * new_z_velocity;
            new_x_pos += new_x_velocity * timeStep;
            new_y_pos += new_y_velocity * timeStep;
            new_z_pos += new_z_velocity * timeStep;
        } else if (new_x_pos + ball_.radius() <= -36) {
            new_x_pos = -36 + ball_.radius();
            new_x_velocity = -0.8 * new_x_velocity;
            new_y_velocity = -0.8 * new_y_velocity;
            new_z_velocity = -0.8 * new_z_velocity;
            new_x_pos += new_x_velocity * timeStep;
            new_y_pos += new_y_velocity * timeStep;
            new_z_pos += new_z_velocity * timeStep;
        }
        // p' correction for z axis
        if (new_z_pos - ball_.radius() >= 45) {
            new_z_pos = 45 - ball_.radius();

            new_x_velocity = -0.8 * new_x_velocity;
            new_y_velocity = -0.8 * new_y_velocity;
            new_z_velocity = -0.8 * new_z_velocity;
            new_x_pos += new_x_velocity * timeStep;
            new_y_pos += new_y_velocity * timeStep;
            new_z_pos += new_z_velocity * timeStep;
        } else if (new_z_pos + ball_.radius() <= -45) {
            new_z_pos = -45 + ball_.radius();

            new_x_velocity = -0.8 * new_x_velocity;
            new_y_velocity = -0.8 * new_y_velocity;
            new_z_velocity = -0.8 * new_z_velocity;
            new_x_pos += new_x_velocity * timeStep;
            new_y_pos += new_y_velocity * timeStep;
            new_z_pos += new_z_velocity * timeStep;
        }
        // p' correction for y axis
        // Ceiling
        if (new_y_pos + ball_.radius() >= 35) {
            new_z_pos = 35 - ball_.radius();
            new_x_velocity = -0.8 * new_x_velocity;
            new_y_velocity = -0.8 * new_y_velocity;
            new_z_velocity = -0.8 * new_z_velocity;
            new_x_pos += new_x_velocity * timeStep;
            new_y_pos += new_y_velocity * timeStep;
            new_z_pos += new_z_velocity * timeStep;
        }
        // Update all values to the class
        ball_pos = Point3(new_x_pos, new_y_pos, new_z_pos);
        ball_velocity = Point3(new_x_velocity, new_y_velocity, new_z_velocity);
        ball_.set_position(ball_pos);
        ball_.set_velocity(ball_velocity);
    }

}

void CarSoccer::updateCar(int id, double timeStep) {
    // Initializer Value
    float new_x_pos, new_z_pos;
    float new_x_velocity, new_z_velocity;
    float angle, speed;

    // Get keyPressed value
    Vector2 dir;
    if (id == 0) {
        dir = CarSoccer::joystick_direction();
    } else {
        dir = CarSoccer::joystick_direction2();
    }

    Point3 car_pos, car_velocity;
    if (id == 0) {
        // Get car position
        car_pos = car_.position();
        // Get car velocity
        car_velocity = car_.velocity();
        new_x_velocity = car_velocity[0];
        new_z_velocity = car_velocity[2];
        // Get car degree
        angle = car_.angle();
        // Get car speed
        speed = car_.speed();
    } else {
        // Get car position
        car_pos = car_2.position();
        // Get car velocity
        car_velocity = car_2.velocity();
        new_x_velocity = car_velocity[0];
        new_z_velocity = car_velocity[2];
        // Get car degree
        angle = car_2.angle();
        // Get car speed
        speed = car_2.speed();
    }



    // Update Car velocity bases on keyPressed
    // Slide Mode
    // new_x_velocity += dir[0];
    // new_z_velocity -= dir[1];
    // Car mode
    angle = angle - (dir[0] / 180);
    speed = speed - (dir[1] * 0.05);
    new_x_velocity += sin(angle) *  speed;
    new_z_velocity += cos(angle) *  speed;
    // add some f to slow it down a little bit
    new_x_velocity *= 0.5;
    new_z_velocity *= 0.5;


    // Slide Mode
    // Update Car position bases on keyPressed
    // new_x_pos = car_pos[0] + dir[0];
    // new_z_pos = car_pos[2] - dir[1];
    // Car mode
    // Update Car position bases on velocity
    new_x_pos = car_pos[0] + new_x_velocity * timeStep;
    new_z_pos = car_pos[2] + new_z_velocity * timeStep;

    // p' correction for x axis
    // Car size (3,2,4)
    // 40 -> 35 for buffer area. 5 pixel for buffer
    if (new_x_pos - 3 >= 35) {
        new_x_pos = 40 - 3;
        new_x_velocity = 0;
        speed = 0;
    } else if (new_x_pos + 3 <= -35) {
        new_x_pos = -40 + 3;
        new_x_velocity = 0;
        speed = 0;
    }
    // p' correction for z axis
    // 50 -> 45 for buffer area. 5 pixel for buffer
    if (new_z_pos - 4 >= 45) {
        new_z_pos = 50 - 4;
        new_z_velocity = 0;
        speed = 0;
    } else if (new_z_pos + 4 <= -45) {
        new_z_pos = -50 + 4;
        new_z_velocity = 0;
        speed = 0;
    }

    // Update all values to the class
    if (id == 0) {
        car_pos = Point3(new_x_pos, car_pos[1], new_z_pos);
        car_velocity = Point3(new_x_velocity, car_velocity[1], new_z_velocity);
        car_.set_position(car_pos);
        car_.set_velocity(car_velocity);
        car_.set_angle(angle);
        car_.set_speed(speed);
    } else {
        car_pos = Point3(new_x_pos, car_pos[1], new_z_pos);
        car_velocity = Point3(new_x_velocity, car_velocity[1], new_z_velocity);
        car_2.set_position(car_pos);
        car_2.set_velocity(car_velocity);
        car_2.set_angle(angle);
        car_2.set_speed(speed);
    }

}
