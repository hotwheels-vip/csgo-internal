#pragma once

#include <string_view>

inline static const char* filtered_key_names[] = {
	"-",   "m1",  "m2",  "cn",  "m3",  "m4",  "m5",  "-",   "bac", "tab", "-",   "-",   "clr", "ret", "-",   "-",   "shi", "ctl", "men", "pau", "cap",
	"kan", "-",   "jun", "fin", "kan", "-",   "esc", "con", "nco", "acc", "mad", "spa", "pgu", "pgd", "end", "hom", "lef", "up",  "rig", "dow", "sel",
	"pri", "exe", "pri", "ins", "del", "hel", "0",   "1",   "2",   "3",   "4",   "5",   "6",   "7",   "8",   "9",   "-",   "-",   "-",   "-",   "-",
	"-",   "-",   "a",   "b",   "c",   "d",   "e",   "f",   "g",   "h",   "i",   "j",   "k",   "l",   "m",   "n",   "o",   "p",   "q",   "r",   "s",
	"t",   "u",   "v",   "w",   "x",   "y",   "z",   "win", "win", "app", "-",   "sle", "num", "num", "num", "num", "num", "num", "num", "num", "num",
	"num", "mul", "add", "sep", "min", "dec", "div", "f1",  "f2",  "f3",  "f4",  "f5",  "f6",  "f7",  "f8",  "f9",  "f10", "f11", "f12", "f13", "f14",
	"f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "-",   "-",   "-",   "-",   "-",   "-",   "-",   "-",   "num", "scr", "equ",
	"mas", "toy", "oya", "oya", "-",   "-",   "-",   "-",   "-",   "-",   "-",   "-",   "-",   "shi", "shi", "ctr", "ctr", "alt", "alt"
};

struct utilities_t {
	void* get_module_handle( const std::string_view module_name );
	void* get_export_address( const void* module_handle, const std::string_view export_name );

	template< typename T = void* >
	constexpr T get_virtual_function( void* thisptr, std::size_t index )
	{
		return ( *static_cast< T** >( thisptr ) )[ index ];
	}

	inline std::uintptr_t get_absolute_address( const std::uintptr_t relative_address )
	{
		return relative_address + 0x4 + *reinterpret_cast< std::int32_t* >( relative_address );
	}

	const char8_t* get_weapon_icon( short item_definition_index );

	const int get_fps( );
};

inline utilities_t utilities = { };
