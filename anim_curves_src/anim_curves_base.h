// anim_curves_base. RCSZ.

#ifndef __ANIM_CURVES_BASE_H
#define __ANIM_CURVES_BASE_H

#include "imgui.h"
#include "imgui_internal.h"

#define IMGUI_ITEM_SPAC ImGui::GetStyle().ItemSpacing.x
namespace ACE_CONTROL_BASE {

	ImVec4 ColorBrightnesScale(const ImVec4& color, float value);
	float ItemCenteredCalc(float width);

	void ListDrawLine(const ImVec2& point0, const ImVec2& point1, const ImVec4& color, float width);
	void ListDrawText(const ImVec2& position, const ImVec4& color, const char* text, ...);

	void ListDrawRectangle(const ImVec2& position, const ImVec2& size, const ImVec4& color, float width);
	void ListDrawRectangleFill(const ImVec2& position, const ImVec2& size, const ImVec4& color);
	void ListDrawCircleFill(const ImVec2& position, float size, const ImVec4& color);
	void ListDrawTriangleFill(const ImVec2& position, const ImVec2& offset1, const ImVec2& offset2, const ImVec4& color);

	namespace IM_ANIM {
#define ANIM_BUTTON_TRANS_STEP 0.078f
		class AnimButton {
		protected:
			ImVec4 ButtonAnimColor = {};
			ImVec2 ButtonAnimSize  = {};

			ImVec4 AnimStatColor[3] = {};
			ImVec2 AnimStatSize[3]  = {};

			size_t TypeModeIndex = NULL;

			bool PreviousState = false, Pulse = false;
		public:
			bool DrawButton(const char* name);
			void CalcInterParameters(float speed);

			void ResetState(bool flag);

			void SetButtonStatColor(const ImVec4& normal, const ImVec4& hover, const ImVec4& active);
			void SetButtonStatSize(const ImVec2& normal, const ImVec2& hover, const ImVec2& active);
		};
	}
}

#endif