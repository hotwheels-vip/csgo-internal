#pragma once

namespace sdk
{
	enum life_state
	{
		LIFE_ALIVE = 0,
		LIFE_DYING,
		LIFE_DEAD,
		LIFE_RESPAWNABLE,
		LIFE_DISCARDBODY
	};

	enum flags
	{
		ONGROUND              = ( 1 << 0 ),
		DUCKING               = ( 1 << 1 ),
		ANIMDUCKING           = ( 1 << 2 ),
		WATERJUMP             = ( 1 << 3 ),
		ONTRAIN               = ( 1 << 4 ),
		INRAIN                = ( 1 << 5 ),
		FROZEN                = ( 1 << 6 ),
		ATCONTROLS            = ( 1 << 7 ),
		CLIENT                = ( 1 << 8 ),
		FAKECLIENT            = ( 1 << 9 ),
		INWATER               = ( 1 << 10 ),
		FLY                   = ( 1 << 11 ),
		SWIM                  = ( 1 << 12 ),
		CONVEYOR              = ( 1 << 13 ),
		NPC                   = ( 1 << 14 ),
		GODMODE               = ( 1 << 15 ),
		NOTARGET              = ( 1 << 16 ),
		AIMTARGET             = ( 1 << 17 ),
		PARTIALGROUND         = ( 1 << 18 ),
		STATICPROP            = ( 1 << 19 ),
		GRAPHED               = ( 1 << 20 ),
		GRENADE               = ( 1 << 21 ),
		STEPMOVEMENT          = ( 1 << 22 ),
		DONTTOUCH             = ( 1 << 23 ),
		BASEVELOCITY          = ( 1 << 24 ),
		WORLDBRUSH            = ( 1 << 25 ),
		OBJECT                = ( 1 << 26 ),
		KILLME                = ( 1 << 27 ),
		ONFIRE                = ( 1 << 28 ),
		DISSOLVING            = ( 1 << 29 ),
		TRANSRAGDOLL          = ( 1 << 30 ),
		UNBLOCKABLE_BY_PLAYER = ( 1 << 31 )
	};
} // namespace sdk
