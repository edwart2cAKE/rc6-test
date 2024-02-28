#include "main.h"
#include "autons.hpp"
#include <limits>

// right motors
pros::Motor rF(14, pros::E_MOTOR_GEARSET_18, false);
pros::Motor rM(16, pros::E_MOTOR_GEARSET_18, false);
pros::Motor rT(15, pros::E_MOTOR_GEARSET_18, true);

// left motors
pros::Motor lF(18, pros::E_MOTOR_GEARSET_18, false);
pros::Motor lM(19, pros::E_MOTOR_GEARSET_18, true);
pros::Motor lT(20, pros::E_MOTOR_GEARSET_18, true);

// intake
pros::Motor intake(8, pros::E_MOTOR_GEARSET_06, true);

// cheat hang
pros::ADIDigitalOut hang(3, false);

// back wings
bool back_wings_engaged = false;
pros::ADIDigitalOut back_wings(7, back_wings_engaged);

// front wings
bool front_wings_engaged = false;
pros::ADIDigitalOut front_wings(6, front_wings_engaged);

// cata / slapper
bool shooting_cata = false;
pros::Motor cata(3, pros::E_MOTOR_GEARSET_36, true);

//  make drivetrain
pros::MotorGroup left_motors({lT, lM, lF});
pros::MotorGroup right_motors({rT, rM, rF});

lemlib::Drivetrain drivetrain{&left_motors, &right_motors,
                              10.55,        lemlib::Omniwheel::NEW_325,
                              1000 / 3,     20};

// sensors (only inertial right now)
pros::Imu inertial_sensor(10);

lemlib::OdomSensors sensors{nullptr, nullptr, nullptr, nullptr,
                            &inertial_sensor};

// PIDs - in tuning

// forward/backward PID
lemlib::ControllerSettings lateralController{
    40.0, // proportional gain (kP)
    0.01, // integral gain (kI)
    60.0, // derivative gain (kD)
    3,    // anti windup
    0.5,  // small error range, in inches
    100,  // small error range timeout, in milliseconds
    1,    // large error range, in inches
    300,  // large error range timeout, in milliseconds
    3     // maximum acceleration (slew)

};

// turning PID
lemlib::ControllerSettings angularController{
    2.75, // proportional gain (kP)
    0.01, // integral gain (kI)
    30,   // derivative gain (kD)
    2,    // anti windup
    1,    // small error range, in degrees
    100,  // small error range timeout, in milliseconds
    3,    // large error range, in degrees
    300,  // large error range timeout, in milliseconds
    3     // maximum acceleration (slew)
};

// make chassis
lemlib::Chassis chassis(drivetrain, lateralController, angularController,
                        sensors);

// define controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::lcd::initialize();
  pros::lcd::set_text(1, "Hello PROS User!");

  // initialize chassis - starts odom
  chassis.calibrate();

  // init motors
  intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  cata.set_brake_mode(MOTOR_BRAKE_HOLD);

  pros::Task screenTask([&]() {
    lemlib::Pose pose(0, 0, 0);
    while (true) {
      // print robot location to the brain screen
      pros::lcd::print(3, "X: %f", chassis.getPose().x);         // x
      pros::lcd::print(4, "Y: %f", chassis.getPose().y);         // y
      pros::lcd::print(5, "Theta: %f", chassis.getPose().theta); // heading

      // print to controller screen
      master.print(4, 0, "Y: %f", chassis.getPose().y);

      // log position telemetry
      lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
      printf("X: %f", chassis.getPose().x);
      printf("Y: %f", chassis.getPose().y);
      printf("Theta: %f", chassis.getPose().theta);
      // delay to save resources
      pros::delay(50);
    }
  });
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

/*
-101: lateral pid tuning
-102: angular pid tuning
0: left side
1: right side
2: auto skills
*/
double auton_select = 2;
void autonomous() {
  chassis.setBrakeMode(MOTOR_BRAKE_HOLD);
  if (auton_select == -101) {
    lateral_tuning(60, 3);
  } else if (auton_select == -102) {
    angular_tuning(90, 3);
  } else if (auton_select == 0) {
    far_defensive();
  } else if (auton_select == 0.1) {
    def_startpos_test();
  } else if (auton_select == 1) {
    ball6_near_offensive();
  } else if (auton_select == 2) {
    skills();
  }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
float curve = 2.3;
bool match_loaded = false;
void opcontrol() {
  // set brake type (driver likes brake)
  chassis.setBrakeMode(MOTOR_BRAKE_BRAKE);


  while (true) {
    pros::lcd::print(5, "           Curve: %f", curve);
    int left = master.get_analog(ANALOG_LEFT_Y);
    int right = master.get_analog(ANALOG_RIGHT_Y);

    chassis.tank(left, right, curve);

    // change drive curve
    if (master.get_digital_new_press(DIGITAL_UP)) {
      curve += 0.1;
    } else if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
      curve -= 0.1;
    }

    // intake
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      intake.move_velocity(600);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      intake.move_velocity(-600);
    } else {
      intake.brake();
    }

    // cata
    if (master.get_digital_new_press(DIGITAL_L1)) {
      shooting_cata = !shooting_cata;
    }
    if (shooting_cata) {
      cata.move_velocity(53);
    } else {
      cata.brake();
    }

    // warn about cata overheat
    if (cata.get_temperature() >= 55) {
      pros::lcd::set_background_color(255,0,0);
    } else {
      pros::lcd::set_background_color(0,255,0);
    }

    // back wings
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
      back_wings_engaged = !back_wings_engaged;
      back_wings.set_value(back_wings_engaged);
    }

    // front wings
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      front_wings_engaged = !front_wings_engaged;
      front_wings.set_value(front_wings_engaged);
    }

    // auto skills
    if (auton_select == 2 && master.get_digital_new_press(DIGITAL_Y) && !match_loaded) {
      match_load();
      match_loaded = true;
    }

    pros::delay(20);
  }
}
