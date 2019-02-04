
#include <iostream>
#include <cmath>

using namespace std;

class Ball {
    private:
        float v0_x;
        float v0_y;

        float vel_x;
        float vel_y;

        float pos_x;
        float pos_y;

        float ang_vel_x;
        float ang_vel_y;
        float ang_vel_z;

    public:
        Ball() = default;
        Ball(float in_v0_x, float in_v0_y, float in_vel_x, float in_vel_y, float in_pos_x, float in_pos_y);
        void ResetV0();
        float *getFriction(float time);
        float *getDirection();
        float getVelX();
        float getVelY();
        float getPosX();
        float getPosY();
        void printVel();
        void updateVel(float newVelx, float newVely);
};
