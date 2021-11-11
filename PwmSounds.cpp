#include "PwmSounds.hpp"
#include "gpio.h"

PwmSounds::PwmSounds(TIM_HandleTypeDef htim, uint32_t channel):
	htim(htim),
	channel(channel)
{
}

void PwmSounds::setSounds(Music* tmphsound){
	sounds = tmphsound;
}

void PwmSounds::startSounds(){
	if( musicState == false ){ // もし停止状態だったら
		// music start
		HAL_TIM_PWM_Start(&htim, channel);
		soundTim = HAL_GetTick(); // music updateに進めるように、とりあえず代入
		count = 0;
		musicState = true; // 再生中にする
	}
}

bool PwmSounds::updateSounds(){
	if( musicState == true && soundTim <= HAL_GetTick() ){ // もし再生中で、かつその音を出す時間を過ぎていたら
		if( count < ( sizeof(sounds)/sizeof(Music))/*音の数*/ ){ // もし次の音があれば
			// music update
			__HAL_TIM_SET_AUTORELOAD( &htim, 64000000/*HAL_RCC_GetHCLKFreq()*/ / (htim.Init.Prescaler) / sounds[count].musicScale ); // counter period変更 → 周波数変更
			__HAL_TIM_SET_COMPARE(&htim, channel, (htim.Init.Period)*0.5); // デューティー比を50%にする
			soundTim = HAL_GetTick() + sounds[count].musicTime; // soundTim 更新
		}else{
			// music stop
			HAL_TIM_PWM_Stop(&htim, channel);
			musicState = false; // 停止状態にする
		}
		count++; // 次の音に移行
	}
	return musicState; // 今何音目かを返す
}
