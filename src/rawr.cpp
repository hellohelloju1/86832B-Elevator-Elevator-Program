#include "rawr.h"
#include "api.h"
#include <iostream>
namespace rawr{
    int LIMIT_TOP = -10;
    int LIMIT_BOTTOMHAHA = 30;

    pros::Controller control(pros::E_CONTROLLER_MASTER);
    pros::Motor MotorLeft (-19);
    pros::Motor MotorRight (20);
    pros::Motor intake (16);

}