#pragma once

#include "main.h"

extern lemlib::Chassis chassis;

extern pros::ADIDigitalOut back_wings;
extern pros::ADIDigitalOut front_wings;
extern pros::Motor intake;
extern pros::Motor cata;

void lateral_tuning(int inches, int max_iter, float curve = 0.66);
void angular_tuning(int degrees, int max_iter, float curve = .5);

void def_startpos_test();
void far_defensive();

void ball6_near_offensive();

void match_load();
void skills();