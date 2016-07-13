#include <iostream>
#include <cassert>
#include <cmath>

#include "clogo.h"

int main() {
  struct clogo_options opt = test_soo();
  struct clogo_state state = clogo_init(&opt);
  while (!clogo_done(&state)) {
    clogo_step(&state);
  }
  clogo_delete(&state);
  return 0;
}
