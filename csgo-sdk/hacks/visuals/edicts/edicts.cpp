#include "edicts.h"
#include "../../../game/sdk/includes/includes.h"
#include "../../../globals/includes/includes.h"

#include "../../entity_cache/entity_cache.h"

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

void n_edicts::impl_t::on_paint_traverse( )
{
	if ( GET_VARIABLE( g_variables.m_dropped_weapons, bool ) )
		this->dropped_weapons( );
	if ( GET_VARIABLE( g_variables.m_thrown_objects, bool ) )
		this->projectiles( );
}

void n_edicts::impl_t::reset( )
{
	if ( m_created ) {
		g_entity_cache.enumerate( e_enumeration_type::type_edicts, [ & ]( c_base_entity* entity ) {
			if ( !entity )
				return;

			const auto client_renderable = entity->get_client_renderable( );
			if ( !client_renderable )
				return;

			const auto client_unknown = client_renderable->get_client_unknown( );
			if ( !client_unknown )
				return;

			const auto client_networkable = client_unknown->get_client_networkable( );
			if ( !client_networkable )
				return;

			auto client_class = client_networkable->get_client_class( );
			if ( !client_class )
				return;

			if ( client_class->m_class_id == e_class_ids::c_precipitation ) {
				const auto rain_networkable = entity->get_client_networkable( );
				if ( !rain_networkable )
					return;

				rain_networkable->pre_data_update( 0 );
				rain_networkable->on_pre_data_changed( 0 );

				*( int* )( ( uintptr_t )entity + 0xA00 ) = -1;

				const auto collideable = entity->get_collideable( );
				if ( !collideable )
					return;

				collideable->get_obb_mins( ) = c_vector{ 0, 0, 0 };
				collideable->get_obb_maxs( ) = c_vector{ 0, 0, 0 };

				rain_networkable->on_data_changed( 0 );
				rain_networkable->post_data_update( 0 );
				rain_networkable->release( );
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

void n_edicts::impl_t::projectiles( )
{
	if ( !g_ctx.m_local )
		return;

	// so it doesnt get called a bunch of times
	const auto should_draw_icon = GET_VARIABLE( g_variables.m_thrown_objects_icon, bool );
	const auto should_draw_name = GET_VARIABLE( g_variables.m_thrown_objects_name, bool );

	ImColor icon_color = GET_VARIABLE( g_variables.m_thrown_objects_icon_color, c_color ).get_u32( 1.f );
	ImColor name_color = GET_VARIABLE( g_variables.m_thrown_objects_name_color, c_color ).get_u32( 1.f );

	g_entity_cache.enumerate( e_enumeration_type::type_edicts, [ & ]( c_base_entity* entity ) {
		if ( !entity || entity == g_ctx.m_local || entity->is_dormant( ) )
			return;

		const auto client_renderable = entity->get_client_renderable( );
		if ( !client_renderable )
			return;

		const auto client_unknown = client_renderable->get_client_unknown( );
		if ( !client_unknown )
			return;

		const auto client_networkable = client_unknown->get_client_networkable( );
		if ( !client_networkable )
			return;

		const auto client_class = client_networkable->get_client_class( );
		if ( !client_class )
			return;

		const auto class_id = client_class->m_class_id;

		auto model = entity->get_model( );

		if ( !model )
			return;

		auto model_name = g_interfaces.m_model_info->get_model_name( model );

		if ( !model_name )
			return;

		const std::string name = model->m_name;

		if ( name.find( "thrown" ) != std::string::npos || class_id == e_class_ids::c_base_cs_grenade_projectile ||
		     class_id == e_class_ids::c_decoy_projectile || class_id == e_class_ids::c_molotov_projectile ||
		     class_id == e_class_ids::c_snowball_projectile ) {
			c_vector_2d out{ };

			if ( !g_render.world_to_screen( entity->get_abs_origin( ), out ) || out.is_zero( ) )
				return;

			auto projectile_icon = [ & ]( const int char_icon_index, const std::string name ) -> void {
				const auto text_size = g_render.m_fonts[ e_font_names::font_name_verdana_11 ]->CalcTextSizeA(
					g_render.m_fonts[ e_font_names::font_name_verdana_11 ]->FontSize, FLT_MAX, 0.f, name.c_str( ) );

				if ( should_draw_name )
					g_render.m_draw_data.emplace_back(
						e_draw_type::draw_type_text,
						std::make_any< text_draw_object_t >( g_render.m_fonts[ e_font_names::font_name_verdana_11 ],
					                                         c_vector_2d( out.m_x - ( text_size.x / 2 ), out.m_y - 15 ), name, name_color,
					                                         c_color( 0.f, 0.f, 0.f, 1.f ).get_u32( ), e_text_flags::text_flag_dropshadow ) );

				auto icon = reinterpret_cast< const char* >( g_utilities.get_weapon_icon( char_icon_index ) );

				const auto icon_size = g_render.m_fonts[ e_font_names::font_name_icon_12 ]->CalcTextSizeA(
					g_render.m_fonts[ e_font_names::font_name_icon_12 ]->FontSize, FLT_MAX, 0.f, icon );
				if ( should_draw_icon )
					g_render.m_draw_data.emplace_back( e_draw_type::draw_type_text,
					                                   std::make_any< text_draw_object_t >( g_render.m_fonts[ e_font_names::font_name_icon_12 ], out,
					                                                                        icon, icon_color, ImColor( 0.f, 0.f, 0.f, 1.f ),
					                                                                        text_flag_dropshadow ) );
			};

			// couldnt think of better method, although i know this is retarded.

			if ( name.find( "flashbang" ) != std::string::npos )
				projectile_icon( weapon_flashbang, "flashbang" );
			else if ( name.find( "smokegrenade" ) != std::string::npos )
				projectile_icon( weapon_smokegrenade, "smoke grenade" );
			else if ( name.find( "incendiarygrenade" ) != std::string::npos )
				projectile_icon( weapon_incgrenade, "incendiary grenade" );
			else if ( name.find( "molotov" ) != std::string::npos )
				projectile_icon( weapon_molotov, "molotov" );
			else if ( name.find( "fraggrenade" ) != std::string::npos )
				projectile_icon( weapon_hegrenade, "high explosive grenade" );
			else if ( name.find( "decoy" ) != std::string::npos )
				projectile_icon( weapon_decoy, "decoy" );
		}
	} );
}

void n_edicts::impl_t::dropped_weapons( )
{
	g_entity_cache.enumerate( e_enumeration_type::type_edicts, [ & ]( c_base_entity* entity ) {
		if ( !entity || entity == g_ctx.m_local || entity->is_dormant( ) )
			return;

		const auto client_renderable = entity->get_client_renderable( );
		if ( !client_renderable )
			return;

		const auto client_unknown = client_renderable->get_client_unknown( );
		if ( !client_unknown )
			return;

		const auto client_networkable = client_unknown->get_client_networkable( );
		if ( !client_networkable )
			return;

		const auto client_class = client_networkable->get_client_class( );
		if ( !client_class )
			return;

		const auto class_id = client_class->m_class_id;

		if ( class_id == e_class_ids::c_base_weapon_world_model )
			return;

		if ( strstr( client_class->m_network_name, "CWeapon" ) || class_id == e_class_ids::c_deagle || class_id == e_class_ids::cak47 ) {
			const short definition_index = entity->get_item_definition_index( );
			if ( !definition_index )
				return;

			const auto weapon_data = g_interfaces.m_weapon_system->get_weapon_data( definition_index );
			if ( !weapon_data || !weapon_data->is_gun( ) )
				return;

			const auto owner_entity = g_interfaces.m_client_entity_list->get< c_base_entity >( entity->get_owner_entity_handle( ) );
			if ( owner_entity )
				return;

			bounding_box_t box{ };
			if ( !entity->get_bounding_box( &box ) )
				return;

			// get distance to weapon and subtract by our threshold(600 units)
			float distance_to_weapon_alpha = static_cast< float >( g_ctx.m_local->get_abs_origin( ).dist_to( entity->get_abs_origin( ) ) - 600.f );

			distance_to_weapon_alpha = g_math.divide_if_less< float >( distance_to_weapon_alpha, 250.f );

			if ( GET_VARIABLE( g_variables.m_dropped_weapons_box, bool ) ) {
				if ( !GET_VARIABLE( g_variables.m_dropped_weapons_box_corner, bool ) ) {
					g_render.m_draw_data.emplace_back(
						e_draw_type::draw_type_rect,
						std::make_any< rect_draw_object_t >(
							c_vector_2d( box.m_left, box.m_top ), c_vector_2d( box.m_right, box.m_bottom ),
							GET_VARIABLE( g_variables.m_dropped_weapons_box_color, c_color ).get_u32( 1.f - distance_to_weapon_alpha ),
							GET_VARIABLE( g_variables.m_dropped_weapons_box_outline_color, c_color ).get_u32( 1.f - distance_to_weapon_alpha ), false,
							0.f, ImDrawFlags_::ImDrawFlags_None, 1.f,
							GET_VARIABLE( g_variables.m_dropped_weapons_box_outline, bool )
								? e_rect_flags::rect_flag_inner_outline | e_rect_flags::rect_flag_outer_outline
								: e_rect_flags::rect_flag_none ) );
				} else {
					if ( GET_VARIABLE( g_variables.m_dropped_weapons_box_outline, bool ) ) {
						g_render.corner_rect( box.m_left, box.m_top, box.m_right, box.m_bottom,
						                      c_color( GET_VARIABLE( g_variables.m_dropped_weapons_box_outline_color, c_color ) )
						                          .get_u32( 1.f - distance_to_weapon_alpha ),
						                      2.f );
					}

					g_render.corner_rect(
						box.m_left, box.m_top, box.m_right, box.m_bottom,
						c_color( GET_VARIABLE( g_variables.m_dropped_weapons_box_color, c_color ) ).get_u32( 1.f - distance_to_weapon_alpha ) );
				}
			}

			float top_padding = 0.f;

			auto icon_color   = GET_VARIABLE( g_variables.m_dropped_weapons_icon_color, c_color );
			auto weapon_color = GET_VARIABLE( g_variables.m_dropped_weapons_name_color, c_color );

			if ( GET_VARIABLE( g_variables.m_dropped_weapons_icon, bool ) ) {
				const auto text      = reinterpret_cast< const char* >( g_utilities.get_weapon_icon( definition_index ) );
				const auto text_size = g_render.m_fonts[ e_font_names::font_name_icon_12 ]->CalcTextSizeA(
					g_render.m_fonts[ e_font_names::font_name_icon_12 ]->FontSize, FLT_MAX, 0.f, text );

				g_render.m_draw_data.emplace_back(
					e_draw_type::draw_type_text,
					std::make_any< text_draw_object_t >(
						g_render.m_fonts[ e_font_names::font_name_icon_12 ],
						c_vector_2d( box.m_left + box.m_width * 0.5f - text_size.x * 0.5f, box.m_top - 3 - text_size.y ), text,
						icon_color.get_u32( 1.f - distance_to_weapon_alpha ), c_color( 0.f, 0.f, 0.f, 1.f ).get_u32( 1.f - distance_to_weapon_alpha ),
						e_text_flags::text_flag_dropshadow ) );

				top_padding -= text_size.y;
			}

			if ( GET_VARIABLE( g_variables.m_dropped_weapons_name, bool ) ) {
				const auto localized_name = g_interfaces.m_localize->find( weapon_data->m_hud_name );

				std::wstring w = localized_name;
				if ( w.empty( ) )
					return;

				std::transform( w.begin( ), w.end( ), w.begin( ), ::towlower );

				const std::string converted_name( w.begin( ), w.end( ) );
				if ( converted_name.empty( ) )
					return;

				const auto text_size = g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
					g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, converted_name.c_str( ) );

				g_render.m_draw_data.emplace_back(
					e_draw_type::draw_type_text,
					std::make_any< text_draw_object_t >(
						g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
						c_vector_2d( box.m_left + box.m_width * 0.5f - text_size.x * 0.5f, box.m_top - 3 - text_size.y - top_padding ),
						converted_name,
						GET_VARIABLE( g_variables.m_dropped_weapons_name_color, c_color )
							.get_u32( weapon_color.get< color_type_a >( ) - distance_to_weapon_alpha ),
						c_color( 0.f, 0.f, 0.f, 1.f ).get_u32( weapon_color.get< color_type_a >( ) - distance_to_weapon_alpha ),
						e_text_flags::text_flag_dropshadow ) );
			}
		}
	} );
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
			if ( client_class->m_class_id == e_class_ids::c_precipitation ) {
				precipitation_client_class = client_class;
				break;
			}
		}
	}

	if ( precipitation_client_class && precipitation_client_class->m_create_fn ) {
		void* rain_networkable = ( ( void* ( * )( int, int ))precipitation_client_class->m_create_fn )( 2048 - 1, 0 );
		if ( !rain_networkable )
			return;

		const auto rain_unknown = ( ( c_client_renderable* )rain_networkable )->get_client_unknown( );
		if ( !rain_unknown )
			return;

		const auto entity = rain_unknown->get_base_entity( );
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

		collideable->get_obb_mins( ) = c_vector( -32768.f, -32768.f, -32768.f );
		collideable->get_obb_maxs( ) = c_vector( 32768.f, 32768.f, 32768.f );

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

		const auto client_renderable = entity->get_client_renderable( );
		if ( !client_renderable )
			return;

		const auto client_unknown = client_renderable->get_client_unknown( );
		if ( !client_unknown )
			return;

		const auto client_networkable = client_unknown->get_client_networkable( );
		if ( !client_networkable )
			return;

		const auto client_class = client_networkable->get_client_class( );
		if ( !client_class )
			return;

		if ( !( client_class->m_class_id == e_class_ids::c_fog_controller ) )
			return;

		const c_color color = GET_VARIABLE( g_variables.m_fog_color, c_color );

		entity->get_fog_enable( )  = GET_VARIABLE( g_variables.m_fog, bool ) ? 1 : 0;
		entity->get_fog_start( )   = GET_VARIABLE( g_variables.m_fog_start, float );
		entity->get_fog_end( )     = GET_VARIABLE( g_variables.m_fog_end, float );
		entity->get_fog_density( ) = color.base< e_color_type::color_type_a >( );
		entity->get_fog_color( )   = ImGui::ColorConvertFloat4ToU32( color.get_vec4( ) );
	} );
}
