#include "geometry.h"

// Специализации с округлением для int
template<> template<>
vec3<int>::vec3(const vec3<float> &v) : x(int(v.x+.5f)), y(int(v.y+.5f)), z(int(v.z+.5f)) {}

template<> template<>
vec3<float>::vec3(const vec3<int> &v) : x(v.x), y(v.y), z(v.z) {}

template<> template<>
vec2<int>::vec2(const vec2<float> &v) : x(int(v.x+.5f)), y(int(v.y+.5f)) {}

template<> template<>
vec2<float>::vec2(const vec2<int> &v) : x(v.x), y(v.y) {}

template<> template<>
vec4<int>::vec4(const vec4<float> &v) : x(int(v.x+.5f)), y(int(v.y+.5f)), z(int(v.z+.5f)), w(int(v.w+.5f)) {}

template<> template<>
vec4<float>::vec4(const vec4<int> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}