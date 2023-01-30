#pragma once
#pragma once
#include "../../../source_engine/enumerations/e_client_frame_stage.h"

#include "../../../source_engine/structs/vcollide_t.h"

#include <optional>

struct weather_t {
	void on_frame_stage_notify( e_client_frame_stage );

	vcollide_t m_collide{ };

	bool m_created{ false };
	std::optional< int > m_last_type = std::nullopt;
	int m_timer                      = -1;

private:
	void reset( );
};

inline weather_t weather = { };