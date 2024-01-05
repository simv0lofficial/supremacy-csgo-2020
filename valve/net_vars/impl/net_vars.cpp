#include "../../../supremacy.hpp"

namespace supremacy::valve {
	void c_net_vars::parse_table(
		const char* name, const recv_table_t* const table, std::uint32_t offset
	) {
		for ( int i{}; i < table->m_props_count; ++i ) {
			const auto prop = &table->m_props[ i ];
			
			const auto child = prop->m_data_table;
			if ( child
				&& child->m_props_count > 0 )
				parse_table( name, child, prop->m_offset + offset );

			char str[ 256u ]{};

			strcpy_s( str, name );
			strcat_s( str, "->" );
			strcat_s( str, prop->m_var_name );

			m_entries[ str ] = { prop, prop->m_offset + offset };
		}
	}

	void c_net_vars::parse_client_classes( ) {
		for ( auto client_class = valve::g_client->all_classes( ); client_class; client_class = client_class->m_next ) {
			if ( !client_class->m_recv_table )
				continue;

			parse_table( client_class->m_network_name, client_class->m_recv_table, 0u );
		}
	}

	void c_net_vars::parse_data_maps( const std::vector< data_map_t* >& data_maps ) {
		for (int i = 0; i < data_maps.size(); ++i) {
			if (i == 180)
				continue;

			auto data_map = data_maps[i];
			if (!data_map)
				continue;

			for (int j = 0; j < data_map->m_size; ++j) {
				auto description = &data_map->m_description[j];
				if (!description->m_name)
					continue;

				auto& entries = m_entries[description->m_name];
				entries = std::make_pair(nullptr, description->m_offset);
			}
		}
	}
}