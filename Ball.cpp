#include "Ball.hpp"

//

Ball::Ball(float in_v0_x, float in_v0_y, float in_vel_x, float in_vel_y, float in_pos_x, float in_pos_y)
:v0_x{in_v0_x}, v0_y{in_v0_y},vel_x{in_vel_x}, vel_y{in_vel_y}, pos_x{in_pos_x}, pos_y{in_pos_y}{
    ang_vel_x = 0.0;
    ang_vel_y = 0.0;
    ang_vel_z = 0.0;
}

//Returns an array with x- and y-direction normalized
float *Ball::getDirection() {

    float dir[2];
    dir[0] = vel_x / abs(vel_x + vel_y);
    dir[1] = vel_y / abs(vel_x + vel_y);

    return dir;
}

void Ball::ResetV0() {
    v0_x = vel_x;
    v0_y = vel_y;
}

float Ball::getVelX() {
    return vel_x;
}

float Ball::getVelY() {
    return vel_y;
}

float Ball::getPosX(){
    return pos_x;
}

float Ball::getPosY(){
    return pos_y;
}

void Ball::updateVel(float newVelx, float newVely){
    vel_x = newVelx;
    vel_y = newVely;
}

//Returns an array with x- and y-direction koefficients
//time is the time passed since collision
float *Ball::getFriction(float time) {

    float ur = 0.01;
    float ug = 0.2;
    float g = 9.82;
    float fricArr[2];

    float tswitch_x = (2/7)*(v0_x/(g*ug));
    float tswitch_y = (2/7)*(v0_y/(g*ug));

    if(time < tswitch_x) {
        fricArr[0] = (time/tswitch_x)*ur + (1-(time/tswitch_x))*ug;
    }
    else {
        fricArr[0] = ur;
    }

    if(time < tswitch_y) {
        fricArr[1] = (time/tswitch_y)*ur + (1-(time/tswitch_y))*ug;
    }
    else {
        fricArr[1] = ur;
    }

    return fricArr;
}

void Ball::printVel() {
    cout << "Velocity in direction x: " << vel_x << endl;
    cout << "Velocity in direction y: " << vel_y << endl;
}
