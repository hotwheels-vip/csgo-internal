#pragma once
#include "../../utilities/macros.h"

class c_player_animation_layer
{
public:
	bool client_bleed;
	float blend_in;
	void* studio_hdr;
	int dispatch_sequence;
	int dispatch_sequence2;
	int order;
	int sequence;
	float prev_cycle;
	float weight;
	float weight_delta_rate;
	float playback_rate;
	float cycle;
	void* owner;
	PAD( 4 );
};
