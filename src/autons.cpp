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
void match_load() {
  // set pose and follow path
  chassis.setPose(41, -54.5, -90);
  chassis.follow(PUSH_BACK_TRIBALLS_txt, 10, 2400, false);
  chassis.waitUntilDone();

  // go to match load
  chassis.moveToPoint(62, -42, 1200, {.minSpeed = 60});
  chassis.turnToPoint(-60, 2, 1200);
  chassis.waitUntilDone();
  chassis.tank(-30, -30);
  pros::delay(300);
  chassis.tank(0, 0);
  back_wings.set_value(true);
  chassis.turnToPoint(-60, 2, 800);
}

ASSET(PUSH_RIGHT_SKILLS_txt)
void skills() {
  // match load
  match_load();

  cata.move_velocity(53);
  pros::delay(27e3);
  cata.move(10);
  back_wings.set_value(false);

  // go to push up
  chassis.turnToPoint(3, -24, 800);
  chassis.moveToPoint(3, -24, 1200);
  chassis.turnToHeading(0, 1200);
  chassis.waitUntilDone();

  // push up
  front_wings.set_value(true);
  intake.move_velocity(-400);
  chassis.moveToPoint(3, 42, 2000, {.minSpeed = 60});
  chassis.moveToPoint(3, 38, 1000, {.forwards = false});
  chassis.moveToPoint(3, 42, 1000, {.minSpeed = 100});
  chassis.moveToPoint(3, 35, 1000, {.forwards = false});

  // set up for push
  chassis.turnToPoint(45, 48, 1200);
  chassis.moveToPoint(45, 48, 1500);
  chassis.turnToPoint(24, 60, 1200);

  // push
  chassis.moveToPoint(24, 60, 2500, {.minSpeed = 110, .earlyExitRange = 10});
  chassis.waitUntilDone();
  chassis.moveToPoint(-36, 63, 1500, {.minSpeed = 110, .earlyExitRange = 10});
  chassis.waitUntil(3);
  front_wings.set_value(false);
  chassis.waitUntil(28);
  front_wings.set_value(true);
  chassis.moveToPoint(-60, 36, 1200, {.minSpeed = 100, .earlyExitRange = 10});
  chassis.moveToPoint(-60, 28, 1200, {.forwards = false, .minSpeed = 127});
  
  // back push
  chassis.moveToPoint(-60, 31, 1200, {.forwards = false});
  chassis.turnToHeading(0,1200);

  chassis.moveToPoint(-60, 27, 1200, {.minSpeed = 110});

  // go to main push
  chassis.turnToPoint(-48, 48, 1000);
  chassis.moveToPoint(-48, 48, 1200);

  chassis.turnToPoint(-12, 12, 1200);
  chassis.moveToPoint(-12, 12, 1500);

  // main push
  chassis.turnToHeading(90, 1200);
  chassis.moveToPoint(-44, 12, 2000, {.forwards = false, .minSpeed = 90});
  back_wings.set_value(true);

  // side push
  chassis.turnToPoint(-12, -12, 1200);
  back_wings.set_value(false);
  chassis.moveToPoint(-12, -12, 1500);
  chassis.turnToHeading(90, 1200);
  chassis.moveToPoint(-44, -12, 1500, {.forwards = false, .minSpeed = 90});
  back_wings.set_value(true);//*/
}
