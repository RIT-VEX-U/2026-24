#pragma once

/**
 * Main entrypoint for the autonomous period
*/
extern void (*autonomous)();

void right_auto_path();
void left_auto_path();
void skills_path();