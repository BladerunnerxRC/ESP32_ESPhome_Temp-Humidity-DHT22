#pragma once
#include <cmath>
#include <cstdint>

namespace enviro {
inline int valid_interval(int value, int fallback) {
  return value >= 10 && value <= 3600 ? value : fallback;
}
inline bool valid_sample(float value, float lower, float upper) {
  return std::isfinite(value) && value >= lower && value <= upper;
}
inline float calibrated_humidity(float value, float offset) {
  return std::fmin(100.0f, std::fmax(0.0f, value + offset));
}
inline bool fresh(uint32_t now, uint32_t last, bool has_value, int interval_s) {
  // Subtract milliseconds before conversion: unsigned wraparound is intentional.
  return has_value && uint32_t(now - last) <= uint32_t(valid_interval(interval_s, 30)) * 3000U;
}
}  // namespace enviro
