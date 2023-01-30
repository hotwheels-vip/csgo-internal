#include "weather.h"
#include "../../../convars/convars.h"
#include "../../../includes.h"

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

void weather_t::on_frame_stage_notify( e_client_frame_stage stage )
{
	if ( stage != e_client_frame_stage::render_start )
		return;

	if ( !( GET_CONFIG_INT( variables.m_world.m_weather_type ) ) || globals.m_unloading )
		reset( );

	static int weather_type = 0;
	switch ( GET_CONFIG_INT( variables.m_world.m_weather_type ) ) {
	case 1: {
		weather_type = e_precipitation_type::precipitation_type_particlerain;
		break;
	}
	case 2: {
		weather_type = e_precipitation_type::precipitation_type_particleash;
		break;
	}
	case 3: {
		weather_type = e_precipitation_type::precipitation_type_particlerainstorm;
		break;
	}
	case 4: {
		weather_type = e_precipitation_type::precipitation_type_particlesnow;
		break;
	}
	}

	if ( m_timer > -1 ) {
		--m_timer;
		if ( m_timer == 0 ) {
			reset( );
		}
	}

	if ( m_last_type.has_value( ) && m_last_type.value( ) != weather_type )
		reset( );

	m_last_type = weather_type;

	if ( m_created )
		return;

	memset( &m_collide, 0, sizeof( m_collide ) );

	static c_base_client* precipitation_client_class = nullptr;
	if ( !precipitation_client_class ) {
		for ( auto client_class = interfaces.m_client->get_all_classes( ); client_class; client_class = client_class->m_next ) {
			if ( client_class->m_class_id == e_class_index::cprecipitation ) {
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

		if ( !entity->client_networkable( ) )
			return;

		entity->client_networkable( )->pre_data_update( 0 );
		entity->client_networkable( )->on_pre_data_changed( 0 );
		entity->other_index( ) = -1;

		static auto r_rainradius = convars.find( fnv1a::hash_const( "r_RainRadius" ) );
		if ( r_rainradius->get_float( ) != 1000.F )
			r_rainradius->set_value( 1000.F );

		*( int* )( ( uintptr_t )entity + 0xA00 ) = weather_type;

		entity->collideable( )->obb_mins( ) = c_vector( -32768.F, -32768.F, -32768.F );
		entity->collideable( )->obb_maxs( ) = c_vector( 32768.F, 32768.F, 32768.F );

		interfaces.m_physics_collisons->v_collide_load( &m_collide, 1, ( const char* )collide_data, sizeof( collide_data ) );

		entity->model_index( ) = -1;

		entity->client_networkable( )->on_data_changed( 0 );
		entity->client_networkable( )->post_data_update( 0 );

		m_created = true;
	}
}

void weather_t::reset( )
{
	if ( m_created ) {
		for ( auto i = 0; i <= interfaces.m_client_entity_list->get_highest_entity_index( ); ++i ) {
			auto entity = reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity( i ) );
			if ( !entity )
				continue;

			auto client_class = entity->client_networkable( )->get_client_class( );
			if ( !client_class )
				continue;

			if ( client_class->m_class_id == e_class_index::cprecipitation ) {
				if ( entity->client_networkable( ) ) {
					entity->client_networkable( )->pre_data_update( 0 );
					entity->client_networkable( )->on_pre_data_changed( 0 );

					*( int* )( ( uintptr_t )entity + 0xA00 ) = -1;

					entity->collideable( )->obb_mins( ) = c_vector{ 0, 0, 0 };
					entity->collideable( )->obb_maxs( ) = c_vector{ 0, 0, 0 };

					entity->client_networkable( )->on_data_changed( 0 );
					entity->client_networkable( )->post_data_update( 0 );
					entity->client_networkable( )->release( );
				}
			}
		}

		interfaces.m_physics_collisons->v_collide_unload( &m_collide );
		m_created = false;
		m_timer   = -1;
	}
}