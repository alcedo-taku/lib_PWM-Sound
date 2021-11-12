#include "stdio.h"
#include "tim.h"

/*struct type begin*/
namespace SoundScale {
//	typedef float Element;
	using Element = float; // C++だと上よりこちらのほうが一般的　テンプレートにも対応

	constexpr Element lB = 246.44f;
	constexpr Element C = 261.63f; //ド
	constexpr Element D = 293.66f;
	constexpr Element E = 329.63f;
	constexpr Element F = 349.23f;
	constexpr Element G = 392.00f;
	constexpr Element A = 440.00f;
	constexpr Element B = 493.88f;
	constexpr Element hC = 523.25f; //ド
	constexpr Element hD = 587.330f;
	constexpr Element hE = 659.255f;
	constexpr Element hF = 698.456f;
	constexpr Element hG = 783.991f;
	constexpr Element hA = 880.000f;
	constexpr Element hB = 987.767f;
	constexpr Element R = 0.0f; // 休符
};
struct Music{
	SoundScale::Element music_scale; // 厳密にはElementは型ではないけれど、型に見せかける方法
	uint32_t music_time;
};
/*struct type end*/


class PwmSounds {
public:
	PwmSounds();
	PwmSounds(TIM_HandleTypeDef* htim, uint32_t channel);
	PwmSounds(TIM_HandleTypeDef& htim, uint32_t channel);
	void set_sounds(Music* sounds, uint8_t number_of_sound);
	void start_sounds();
	bool update_sounds();
private:
	TIM_HandleTypeDef* sound_htim;
	uint32_t sound_channel;
	uint32_t end_time_of_sound; // 各音を終了する時間
	uint8_t playing_sound; // 何個目の音を再生中か
	bool is_playing = false; // 再生中か　再生中にスタート処理を行わないため
	Music *sounds;
	uint8_t number_of_sounds; // 合計何音か
};
