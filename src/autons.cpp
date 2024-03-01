#include "autons.hpp"
#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
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
  chassis.turnToPoint(-58, -36, 1200);
  chassis.moveToPoint(-58, -36, 1200, {.maxSpeed = 127});
  chassis.turnToHeading(0, 1200);
  chassis.waitUntilDone();

  // outake
  intake.move_velocity(-400);
  pros::delay(300);
  chassis.moveToPoint(-58, -40, 700, {.forwards = false});

  // push
  chassis.turnToHeading(180, 1500);
  chassis.moveToPoint(-56, -29, 1200,
                      {.forwards = false, .minSpeed = 110}); //*/
  chassis.waitUntilDone();
  pros::delay(200);

  // take tribal out
  const int wing_dist = 13;
  chassis.moveToPoint(-60, -39, 1200);
  chassis.turnToPoint(-60 + wing_dist, -40 - wing_dist, 1200);
  chassis.moveToPoint(-60 + wing_dist, -40 - wing_dist, 2000, {.maxSpeed = 90});
  back_wings.set_value(true);

  // go to bar
  chassis.turnToPoint(-8, -60, 1200);
  chassis.moveToPoint(-8, -60, 2400, {.maxSpeed = 90});
  chassis.waitUntil(2);
  back_wings.set_value(false);
  chassis.turnToHeading(90, 1000);
}
ASSET(PUSH_BACK_TRIBALLS_txt)
void ball6_near_offensive() {
  // set pose
  chassis.setPose(24, -60, -90);

  // go to mid triball
  chassis.moveToPoint(9.25, -60, 1200);
  chassis.waitUntil(9);
  intake.move_velocity(400);
  chassis.waitUntilDone();
  pros::delay(300);

  /*/ unload matchload
  const int matchload_movement = 13;
  chassis.moveToPoint(42, -60, 2000, {.forwards = false});
  // move hiang
  chassis.turnToHeading(-135, 1200);
  intake.move(0);
  chassis.moveToPoint(42 + matchload_movement, -60 + matchload_movement, 1200,
                      {.forwards = false});
  back_wings.set_value(true);
  chassis.turnToHeading(-180, 1200);
  chassis.moveToPoint(53, -36, 1200, {.forwards = false});

  back_wings.set_value(false);
  chassis.turnToPoint(53, 0, 1200);
  chassis.moveToPoint(53, -27.5, 1200);
  intake.move_velocity(-400);//*/

  // unload
  chassis.moveToPoint(41, -60, 1200, {.forwards = false});
  chassis.follow(PUSH_BACK_TRIBALLS_txt, 10, 2400, false);
  chassis.waitUntil(10);
  back_wings.set_value(true);
  chassis.moveToPoint(60, -40, 1200);
  chassis.turnToHeading(0, 1200);
}

void ball4_safe() {
  // set pose
  chassis.setPose(33, -56, 0);

  // put first
  chassis.moveToPoint(36, -24, 1500);
  chassis.turnToPoint(48, 0, 1200);
  chassis.waitUntilDone();
  intake.move_velocity(-400);
  pros::delay(300);

  // get second
  chassis.turnToPoint(-9, -24, 1200);
  chassis.moveToPoint(-9.5, -24, 1200);
  intake.move_velocity(400);
  chassis.waitUntilDone();
  pros::delay(300);

  // outake second
  chassis.turnToPoint(-48, 0, 1200);
  chassis.waitUntilDone();
  intake.move_velocity(-400);
  pros::delay(300);

  // go to third
  chassis.moveToPoint(10, 0, 1200);
}

void match_load() {
  // set pose and follow path
  chassis.setPose(41, -55, -90);
  chassis.follow(PUSH_BACK_TRIBALLS_txt, 11, 2400, false);
  chassis.waitUntilDone();
  chassis.moveToPoint(60, -36, 1000, {.minSpeed = 80});
  chassis.moveToPoint(60, -28, 1000, {.forwards = false, .minSpeed = 110});

  // go to match load
  chassis.moveToPoint(60, -40, 1200, {.minSpeed = 60});
  chassis.turnToPoint(-60, 9.25, 1200);
  chassis.waitUntilDone();
  chassis.tank(-30, -30);
  pros::delay(300);
  chassis.tank(0, 0);
  back_wings.set_value(true);
  chassis.turnToPoint(-60, 9.25, 1000); //*/
}

ASSET(PUSH_RIGHT_SKILLS_txt)
ASSET(PUSH_LEFT_FINAL_txt)
void skills() {
  // match load
  match_load();

  cata.move_velocity(75);
  //pros::delay(22e3);
  cata.move(10);
  back_wings.set_value(false);

  // go to push up
  chassis.turnToPoint(14, -24, 1000);
  chassis.moveToPoint(14, -24, 1200);
  chassis.turnToHeading(0, 1200);
  chassis.waitUntilDone();

  // push up
  front_wings.set_value(true);
  intake.move_velocity(-400);
  chassis.moveToPoint(12, 38.5, 1500, {.minSpeed = 120});
  chassis.moveToPoint(12, 34.5, 1000, {.forwards = false, .minSpeed = 90});
  chassis.moveToPoint(12, 38.5, 1000, {.minSpeed = 110});
  chassis.moveToPoint(12, 31.5, 1000, {.forwards = false, .minSpeed = 90});

  // set up for push
  chassis.turnToPoint(45, 48, 1200, {.minSpeed = 10});
  chassis.moveToPoint(45, 48, 1200, {.maxSpeed = 90});
  chassis.turnToPoint(24, 62, 1200);

  // side push
  chassis.moveToPoint(24, 62, 2000, {.minSpeed = 90, .earlyExitRange = 7});
  chassis.waitUntilDone();
  chassis.moveToPoint(-36, 63, 1500, {.minSpeed = 110, .earlyExitRange = 7});
  chassis.waitUntil(6);
  front_wings.set_value(false);
  chassis.waitUntil(28);
  front_wings.set_value(true);
  chassis.moveToPoint(-70, 30, 1500, {.minSpeed = 127});
  chassis.turnToHeading(-180, 400);

  front_wings.set_value(false);

  // go to right push
  chassis.moveToPoint(-48, 52, 1200, {.forwards = false});
  chassis.turnToPoint(-38, 36, 1200);
  chassis.moveToPoint(-38, 36, 1200);
  chassis.turnToPoint(-14, 14, 1200);
  chassis.moveToPoint(-14, 14, 1200);
  chassis.turnToHeading(90, 1200);

  // right push
  chassis.moveToPoint(-48, 14, 1000, {.forwards = false, .minSpeed = 127});
  back_wings.set_value(true);
  chassis.moveToPoint(-34, 14, 1000, {.minSpeed = 110});
  chassis.moveToPoint(-48, 14, 1000, {.forwards = false, .minSpeed = 127});
  chassis.moveToPoint(-14, 14, 1000, {.minSpeed = 110});
  back_wings.set_value(false);

  // left push
  chassis.turnToPoint(-14, 0, 1200);
  chassis.moveToPoint(-14, 0, 1500);

  chassis.turnToHeading(90, 1200);
  chassis.moveToPoint(-48, 0, 1000, {.forwards = false, .minSpeed = 127});
  back_wings.set_value(true);
  chassis.turnToPoint(-24, -0, 1200);
  chassis.moveToPoint(-24, -0, 1000, {.minSpeed = 100});
  chassis.moveToPoint(-49, -0, 1000, {.forwards = false, .minSpeed = 127});
  chassis.waitUntilDone();

  // side push left
  back_wings.set_value(false);
  chassis.moveToPoint(-24, -36, 1200, {.minSpeed = 110});
  chassis.turnToPoint(-48, -36, 1200);
  chassis.moveToPoint(-48, -36, 1200);
  chassis.moveToPose(-24, -60, 90, 1500, {.forwards = false,.lead=1});

  chassis.turnToHeading(-90, 400);
  chassis.follow(PUSH_LEFT_FINAL_txt, 10, 2400);
  chassis.moveToPoint(36, 60, 400);
}
