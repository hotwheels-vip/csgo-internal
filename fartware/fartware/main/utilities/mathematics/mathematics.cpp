#include "mathematics.h"

c_vector mathematics_t::vector_transform( const c_vector& vector_to_transform, const matrix3x4_t& matrix )
{
	return c_vector( vector_to_transform.dot_product( matrix[ 0 ] ) + matrix[ 0 ][ 3 ],
	                 vector_to_transform.dot_product( matrix[ 1 ] ) + matrix[ 1 ][ 3 ],
	                 vector_to_transform.dot_product( matrix[ 2 ] ) + matrix[ 2 ][ 3 ] );
}