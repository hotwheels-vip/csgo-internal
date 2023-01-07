#pragma once

// could've done this in modules.h but too much work

namespace detour
{
	class hook
	{
	public:
		hook( ) = default;
		explicit hook( void* func, void* detour ) : m_base_fn( func ), m_replace_fn( detour ){ };

		bool create( void* func, void* detour );

		bool replace( );

		bool remove( );

		bool restore( );

		inline bool is_hooked( ) const;

		template< typename T >
		T get_original( )
		{
			return static_cast< T >( m_original_fn );
		}

	private:
		bool m_hooked       = false;
		void* m_base_fn     = nullptr;
		void* m_replace_fn  = nullptr;
		void* m_original_fn = nullptr;
	};
} // namespace detour
