#pragma once

using vec2_t = ImVec2;

using vec3_t = ImVec3;

using qangle_t = ImVec3;

struct mat3x4_t {
	__forceinline float* operator []( const int i ) { return m_matrix[ i ]; }

	__forceinline const float* operator []( const int i ) const { return m_matrix[ i ]; }

	__forceinline mat3x4_t operator+(const mat3x4_t& other) const {
		mat3x4_t ret;
		auto& m = m_matrix;
		for (int i = 0; i < 12; i++) {
			((float*)ret.m_matrix)[i] = ((float*)m)[i] + ((float*)other.m_matrix)[i];
		}
		return ret;
	}

	__forceinline mat3x4_t operator-(const mat3x4_t& other) const {
		mat3x4_t ret;
		auto& m = m_matrix;
		for (int i = 0; i < 12; i++) {
			((float*)ret.m_matrix)[i] = ((float*)m)[i] - ((float*)other.m_matrix)[i];
		}
		return ret;
	}

	__forceinline mat3x4_t operator*(const float& other) const {
		mat3x4_t ret;
		auto& m = m_matrix;
		for (int i = 0; i < 12; i++) {
			((float*)ret.m_matrix)[i] = ((float*)m)[i] * other;
		}
		return ret;
	}

	float m_matrix[ 3u ][ 4u ]{};

	__forceinline vec3_t get_origin() { return vec3_t(m_matrix[0][3], m_matrix[1][3], m_matrix[2][3]);	}
};

struct mat4x4_t {
	__forceinline float* operator []( const int i ) { return m_matrix[ i ]; }

	__forceinline const float* operator []( const int i ) const { return m_matrix[ i ]; }

	float m_matrix[ 4u ][ 4u ]{};
};

namespace math {
	constexpr auto k_pi = 3.14159265358979323846f;

	constexpr auto k_pi2 = k_pi * 2.f;

	constexpr auto k_rad_pi = 180.f / k_pi;

	constexpr auto k_deg_pi = k_pi / 180.f;

	__forceinline void sin_cos( const float rad, float& sin, float& cos );

	__forceinline float to_deg( const float rad );

	__forceinline float to_rad( const float deg );

	__forceinline float angle_diff( float src, float dst );

	__forceinline float approach_angle(float target, float value, float speed);

	__forceinline vec3_t calculate_angle(const vec3_t& src, const vec3_t& dst);

	__forceinline void concat_transforms( const mat3x4_t& in0, const mat3x4_t& in1, mat3x4_t& out );

	__forceinline void angle_vectors(
		const qangle_t& angle, vec3_t* const fwd, vec3_t* const right = nullptr, vec3_t* const up = nullptr
	);

	__forceinline void vector_angles( const vec3_t& in, qangle_t& out );

	__forceinline float calc_fov( const qangle_t& view_angles, const vec3_t& src, const vec3_t& dst );

	__forceinline void vector_transform( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline void vector_rotate( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline void vector_i_transform( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline void vector_i_rotate( const vec3_t& in, const mat3x4_t& matrix, vec3_t& out );

	__forceinline mat3x4_t vector_matrix( const vec3_t& in );

	__forceinline float segment_to_segment( const vec3_t& s1, const vec3_t& s2, const vec3_t& k1, const vec3_t& k2 );

	__forceinline bool line_vs_bb( const vec3_t& src, const vec3_t& dst, const vec3_t& min, const vec3_t& max );

	__forceinline bool to_screen( const vec3_t& world, const vec2_t& screen_size, const mat4x4_t& matrix, vec2_t& out );
}

#include "impl/math.inl"