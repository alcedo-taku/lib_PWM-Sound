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
	if( musicState == false ){ //music start
		HAL_TIM_PWM_Start(&htim, channel);
		//__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (htim1.Init.Period)*0.5);
		soundTim = HAL_GetTick();//music updateに進めるように、とりあえず代入
		count = 0;
		musicState = true; // 再生中にする
//		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET); // インジゲータLED点灯
	}
}

bool PwmSounds::updateSounds(){
	if( soundTim <= HAL_GetTick() && musicState == true ){
		if( count < ( sizeof(music)/sizeof(Music))/*いくつ音があるか*/ ){ //music update
			__HAL_TIM_SET_AUTORELOAD( &htim, 64000000/*HAL_RCC_GetHCLKFreq()*/ / (htim.Init.Prescaler) / music[count].musicScale ); //counter period変更 → 周波数変更
			__HAL_TIM_SET_COMPARE(&htim, channel, (htim.Init.Period)*0.5); //count変更？ → デューティー比変更
			soundTim = HAL_GetTick() + music[count].musicTime; //soundTim 更新
		}else{ //music stop
			HAL_TIM_PWM_Stop(&htim, channel);
			musicState = false; // 再生停止状態にする
//			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
		}
		count++;
	}
	return musicState;
}
