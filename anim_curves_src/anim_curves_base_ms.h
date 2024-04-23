// anim_curves_base_ms. RCSZ.
// version_mode: [x] NORMAL [ ] SIMD [ ] INTEL_SIMD [ ] AMD_SIMD
// update: 2024.04.22, v0.2

#ifndef __ANIM_CURVES_BASE_MS_H
#define __ANIM_CURVES_BASE_MS_H

#include "imgui.h"
#include "imgui_internal.h"

#define IMFXC_V_CLAMP(value, min, max) ((value) < (min) ? (min) : (value) > (max) ? (max) : (value))
#define IMFXC_PI 3.14159265f
#define IMFXC_DEGTORAD(deg) ((deg) * IMFXC_PI / 180.0f)

#define IMVEC2_DISTANCE(pos1, pos2) std::sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y))

#define COLOR_FMT_FP32(V) ImU32((V) * 255.0f)
#define IMFP32_CVT_COLU32(R, G, B, A) IM_COL32(ImU32(R * 255.0f), ImU32(G * 255.0f), ImU32(B * 255.0f), ImU32(A * 255.0f))
#define IMVEC4_CVT_COLU32(COL) IM_COL32(COLOR_FMT_FP32((COL).x), COLOR_FMT_FP32((COL).y), COLOR_FMT_FP32((COL).z), COLOR_FMT_FP32((COL).w))

#ifdef IMC_CONTROL_BASE_MATHS
static inline ImVec2 operator+(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x + s.x, v.y + s.y); }
static inline ImVec2 operator+(const ImVec2& v, float s)         { return ImVec2(v.x + s, v.y + s); }

static inline ImVec2 operator-(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x - s.x, v.y - s.y); }
static inline ImVec2 operator-(const ImVec2& v, float s)         { return ImVec2(v.x - s, v.y - s); }
static inline ImVec2 operator-(float s, const ImVec2& v)         { return ImVec2(s - v.x, s - v.y); }

static inline ImVec2 operator*(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x * s.x, v.y * s.y); }
static inline ImVec2 operator*(const ImVec2& v, float s)         { return ImVec2(v.x * s, v.y * s); }

static inline ImVec2 operator/(const ImVec2& v, const ImVec2& s) { return ImVec2(v.x / s.x, v.y / s.y); }
static inline ImVec2 operator/(const ImVec2& v, float s)         { return ImVec2(v.x / s, v.y / s); }
static inline ImVec2 operator/(float s, const ImVec2& v)         { return ImVec2(s / v.x, s / v.y); }

static inline ImVec2& operator+=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 + v2; }
static inline ImVec2& operator-=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 - v2; }
static inline ImVec2& operator*=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 * v2; }
static inline ImVec2& operator/=(ImVec2& v1, const ImVec2& v2) { return v1 = v1 / v2; }

static inline ImVec4 operator+(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x + s.x, v.y + s.y, v.z + s.z, v.w + s.w); }
static inline ImVec4 operator+(const ImVec4& v, float s)         { return ImVec4(v.x + s, v.y + s, v.z + s, v.w + s); }

static inline ImVec4 operator-(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x - s.x, v.y - s.y, v.z - s.z, v.w - s.w); }
static inline ImVec4 operator-(const ImVec4& v, float s)         { return ImVec4(v.x - s, v.y - s, v.z - s, v.w - s); }
static inline ImVec4 operator-(float s, const ImVec4& v)         { return ImVec4(s - v.x, s - v.y, s - v.z, s - v.w); }

static inline ImVec4 operator*(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x * s.x, v.y * s.y, v.z * s.z, v.w * s.w); }
static inline ImVec4 operator*(const ImVec4& v, float s)         { return ImVec4(v.x * s, v.y * s, v.z * s, v.w * s); }

static inline ImVec4 operator/(const ImVec4& v, const ImVec4& s) { return ImVec4(v.x / s.x, v.y / s.y, v.z / s.z, v.w / s.w); }
static inline ImVec4 operator/(const ImVec4& v, float s)         { return ImVec4(v.x / s, v.y / s, v.z / s, v.w / s); }
static inline ImVec4 operator/(float s, const ImVec4& v)         { return ImVec4(s / v.x, s / v.y, s / v.z, s / v.w); }

static inline ImVec4& operator+=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 + v2; }
static inline ImVec4& operator-=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 - v2; }
static inline ImVec4& operator*=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 * v2; }
static inline ImVec4& operator/=(ImVec4& v1, const ImVec4& v2) { return v1 = v1 / v2; }

#endif
#endif