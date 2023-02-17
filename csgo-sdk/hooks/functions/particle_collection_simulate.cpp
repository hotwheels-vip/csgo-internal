#include "../hooks.h"

#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"

#include <ranges>

/* TODO ~ move to other files (keep includes clean.) */
#define PARTICLE_ATTRIBUTE_TINT_RGB 6
#define PARTICLE_ATTRIBUTE_ALPHA    7

#define MAX_PARTICLE_ATTRIBUTES 24

struct c_particle_attribute_address_table {
	float* m_attributes[ MAX_PARTICLE_ATTRIBUTES ];
	unsigned int m_float_strides[ MAX_PARTICLE_ATTRIBUTES ];

	float* float_attribute( int attribute, int particle_number ) const
	{
		int blockofs = particle_number / 4;
		return m_attributes[ attribute ] + m_float_strides[ attribute ] * blockofs + ( particle_number & 3 );
	}

	void modulate_color( const c_color& color, int num )
	{
		auto rgb = this->float_attribute( PARTICLE_ATTRIBUTE_TINT_RGB, num );
		auto a   = this->float_attribute( PARTICLE_ATTRIBUTE_ALPHA, num );

		rgb[ 0 ] = color.base< e_color_type::color_type_r >( );
		rgb[ 4 ] = color.base< e_color_type::color_type_g >( );
		rgb[ 8 ] = color.base< e_color_type::color_type_b >( );

		*a = color.base< e_color_type::color_type_a >( );
	}

	void modulate_alpha( float alpha, int num )
	{
		auto a = float_attribute( PARTICLE_ATTRIBUTE_ALPHA, num );
		*a     = alpha;
	}
};

class c_particle_system_definition
{
public:
	unsigned char pad0[ 308 ];
	c_utl_string m_name;
};

class c_particle_collection
{
public:
	unsigned char pad0[ 48 ];
	int m_active_particles;
	unsigned char pad1[ 12 ];
	c_utl_reference< c_particle_system_definition > m_def;
	unsigned char pad2[ 60 ];
	c_particle_collection* m_parent;
	unsigned char pad3[ 84 ];
	c_particle_attribute_address_table m_particle_attributes;
};

/* https://github.com/Synth1a/uwu.beta/blob/31f46f3ff6fe283d69e4e261222dc7abb987dba0/uwu.beta/csgo_sdk/hooks.cpp#L347 */
void __fastcall n_detoured_functions::particle_collection_simulate( void* ecx, void* edx )
{
	static auto original = g_hooks.m_particle_collection_simulate.get_original< void( __fastcall* )( void* ) >( );

	original( ecx );

	const auto particle_collection = reinterpret_cast< c_particle_collection* >( ecx );

	c_particle_collection* root = particle_collection;
	while ( root->m_parent )
		root = root->m_parent;

	const auto hash = HASH_RT( std::string( root->m_def.m_obj->m_name.m_buffer ).c_str( ) );

	/* looks so ugly :skull: */
	if ( GET_VARIABLE( g_variables.m_precipitation, bool ) &&
	     ( hash == HASH_BT( "rain" ) || hash == HASH_BT( "rain_storm" ) || hash == HASH_BT( "snow" ) || hash == HASH_BT( "ash" ) ) )
		for ( auto iterator : std::views::iota( 0, particle_collection->m_active_particles ) )
			particle_collection->m_particle_attributes.modulate_color( GET_VARIABLE( g_variables.m_precipitation_color, c_color ), iterator );

	if ( GET_VARIABLE( g_variables.m_custom_smoke, bool ) &&
	     ( hash == HASH_BT( "explosion_smokegrenade" ) || hash == HASH_BT( "explosion_smokegrenade_fallback" ) ) )
		for ( auto iterator : std::views::iota( 0, particle_collection->m_active_particles ) )
			particle_collection->m_particle_attributes.modulate_color( GET_VARIABLE( g_variables.m_custom_smoke_color, c_color ), iterator );

	if ( GET_VARIABLE( g_variables.m_custom_molotov, bool ) &&
	     ( hash == HASH_BT( "explosion_molotov_air" ) || hash == HASH_BT( "extinguish_fire" ) || hash == HASH_BT( "molotov_groundfire" ) ||
	       hash == HASH_BT( "molotov_groundfire_fallback" ) || hash == HASH_BT( "molotov_groundfire_fallback2" ) ||
	       hash == HASH_BT( "molotov_explosion" ) || hash == HASH_BT( "weapon_molotov_held" ) || hash == HASH_BT( "weapon_molotov_fp" ) ||
	       hash == HASH_BT( "weapon_molotov_thrown" ) ) )
		for ( auto iterator : std::views::iota( 0, particle_collection->m_active_particles ) )
			particle_collection->m_particle_attributes.modulate_color( GET_VARIABLE( g_variables.m_custom_molotov_color, c_color ), iterator );

	if ( GET_VARIABLE( g_variables.m_custom_blood, bool ) &&
	     ( hash == HASH_BT( "blood_impact_light" ) || hash == HASH_BT( "blood_impact_medium" ) || hash == HASH_BT( "blood_impact_heavy" ) ||
	       hash == HASH_BT( "blood_impact_light_headshot" ) ) )
		for ( auto iterator : std::views::iota( 0, particle_collection->m_active_particles ) )
			particle_collection->m_particle_attributes.modulate_color( GET_VARIABLE( g_variables.m_custom_blood_color, c_color ), iterator );
}
