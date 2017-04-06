#include "moveable.hxx"

using namespace glm;

vec2 Moveable::set_velocity(vec2 new_vel) {
    vec2 old_vel = this->velocity;
    this->velocity = new_vel;
    /*
     * TODO: calculate drag
     * we could make different objects have different parameters, or not
     */
    return old_vel;
}

vec2 Moveable::get_velocity() {
    return this->velocity;
}

vec2 Moveable::set_acceleration(vec2 new_acc) {
    vec2 old_acc = this->acceleration;
    vec2 real_acc = new_acc;
    double len = length(real_acc);
    /*
     * TODO: I'm fucking certain this isn't the most efficient way
     */
    if (len > this->max_acceleration) {
        double scalar = max_acceleration / len;
        vec2 scale(scalar);
        real_acc = dot(&real_acc, &scale);
    }
    this->acceleration = real_acc;
    return old_acc;
}

vec2 Moveable::get_acceleration() {
    return this->acceleration;
}

double Moveable::get_max_acceleration() {
    return this->max_acceleration;
}

double Moveable::set_max_acceleration(double new_max_acc) {
    double old_max_acc = this->max_acceleraion;
    this->max_acceleration = new_max_acc;
    return old_max_acc;
}

double Moveable::get_drag() {
    return this->drag;
}

double Moveable::set_drag(double new_drag) {
    double old_drag = this->drageleraion;
    this->drag = new_drag;
    return old_drag;
}

virtual int Moveable::moveable_overwrite(vec2 velocity, vec2 acceleration, double max_acc, double drag) {
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->max_acc = max_acc;
    this->drag = drag;
    return 0;
}

virtual int Moveable::moveable_prepare_message() {
    return 0;
}

int Moveable::adjust_velocity(double t_step) {
    vec2 accel = this->acceleration - (this->drag * this->velocity);
    set_velocity(this->velocity + accel);

    return 0;
}
