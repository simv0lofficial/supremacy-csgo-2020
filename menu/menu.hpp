#pragma once

namespace supremacy {
	class c_menu {
	private:
		ImWchar* get_font_glyph_ranges(ImFontAtlas* fonts) noexcept
		{
			static ImVector<ImWchar> ranges;
			if (ranges.empty()) {
				ImFontGlyphRangesBuilder builder;
				constexpr ImWchar base_ranges[]{
					0x100u, 0x24fu,
					0x300u, 0x3ffu,
					0x600u, 0x6ffu,
					0xe00u, 0xe7fu,
					0
				};
				builder.AddRanges(base_ranges);
				builder.AddRanges(fonts->GetGlyphRangesCyrillic());
				builder.AddRanges(fonts->GetGlyphRangesChineseSimplifiedCommon());
				builder.AddText("\u9F8D\u738B\u2122");
				builder.BuildRanges(&ranges);
			}
			return ranges.Data;
		}
	public:
		void init(IDirect3DDevice9* const pDevice);
		void draw_begin(IDirect3DDevice9* const pDevice);
		void draw();
		void draw_end();
		void handle();
		bool is_menu_initialized();
		bool is_menu_opened();
		void set_menu_opened(bool v);

		bool m_initialized;
		bool m_is_opened;
		int m_current_tab;

		bool color_picker(const char* name, float* color, bool alpha = true) {
			ui::SameLine(167.f);
			ImGuiWindow* window = ui::GetCurrentWindow();

			ui::SameLine(0 + window->Size.x - 43 - 7 + 18.f);
			auto alpha_slider_flag = alpha ? ImGuiColorEditFlags_AlphaBar : ImGuiColorEditFlags_NoAlpha;

			return ui::ColorEdit4(std::string{ "##" }.append(name).append("picker").c_str(), color, alpha_slider_flag | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
		}
	};

	inline const auto g_menu = std::make_unique< c_menu >( );

	class c_input {
	public:
		struct key_t {
			bool down;
			bool pressed;
			int  tick;
			int  oldtick;
		};

		struct rect_t {
			int x;
			int y;
			int w;
			int h;
		};

		std::array< key_t, 256 > m_keys;
		int m_scroll;
		std::string				 m_buffer;

	public:

		__forceinline void update()
		{
			// iterate all keys.
			for (int i{}; i <= 254; ++i) {
				key_t* key = &m_keys[i];

				key->pressed = false;

				if (key->down && key->tick > key->oldtick) {
					key->oldtick = key->tick;
					key->pressed = true;
				}
			}
		}

		__forceinline void set_down(int vk) {
			key_t* key = &m_keys[vk];

			key->down = true;
			key->tick = GetTickCount();
		}

		__forceinline void set_up(int vk) {
			key_t* key = &m_keys[vk];
			key->down = false;
		}

		// key is being held.
		__forceinline bool get_key_state(int vk) {
			if (vk == -1)
				return false;

			return m_keys[vk].down;
		}

		// key was pressed.
		__forceinline bool get_key_press(int vk) {
			if (vk == -1)
				return false;

			key_t* key = &m_keys[vk];
			return key->pressed;
		}
	};

	inline const auto g_input = std::make_unique< c_input >();
}