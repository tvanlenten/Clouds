#include "GuiUtils.h"
#include "../ImGui/imgui.h"

std::string Utils::TextInputVariableGui(const char* infoText)
{
	struct TextFilters 
	{ 
		static int FilterImGuiLetters(ImGuiInputTextCallbackData* data) 
		{ 
			if (data->EventChar < 256 && strchr("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", (char)data->EventChar)) 
				return 0; 
			return 1; 
		} 
	};
	static char buf[64] = "";
	ImGui::InputText(infoText, buf, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);

	return std::string(buf);
}
