#pragma once

#include "../net_vars.hpp"

namespace supremacy::valve {
	__forceinline c_net_vars::entry_t& c_net_vars::entry( const std::string& name ) {
		return m_entries.at( name );
	}
}