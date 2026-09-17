#include "../enviro_helpers.h"
#include <cassert>
#include <limits>

int main() {
  using namespace enviro;
  assert(valid_interval(10, 30) == 10);
  assert(valid_interval(3600, 30) == 3600);
  assert(valid_interval(0, 30) == 30);
  assert(valid_interval(3601, 30) == 30);
  assert(valid_sample(-40, -40, 80));
  assert(valid_sample(80, -40, 80));
  assert(!valid_sample(80.1f, -40, 80));
  assert(!valid_sample(std::numeric_limits<float>::quiet_NaN(), -40, 80));
  assert(!valid_sample(std::numeric_limits<float>::infinity(), 0, 100));
  assert(calibrated_humidity(96, 10) == 100);
  assert(calibrated_humidity(3, -10) == 0);
  assert(calibrated_humidity(50, 2) == 52);
  assert(!fresh(0, 0, false, 30));
  assert(fresh(90000, 0, true, 30));
  assert(!fresh(90001, 0, true, 30));
  assert(fresh(10800000, 0, true, 3600));
  assert(!fresh(10800001, 0, true, 3600));
  const uint32_t before_wrap = UINT32_MAX - 999;
  assert(fresh(29000, before_wrap, true, 10));
  assert(!fresh(29001, before_wrap, true, 10));
  // Reducing the selected interval must immediately tighten freshness policy.
  assert(fresh(60000, 0, true, 30));
  assert(!fresh(60000, 0, true, 10));
}
