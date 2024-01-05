#pragma once

namespace supremacy::valve {
	class c_net_vars {
	private:
		using entry_t = std::pair< recv_prop_t*, std::uint32_t >;

		void parse_table( 
			const char* name, const recv_table_t* const table, std::uint32_t offset
		);

		std::unordered_map< std::string, entry_t > m_entries{};
	public:
		void parse_client_classes( );

		void parse_data_maps( const std::vector< data_map_t* >& data_maps );

		__forceinline entry_t& entry( const std::string& name );
	};

	inline const auto g_net_vars = std::make_unique< c_net_vars >( );
}

#include "impl/net_vars.inl"