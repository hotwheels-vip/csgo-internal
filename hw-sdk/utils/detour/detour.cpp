#include "detour.h"
#include "../../dependencies/minhook/include/MinHook.h"
#include "../console/console.h"

bool detour::hook::create( void* func, void* detour )
{
	m_base_fn = func;

	if ( !m_base_fn )
		return false;

	m_replace_fn = detour;

	if ( !m_replace_fn )
		return false;

	if ( const auto status = MH_CreateHook( m_base_fn, m_replace_fn, &m_original_fn ); status != MH_OK ) {
		// clang-format off

		console::print< console::log_level::FATAL >(
			_( "detour::hook::create( ) failed to create hook [ status: {} | base: {:#08X} | replace: {:#08X} | original: {:#08X} ]" ),
			MH_StatusToString( status ), 
			reinterpret_cast< std::uintptr_t >( m_base_fn ),
			reinterpret_cast< std::uintptr_t >( m_replace_fn ),
			reinterpret_cast< std::uintptr_t >( m_original_fn ) );

		// clang-format on

		return false;
	}

	if ( !replace( ) )
		return false;

	// maybe remove this
	console::print< console::log_level::NORMAL >( _( "Hooked function -> [ base: {:#08X} | replace: {:#08X} | original: {:#08X} ]" ),
	                                              reinterpret_cast< std::uintptr_t >( m_base_fn ), reinterpret_cast< std::uintptr_t >( m_replace_fn ),
	                                              reinterpret_cast< std::uintptr_t >( m_original_fn ) );

	return true;
}

bool detour::hook::replace( )
{
	if ( !m_base_fn )
		return false;

	if ( m_hooked )
		return false;

	if ( const auto status = MH_EnableHook( m_base_fn ); status != MH_OK ) {
		console::print< console::log_level::FATAL >( _( "detour::hook::replace( ) failed [ status: {} | base: {:#08X} ]" ),
		                                             MH_StatusToString( status ), reinterpret_cast< std::uintptr_t >( m_base_fn ) );

		return false;
	};

	m_hooked = true;

	return true;
}

bool detour::hook::remove( )
{
	if ( !restore( ) ) {
		console::print< console::log_level::WARNING >( _( "detour::hook::remove( ) failed, restore returned false" ) );
		return false;
	}

	if ( const auto status = MH_RemoveHook( m_base_fn ); status != MH_OK ) {
		console::print< console::log_level::FATAL >( _( "detour::hook::remove( ) failed [ status: {} | base: {:#08X} ]" ),
		                                             MH_StatusToString( status ), reinterpret_cast< std::uintptr_t >( m_base_fn ) );

		return false;
	}

	return true;
}

bool detour::hook::restore( )
{
	if ( !is_hooked( ) )
		return false;

	if ( const auto status = MH_DisableHook( m_base_fn ); status != MH_OK ) {
		console::print< console::log_level::FATAL >( _( "detour::hook::restore( ) failed [ status: {} | base: {:#08X} ]" ),
		                                             MH_StatusToString( status ), reinterpret_cast< std::uintptr_t >( m_base_fn ) );

		return false;
	}

	m_hooked = false;

	return true;
}

bool detour::hook::is_hooked( ) const
{
	return m_hooked;
}
