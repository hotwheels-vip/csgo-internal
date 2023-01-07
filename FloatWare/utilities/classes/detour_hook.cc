#include "detour_hook.hh"
#include "../../dependencies/xorstr/xorstr.hh"

#include <format>

bool c_detour_hook::create( void* function, void* detour )
{
	this->m_base_fn = function;

	if ( !this->m_base_fn )
		return false;

	this->m_replace_fn = detour;

	if ( !this->m_replace_fn )
		return false;

	const MH_STATUS status = MH_CreateHook( this->m_base_fn, this->m_replace_fn, &this->m_original_fn );

	if ( status != MH_OK )
		throw std::runtime_error(
			std::vformat( xs( "failed to create hook function, status: {}\nbase function -> {:#08X}" ),
		                  std::make_format_args( MH_StatusToString( status ), reinterpret_cast< std::uintptr_t >( this->m_base_fn ) ) ) );

	if ( !this->replace( ) )
		return false;

	return true;
}

bool c_detour_hook::replace( )
{
	if ( !this->m_base_fn )
		return false;

	if ( this->m_is_hooked )
		return false;

	const MH_STATUS status = MH_EnableHook( this->m_base_fn );

	if ( status != MH_OK )
		throw std::runtime_error(
			std::vformat( xs( "failed to enable hook function, status: {}\nbase function -> {:#08X} address" ),
		                  std::make_format_args( MH_StatusToString( status ), reinterpret_cast< std::uintptr_t >( this->m_base_fn ) ) ) );

	this->m_is_hooked = true;
	return true;
}