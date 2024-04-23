# AnimationCurvesEditor
> 依赖于 ImGuiCore 和 C/C++标准库

- __ImGui__, __C11__, __CPP17__
- __Version:__ 0.1.2

---

```cpp 
namespace AnimCurves
```

## 结构体：

单个坐标轴采样点
```cpp
struct CoordAxisSample {
	float AnimNodeSample; // 值 (纵坐标).
	float TimePoint;      // 时间点 (横坐标).

	CoordAxisSample() : AnimNodeSample(0.0f), TimePoint(0.0f) {}
	CoordAxisSample(float node, float time) : AnimNodeSample(node), TimePoint(time) {}
};
```

采样六轴坐标组
```cpp
#define ACE_COORD_PARAMS 6
using SmpIdx = const std::vector<CoordAxisSample>&;
struct CoordAxisGroup {
	
    // 0: x轴 1: y轴 2: z轴 3: 俯仰轴 4: 偏航轴 5: 横滚轴
	std::vector<CoordAxisSample> AxisSamples[ACE_COORD_PARAMS] = {};

	int SamplingRateDraw; // 绘制采样率 (值越大精度越高).
	int SamplingStepBake; // 烘焙采样步长 (值越小精度越高n>0).

	CoordAxisGroup() : AxisSamples{}, SamplingRateDraw(NULL), SamplingStepBake(NULL) {}
	CoordAxisGroup(SmpIdx x, SmpIdx y, SmpIdx z, SmpIdx pt, SmpIdx yw, SmpIdx rl, int ps = 128, int gs = 2) :
		AxisSamples{ x, y, z, pt, yw, rl }, SamplingRateDraw(ps), SamplingStepBake(gs)
	{}
};
```

烘焙曲线生成坐标组
```cpp
struct AnimGenCoord {
	float AnimGenVector[ACE_COORD_PARAMS]; // 同上一样.
};
```

---

## 调用绘制编辑器

```cpp
class AnimationCurvesEditor;
```

编辑器成员

```cpp
// 输入坐标组数据指针.
void InDataPointer(CoordAxisGroup* ptr);

// 绘制(运行)编辑器.
void DrawEditorWindow(
	uint32_t         unqiue_id,                  // ImGui Frame Unique ID
	std::string      name,                       // 编辑器窗口名称
	float            track_length,               // 播放轨道最大长度
	float            player_step,                // 播放步长 (非烘焙步长)
	bool             fixed_window = false,       // 固定ImGui窗口
	bool*            p_open       = (bool*)NULL, // 与ImGui::Begin(...)中参数相同
	ImGuiWindowFlags flags        = NULL         // 与ImGui::Begin(...)中参数相同
);

// 获取当前播放位置 六轴坐标信息.
// return false: 暂停状态, true: 播放状态.
bool PlayerRunSample(AnimGenCoord& CoordParam);

// 烘焙生成曲线采样数据 (连续点阵).
std::vector<AnimGenCoord> BakeCurvesData();

// 设置面板页面 自绘项目 (系统已创建子窗口).
// 输入: value = true 显示子窗口边框.
std::function<void(bool&)> SettingsPanelDrawWinObj = [&](bool&) {};

// 编辑器各插值动画部件 插值速度缩放.
float InterSpeed = 1.0f;
// 编辑器色系 (编辑器所有控件会围绕这个颜色计算绘制).
ImVec4 EditorColorSystem = ImVec4(0.86f, 0.86f, 0.86f, 0.94f);
```

---
__Update:__ 2024_04_24 RCSZ.