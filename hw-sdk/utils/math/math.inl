
template< typename T >
T math::get_fov(const math::vec3& view_angles, const math::vec3& start, const math::vec3& end)
{
	math::vec3 fw{ },
		dir = ( end - start ).normalized( );

	math::angle_vectors( view_angles, &fw );

	return math::max< float >( math::rad2deg( std::acos( fw.dot_product( dir ) ) ), 0.f );
}

template< typename T >
constexpr T math::min( const T& t1, const T& t2 )
{
	return t1 < t2 ? t1 : t2;
}

template< typename T, typename... ts_ >
constexpr T math::min( const T& t1, const T& t2, ts_&&... ts )
{
	return t1 < t2 ? math::min( t1, std::forward< ts_ >( ts )... ) : math::min( t2, std::forward< ts_ >( ts )... );
}

template< typename T >
constexpr T math::max( const T& t1, const T& t2 )
{
	return t1 > t2 ? t1 : t2;
}

template< typename T, typename... ts_ >
constexpr T math::max( const T& t1, const T& t2, ts_&&... ts )
{
	return t1 > t2 ? math::max( t1, std::forward< ts_ >( ts )... ) : math::max( t2, std::forward< ts_ >( ts )... );
}

template< typename T >
constexpr T math::random(const T min, const T max)
{
	if constexpr ( !std::is_integral_v< T > ) {
		std::uniform_real_distribution< T > dist( min, max );
		return dist( gen );
	} else {
		std::uniform_int_distribution< T > dist( min, max );
		return dist( gen );
	}
}