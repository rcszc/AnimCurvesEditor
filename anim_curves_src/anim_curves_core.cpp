// anim_curves_core. RCSZ. [20240422]
#define IMC_CONTROL_BASE_MATHS
#include "anim_curves_base_ms.h"
#include "anim_curves.h"

using namespace std;

constexpr ImVec4 XYZCOL[3] = { ImVec4(1.0f, 0.2f, 0.2f, 0.78f), ImVec4(0.2f, 1.0f, 0.2f, 0.78f), ImVec4(0.5f, 0.5f, 1.0f, 0.78f) };
constexpr ImVec4 PYRCOL[3] = { ImVec4(1.0f, 0.0f, 1.0f, 0.78f), ImVec4(1.0f, 1.0f, 0.0f, 0.78f), ImVec4(0.0f, 1.0f, 1.0f, 0.78f) };

constexpr const char* XYZTEXT[3] = { "Xaxis: %.2f","Yaxis: %.2f","Zaxis: %.2f" };
constexpr const char* PYRTEXT[3] = { "Pitch: %.2f","Yaw: %.2f","Roll: %.2f" };

constexpr float PointDrawSize = 4.2f;

constexpr float EditorSpaceTop   = 28.0f;
constexpr float EditorSpaceLeft  = 200.0f;
constexpr float EditorSpaceBelow = 32.0f;

namespace AnimCurves {
	AnimationCurvesEditor::AnimationCurvesEditor() {
		// config anim_button color & size.
		for (size_t i = 0; i < EDITOR_ANIM_BUTTON; ++i) {
			AnimButton[i].SetButtonStatColor(
				ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.58f),
				ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.2f),
				EditorColorSystem
			);
		}

		float ButtonWidthA = (EditorSpaceLeft - IMGUI_ITEM_SPAC * 3.0f) * 0.5f;
		AnimButton[0].SetButtonStatSize(ImVec2(ButtonWidthA, 28.0f), ImVec2(ButtonWidthA, 28.0f), ImVec2(ButtonWidthA, 28.0f));
		AnimButton[1].SetButtonStatSize(ImVec2(ButtonWidthA, 28.0f), ImVec2(ButtonWidthA, 28.0f), ImVec2(ButtonWidthA, 28.0f));
		AnimButton[8].SetButtonStatSize(ImVec2(ButtonWidthA, 28.0f), ImVec2(ButtonWidthA, 28.0f), ImVec2(ButtonWidthA, 28.0f));

		float ButtonWidthB = ButtonWidthA * 2.0f + IMGUI_ITEM_SPAC;
		AnimButton[2].SetButtonStatSize(ImVec2(ButtonWidthB, 28.0f), ImVec2(ButtonWidthB, 28.0f), ImVec2(ButtonWidthB, 28.0f));
		AnimButton[3].SetButtonStatSize(ImVec2(ButtonWidthB, 28.0f), ImVec2(ButtonWidthB, 28.0f), ImVec2(ButtonWidthB, 28.0f));

		AnimButton[6].SetButtonStatSize(ImVec2(ButtonWidthB, 28.0f), ImVec2(ButtonWidthB, 28.0f), ImVec2(ButtonWidthB, 28.0f));
		AnimButton[7].SetButtonStatSize(ImVec2(ButtonWidthB, 28.0f), ImVec2(ButtonWidthB, 28.0f), ImVec2(ButtonWidthB, 28.0f));

		AnimButton[4].SetButtonStatSize(ImVec2(34.0f, 26.0f), ImVec2(32.0f, 26.0f), ImVec2(32.0f, 26.0f));
		AnimButton[5].SetButtonStatSize(ImVec2(34.0f, 26.0f), ImVec2(32.0f, 26.0f), ImVec2(32.0f, 26.0f));

		// initialization flags.
		SettingsFlags[0] = true;
		SettingsFlags[2] = true;
	}

	void AnimationCurvesEditor::MouseSetPointValue() {
		if (NodeSetValuePointer && ImGui::IsMouseDown(0)) {
			*NodeSetValuePointer -= ImGui::GetIO().MouseDelta.y * 0.92f;

			// floating window(view value).
			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.25f, 0.25f, 0.25f, 0.72f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.8f);
			{
				ImGui::BeginTooltip();
				ImGui::Text("Value:%.1f", *NodeSetValuePointer);
				ImGui::EndTooltip();
			}
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}
		else
			NodeSetValuePointer = nullptr;
	}

	void AnimationCurvesEditor::SpacingLine(const ImVec4& color) {
		ImGui::Spacing();
		ACE_CONTROL_BASE::ListDrawLine(
			ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - ImGui::GetScrollY()),
			ImVec2(ImGui::GetCursorPosX() + EditorSpaceLeft - IMGUI_ITEM_SPAC * 2.0f, ImGui::GetCursorPosY() - ImGui::GetScrollY()),
			color, 2.0f
		);
		ImGui::Spacing();
	}

	void AnimationCurvesEditor::DrawCubicBezierCurve(
		const ImVec2& point0, const ImVec2& point1, const ImVec4& color, const ImVec2& scale,
		int sample, float centerh
	) {
		// calculate bezier_curve draw_points.
		ImVec2 DrawPoint0(point0.x * scale.x - TrackScrollx, centerh - point0.y * scale.y);
		ImVec2 DrawPoint1(point1.x * scale.x - TrackScrollx, centerh - point1.y * scale.y);

		ImVec2 MidPoint = (DrawPoint0 + DrawPoint1) * 0.5f;
		ImVec2 ControlPointA = MidPoint - ImVec2(0.0f, (DrawPoint1 - MidPoint).y);
		ImVec2 ControlPointB = MidPoint + ImVec2(0.0f, (DrawPoint1 - MidPoint).y);

		ImVec2 BeginPointTemp = ImBezierCubicCalc(DrawPoint0, ControlPointA, ControlPointB, DrawPoint1, 0.0f);
		for (int i = 0; i < sample; ++i) {

			ImVec2 DrawPoint = ImBezierCubicCalc(DrawPoint0, ControlPointA, ControlPointB, DrawPoint1, (float)i / (float)sample);
			ACE_CONTROL_BASE::ListDrawLine(BeginPointTemp, DrawPoint, color, EditorScaleLinesWidth * 2.0f);
			BeginPointTemp = DrawPoint;
		}
	}

	void AnimationCurvesEditor::DrawAnimationPoints(const ImVec2& position, const ImVec4& color, float& value) {
		if (&value == NodeSetValuePointer)
			ACE_CONTROL_BASE::ListDrawCircleFill(position, 4.2f, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
		else
			ACE_CONTROL_BASE::ListDrawCircleFill(position, PointDrawSize, color);

		ImGui::SetCursorPos(ImVec2(position.x - PointDrawSize, position.y - PointDrawSize));
		ImGui::InvisibleButton("@SETVALUE", ImVec2(PointDrawSize * 2.0f, PointDrawSize * 2.0f));

		if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0) && !NodeSetValuePointer && !PlayerSetScrollyFlag)
			NodeSetValuePointer = &value;
	}

	void AnimationCurvesEditor::DrawPlayerLine(float max) {
		float PlayerRectWidth = IMGUI_ITEM_SPAC * 3.2f;
		float PlayerLineWidth = 2.2f;

		ACE_CONTROL_BASE::ListDrawLine(
			ImVec2(PlayerLineScrollx.y * TrackWidthValueScale - TrackScrollx - PlayerLineWidth * 0.5f, IMGUI_ITEM_SPAC + PlayerRectWidth * 0.5f),
			ImVec2(PlayerLineScrollx.y * TrackWidthValueScale - TrackScrollx - PlayerLineWidth * 0.5f, ImGui::GetWindowHeight() - IMGUI_ITEM_SPAC * 4.0f),
			EditorPlayerBoxColor,
			EditorScaleLinesWidth * 2.2f
		);

		ACE_CONTROL_BASE::ListDrawLine(
			ImVec2(PlayerLineScrollx.y * TrackWidthValueScale - TrackScrollx + IMGUI_ITEM_SPAC, ImGui::GetWindowHeight() - IMGUI_ITEM_SPAC * 4.0f),
			ImVec2(PlayerLineScrollx.y * TrackWidthValueScale - TrackScrollx - IMGUI_ITEM_SPAC, ImGui::GetWindowHeight() - IMGUI_ITEM_SPAC * 4.0f),
			EditorPlayerBoxColor,
			EditorScaleLinesWidth * 2.2f
		);

		ACE_CONTROL_BASE::ListDrawRectangleFill(
			ImVec2(PlayerLineScrollx.y * TrackWidthValueScale - TrackScrollx - PlayerRectWidth * 0.5f, IMGUI_ITEM_SPAC),
			ImVec2(PlayerRectWidth, IMGUI_ITEM_SPAC * 2.0f),
			EditorPlayerBoxColor
		);

		ImGui::SetCursorPos(ImVec2(PlayerLineScrollx.y * TrackWidthValueScale - TrackScrollx - PlayerRectWidth * 0.5f, IMGUI_ITEM_SPAC));
		ImGui::InvisibleButton("@SETPLAYER", ImVec2(PlayerRectWidth, IMGUI_ITEM_SPAC * 2.0f));

		if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0) && !NodeSetValuePointer)
			PlayerSetScrollyFlag = true;
		if (PlayerSetScrollyFlag && !ImGui::IsMouseDown(0))
			PlayerSetScrollyFlag = false;
		if (PlayerSetScrollyFlag)
			PlayerLineScrollx.x += ImGui::GetIO().MouseDelta.x / TrackWidthValueScale;

		PlayerLineScrollx.x = PlayerLineScrollx.x < 0.0f ? 0.0f : PlayerLineScrollx.x;
		PlayerLineScrollx.x = PlayerLineScrollx.x > max ? max : PlayerLineScrollx.x;
	}

	void AnimationCurvesEditor::DrawSettingsPanel() {
		SettingsFlag = !AnimButton[7].DrawButton("Return");

		ImGui::Checkbox("TimeRuler",      &SettingsFlags[0]);
		ImGui::Checkbox("IndicatorLight", &SettingsFlags[1]);
		ImGui::Checkbox("PushNodes",      &SettingsFlags[2]);

		ImGui::Spacing();
		ImGui::PushStyleColor(ImGuiCol_Button,        ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.58f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,  EditorColorSystem);
		{
			ImGui::SetNextItemWidth(128.0f);
			ImGui::InputInt("ViewSample", &SampleGroupPtr->SamplingRateDraw);
			ImGui::SetNextItemWidth(128.0f);
			ImGui::InputInt("BakeStep", &SampleGroupPtr->SamplingStepBake);
		}
		ImGui::PopStyleColor();

		if (SampleGroupPtr != nullptr) {
			// sampling rate limit.
			SampleGroupPtr->SamplingRateDraw = IMFXC_V_CLAMP(SampleGroupPtr->SamplingRateDraw, 4, 32768);
			SampleGroupPtr->SamplingStepBake = IMFXC_V_CLAMP(SampleGroupPtr->SamplingStepBake, 1, 60);
		}

		ImGui::SetCursorPos(ImVec2(256.0f, IMGUI_ITEM_SPAC * 4.0f));
		ImGui::BeginChild("SettingsWin", ImGui::GetWindowSize() - ImVec2(262.0f, IMGUI_ITEM_SPAC * 5.0f), ChildWindowBorder);
		// ex_draw function object.
		SettingsPanelDrawWinObj(ChildWindowBorder);
		ImGui::EndChild();
	}

	void AnimationCurvesEditor::DrawCreateNodesPanel(const ImVec4 colors[3], bool flag) {
		ImGui::SameLine();
		ImGui::SetNextItemWidth(240.0f);
		ImGui::InputFloat3("VALUES  ", CreateNodesAxisTemp, "%.2f");

		ImGui::PushStyleColor(ImGuiCol_CheckMark, colors[0]); ImGui::SameLine(); ImGui::Checkbox("##CTX", &CreateNodesAxisFlag[0]);
		ImGui::PushStyleColor(ImGuiCol_CheckMark, colors[1]); ImGui::SameLine(); ImGui::Checkbox("##CTY", &CreateNodesAxisFlag[1]);
		ImGui::PushStyleColor(ImGuiCol_CheckMark, colors[2]); ImGui::SameLine(); ImGui::Checkbox("##CTZ", &CreateNodesAxisFlag[2]);
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (AnimButton[8].DrawButton("Push")) {
			for (size_t i = 0; i < 3; ++i) {
				auto& Dataset = SampleGroupPtr->AxisSamples[i + (size_t)flag * 3];
				if (CreateNodesAxisFlag[i] && SampleGroupPtr != nullptr) {

					auto PushDataFunc = [&]() {
						SampleGroupPtr->AxisSamples[i + (size_t)flag * 3].push_back(CoordAxisSample(CreateNodesAxisTemp[i], PlayerLineScrollx.x));
					};
					// not empty => player_tiem > prev => push.
					if (!Dataset.empty()) {
						if (Dataset[Dataset.size() - 1].TimePoint < PlayerLineScrollx.x)
							PushDataFunc();
					}
					else // is empty => push.
						PushDataFunc();
				}
			}
		}
		ImGui::SameLine();
		ImGui::Text("  ");
	}

	bool AnimationCurvesEditor::SampleCubicBezierCurve(
		const ImVec2& point0, const ImVec2& point1, float& value, float timepoint,
		vector<CoordAxisSample>& src, size_t index
	) {
		float PrevDist = src[index].TimePoint;
		// calculate bezier_curve resul_value.
		if (point0.x < timepoint && point1.x > timepoint) {

			// bezier_curve control_points.
			ImVec2 MidPoint = (point0 + point1) * 0.5f;
			ImVec2 ControlPointA = MidPoint - ImVec2(0.0f, (point1 - MidPoint).y);
			ImVec2 ControlPointB = MidPoint + ImVec2(0.0f, (point1 - MidPoint).y);

			value = ImBezierCubicCalc(
				point0, ControlPointA, ControlPointB, point1, float((int)timepoint - PrevDist) / (point1.x - point0.x)
			).y;
			return true;
		}
		else {
			value = 0.0f;
			return false;
		}
	}

	constexpr ImGuiColorEditFlags IndicatorButtonFlags =
		ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoTooltip;
	constexpr float IndicatorButtonSize = 20.0f;
	constexpr float CenOffset = (28.0f - IndicatorButtonSize) * 0.5f;

	void AnimationCurvesEditor::IndicatorLight(bool flag, const ImVec4& on, const ImVec4& off) {
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + CenOffset);
		if (flag) ImGui::ColorButton("", on,  IndicatorButtonFlags, ImVec2(1.0f, 1.0f) * IndicatorButtonSize);
		else      ImGui::ColorButton("", off, IndicatorButtonFlags, ImVec2(1.0f, 1.0f) * IndicatorButtonSize);
	}

	bool AnimationCurvesEditor::PlayerRunSample(AnimGenCoord& CoordParam) {
		CoordParam = PlayerRunCoord;
		return PlayerFlag;
	};

	void AnimationCurvesEditor::DrawEditorWindow(
		uint32_t         unqiue_id,
		string           name,
		float            track_length,
		float            player_step,
		bool             fixed_window,
		bool*            p_open,
		ImGuiWindowFlags flags
	) {
		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_None;
		if (fixed_window)
			WindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		ImGui::PushID(unqiue_id);

		// frame_background color.
		ImGui::PushStyleColor(ImGuiCol_FrameBg,        ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.58f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.58f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.58f));
		// slider_block color.
		ImGui::PushStyleColor(ImGuiCol_SliderGrab,       ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.16f));
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.16f));
		ImGui::PushStyleColor(ImGuiCol_Text,             ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.24f));
		// "Checkbox" color.
		ImGui::PushStyleColor(ImGuiCol_CheckMark, ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.12f));

		for (size_t i = 0; i < EDITOR_ANIM_BUTTON; ++i) {
			AnimButton[i].ResetState(SettingsFlag);
			AnimButton[i].CalcInterParameters(InterSpeed);
		}

		ImGui::Begin(name.c_str(), p_open, WindowFlags | flags);
		if (!SettingsFlag) {
			ImVec2 ChildWindowSize = ImVec2(ImGui::GetWindowSize().x - EditorSpaceLeft, ImGui::GetWindowSize().y - EditorSpaceBelow);

			ImGui::SetCursorPos(ImVec2(
				EditorSpaceLeft,
				ImGui::GetWindowSize().y - EditorSpaceBelow - IMGUI_ITEM_SPAC * 0.5f
			));
			ImGui::SetNextItemWidth(ChildWindowSize.x - ImGui::CalcTextSize("TRACK").x - IMGUI_ITEM_SPAC * 2.0f);

			TrackWindowScrollx.y = track_length * TrackWidthValueScale - ChildWindowSize.x * 0.85f;
			TrackWindowScrollx.y = TrackWindowScrollx.y < 0.0f ? 0.0f : TrackWindowScrollx.y;

			// move track_view.
			ImGui::SliderFloat("TRACK", &TrackWindowScrollx.x, 0.0f, TrackWindowScrollx.y, "");

			ImGui::SetCursorPos(ImVec2(EditorSpaceLeft, IMGUI_ITEM_SPAC * 5.32f + EditorSpaceTop));
			// draw animation_track editor window.
			ImVec2 WindowSizeTemp(ChildWindowSize.x - IMGUI_ITEM_SPAC, ChildWindowSize.y - IMGUI_ITEM_SPAC * 6.5f - EditorSpaceTop);
			ImGui::BeginChild("@ANIMTRACK", WindowSizeTemp, false);
			float HighCenterPosition = ImGui::GetWindowHeight() * 0.5f;

			// track window background rect.
			ACE_CONTROL_BASE::ListDrawRectangleFill(
				ImVec2(0.0f, 0.0f), WindowSizeTemp, ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.65f)
			);
			
			TrackScrollx += (TrackWindowScrollx.x - IMGUI_ITEM_SPAC * 4.0f - TrackScrollx) * 0.1f * InterSpeed;

			ImVec4 LinesColor = ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.42f);
			for (float i = 50.0f; i <= track_length; i += 100.0f) {
				LinesColor.w = 0.32f; // set_alpha.
				float LinesScrollxTemp = i * TrackWidthValueScale - TrackScrollx;

				ACE_CONTROL_BASE::ListDrawLine(
					ImVec2(LinesScrollxTemp, IMGUI_ITEM_SPAC * 7.0f),
					ImVec2(LinesScrollxTemp, ImGui::GetWindowHeight() - IMGUI_ITEM_SPAC * 10.0f),
					LinesColor,
					EditorScaleLinesWidth
				);
			}

			if (SettingsFlags[0]) {
				float TimeRulerTop = IMGUI_ITEM_SPAC * 7.0f;
				char TimeRulerTemp[2][8] = {};

				sprintf_s(TimeRulerTemp[0], "%.1f", BakeCurveTimePoint.x);
				sprintf_s(TimeRulerTemp[1], "%.1f", BakeCurveTimePoint.y);

				LinesColor.w = 0.72f;
				// ruler left_begin_text.
				ImVec2 TextPos(
					TrackWidthValueScale - TrackScrollx + BakeCurveTimePoint.x * TrackWidthValueScale - ImGui::CalcTextSize(TimeRulerTemp[0]).x * 0.5f,
					TimeRulerTop - IMGUI_ITEM_SPAC * 3.5f
				);
				ACE_CONTROL_BASE::ListDrawText(TextPos, LinesColor, "%.1f", BakeCurveTimePoint.x);

				// ruler right_begin_text.
				ImVec2 FontsSize = ImGui::CalcTextSize(TimeRulerTemp[1]);
				TextPos.x = TrackWidthValueScale - TrackScrollx + BakeCurveTimePoint.y * TrackWidthValueScale - FontsSize.x * 0.5f;
				TextPos.y = TimeRulerTop - IMGUI_ITEM_SPAC * 1.5f + FontsSize.y + 32.0f;

				ACE_CONTROL_BASE::ListDrawText(TextPos, LinesColor, "%.1f", BakeCurveTimePoint.y);

				float RulerDrawTempValue = TrackWidthValueScale - TrackScrollx - 1.0f + BakeCurveTimePoint.x * TrackWidthValueScale;
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
				{
					// draw time_ruler rcet.
					ACE_CONTROL_BASE::ListDrawRectangleFill(
						ImVec2(RulerDrawTempValue, TimeRulerTop),
						ImVec2((BakeCurveTimePoint.y - BakeCurveTimePoint.x) * TrackWidthValueScale, 32.0f),
						ACE_CONTROL_BASE::ColorBrightnesScale(EditorColorSystem, 0.58f)
					);
				}
				ImGui::PopStyleVar();

				// draw ruler_scale left.
				ImVec2 LinesPosLeft(RulerDrawTempValue, TimeRulerTop + 32.0f);
				ACE_CONTROL_BASE::ListDrawLine(
					LinesPosLeft, ImVec2(LinesPosLeft.x, LinesPosLeft.y - IMGUI_ITEM_SPAC * 4.8f),
					LinesColor, EditorScaleLinesWidth * 1.32f
				);

				// draw ruler_scale right.
				ImVec2 LinesPosRight(RulerDrawTempValue + (BakeCurveTimePoint.y - BakeCurveTimePoint.x) * TrackWidthValueScale, TimeRulerTop);
				ACE_CONTROL_BASE::ListDrawLine(
					LinesPosRight, ImVec2(LinesPosRight.x, LinesPosRight.y + IMGUI_ITEM_SPAC * 4.8f),
					LinesColor, EditorScaleLinesWidth * 1.32f
				);
			}

			char TimeTickText[8] = {};
			for (float i = 0.0f; i <= track_length; i += 100.0f) {
				// calc,convert time_scale.
				sprintf_s(TimeTickText, "%.0f", i);
				float LinesScrollxTemp = i * TrackWidthValueScale - TrackScrollx;

				// draw time_scale text.
				ImVec2 TextPos(LinesScrollxTemp - ImGui::CalcTextSize(TimeTickText).x * 0.5f, ImGui::GetWindowHeight() - IMGUI_ITEM_SPAC * 3.5f);
				LinesColor.w = 0.92f; // color ahpla.
				ACE_CONTROL_BASE::ListDrawText(TextPos, LinesColor, "%.0f", i);

				LinesColor.w = 0.32f;
				ACE_CONTROL_BASE::ListDrawLine(
					ImVec2(LinesScrollxTemp, IMGUI_ITEM_SPAC),
					ImVec2(LinesScrollxTemp, ImGui::GetWindowHeight() - IMGUI_ITEM_SPAC * 4.0f),
					LinesColor,
					EditorScaleLinesWidth * 3.2f
				);
			}

			// draw center_line[value: 0.0f].
			ACE_CONTROL_BASE::ListDrawLine(
				ImVec2(0.0f, HighCenterPosition),
				ImVec2(ImGui::GetWindowWidth(), HighCenterPosition),
				LinesColor,
				EditorScaleLinesWidth * 1.8f
			);

			if (EditorModeType) {
				std::copy(std::begin(PYRCOL), std::end(PYRCOL), std::begin(SystemAsixColors));
				std::copy(std::begin(PYRTEXT), std::end(PYRTEXT), std::begin(SystemAsixTexts));
			}
			else {
				std::copy(std::begin(XYZCOL), std::end(XYZCOL), std::begin(SystemAsixColors));
				std::copy(std::begin(XYZTEXT), std::end(XYZTEXT), std::begin(SystemAsixTexts));
			}

			if (SampleGroupPtr != nullptr) {
				float SampleCoordIndex[ACE_COORD_PARAMS] = {};
				for (size_t i = (size_t)EditorModeType * 3; i < ACE_COORD_PARAMS / (1 * (size_t)!EditorModeType + 1); ++i) {

					auto& NodesData = SampleGroupPtr->AxisSamples[i];
					if (!NodesData.empty()) {
						size_t OffsetCount = size_t(NodesData.size() >= 2);
						// draw fast_item.
						for (size_t j = 0; j < NodesData.size() - 1; ++j) {
							DrawCubicBezierCurve(
								ImVec2(NodesData[j].TimePoint, NodesData[j].AnimNodeSample),
								ImVec2(NodesData[j + OffsetCount].TimePoint, NodesData[j + OffsetCount].AnimNodeSample),
								SystemAsixColors[i - (size_t)EditorModeType * 3],
								ImVec2(TrackWidthValueScale, TrackHeightValueScale),
								SampleGroupPtr->SamplingRateDraw,
								HighCenterPosition
							);
							DrawAnimationPoints(
								ImVec2(
									NodesData[j].TimePoint * TrackWidthValueScale - TrackScrollx,
									HighCenterPosition - NodesData[j].AnimNodeSample * TrackHeightValueScale
								),
								SystemAsixColors[i - (size_t)EditorModeType * 3],
								NodesData[j].AnimNodeSample
							);

							if (SampleCubicBezierCurve(
								ImVec2(NodesData[j].TimePoint, NodesData[j].AnimNodeSample),
								ImVec2(NodesData[j + OffsetCount].TimePoint, NodesData[j + OffsetCount].AnimNodeSample),
								SampleCoordIndex[i],
								PlayerLineScrollx.y,
								NodesData, j
							))
								PlayerRunCoord.AnimGenVector[i] = SampleCoordIndex[i];
						}
						// draw last_item.
						size_t Last = NodesData.size() - 1;
						for (size_t j = 0; j < 3; ++j) {
							DrawAnimationPoints(
								ImVec2(
									NodesData[Last].TimePoint * TrackWidthValueScale - TrackScrollx,
									HighCenterPosition - NodesData[Last].AnimNodeSample * TrackHeightValueScale
								),
								SystemAsixColors[i - (size_t)EditorModeType * 3],
								NodesData[Last].AnimNodeSample
							);
						}
					}
				}
			}
			MouseSetPointValue();
			DrawPlayerLine(track_length);

			ImGui::EndChild();

			PlayerLineScrollx.y += (PlayerLineScrollx.x - PlayerLineScrollx.y) * 0.12f * InterSpeed;
			PlayerLineScrollx.y = abs(PlayerLineScrollx.x - PlayerLineScrollx.y) < 0.01f ? PlayerLineScrollx.x : PlayerLineScrollx.y;

			// TopControl.
			ImGui::SetCursorPos(ImVec2(IMGUI_ITEM_SPAC, IMGUI_ITEM_SPAC * 4.32f));
			ImGui::Text("TIME-POINT: %.0f", PlayerLineScrollx.y);
			SpacingLine(LinesColor);
			ImVec2 DrawPointTemp = ImGui::GetCursorPos();

			ImGui::SetCursorPos(ImVec2(
				EditorSpaceLeft,
				IMGUI_ITEM_SPAC * 4.32f
			));
			ImGui::SetNextItemWidth(128.0f);
			ImGui::InputFloat("SET-POS", &PlayerLineScrollx.x, 0.0f, 0.0f, "%.1f");

			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + IMGUI_ITEM_SPAC);
			ImGui::SetNextItemWidth(200.0f);
			ImGui::InputFloat2("BEGIN,END  ", &BakeCurveTimePoint.x, "%.1f");

			BakeCurveTimePoint.x = BakeCurveTimePoint.x < 0.0f ? 0.0f : BakeCurveTimePoint.x;
			BakeCurveTimePoint.y = IMFXC_V_CLAMP(BakeCurveTimePoint.y, BakeCurveTimePoint.x + 1.0f, track_length);

			// panel: create push axis_value.
			if (SettingsFlags[2])
				DrawCreateNodesPanel(SystemAsixColors, EditorModeType);

			if (SettingsFlags[1]) {
				// editor status indicator.
				ImGui::SameLine();
				IndicatorLight(!PlayerFlag, ImVec4(1.0f, 0.0f, 0.0f, 0.92f), ImVec4(0.2f, 0.2f, 0.2f, 0.72f));
				ImGui::SameLine();
				IndicatorLight(PlayerFlag, ImVec4(0.0f, 1.0f, 0.0f, 0.92f), ImVec4(0.2f, 0.2f, 0.2f, 0.72f));

				PlayerFlagIsTimeperiod = true;
				if (PlayerLineScrollx.x < BakeCurveTimePoint.x || PlayerLineScrollx.x > BakeCurveTimePoint.y)
					PlayerFlagIsTimeperiod = false;

				ImGui::SameLine();
				IndicatorLight(!PlayerFlagIsTimeperiod, ImVec4(1.0f, 1.0f, 0.0f, 0.92f), ImVec4(0.2f, 0.2f, 0.2f, 0.72f));

				bool ScaleFLag = 
					std::abs(TrackHeightValueScale - 1.0f) < numeric_limits<float>::epsilon() &&
					std::abs(TrackWidthValueScale  - 1.0f) < numeric_limits<float>::epsilon();
				ImGui::SameLine();
				IndicatorLight(!ScaleFLag, ImVec4(1.0f, 1.0f, 0.0f, 0.92f), ImVec4(0.2f, 0.2f, 0.2f, 0.72f));
			}

			ImGui::SetCursorPos(ImVec2(DrawPointTemp.x, DrawPointTemp.y + IMGUI_ITEM_SPAC));

			if (AnimButton[0].DrawButton("Begin")) {
				PlayerLineScrollx.x = 0.0f;
				// set player_box => begin_pos.
				if (PlayerFlag) PlayerLineScrollx.y = 0.0f;
			}
			ImGui::SameLine();
			if (AnimButton[1].DrawButton("End")) {
				PlayerLineScrollx.x = track_length;
				// set player_box => end_pos.
				if (PlayerFlag) PlayerLineScrollx.y = track_length;
			}
			ImGui::Spacing();

			auto ModeButtonFunc = [&](bool& mode_flag, const char* fname, const char* tname, size_t butidx) {
				if (mode_flag) {
					if (AnimButton[butidx].DrawButton(fname)) mode_flag = !mode_flag;
				}
				else if (AnimButton[butidx].DrawButton(tname)) mode_flag = !mode_flag;
			};

			ModeButtonFunc(PlayerFlag, "STOP", "PLAY", 2);
			if (PlayerFlag)
				PlayerLineScrollx.x += player_step * TrackWidthValueScale;

			ImGui::Spacing();
			ModeButtonFunc(EditorModeType, "X.Y.Z", "P.Y.R", 3);

			float SpaceItemLeft = EditorSpaceLeft - IMGUI_ITEM_SPAC * 2.0f;

			ImGui::Spacing();
			if (AnimButton[4].DrawButton("RH"))
				TrackHeightValueScale = 1.0f;

			ImGui::SameLine();
			ImGui::SetNextItemWidth(SpaceItemLeft - 34.0f - IMGUI_ITEM_SPAC);
			ImGui::SliderFloat("##HIGHSCALE", &TrackHeightValueScale, 0.02f, 2.0f, "%.2f");

			ImGui::Spacing();
			if (AnimButton[5].DrawButton("RW"))
				TrackWidthValueScale = 1.0f;

			ImGui::SameLine();
			ImGui::SetNextItemWidth(SpaceItemLeft - 34.0f - IMGUI_ITEM_SPAC);
			ImGui::SliderFloat("##WIDTHSCALE", &TrackWidthValueScale, 0.5f, 2.0f, "%.2f");

			ImGui::Spacing();
			SettingsFlag = AnimButton[6].DrawButton("Settings");

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
			SpacingLine(LinesColor);
			{
				for (size_t i = 0; i < 3; ++i)
					ImGui::TextColored(SystemAsixColors[i], SystemAsixTexts[i], PlayerRunCoord.AnimGenVector[i + (size_t)EditorModeType * 3]);
			}
			SpacingLine(LinesColor);

			size_t AnimNodesTotal = NULL;
			if (SampleGroupPtr != nullptr)
				for (size_t i = 0; i < ACE_COORD_PARAMS; ++i)
					AnimNodesTotal += SampleGroupPtr->AxisSamples[i].size();
			ImGui::Text("Animations: %u", AnimNodesTotal);
		}
		else
			DrawSettingsPanel();
		ImGui::End();
		ImGui::PopStyleColor(7);
		ImGui::PopID();
	}
}