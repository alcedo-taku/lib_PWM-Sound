#include "PwmSounds.hpp"
#include "gpio.h"

PwmSounds::PwmSounds(){//引数をアドレスにしたい
}

void PwmSounds::setTimer(TIM_HandleTypeDef tmpTim, uint32_t tmpChannel){
	htim = tmpTim;
	channel = tmpChannel;
}

void PwmSounds::setSounds(Music* tmphsound, uint8_t dataSize){
	hsound = tmphsound;
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
		if( count < ( sizeof(music)/sizeof(Music))/*音の数*/ ){ // もし次の音があれば
			// music update
			__HAL_TIM_SET_AUTORELOAD( &htim, 64000000/*HAL_RCC_GetHCLKFreq()*/ / (htim.Init.Prescaler) / music[count].musicScale ); // counter period変更 → 周波数変更
			__HAL_TIM_SET_COMPARE(&htim, channel, (htim.Init.Period)*0.5); // デューティー比を50%にする
			soundTim = HAL_GetTick() + music[count].musicTime; // soundTim 更新
		}else{
			// music stop
			HAL_TIM_PWM_Stop(&htim, channel);
			musicState = false; // 停止状態にする
		}
		count++; // 次の音に移行
	}
	return musicState; // 今何音目かを返す
}
