// anim_curves_sampler. RCSZ. [20240424]
#define IMC_CONTROL_BASE_MATHS
#include "anim_curves_base_ms.h"
#include "anim_curves.h"

using namespace std;

namespace AnimCurves {
	// animation bezier cubic_calc sampler. v0.12
	bool AnimationCurvesEditor::AnimDatasetCurvesSampler(vector<AnimGenCoord>& smp_dataset) {
		float SamplerCountStep = (float)SampleGroupPtr->SamplingStepBake;

		if (SampleGroupPtr == nullptr)
			return false;

		AnimGenCoord SampleDatasetTemp = {};
		float SampleCoordValue[ACE_COORD_PARAMS] = {};

		for (float SmpPoint = BakeCurveTimePoint.x; SmpPoint < BakeCurveTimePoint.y; SmpPoint += SamplerCountStep) {
			// sample time_begin => time_end.
			for (size_t i = 0; i < ACE_COORD_PARAMS; ++i) {

				auto& NodesData = SampleGroupPtr->AxisSamples[i];
				if (!NodesData.empty()) {
					size_t OffsetCount = size_t(NodesData.size() >= 2);

					// sample time => points.
					for (size_t j = 0; j < NodesData.size() - 1; ++j) {
						if (SampleCubicBezierCurve(
							ImVec2(NodesData[j].TimePoint, NodesData[j].AnimNodeSample),
							ImVec2(NodesData[j + OffsetCount].TimePoint, NodesData[j + OffsetCount].AnimNodeSample),
							SampleCoordValue[i],
							SmpPoint,
							NodesData, j
						))
							SampleDatasetTemp.AnimGenVector[i] = SampleCoordValue[i];
					}
				}
			}
			smp_dataset.push_back(SampleDatasetTemp);
		}
		return true;
	}

	vector<AnimGenCoord> AnimationCurvesEditor::BakeCurvesData() {
		vector<AnimGenCoord> DatasetTemp = {};
		AnimDatasetCurvesSampler(DatasetTemp);
		return DatasetTemp;
	}
}