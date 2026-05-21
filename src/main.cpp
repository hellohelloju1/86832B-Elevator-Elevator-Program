#include "main.h"
#include "screen.h"
#include "ctrl/drive.h"
#include "rawr.h"


// potent min 40 max 4095
void initialize() {
	screen::init();
	pros::delay(100);
	// screen::autonselector();

}

void disabled() {}

void competition_initialize() {}


void autonomous() {
	
}



            #define ROOT2 (sqrtf(2.0f)/2.0f)

    struct MotorAndPosition {
        pros::Motor motor;
        float dx, dy;

        MotorAndPosition(int port, float dx, float dy) : 
            dx(dx), dy(dy), 
            motor(pros::Motor(port, pros::v5::MotorGears::blue))
        { 
            motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        }
    };

void opcontrol() {
    while (true) {

     MotorAndPosition q1(10, ROOT2, -ROOT2);
     MotorAndPosition q2(9, ROOT2, ROOT2);
     MotorAndPosition q3(1, -ROOT2, ROOT2);
     MotorAndPosition q4(11, -ROOT2, -ROOT2);
     std::vector<MotorAndPosition*> motors = {&q1, &q2, &q3, &q4};
     std::vector<std::pair<MotorAndPosition*, float > > exertions;


        static int lset = 0;
        static int rset = 0;
        static long long ltime = 0;
        constexpr float slew_rate = 900;
        exertions.clear();
        float desired_vx = (float) rawr::control.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) / 127.0f;
        float desired_vy = (float) rawr:: control.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127.0f;
		float desired_turn = rawr::control.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) / 127.0f;

        float lefty = desired_vy * 200;
        float rightx = desired_turn * 200;
        float dt = (pros::micros() - ltime) / 1e6f;
        ltime = pros::micros();
        if (lefty<rawr::LIMIT_TOP && rawr::LIMIT_BOTTOMHAHA<lefty) {
        rawr::MotorLeft.brake(); rset = 0;
        rawr::MotorRight.brake(); lset = 0;
        } else if (lefty>rawr::LIMIT_TOP or lefty<rawr::LIMIT_BOTTOMHAHA){
        float slew_range = slew_rate * dt;
        rset = std::clamp(lefty-rightx, rset - slew_range, rset + slew_range);
        lset = std::clamp(lefty+rightx, lset - slew_range, lset + slew_range);
        rawr::MotorLeft.move_velocity(lset);
        rawr::MotorRight.move_velocity(rset);
        }
            float max_exert = 1e-6f;
        for (auto ptr : motors) {
            float exert = desired_vx * ptr->dx + desired_vy * ptr->dy;
            max_exert = fmax(max_exert, fabs(exert));
            exertions.push_back(std::make_pair(ptr, exert));
        }

        float desired_mag = sqrtf(desired_vx * desired_vx + desired_vy * desired_vy);
        for (auto& p : exertions) {
            int voltage = (int) (12000 * (p.second / max_exert) * desired_mag);
            voltage = std::clamp(voltage, -12000, 12000);
            p.first->motor.move_voltage(voltage);
        }
        
    drive::intake();
    pros::delay(50);
    }
}