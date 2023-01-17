#include "aimbot.h"
#include "../../globals.h"

void aimbot_t::on_create_move() {
	constexpr float field_of_view = 180.f;
	const auto eye_position       = globals.m_local->get_eye_position( );
}