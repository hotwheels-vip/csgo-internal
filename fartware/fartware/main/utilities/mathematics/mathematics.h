#pragma once
#include "../../source_engine/classes/c_angle.h"
#include "../../source_engine/classes/c_vector.h"
#include "../../source_engine/structs/matrix_t.h"
#include <directxmath.h>

struct mathematics_t {
	c_vector vector_transform( const c_vector& vector_to_transform, const matrix3x4_t& matrix );

	c_vector to_angle( const c_vector& in );

	void angle_vectors( const c_angle& angle, c_vector* forward, c_vector* right = nullptr, c_vector* up = nullptr );

	float normalize_yaw( float yaw );
};

inline mathematics_t mathematics = { };

#define HPI   DirectX::XM_PIDIV2
#define QPI   DirectX::XM_PIDIV4
#define PI    DirectX::XM_PI
#define GPI   1.6180339887498f
#define RADPI 57.295779513082f

#define METRE2INCH( x ) ( ( x ) / 0.0254f )
#define INCH2METRE( x ) ( ( x )*0.0254f )
#define METRE2FOOT( x ) ( ( x )*3.28f )
#define FOOT2METRE( x ) ( ( x ) / 3.28f )
#define RAD2DEG( x )    DirectX::XMConvertToDegrees( x )
#define DEG2RAD( x )    DirectX::XMConvertToRadians( x )
