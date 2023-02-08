#pragma once
#include "../../dependencies/fnv1a/fnv1a.h"

// RUN TIME HASHING
#define HASH_RT( s ) fnv1a::hash( s )

// BUILD TIME HASHING
#define HASH_BT( s ) fnv1a::hash_const( s )

#define CLIENT_DLL              HASH_BT( "client.dll" )
#define ENGINE_DLL              HASH_BT( "engine.dll" )
#define LOCALIZE_DLL            HASH_BT( "localize.dll" )
#define MATERIALSYSTEM_DLL      HASH_BT( "materialsystem.dll" )
#define VGUI_DLL                HASH_BT( "vguimatsurface.dll" )
#define VGUI2_DLL               HASH_BT( "vgui2.dll" )
#define SHADERAPIDX9_DLL        HASH_BT( "shaderapidx9.dll" )
#define GAMEOVERLAYRENDERER_DLL HASH_BT( "gameoverlayrenderer.dll" )
#define PHYSICS_DLL             HASH_BT( "vphysics.dll" )
#define VSTDLIB_DLL             HASH_BT( "vstdlib.dll" )
#define TIER0_DLL               HASH_BT( "tier0.dll" )
#define INPUTSYSTEM_DLL         HASH_BT( "inputsystem.dll" )
#define STUDIORENDER_DLL        HASH_BT( "studiorender.dll" )
#define DATACACHE_DLL           HASH_BT( "datacache.dll" )
#define STEAM_API_DLL           HASH_BT( "steam_api.dll" )
#define MATCHMAKING_DLL         HASH_BT( "matchmaking.dll" )
#define SERVER_DLL              HASH_BT( "server.dll" )
#define SERVERBROWSER_DLL       HASH_BT( "serverbrowser.dll" )
#define FILESYSTEM_DLL          HASH_BT( "filesystem_stdio.dll" )
