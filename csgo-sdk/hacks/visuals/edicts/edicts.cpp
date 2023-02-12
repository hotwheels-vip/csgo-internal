#include "edicts.h"
#include "../../../game/sdk/includes/includes.h"
#include "../../../globals/includes/includes.h"

#include "../../entity_cache/entity_cache.h"

constexpr static char collide_data[] = {
	char( 0xB8 ), char( 0x01 ), char( 0x00 ), char( 0x00 ), char( 0x56 ), char( 0x50 ), char( 0x48 ), char( 0x59 ), char( 0x00 ), char( 0x01 ),
	char( 0x00 ), char( 0x00 ), char( 0x9C ), char( 0x01 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x80 ), char( 0x3F ),
	char( 0x00 ), char( 0x00 ), char( 0x80 ), char( 0x3F ), char( 0x00 ), char( 0x00 ), char( 0x80 ), char( 0x3F ), char( 0x00 ), char( 0x00 ),
	char( 0x00 ), char( 0x00 ), char( 0x20 ), char( 0x16 ), char( 0x6A ), char( 0xC1 ), char( 0xC0 ), char( 0x0E ), char( 0x1C ), char( 0xC1 ),
	char( 0x80 ), char( 0x13 ), char( 0xD0 ), char( 0x3F ), char( 0xE2 ), char( 0x26 ), char( 0x11 ), char( 0x48 ), char( 0xE2 ), char( 0x26 ),
	char( 0x11 ), char( 0x48 ), char( 0xE2 ), char( 0x26 ), char( 0x11 ), char( 0x48 ), char( 0x72 ), char( 0x4E ), char( 0x08 ), char( 0xF44 ),
	char( 0xD1 ), char( 0x9C ), char( 0x01 ), char( 0x00 ), char( 0x80 ), char( 0x01 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ),
	char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x49 ), char( 0x56 ), char( 0x50 ), char( 0x53 ),
	char( 0xD0 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x06 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x04 ), char( 0x15 ),
	char( 0x00 ), char( 0x00 ), char( 0x0C ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x90 ), char( 0x00 ), char( 0x00 ),
	char( 0x00 ), char( 0x00 ), char( 0x0A ), char( 0x00 ), char( 0x01 ), char( 0x00 ), char( 0x03 ), char( 0x00 ), char( 0x02 ), char( 0x00 ),
	char( 0x12 ), char( 0x00 ), char( 0x01 ), char( 0x80 ), char( 0x00 ), char( 0x00 ), char( 0x02 ), char( 0x00 ), char( 0xFD ), char( 0x7F ),
	char( 0x01 ), char( 0x00 ), char( 0x15 ), char( 0x00 ), char( 0x03 ), char( 0x00 ), char( 0x28 ), char( 0x00 ), char( 0x02 ), char( 0xB0 ),
	char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x06 ), char( 0x00 ), char( 0x04 ), char( 0x00 ), char( 0x0F ), char( 0x00 ),
	char( 0x01 ), char( 0x00 ), char( 0xF6 ), char( 0x7F ), char( 0x03 ), char( 0xA0 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ),
	char( 0x06 ), char( 0x00 ), char( 0x05 ), char( 0x00 ), char( 0x15 ), char( 0x00 ), char( 0x04 ), char( 0x00 ), char( 0xFA ), char( 0x7F ),
	char( 0x04 ), char( 0x70 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x06 ), char( 0x00 ), char( 0x06 ), char( 0x00 ),
	char( 0x13 ), char( 0x00 ), char( 0x05 ), char( 0x00 ), char( 0xFA ), char( 0x7F ), char( 0x05 ), char( 0x60 ), char( 0x00 ), char( 0x00 ),
	char( 0x00 ), char( 0x00 ), char( 0xEE ), char( 0x7F ), char( 0x02 ), char( 0x00 ), char( 0x18 ), char( 0x00 ), char( 0x06 ), char( 0x00 ),
	char( 0xFA ), char( 0x7F ), char( 0x06 ), char( 0x50 ), char( 0x00 ), char( 0x00 ), char( 0x01 ), char( 0x00 ), char( 0xF1 ), char( 0x7F ),
	char( 0x04 ), char( 0x00 ), char( 0x03 ), char( 0x00 ), char( 0x03 ), char( 0x00 ), char( 0xEB ), char( 0x7F ), char( 0x07 ), char( 0x40 ),
	char( 0x00 ), char( 0x00 ), char( 0x03 ), char( 0x00 ), char( 0xFD ), char( 0x7F ), char( 0x04 ), char( 0x00 ), char( 0x04 ), char( 0x00 ),
	char( 0x07 ), char( 0x00 ), char( 0x0C ), char( 0x00 ), char( 0x08 ), char( 0x10 ), char( 0x00 ), char( 0x00 ), char( 0x05 ), char( 0x00 ),
	char( 0x06 ), char( 0x00 ), char( 0x07 ), char( 0x00 ), char( 0xFC ), char( 0x7F ), char( 0x04 ), char( 0x00 ), char( 0xEB ), char( 0x7F ),
	char( 0x09 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x05 ), char( 0x00 ), char( 0xED ), char( 0x7F ), char( 0x06 ), char( 0x00 ),
	char( 0x03 ), char( 0x00 ), char( 0x07 ), char( 0x00 ), char( 0xFA ), char( 0x7F ), char( 0x0A ), char( 0x30 ), char( 0x00 ), char( 0x00 ),
	char( 0x07 ), char( 0x00 ), char( 0xFD ), char( 0x7F ), char( 0x06 ), char( 0x00 ), char( 0x03 ), char( 0x00 ), char( 0x03 ), char( 0x00 ),
	char( 0xF4 ), char( 0x7F ), char( 0x0B ), char( 0x20 ), char( 0x00 ), char( 0x00 ), char( 0x03 ), char( 0x00 ), char( 0xFD ), char( 0x7F ),
	char( 0x06 ), char( 0x00 ), char( 0xE8 ), char( 0x7F ), char( 0x02 ), char( 0x00 ), char( 0xD8 ), char( 0x7F ), char( 0x46 ), char( 0xAD ),
	char( 0x9D ), char( 0xC3 ), char( 0x1F ), char( 0x0D ), char( 0x9C ), char( 0xC3 ), char( 0x80 ), char( 0xAE ), char( 0xAA ), char( 0x43 ),
	char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x46 ), char( 0xAD ), char( 0x9D ), char( 0xC3 ), char( 0x33 ), char( 0x4C ),
	char( 0x92 ), char( 0x43 ), char( 0x80 ), char( 0xAE ), char( 0xAA ), char( 0x43 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ),
	char( 0x46 ), char( 0xAD ), char( 0x9D ), char( 0xC3 ), char( 0x1F ), char( 0x0D ), char( 0x9C ), char( 0xC3 ), char( 0x59 ), char( 0x0E ),
	char( 0xA9 ), char( 0xC3 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x46 ), char( 0xAD ), char( 0x9D ), char( 0xC3 ),
	char( 0x33 ), char( 0x4C ), char( 0x92 ), char( 0x43 ), char( 0x59 ), char( 0x0E ), char( 0xA9 ), char( 0xC3 ), char( 0x00 ), char( 0x00 ),
	char( 0x00 ), char( 0x00 ), char( 0xE4 ), char( 0x0B ), char( 0x8F ), char( 0x43 ), char( 0x33 ), char( 0x4C ), char( 0x92 ), char( 0x43 ),
	char( 0x80 ), char( 0xAE ), char( 0xAA ), char( 0x43 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0xE4 ), char( 0x0B ),
	char( 0x8F ), char( 0x43 ), char( 0x1F ), char( 0x0D ), char( 0x9C ), char( 0xC3 ), char( 0x80 ), char( 0xAE ), char( 0xAA ), char( 0x43 ),
	char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0xE4 ), char( 0x0B ), char( 0x8F ), char( 0x43 ), char( 0x1F ), char( 0x0D ),
	char( 0x9C ), char( 0xC3 ), char( 0x59 ), char( 0x0E ), char( 0xA9 ), char( 0xC3 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ),
	char( 0xE4 ), char( 0x0B ), char( 0x8F ), char( 0x43 ), char( 0x33 ), char( 0x4C ), char( 0x92 ), char( 0x43 ), char( 0x59 ), char( 0x0E ),
	char( 0xA9 ), char( 0xC3 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ), char( 0x00 ),
	char( 0xB0 ), char( 0xFE ), char( 0xFF ), char( 0xFF ), char( 0x20 ), char( 0x16 ), char( 0x6A ), char( 0xC1 ), char( 0xC0 ), char( 0x0E ),
	char( 0x1C ), char( 0xC1 ), char( 0x80 ), char( 0x13 ), char( 0xD0 ), char( 0x3F ), char( 0x72 ), char( 0x4E ), char( 0x08 ), char( 0x44 ),
	char( 0x8A ), char( 0x8B ), char( 0x9C ), char( 0x00 ), char( 0x73 ), char( 0x6F ), char( 0x6C ), char( 0x69 ), char( 0x64 ), char( 0x20 ),
	char( 0x7B ), char( 0x0A ), char( 0x22 ), char( 0x69 ), char( 0x6E ), char( 0x64 ), char( 0x65 ), char( 0x78 ), char( 0x22 ), char( 0x20 ),
	char( 0x22 ), char( 0x30 ), char( 0x22 ), char( 0x0A ), char( 0x22 ), char( 0x6D ), char( 0x61 ), char( 0x73 ), char( 0x73 ), char( 0x22 ),
	char( 0x20 ), char( 0x22 ), char( 0x35 ), char( 0x30 ), char( 0x30 ), char( 0x30 ), char( 0x30 ), char( 0x2E ), char( 0x30 ), char( 0x30 ),
	char( 0x30 ), char( 0x30 ), char( 0x30 ), char( 0x30 ), char( 0x22 ), char( 0x0A ), char( 0x22 ), char( 0x73 ), char( 0x75 ), char( 0x72 ),
	char( 0x66 ), char( 0x61 ), char( 0x63 ), char( 0x65 ), char( 0x70 ), char( 0x72 ), char( 0x6F ), char( 0x70 ), char( 0x22 ), char( 0x20 ),
	char( 0x22 ), char( 0x64 ), char( 0x65 ), char( 0x66 ), char( 0x61 ), char( 0x75 ), char( 0x6C ), char( 0x74 ), char( 0x22 ), char( 0x0A ),
	char( 0x22 ), char( 0x76 ), char( 0x6F ), char( 0x6C ), char( 0x75 ), char( 0x6D ), char( 0x65 ), char( 0x22 ), char( 0x20 ), char( 0x22 ),
	char( 0x31 ), char( 0x35 ), char( 0x30 ), char( 0x38 ), char( 0x30 ), char( 0x32 ), char( 0x33 ), char( 0x32 ), char( 0x30 ), char( 0x35 ),
	char( 0x38 ), char( 0x38 ), char( 0x38 ), char( 0x30 ), char( 0x2E ), char( 0x30 ), char( 0x30 ), char( 0x30 ), char( 0x30 ), char( 0x30 ),
	char( 0x30 ), char( 0x22 ), char( 0x0A ), char( 0x7D ), char( 0x0A ), char( 0x00 )
};

static vcollide_t precipitation_collideable{ };

void n_edicts::impl_t::on_frame_stage_notify( int stage )
{
	switch ( stage ) {
	case 5: {
		this->precipitation( );
		this->fog( );
		break;
	}
	}
}

void n_edicts::impl_t::reset( )
{
	if ( m_created ) {
		g_entity_cache.enumerate( e_enumeration_type::type_edicts, [ & ]( c_base_entity* entity ) {
			if ( !entity )
				return;

			auto client_class = entity->get_client_networkable( )->get_client_class( );
			if ( !client_class )
				return;

			if ( static_cast< int >( client_class->m_class_id ) == 138 /* cprecipitation class id */ ) {
				const auto networkable = entity->get_client_networkable( );
				if ( !networkable )
					return;

				networkable->pre_data_update( 0 );
				networkable->on_pre_data_changed( 0 );

				*( int* )( ( uintptr_t )entity + 0xA00 ) = -1;

				const auto collideable = entity->get_collideable( );
				if ( !collideable )
					return;

				collideable->obb_mins( ) = c_vector{ 0, 0, 0 };
				collideable->obb_maxs( ) = c_vector{ 0, 0, 0 };

				networkable->on_data_changed( 0 );
				networkable->post_data_update( 0 );
				networkable->release( );
			}
		} );

		g_interfaces.m_physics_collison->v_collide_unload( &precipitation_collideable );
		m_created = false;
		m_timer   = -1;
	}
}

void* n_edicts::impl_t::get_precipitation_collideable( )
{
	return &precipitation_collideable;
}

void n_edicts::impl_t::precipitation( )
{
	if ( !GET_VARIABLE( g_variables.m_precipitation, bool ) ) {
		this->reset( );
		return;
	}

	static int weather_type = 0;
	switch ( GET_VARIABLE( g_variables.m_precipitation_type, int ) ) {
	case 0: {
		weather_type = 4;
		break;
	}
	case 1: {
		weather_type = 5;
		break;
	}
	case 2: {
		weather_type = 6;
		break;
	}
	case 3: {
		weather_type = 7;
		break;
	}
	}

	if ( m_timer > -1 ) {
		--m_timer;
		if ( m_timer == 0 ) {
			this->reset( );
		}
	}

	static std::optional< int > last_type{ };

	if ( last_type.has_value( ) && last_type.value( ) != weather_type )
		reset( );

	last_type = weather_type;

	if ( m_created )
		return;

	memset( &precipitation_collideable, 0, sizeof( precipitation_collideable ) );

	static c_base_client* precipitation_client_class = nullptr;
	if ( !precipitation_client_class ) {
		for ( auto client_class = g_interfaces.m_base_client->get_all_classes( ); client_class; client_class = client_class->m_next ) {
			if ( static_cast< int >( client_class->m_class_id ) == 138 /* cprecipitation class id */ ) {
				precipitation_client_class = client_class;
				break;
			}
		}
	}

	if ( precipitation_client_class && precipitation_client_class->m_create_fn ) {
		void* rain_networkable = ( ( void* ( * )( int, int ))precipitation_client_class->m_create_fn )( 2048 - 1, 0 );
		if ( !rain_networkable )
			return;

		c_client_unknown* rain_unknown = ( ( c_client_renderable* )rain_networkable )->get_client_unknown( );
		if ( !rain_unknown )
			return;

		c_base_entity* entity = rain_unknown->get_base_entity( );
		if ( !entity )
			return;

		const auto networkable = entity->get_client_networkable( );
		if ( !networkable )
			return;

		networkable->pre_data_update( 0 );
		networkable->on_pre_data_changed( 0 );
		entity->get_index( ) = -1;

		*( int* )( ( uintptr_t )entity + 0xA00 ) = weather_type;

		const auto collideable = entity->get_collideable( );
		if ( !collideable )
			return;

		collideable->obb_mins( ) = c_vector( -32768.f, -32768.f, -32768.f );
		collideable->obb_maxs( ) = c_vector( 32768.f, 32768.f, 32768.f );

		g_interfaces.m_physics_collison->v_collide_load( &precipitation_collideable, 1, ( const char* )collide_data, sizeof( collide_data ) );

		entity->get_model_index( ) = -1;

		networkable->on_data_changed( 0 );
		networkable->post_data_update( 0 );

		m_created = true;
	}
}

void n_edicts::impl_t::fog( )
{
	g_entity_cache.enumerate( e_enumeration_type::type_edicts, [ & ]( c_base_entity* entity ) {
		if ( !entity || entity->is_dormant( ) )
			return;

		const auto client_networkable = entity->get_client_networkable( );
		if ( !client_networkable )
			return;

		const auto client_class = client_networkable->get_client_class( );
		if ( !client_class )
			return;

		if ( !( static_cast< int >( client_class->m_class_id ) == 78 ) )
			return;

		const c_color color = GET_VARIABLE( g_variables.m_fog_color, c_color );

		entity->get_fog_enable( )  = GET_VARIABLE( g_variables.m_fog, bool ) ? 1 : 0;
		entity->get_fog_start( )   = GET_VARIABLE( g_variables.m_fog_start, float );
		entity->get_fog_end( )     = GET_VARIABLE( g_variables.m_fog_end, float );
		entity->get_fog_density( ) = color.base< e_color_type::color_type_a >( );
		entity->get_fog_color( )   = ImGui::ColorConvertFloat4ToU32( color.get_vec4( ) );
	} );
}