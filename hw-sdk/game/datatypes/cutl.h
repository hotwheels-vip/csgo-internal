#pragma once
#include <assert.h>
#include <vcruntime_string.h>

namespace sdk::datatypes
{
	template< class T, class M = int >
	class c_utl_memory
	{
	public:
		T& operator[]( M _index )
		{
			return m_memory[ _index ];
		}

		T* m_memory;
		int m_allocation_count;
		int m_grow_size;
	};

	template< class T, class A = c_utl_memory< T > >
	class c_utl_vector
	{
	private:
		typedef A allocator_t;

	public:
		constexpr T& operator[]( int i ) noexcept
		{
			return m_memory[ i ];
		};

		constexpr const T& operator[]( int i ) const noexcept
		{
			return m_memory[ i ];
		};

		constexpr int count( ) const
		{
			return m_size;
		}

		void set_size( int _value )
		{
			m_size = _value;
		}

		T& element( int i )
		{
			assert( i < m_size );
			return m_memory[ i ];
		};

		void erase( )
		{
			auto self_destruct = []( T* m_class ) -> void {
				m_class->~T( );
#ifdef _DEBUG
				std::memset( m_class, 0xDD, sizeof( T ) );
#endif
			};

			for ( int i = m_size; --i >= 0; )
				self_destruct( &element( i ) );

			m_size = 0;
		}

	protected:
		allocator_t m_memory;
		int m_size;
		T* m_elements;
	};
} // namespace sdk::datatypes
