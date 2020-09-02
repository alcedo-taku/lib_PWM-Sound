#include "stdio.h"

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
	SoundScale::Element musicScale;//厳密には型ではないけれど、型に見せかける方法
	uint32_t musicTime;
};
/*struct type end*/

class PwmSounds{
public:
	constexpr void setSounds();
	void startSounds();
	void updateSounds();
private:
};

Music music[3] = { {SoundScale::hC, 200}, {SoundScale::hE, 200}, {SoundScale::hG, 250} }; // a音階と各音の時間を指定
uint32_t soundTim; // 再生時間の制御
uint8_t count; // 何個目の音を再生中か
bool musicState = false; // 今音楽が流れているか　再生中にスタート処理を行わないため

if( HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET && musicState == false ){ //music start
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	//__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (htim1.Init.Period)*0.5);
	soundTim = HAL_GetTick();//music updateに進めるように、とりあえず代入
	count = 0;
	musicState = true; // 再生中にする
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET); // インジゲータLED点灯
}
if( soundTim <= HAL_GetTick() && musicState == true ){
	if( count < ( sizeof(music)/sizeof(Music))/*いくつ音があるか*/ ){ //music update
		__HAL_TIM_SET_AUTORELOAD( &htim1, 64000000/*HAL_RCC_GetHCLKFreq()*/ / (htim1.Init.Prescaler) / music[count].musicScale ); //counter period変更 → 周波数変更
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (htim1.Init.Period)*0.5); //count変更？ → デューティー比変更
		soundTim = HAL_GetTick() + music[count].musicTime; //soundTim 更新
	}else{ //music stop
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		musicState = false; // 再生停止状態にする
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET); // インジゲータLED消灯
	}
	count++;
}
