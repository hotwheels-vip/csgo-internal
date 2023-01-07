#include "visuals.h"
#include "../../../globals/ctx/ctx.h"
#include "../../menu/config/config.h"
#include <comdef.h>

math::box visuals::esp_box::calculate_box( sdk::c_cs_player* player, bool& on_screen )
{
	auto& player_info = g_entity_list.players[ player->entity_index( ) ];

	math::vec3 mins = player_info.m_mins;
	math::vec3 maxs = player_info.m_maxs;

	math::matrix_3x4 transform = player_info.m_rgfl;

	math::vec3 points[] = { math::vec3( mins.x, mins.y, mins.z ), math::vec3( mins.x, maxs.y, mins.z ), math::vec3( maxs.x, maxs.y, mins.z ),
		                    math::vec3( maxs.x, mins.y, mins.z ), math::vec3( maxs.x, maxs.y, maxs.z ), math::vec3( mins.x, maxs.y, maxs.z ),
		                    math::vec3( mins.x, mins.y, maxs.z ), math::vec3( maxs.x, mins.y, maxs.z ) };

	math::vec3 translated_points[ 8 ];

	for ( int iterator = 0; iterator < 8; iterator++ )
		translated_points[ iterator ] = math::vector_transform( points[ iterator ], transform );

	math::vec2< int > screen_points[ 8 ];

	for ( int iterator = 0; iterator < 8; iterator++ ) {
		bool buffer_on_screen = true;

		auto& screen_point = screen_points[ iterator ];

		screen_point = utils::world_to_screen( translated_points[ iterator ], buffer_on_screen );

		if ( buffer_on_screen && !( screen_point.x < 0 ) && !( screen_point.y < 0 ) && !( screen_point.x > g_ctx.screen_size.x ) &&
		     !( screen_point.y > g_ctx.screen_size.y ) )
			on_screen = true;
	}

	float top    = screen_points[ 0 ].y;
	float left   = screen_points[ 0 ].x;
	float right  = screen_points[ 0 ].x;
	float bottom = screen_points[ 0 ].y;

	for ( int iterator = 1; iterator < 8; iterator++ ) {
		if ( left > screen_points[ iterator ].x )
			left = screen_points[ iterator ].x;
		if ( top > screen_points[ iterator ].y )
			top = screen_points[ iterator ].y;
		if ( right < screen_points[ iterator ].x )
			right = screen_points[ iterator ].x;
		if ( bottom < screen_points[ iterator ].y )
			bottom = screen_points[ iterator ].y;
	}

	return math::box( ROUND_UP( left ), ROUND_UP( top ), ROUND_UP( right ), ROUND_UP( bottom ) );
}

void visuals::impl::update_object( esp_object& object )
{
	if ( !object.m_owner->name( ).c_str( ) )
		return;

	auto& dormant_info = g_entity_list.players[ object.m_owner->entity_index( ) ].m_dormant_info;
	auto dormant_time  = sdk::ticks_to_time( g_interfaces.globals->tick_count - dormant_info.m_found_tick );

	if ( dormant_time < 2.5f )
		dormant_time = 0.f;

	auto dormant_alpha_modulation = dormant_info.m_valid ? -( ( dormant_time - 2.5f ) / .5f ) + 1.f : 1.f;

	object.m_box.m_cornered     = false;
	object.m_box.m_outline[ 0 ] = true;
	object.m_box.m_outline[ 1 ] = true;
	object.m_box.m_draw         = false;
	object.m_box.m_skeleton     = false;
	object.m_box.m_backtrack    = true;

	object.m_box.m_color = color( 255, 255, 255, 255 * dormant_alpha_modulation );

	object.m_box.m_titles.clear( );
	object.m_box.m_texts.clear( );
	object.m_box.m_bars.clear( );

	auto text_name = esp_text( );

	auto health_bar  = esp_bar( );
	auto health_text = esp_text( );

	auto weapon_ammo_bar  = esp_bar( );
	auto weapon_name_text = esp_text( );

	text_name.m_location = esp_location::LOCATION_TOP;
	text_name.m_text     = object.m_owner->name( );
	text_name.m_color    = color( 255, 255, 255, 255 * dormant_alpha_modulation );
	text_name.m_font     = g_fonts[ HASH_BT( "esp_font" ) ];
	text_name.m_flags    = font_flags::FLAG_DROPSHADOW;

	object.m_box.m_texts.push_back( text_name );

	health_bar.m_location   = esp_location::LOCATION_LEFT;
	health_bar.m_width      = 2;
	health_bar.m_color_from = color( 0, 255, 0, 255 * dormant_alpha_modulation );
	health_bar.m_color_to   = color( 255, 0, 0, 255 * dormant_alpha_modulation );
	health_bar.m_min        = 0;
	health_bar.m_max        = 100;
	health_bar.m_cur        = object.m_owner->health( );

	object.m_box.m_bars.push_back( health_bar );

	health_text.m_location = esp_location::LOCATION_LEFT;
	health_text.m_text     = std::to_string( object.m_owner->health( ) );
	health_text.m_color    = color( 255, 255, 255, 255 * dormant_alpha_modulation );
	health_text.m_font     = g_fonts[ HASH_BT( "esp_font" ) ];
	health_text.m_flags    = font_flags::FLAG_OUTLINE;

	object.m_box.m_texts.push_back( health_text );

	auto player_weapon = g_interfaces.entity_list->get_client_entity_from_handle< sdk::c_base_combat_weapon* >( object.m_owner->active_weapon( ) );

	if ( player_weapon && player_weapon->definition_index( ) ) {
		auto weapon_info = player_weapon->get_weapon_data( );

		if ( weapon_info ) {
			weapon_ammo_bar.m_location   = esp_location::LOCATION_BOTTOM;
			weapon_ammo_bar.m_width      = 2;
			weapon_ammo_bar.m_color_from = color( 173, 216, 230, 255 * dormant_alpha_modulation );
			weapon_ammo_bar.m_color_to   = color( 173, 216, 230, 255 * dormant_alpha_modulation );
			weapon_ammo_bar.m_min        = 0;
			weapon_ammo_bar.m_max        = weapon_info->max_clip1;
			weapon_ammo_bar.m_cur        = player_weapon->clip_mag( );

			object.m_box.m_bars.push_back( weapon_ammo_bar );

			std::wstring weapon_name_wide = g_interfaces.localize->find( weapon_info->hud_name );
			auto weapon_name_string       = std::string( weapon_name_wide.begin( ), weapon_name_wide.end( ) );

			weapon_name_text.m_location = esp_location::LOCATION_BOTTOM;
			weapon_name_text.m_text     = weapon_name_string;
			weapon_name_text.m_color    = color( 255, 255, 255, 255 * dormant_alpha_modulation );
			weapon_name_text.m_font     = g_fonts[ HASH_BT( "esp_font" ) ];
			weapon_name_text.m_flags    = font_flags::FLAG_OUTLINE;

			object.m_box.m_texts.push_back( weapon_name_text );
		}
	}
}

void visuals::impl::update( )
{
	for ( auto& player_info : g_entity_list.players ) {
		auto player = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( player_info.m_index );

		if ( ( !player_info.m_valid && !player_info.m_dormant_info.m_valid ) || !player )
			continue;

		esp_object& buffer_object = esp_objects[ player->entity_index( ) ];

		buffer_object.m_owner = player;

		if ( true )
			update_object( buffer_object );
	}
}

void visuals::impl::render( )
{
	update( );

	for ( auto& player_info : g_entity_list.players ) {
		auto player = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( player_info.m_index );

		if ( ( !player_info.m_valid && !player_info.m_dormant_info.m_valid ) || !player )
			continue;

		esp_object& object = esp_objects[ player->entity_index( ) ];

		object.m_box.render( player );
	}
}

void visuals::esp_box::render( sdk::c_cs_player* owner )
{
	bool on_screen  = false;
	auto dimensions = calculate_box( owner, on_screen );
	auto position   = math::vec2< int >( static_cast< int >( dimensions.x ), static_cast< int >( dimensions.y ) );
	auto size       = math::vec2< int >( static_cast< int >( dimensions.w ), static_cast< int >( dimensions.h ) ) - position;

	if ( !on_screen )
		return;

	if ( m_draw ) {
		// outer outline
		if ( m_outline[ 0 ] )
			g_render.render_rectangle< int >( position - math::vec2< int >( 1, 1 ), size + math::vec2< int >( 2, 2 ), color( 0, 0, 0, m_color.a ) );
		// inner outline
		if ( m_outline[ 1 ] )
			g_render.render_rectangle< int >( position + math::vec2< int >( 1, 1 ), size - math::vec2< int >( 2, 2 ), color( 0, 0, 0, m_color.a ) );

		g_render.render_rectangle< int >( position, size, m_color );
	}

	if ( m_skeleton ) {
		if ( auto studio = g_interfaces.model_info->get_studio_model( owner->get_model( ) ) ) {
			for ( int bone_index = 0; bone_index < studio->bones; bone_index++ ) {
				auto bone = studio->get_bone( bone_index );

				if ( !bone || !( bone->flags & 0x100 ) || bone->parent == -1 )
					continue;

				auto parent_bone_index = bone->parent;

				math::vec2< int > parent_bone_screen{ }, bone_screen{ };

				bool bone_screen_on_screen   = true;
				bool parent_screen_on_screen = true;

				parent_bone_screen = utils::world_to_screen( owner->get_bone_position( parent_bone_index ), bone_screen_on_screen );
				bone_screen        = utils::world_to_screen( owner->get_bone_position( bone_index ), parent_screen_on_screen );

				if ( !bone_screen_on_screen || !parent_screen_on_screen )
					continue;

				g_render.render_line( parent_bone_screen.x, parent_bone_screen.y, bone_screen.x, bone_screen.y, { 255, 255, 255 } );
			}
		}
	}

	if ( m_backtrack ) {
		if ( g_lagcomp.heap_records[ owner->entity_index( ) ] ) {
			static auto unlag_pointer = g_convars[ _( "sv_maxunlag" ) ];
			auto sv_maxunlag          = unlag_pointer->get_float( );
			auto sv_maxunlag_ticks    = sdk::time_to_ticks( sv_maxunlag );

			int last_valid_heap_record   = 0;
			float lowest_simulation_time = FLT_MAX;

			for ( int current_heap_iterator = 0; current_heap_iterator < sv_maxunlag_ticks; current_heap_iterator++ ) {
				lagcomp::record* current_record = &g_lagcomp.heap_records[ owner->entity_index( ) ][ current_heap_iterator ];

				if ( !current_record )
					continue;

				if ( current_record->simulation_time > lowest_simulation_time )
					continue;

				last_valid_heap_record = current_heap_iterator;
				lowest_simulation_time = current_record->simulation_time;
			}

			lagcomp::record current_record = g_lagcomp.heap_records[ owner->entity_index( ) ][ last_valid_heap_record ];

			if ( current_record.player ) {
				if ( auto studio = g_interfaces.model_info->get_studio_model( owner->get_model( ) ) ) {
					for ( int bone_index = 0; bone_index < studio->bones; bone_index++ ) {
						auto bone = studio->get_bone( bone_index );

						if ( !bone || !( bone->flags & 0x100 ) || bone->parent == -1 )
							continue;

						auto parent_bone_index = bone->parent;

						math::vec2< int > parent_bone_screen{ }, bone_screen{ };

						parent_bone_screen = utils::world_to_screen( owner->get_bone_position( parent_bone_index, current_record.bone_matrix ) );
						bone_screen        = utils::world_to_screen( owner->get_bone_position( bone_index, current_record.bone_matrix ) );

						g_render.render_line( parent_bone_screen.x, parent_bone_screen.y, bone_screen.x, bone_screen.y, { 255, 255, 255, 150 } );
					}
				}
			}
		}
	}

	float offset_x[ 4 ]{ };
	float offset_y[ 4 ]{ };

	for ( auto& bar : m_bars )
		bar.render( dimensions, offset_x[ static_cast< unsigned int >( bar.m_location ) ],
		            offset_y[ static_cast< unsigned int >( bar.m_location ) ] );

	for ( auto& title : m_titles )
		title.render( dimensions, offset_x[ static_cast< unsigned int >( title.m_location ) ],
		              offset_y[ static_cast< unsigned int >( title.m_location ) ] );

	for ( auto& text : m_texts )
		text.render( dimensions, offset_x[ static_cast< unsigned int >( text.m_location ) ],
		             offset_y[ static_cast< unsigned int >( text.m_location ) ] );
}

void visuals::esp_title::render( math::box box, float& offset_x, float& offset_y )
{
	auto text_size_buffer = g_render.render_text_size( m_text.c_str( ), m_font );
	auto text_size        = math::vec2< int >( text_size_buffer.x, text_size_buffer.y );

	math::vec2< int > position;

	switch ( m_location ) {
	case esp_location::LOCATION_TOP:
		break;
	case esp_location::LOCATION_LEFT:
		break;
	case esp_location::LOCATION_BOTTOM:
		break;
	case esp_location::LOCATION_RIGHT:
		break;
	}

	g_render.render_filled_rectangle( position, math::vec2< int >( text_size.x + 5, text_size.y + 4 ), color( 0, 0, 0, m_color.a * 0.7 ) );
	g_render.render_filled_rectangle( position, math::vec2< int >( 1, text_size.y + 4 ), m_color );

	g_render.render_text( position + math::vec2< int >( 3, 2 ), font_alignment::AL_DEFAULT, m_flags, m_text.c_str( ), m_font,
	                      color( 255, 255, 255, 255 ) );
}

void visuals::esp_text::render( math::box box, float& offset_x, float& offset_y )
{
	auto text_size_buffer = g_render.render_text_size( m_text.c_str( ), m_font );
	auto text_size        = math::vec2< int >( text_size_buffer.x, text_size_buffer.y );

	math::vec2< int > position;

	switch ( m_location ) {
	case esp_location::LOCATION_TOP:
		position.x = ( box.x + box.width( ).x / 2 ) - text_size.x / 2;
		position.y = ( box.y - text_size.y ) - 1;

		position.y -= offset_y;

		offset_y += text_size.y;
		break;
	case esp_location::LOCATION_BOTTOM:
		position.x = ( box.x + box.width( ).x / 2 ) - text_size.x / 2;
		position.y = box.h + 1;

		position.y += offset_y;

		offset_y += text_size.y;
		break;
	case esp_location::LOCATION_LEFT:
		position.x = ( box.x - text_size.x ) - 3;
		position.y = box.y;

		position.x -= offset_x;
		position.y += offset_y;

		offset_x += text_size.x;
		break;
	case esp_location::LOCATION_RIGHT:
		position.x = box.w + 2;
		position.y = box.y;

		position.x += offset_x;
		position.y += offset_y;

		offset_x += text_size.x;
		break;
	}

	g_render.render_text( position, font_alignment::AL_DEFAULT, m_flags, m_text.c_str( ), m_font, m_color );
}

void visuals::esp_bar::render( math::box box, float& offset_x, float& offset_y )
{
	math::vec2< int > border_start_position;
	math::vec2< int > border_end_position;

	math::vec2< int > start_position;
	math::vec2< int > end_position;

	auto current_percentage = -( m_cur / ( m_max - m_min ) ) + 1.f;
	auto current_color      = m_color_from.lerp( m_color_to, current_percentage );

	switch ( m_location ) {
	case esp_location::LOCATION_TOP:
		border_start_position.x = box.x;
		border_start_position.y = box.y - 3 - m_width;

		border_end_position.x = box.w + 2;
		border_end_position.y = box.y - 2;

		start_position.x = box.x + ( box.w - box.x ) * current_percentage;
		start_position.y = box.y - 3 - m_width;

		end_position.x = box.w + 1;
		end_position.y = box.y - 3;

		border_start_position.x += offset_x;
		border_start_position.y += offset_y;

		border_end_position.x += offset_x;
		border_end_position.y += offset_y;

		start_position.x += offset_x;
		start_position.y += offset_y;

		end_position.x += offset_x;
		end_position.y += offset_y;

		offset_y += 5;
		break;
	case esp_location::LOCATION_LEFT:
		border_start_position.x = box.x - 3 - m_width;
		border_start_position.y = box.y;

		border_end_position.x = box.x - 2;
		border_end_position.y = box.h + 2;

		start_position.x = box.x - 3 - m_width;
		start_position.y = box.y + ( box.h - box.y ) * current_percentage;

		end_position.x = box.x - 3;
		end_position.y = box.h + 1;

		border_start_position.x += offset_x;
		border_start_position.y += offset_y;

		border_end_position.x += offset_x;
		border_end_position.y += offset_y;

		start_position.x += offset_x;
		start_position.y += offset_y;

		end_position.x += offset_x;
		end_position.y += offset_y;

		offset_x += 3 + m_width;
		break;
	case esp_location::LOCATION_BOTTOM:
		border_start_position.x = box.x;
		border_start_position.y = box.h + 4;

		border_end_position.x = box.w + 2;
		border_end_position.y = box.h + 5 + m_width;

		start_position.x = box.x + ( box.w - box.x ) * current_percentage;
		start_position.y = box.h + 4;

		end_position.x = box.w + 1;
		end_position.y = box.h + 4 + m_width;

		border_start_position.x += offset_x;
		border_start_position.y += offset_y;

		border_end_position.x += offset_x;
		border_end_position.y += offset_y;

		start_position.x += offset_x;
		start_position.y += offset_y;

		end_position.x += offset_x;
		end_position.y += offset_y;

		offset_y += 5;
		break;
	case esp_location::LOCATION_RIGHT:
		border_start_position.x = box.w + 4;
		border_start_position.y = box.y;

		border_end_position.x = box.w + 5 + m_width;
		border_end_position.y = box.h + 2;

		start_position.x = box.w + 4;
		start_position.y = box.y + ( box.h - box.y ) * current_percentage;

		end_position.x = box.w + 4 + m_width;
		end_position.y = box.h + 1;

		border_start_position.x += offset_x;
		border_start_position.y += offset_y;

		border_end_position.x += offset_x;
		border_end_position.y += offset_y;

		start_position.x += offset_x;
		start_position.y += offset_y;

		end_position.x += offset_x;
		end_position.y += offset_y;

		offset_x += 3 + m_width;
		break;
	}

	g_render.render_filled_rectangle( border_start_position - math::vec2< int >( 1, 1 ),
	                                  border_end_position + math::vec2< int >( 2, 2 ) - border_start_position - math::vec2< int >( 1, 1 ),
	                                  color( 32, 32, 32, std::clamp( current_color.a - 50, 0, 255 ) ) );
	g_render.render_filled_rectangle( start_position, end_position - start_position, current_color );
}
