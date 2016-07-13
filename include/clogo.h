#pragma once

#include <fstream>

extern "C" {
#include "clogo/clogo.h"
}

void eval_soo(std::ofstream& of);
void eval_logo(std::ofstream& of);
struct clogo_options test_soo();
struct clogo_options test_logo();
double eval_logo_final();
