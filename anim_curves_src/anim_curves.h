// anim_curves.
// PROJ: 2024_04_22 RCSZ. Update: 2024_04_22 RCSZ.
// CoreImGui, ISO_C11, ISO_C++17, MSVCx64.

#ifndef __ANIM_CURVES_H
#define __ANIM_CURVES_H
#include <functional>
#include <string>
#include <vector>
#include <deque>
#include <cstdio>

#include "anim_curves_base.h"

namespace AnimCurves {
	struct CoordAxisSample {
		// sample_value, time_point.
		float AnimNodeSample;
		float TimePoint;

		CoordAxisSample() : AnimNodeSample(0.0f), TimePoint(0.0f) {}
		CoordAxisSample(float node, float time) : AnimNodeSample(node), TimePoint(time) {}
	};

#define ACE_COORD_PARAMS 6
	using SmpIdx = const std::vector<CoordAxisSample>&;
	struct CoordAxisGroup {
		// axis[6]: x, y, z, pitch, yaw, roll.
		std::vector<CoordAxisSample> AxisSamples[ACE_COORD_PARAMS] = {};

		int SamplingRateDraw;
		int SamplingStepBake;

		CoordAxisGroup() : AxisSamples{}, SamplingRateDraw(NULL), SamplingStepBake(NULL) {}
		CoordAxisGroup(SmpIdx x, SmpIdx y, SmpIdx z, SmpIdx pt, SmpIdx yw, SmpIdx rl, int ps = 128, int gs = 2) :
			AxisSamples{ x, y, z, pt, yw, rl }, SamplingRateDraw(ps), SamplingStepBake(gs)
		{}
	};

	struct AnimGenCoord {
		float AnimGenVector[ACE_COORD_PARAMS];
	};

#define EDITOR_ANIM_BUTTON 9
	class AnimationCurvesEditor {
	protected:
		// editor player_box const_color.
		const ImVec4 EditorPlayerBoxColor = ImVec4(0.0f, 0.84f, 0.84f, 1.0f);
		// editor animation button.
		ACE_CONTROL_BASE::IM_ANIM::AnimButton AnimButton[EDITOR_ANIM_BUTTON] = {};
		// axis sample_group.
		CoordAxisGroup* SampleGroupPtr = nullptr;
		AnimGenCoord PlayerRunCoord = {};

		float EditorScaleLinesWidth = 1.0f;
		// false:[x.y.z], true:[pitch.yaw.roll]
		bool EditorModeType = false;

		ImVec2 TrackWindowScrollx    = {};   // x:pos, y:max.
		float  TrackScrollx          = 0.0f; // tick inter_smooth.
		float  TrackWidthValueScale  = 1.0f;
		float  TrackHeightValueScale = 1.0f;

		ImVec2 PlayerLineScrollx      = {}; // x:pos, y:smooth.
		bool   PlayerSetScrollyFlag   = false;
		bool   PlayerFlag             = false;
		bool   PlayerFlagIsTimeperiod = true;

		float* NodeSetValuePointer = nullptr;

		ImVec4      SystemAsixColors[3] = {};
		const char* SystemAsixTexts[3]  = {};

		bool SettingsFlag = false;
		// flags: 0:time_ruler, 1:indicator, 2:push_nodes
		bool SettingsFlags[3] = {};
		// bake-curve time(x:begin, y:end).
		ImVec2 BakeCurveTimePoint = ImVec2(0.0f, 120.0f);

		void MouseSetPointValue();
		void SpacingLine(const ImVec4& color);

		void DrawCubicBezierCurve(
			const ImVec2& point0, const ImVec2& point1, const ImVec4& color, const ImVec2& scale,
			int sample, float centerh
		);
		void DrawAnimationPoints(const ImVec2& position, const ImVec4& color, float& value);
		void DrawPlayerLine(float max);

		bool ChildWindowBorder = false;
		void DrawSettingsPanel();
		void IndicatorLight(bool flag, const ImVec4& on, const ImVec4& off);

		float CreateNodesAxisTemp[3] = {};
		bool  CreateNodesAxisFlag[3] = {};
		void DrawCreateNodesPanel(const ImVec4 colors[3], bool flag);

		bool SampleCubicBezierCurve(
			const ImVec2& point0, const ImVec2& point1, float& value, float timepoint,
			std::vector<CoordAxisSample>& src, size_t index
		);

		// anim curves sampler.
		bool AnimDatasetCurvesSampler(std::vector<AnimGenCoord>& smp_dataset);
	public:
		AnimationCurvesEditor();

		void InDataPointer(CoordAxisGroup* ptr) { 
			SampleGroupPtr = ptr; 
		};
		void DrawEditorWindow(
			uint32_t         unqiue_id,
			std::string      name,
			float            track_length,
			float            player_step,
			bool             fixed_window = false,
			bool*            p_open       = (bool*)NULL,
			ImGuiWindowFlags flags        = NULL
		);
		// 获取当前位置采样.
		bool PlayerRunSample(AnimGenCoord& CoordParam);
		// 烘焙曲线数据.
		std::vector<AnimGenCoord> BakeCurvesData();

		// 设置面板子窗口自绘, idx_flag: dw_border.
		std::function<void(bool&)> SettingsPanelDrawWinObj = [&](bool&) {};
		
		float InterSpeed = 1.0f;
		ImVec4 EditorColorSystem = ImVec4(0.86f, 0.86f, 0.86f, 0.94f);
	};
}
#endif