#include "../../supremacy.hpp"

namespace util {
	void c_notify::print_notify(bool print_in_console, bool print_notify, const std::uint32_t& color, const char* format, ...) {
		auto temp = std::vector<char>{};
		auto length = std::size_t{ 63 };
		va_list args;
		while (temp.size() <= length) {
			temp.resize(length + 1);
			va_start(args, format);
			const auto status = std::vsnprintf(temp.data(), temp.size(), format, args);
			va_end(args);

			length = static_cast<std::size_t>(status);
		}
		std::string out{ temp.data(), length };

		if (print_notify) {
			using add_notify_t = void(__stdcall*)(const std::uint32_t&, const char*);
			reinterpret_cast<add_notify_t>(supremacy::g_context->addresses().m_add_notify)(0xffffffffu, out.c_str());
		}

		if (print_in_console)
			supremacy::valve::g_cvar->con_print(color, out.c_str());
	}

	void c_notify::print_logo() {
		supremacy::valve::g_cvar->con_print(static_cast<std::uint8_t>(sdk::g_config_system->menu_color[0] * 255.f)
			| (static_cast<std::uint8_t>(sdk::g_config_system->menu_color[1] * 255.f) << 8u)
			| (static_cast<std::uint8_t>(sdk::g_config_system->menu_color[2] * 255.f) << 16u)
			| (static_cast<std::uint8_t>(sdk::g_config_system->menu_color[3] * 255.f) << 24u),
			xorstr_("[supremacy] "));
	}
}