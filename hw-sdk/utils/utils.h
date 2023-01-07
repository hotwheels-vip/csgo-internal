#pragma once

#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <thread>

#include "math/types/matrix.h"
#include "math/types/vector.h"

namespace utils
{
	void sleep( std::uint32_t time );

	BOOL cheat_create_thread( DWORD WINAPI function( void* ), void* param );

	math::vec2< int > world_to_screen( math::vec3 position );
	math::vec2< int > world_to_screen( math::vec3 position, bool& on_screen );

	namespace str
	{
		std::string wide_to_multi_byte( std::wstring_view str );
		std::wstring multi_byte_to_wide( std::string_view str );
	}
} // namespace utils
