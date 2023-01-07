#pragma once
#include <iostream>

#include "../../../globals/includes/includes.h"
#include "../../../utils/math/types/vector.h"
#include "../../../utils/netvar/netvar.h"
#include "../structs/data_map.h"
#include "../structs/var_mapping.h"

#include "c_handle.h"

#include "../enums/teams.h"
#include "i_client_entity.h"

namespace sdk
{
	struct c_base_entity : public i_client_entity {
	public:
		NETVAR( model_index, std::int32_t, "CBaseEntity", "m_nModelIndex" );
		NETVAR( team_number, sdk::teams, "CBaseEntity", "m_iTeamNum" );
		NETVAR( vector_origin, math::vec3, "CBaseEntity", "m_vecOrigin" );
		NETVAR( vector_angles, math::vec3, "CBaseEntity", "m_vecAngles" );
		NETVAR( owner_entity, std::uintptr_t, "CBaseEntity", "m_hOwnerEntity" );
		NETVAR( spotted, bool, "CBaseEntity", "m_bSpotted" );
		NETVAR( simulation_time, float, "CBaseEntity", "m_flSimulationTime" );
		NETVAR( effects, sdk::bit_flag_t< std::int32_t >, "CBaseEntity", "m_fEffects" );

		// NETVAR( surface_friction, float, this->prediction_description_map( ), "m_surfaceFriction");

		[[nodiscard]] math::matrix_3x4& rgfl_coordinate_frame( )
		{
			static std::uintptr_t offset = netvar::get_table( _( "CBaseEntity" ), _( "m_CollisionGroup" ) ) - 0x30;
			return *reinterpret_cast< math::matrix_3x4* >( reinterpret_cast< std::uintptr_t >( this ) + offset );
		}

		bool is_player( )
		{
			return virtual_func::call< bool >( this, 158 );
		}

		data_map* data_description_map( )
		{
			return virtual_func::call< data_map* >( this, 15 );
		}

		data_map* prediction_description_map( )
		{
			return virtual_func::call< data_map* >( this, 17 );
		}

		var_mapping* get_var_map( )
		{
			return reinterpret_cast< var_mapping* >( reinterpret_cast< std::uintptr_t >( this ) + 0x24 );
		}
	};
} // namespace sdk
