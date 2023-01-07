#pragma once

#include "../../../dependencies/checksum/checksum.hpp"
#include "../../../globals/macros/macros.h"
#include "../../../utils/helpers/bit_flag.h"
#include "../../../utils/math/types/vector.h"

#include <cstdint>

namespace sdk
{
	class c_user_cmd
	{
	public:
		inline CRC32_t checksum( )
		{
			CRC32_t crc;
			CRC32_Init( &crc );

			CRC32_ProcessBuffer( &crc, &command_number, sizeof( command_number ) );
			CRC32_ProcessBuffer( &crc, &tick_count, sizeof( tick_count ) );
			CRC32_ProcessBuffer( &crc, &view_angles, sizeof( view_angles ) );
			CRC32_ProcessBuffer( &crc, &aim_direction, sizeof( aim_direction ) );
			CRC32_ProcessBuffer( &crc, &forward_move, sizeof( forward_move ) );
			CRC32_ProcessBuffer( &crc, &side_move, sizeof( side_move ) );
			CRC32_ProcessBuffer( &crc, &up_move, sizeof( up_move ) );
			CRC32_ProcessBuffer( &crc, &buttons, sizeof( buttons ) );
			CRC32_ProcessBuffer( &crc, &impulse, sizeof( impulse ) );
			CRC32_ProcessBuffer( &crc, &weapon_select, sizeof( weapon_select ) );
			CRC32_ProcessBuffer( &crc, &weapon_subtype, sizeof( weapon_subtype ) );
			CRC32_ProcessBuffer( &crc, &random_seed, sizeof( random_seed ) );
			CRC32_ProcessBuffer( &crc, &moused_x, sizeof( moused_x ) );
			CRC32_ProcessBuffer( &crc, &moused_y, sizeof( moused_y ) );

			CRC32_Final( &crc );
			return crc;
		}

		virtual ~c_user_cmd( ){ };

		int command_number;
		int tick_count;
		math::vec3 view_angles;
		math::vec3 aim_direction;
		float forward_move;
		float side_move;
		float up_move;
		sdk::int_flag_t buttons;
		char impulse;
		int weapon_select;
		int weapon_subtype;
		int random_seed;
		short moused_x;
		short moused_y;
		bool has_been_predicted;
		PAD( 0x18 );
	};
} // namespace sdk
