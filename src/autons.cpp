#include "autons.hpp"
#include "lemlib/asset.hpp"
#include "pros/rtos.hpp"

void lateral_tuning(int inches, int max_iter, float curve) {
  chassis.setPose(0, 0, 0);
  chassis.moveToPoint(0, inches, 10e3);
  chassis.waitUntilDone();
  for (int iteration = 0; iteration < max_iter; iteration++) {
    chassis.moveToPoint(0, chassis.getPose().y * curve, 10e3,
                        {.forwards = false});
    chassis.waitUntilDone();
  }
}

void angular_tuning(int degrees, int max_iter, float curve) {
  chassis.setPose(0, 0, 0);
  chassis.turnToHeading(degrees, 10e3);
  chassis.waitUntilDone();
  for (int iteration = 0; iteration < max_iter; iteration++) {
    chassis.turnToHeading(chassis.getPose().theta * curve, 10e3);
    chassis.waitUntilDone();
  }
}

void def_startpos_test() {
  // set pos
  chassis.setPose(-38, -56, 0);

  // move to mid
  chassis.turnToPoint(-24, -24, 1500);
  chassis.moveToPoint(-24, -24, 1500);
}

void far_defensive() {
  // set pose
  chassis.setPose(-42, -56, 0);

  // go to outake pos
  chassis.turnToPoint(-58, -36, 1000);
  chassis.moveToPoint(-58, -36, 1200, {.maxSpeed = 127});
  chassis.turnToHeading(0, 1000);
  chassis.waitUntilDone();

  // outake
  intake.move_velocity(-400);
  pros::delay(300);
  chassis.moveToPoint(-58, -40, 700, {.forwards = false});

  // push
  chassis.turnToHeading(180, 1500);
  chassis.moveToPoint(-56, -29, 1000,
                      {.forwards = false, .minSpeed = 110}); //*/
  chassis.waitUntilDone();
  pros::delay(200);

  // take tribal out
  const int wing_dist = 13;
  chassis.moveToPoint(-58, -39, 1200);
  chassis.turnToPoint(-58 + wing_dist, -40 - wing_dist, 1200);
  chassis.moveToPoint(-58 + wing_dist, -40 - wing_dist, 2000, {.maxSpeed = 90});
  back_wings.set_value(true);

  // go to bar
  chassis.turnToPoint(-8.5, -60, 1200);
  chassis.moveToPoint(-8.5, -60, 2400, {.maxSpeed = 90});
  chassis.waitUntil(2);
  back_wings.set_value(false);
}

void ball6_near_offensive() {
  // set pose
  chassis.setPose(17, -60, -90);

  // go to mid triball
  chassis.moveToPoint(6, -60, 1200);
  chassis.waitUntil(9);
  intake.move_velocity(400);
  chassis.waitUntilDone();
  pros::delay(300);

  // unload matchload
  const int matchload_movement = 13;
  chassis.moveToPoint(40, -58, 2000, {.forwards = false});
  // move hiang
  chassis.turnToHeading(-225, 1000);
  chassis.moveToPoint(40 + matchload_movement, -58 + matchload_movement, 1200,
                      {.forwards = false});
  back_wings.set_value(true);
  chassis.turnToPoint(60, -10e5, 800);
  chassis.moveToPoint(60, -36, 1200, {.forwards = false});
  chassis.waitUntilDone();
  back_wings.set_value(false);
  chassis.turnToPoint(60, 0, 1000);
  chassis.moveToPoint(-60, -27.5, 1200);
  intake.move_velocity(-400);
}

ASSET(PUSH_BACK_TRIBALLS_txt)
ASSET(PUSH_RIGHT_SKILLS_txt)
void skills() {
  // set pose and follow path
  chassis.setPose(41, -54.5, -90);
  chassis.follow(PUSH_BACK_TRIBALLS_txt, 10, 2400, false);
  chassis.waitUntilDone();

  // go to match load
  chassis.moveToPoint(62, -40, 1200, {.minSpeed = 60});
  chassis.turnToPoint(-60, 0, 1200);
  chassis.waitUntilDone();
  chassis.tank(-20, -20);
  pros::delay(300);
  chassis.tank(0, 0);

  // match load
  chassis.turnToPoint(-60, 2, 600);
  back_wings.set_value(true);

  cata.move_velocity(50);
  pros::delay(30e3);
  cata.move(10);
  back_wings.set_value(false);

  // go topush up
  chassis.turnToPoint(8, -24, 800);
  chassis.moveToPoint(8, -24, 1200);
  chassis.turnToHeading(0, 1200);
  chassis.waitUntilDone();

  // push up
  front_wings.set_value(true);
  chassis.moveToPoint(8, 41, 1500);

  // set up for push
  chassis.moveToPoint(8, 34, 1200, {.forwards = false});
  chassis.turnToPoint(48, 48, 1200);
  chassis.moveToPoint(48, 48, 1200);
  chassis.turnToHeading(-45, 1200);

  // push
  chassis.follow(PUSH_RIGHT_SKILLS_txt, 10, 4000);

}

void ani_practice() {
  // set pose
  chassis.setPose(41, -54.5, -90);
}