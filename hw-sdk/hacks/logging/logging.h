#pragma once
#include "../../globals/macros/macros.h"
#include <string>
#include <string_view>
#include <vector>

namespace logging
{
	struct log_text_t {
		log_text_t( ) = default;
		log_text_t( const std::string text, const std::string prefix, float time ) : m_text{ text }, m_prefix{ prefix }, m_time{ time } { };

		std::string m_text{ }, m_prefix{ };
		float m_time{ };
	};

	class impl
	{
	public:
		impl( ) : logs{ } { };

		void print( const std::string text, const std::string prefix = _( "[log]" ), float time = 8.f );

		void think( );

	private:
		std::vector< logging::log_text_t > logs{ };
	};
} // namespace logging

inline logging::impl g_log;
