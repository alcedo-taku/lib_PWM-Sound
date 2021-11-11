#include "stdio.h"
#include "tim.h"

/*struct type begin*/
namespace SoundScale {
//	typedef float Element;
	using Element = float;//C++だと上よりこちらのほうが一般的　テンプレートにも対応

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
};
struct Music{
	SoundScale::Element musicScale; // 厳密には型ではないけれど、型に見せかける方法
	uint32_t musicTime;
};
/*struct type end*/


class PwmSounds {
public:
	PwmSounds(TIM_HandleTypeDef htim, uint32_t channel);
	void setSounds(Music* sounds);
	void startSounds();
	bool updateSounds();
private:
	TIM_HandleTypeDef htim;
	uint32_t channel;
	uint32_t soundTim; // 再生時間の制御
	uint8_t count; // 何個目の音を再生中か
	bool musicState = false; // 今音楽が流れているか　再生中にスタート処理を行わないため
	Music *sounds;
};
