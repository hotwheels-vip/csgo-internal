#pragma once
#include "../../../utilities/memory/virtual.h"

#define TEXTURE_GROUP_LIGHTMAP                    "Lightmaps"
#define TEXTURE_GROUP_WORLD                       "World textures"
#define TEXTURE_GROUP_MODEL                       "Model textures"
#define TEXTURE_GROUP_VGUI                        "VGUI textures"
#define TEXTURE_GROUP_PARTICLE                    "Particle textures"
#define TEXTURE_GROUP_DECAL                       "Decal textures"
#define TEXTURE_GROUP_SKYBOX                      "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS              "ClientEffect textures"
#define TEXTURE_GROUP_OTHER                       "Other textures"
#define TEXTURE_GROUP_PRECACHED                   "Precached"
#define TEXTURE_GROUP_CUBE_MAP                    "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET               "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED                 "Unaccounted textures"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER        "Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER         "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP   "Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR  "Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD  "World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS "Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER  "Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER        "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER       "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER                "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL                  "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS               "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS              "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE       "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS               "Morph Targets"

#define TEXTURE_SKYBOX_BAGGAGE     "cs_baggage_skybox_"
#define TEXTURE_SKYBOX_TIBET       "cs_tibet"
#define TEXTURE_SKYBOX_EMBASSY     "embassy"
#define TEXTURE_SKYBOX_ITALY       "italy"
#define TEXTURE_SKYBOX_JUNGLE      "jungle"
#define TEXTURE_SKYBOX_NUKEBLANK   "nukeblank"
#define TEXTURE_SKYBOX_OFFICE      "office"
#define TEXTURE_SKYBOX_DAYLIGHT01  "sky_cs15_daylight01_hdr"
#define TEXTURE_SKYBOX_DAYLIGHT02  "sky_cs15_daylight02_hdr"
#define TEXTURE_SKYBOX_DAYLIGHT03  "sky_cs15_daylight03_hdr"
#define TEXTURE_SKYBOX_DAYLIGHT04  "sky_cs15_daylight04_hdr"
#define TEXTURE_SKYBOX_CLOUDY01    "sky_csgo_cloudy01"
#define TEXTURE_SKYBOX_NIGHT02     "sky_csgo_night02"
#define TEXTURE_SKYBOX_NIGHT02B    "sky_csgo_night02b"
#define TEXTURE_SKYBOX_DUST        "sky_dust"
#define TEXTURE_SKYBOX_VENICE      "sky_venice"
#define TEXTURE_SKYBOX_VERTIGO     "vertigo"
#define TEXTURE_SKYBOX_VERTIGO_HDR "vertigoblue_hdr"
#define TEXTURE_SKYBOX_VIETNAM     "vietnam"

class c_material;
class c_material_var;
class c_key_values;
class c_texture;

class c_material_system
{
public:
	c_material* create_material( const char* name, c_key_values* key_values )
	{
		return g_virtual.call< c_material* >( this, 83, name, key_values );
	}

	c_material* find_material( char const* material_name, const char* texture_group_name = TEXTURE_GROUP_MODEL, bool complain = true,
	                           const char* complain_prefix = nullptr )
	{
		return g_virtual.call< c_material* >( this, 84, material_name, texture_group_name, complain, complain_prefix );
	}

	c_texture* find_texture( char const* texture_name, const char* texture_group_name, bool complain = true, int additional_creation_flags = 0 )
	{
		return g_virtual.call< c_texture* >( this, 91, texture_name, texture_group_name, complain, additional_creation_flags );
	}
};