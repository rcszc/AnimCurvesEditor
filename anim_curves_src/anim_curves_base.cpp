// anim_curves_base. RCSZ.
#include <float.h>

#define IMC_CONTROL_BASE_MATHS
#include "anim_curves_base_ms.h"
#include "anim_curves_base.h"

ImVec4 FLOAT_COLOR_CLAMP(const ImVec4& color) {
	return ImVec4(
		IMFXC_V_CLAMP(color.x, 0.0f, 1.0f), IMFXC_V_CLAMP(color.y, 0.0f, 1.0f),
		IMFXC_V_CLAMP(color.z, 0.0f, 1.0f), IMFXC_V_CLAMP(color.w, 0.0f, 1.0f)
	);
}

// function: imgui window_draw_list.
namespace ACE_CONTROL_BASE {

#define IMCB_SCALE_CALC(high, value) high - value < 0.0f ? 0.0f : high - value && high - value > 1.0f ? 1.0f : high - value
	ImVec4 ColorBrightnesScale(const ImVec4& color, float value) {
		return ImVec4(
			IMCB_SCALE_CALC(color.x, value), IMCB_SCALE_CALC(color.y, value), // red,green
			IMCB_SCALE_CALC(color.z, value), IMCB_SCALE_CALC(color.w, value)  // blue,alpha
		);
	}

	float ItemCenteredCalc(float width) { 
		// window,child_window item centered.
		return ImGui::GetWindowSize().x / 2.0f - width / 2.0f; 
	}

	void ListDrawLine(const ImVec2& point0, const ImVec2& point1, const ImVec4& color, float width) {
		// draw line_segment.
		ImGui::GetWindowDrawList()->AddLine(
			ImGui::GetWindowPos() + point0,
			ImGui::GetWindowPos() + point1,
			IMVEC4_CVT_COLU32(FLOAT_COLOR_CLAMP(color)),
			width
		);
	}

#define IMCB_CHARTMEP_BYTES 2048
	void ListDrawText(const ImVec2& position, const ImVec4& color, const char* text, ...) {
		char TempChararray[IMCB_CHARTMEP_BYTES] = {};

		va_list ParamArgs;
		va_start(ParamArgs, text);
		vsnprintf(TempChararray, IMCB_CHARTMEP_BYTES, text, ParamArgs);
		va_end(ParamArgs);

		ImGui::GetWindowDrawList()->AddText(
			ImGui::GetWindowPos() + position,
			IMVEC4_CVT_COLU32(FLOAT_COLOR_CLAMP(color)),
			TempChararray
		);
	}

	void ListDrawRectangle(const ImVec2& position, const ImVec2& size, const ImVec4& color, float width) {
		// draw line_rectangle.
		ImGui::GetWindowDrawList()->AddRect(
			ImGui::GetWindowPos() + position,
			ImGui::GetWindowPos() + position + size,
			IMVEC4_CVT_COLU32(FLOAT_COLOR_CLAMP(color)),
			ImGui::GetStyle().FrameRounding,
			NULL, width
		);
	}

	void ListDrawRectangleFill(const ImVec2& position, const ImVec2& size, const ImVec4& color) {
		// draw fill_rectangle.
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetWindowPos() + position,
			ImGui::GetWindowPos() + position + size,
			IMVEC4_CVT_COLU32(FLOAT_COLOR_CLAMP(color)),
			ImGui::GetStyle().FrameRounding,
			NULL
		);
	}

	void ListDrawCircleFill(const ImVec2& position, float size, const ImVec4& color) {
		// draw fill_circle.
		ImGui::GetWindowDrawList()->AddCircleFilled(
			ImGui::GetWindowPos() + position,
			size,
			IMVEC4_CVT_COLU32(FLOAT_COLOR_CLAMP(color))
		);
	}

	void ListDrawTriangleFill(const ImVec2& position, const ImVec2& offset1, const ImVec2& offset2, const ImVec4& color) {
		// draw fill_triangle.
		ImGui::GetWindowDrawList()->AddTriangleFilled(
			ImGui::GetWindowPos() + position,
			ImGui::GetWindowPos() + position + offset1,
			ImGui::GetWindowPos() + position + offset2,
			IMVEC4_CVT_COLU32(FLOAT_COLOR_CLAMP(color))
		);
	}

	namespace IM_ANIM {
#define FLOAT_SIZE_CLAMP(size) ImVec2(IMFXC_V_CLAMP((size).x, 0.0f, FLT_MAX), IMFXC_V_CLAMP((size).y, 0.0f, FLT_MAX))

		bool AnimButton::DrawButton(const char* name) {
			bool ReturnStatFlag = false;

			ImGui::PushStyleColor(ImGuiCol_Button, ButtonAnimColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ButtonAnimColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ButtonAnimColor);

			// ÖØµþ»æÖÆ: "InvisibleButton" & "Button".
			ImVec2 ReposTemp = ImGui::GetCursorPos();
			{
				ReturnStatFlag = ImGui::InvisibleButton(name, AnimStatSize[0]);
				TypeModeIndex = NULL;
				
				if (!ImGui::IsItemHovered() && !ImGui::IsMouseDown(0)) TypeModeIndex = 0; // status: normal.
				if (ImGui::IsItemHovered() && !ImGui::IsMouseDown(0))  TypeModeIndex = 1; // status: hover.
				if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))   TypeModeIndex = 2; // status: active.
			}
			ImGui::SetCursorPos(ReposTemp);
			// fixed center_point.
			ImVec2 PosTemp = ImGui::GetCursorPos() + (AnimStatSize[0] - ButtonAnimSize) * 0.5f;
			ImGui::SetCursorPos(PosTemp);
			ImGui::Button(name, ButtonAnimSize);

			ImGui::PopStyleColor(3);
			return ReturnStatFlag;
		}

		void AnimButton::CalcInterParameters(float speed) {
			ButtonAnimColor += (AnimStatColor[TypeModeIndex] - ButtonAnimColor) * ANIM_BUTTON_TRANS_STEP * speed;
			ButtonAnimSize += (AnimStatSize[TypeModeIndex] - ButtonAnimSize) * ANIM_BUTTON_TRANS_STEP * speed;
			// reset button_state.
			if (Pulse) {
				ButtonAnimColor = AnimStatColor[0];
				ButtonAnimSize = AnimStatSize[0];
			}
		}

		void AnimButton::ResetState(bool flag) {
			flag != PreviousState ? Pulse = true : Pulse = false;
			PreviousState = flag;
		}

		void AnimButton::SetButtonStatColor(const ImVec4& normal, const ImVec4& hover, const ImVec4& active) {
			// set status colors. rgba4_stat3.
			AnimStatColor[0] = FLOAT_COLOR_CLAMP(normal);
			AnimStatColor[1] = FLOAT_COLOR_CLAMP(hover);
			AnimStatColor[2] = FLOAT_COLOR_CLAMP(active);
			ButtonAnimColor = AnimStatColor[0];
		}

		void AnimButton::SetButtonStatSize(const ImVec2& normal, const ImVec2& hover, const ImVec2& active) {
			// set status size. xy2_stat3.
			AnimStatSize[0] = FLOAT_SIZE_CLAMP(normal);
			AnimStatSize[1] = FLOAT_SIZE_CLAMP(hover);
			AnimStatSize[2] = FLOAT_SIZE_CLAMP(active);
			ButtonAnimSize = AnimStatSize[0];
		}
	}
}