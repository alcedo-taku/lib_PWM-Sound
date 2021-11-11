#include "PwmSounds.hpp"
#include "gpio.h"

PwmSounds::PwmSounds(TIM_HandleTypeDef* sound_htim, uint32_t sound_channel):
	sound_htim(sound_htim),
	sound_channel(sound_channel)
{
}

PwmSounds::PwmSounds(TIM_HandleTypeDef& sound_htim, uint32_t sound_channel):
	PwmSounds(&sound_htim, sound_channel)
{
}

void PwmSounds::setSounds(Music* sounds, uint8_t number_of_sound){
	this->sounds = sounds;
	this->number_of_sound = number_of_sound;
}

void PwmSounds::startSounds(){
	if( musicState == false ){ // もし停止状態だったら
		// music start
		HAL_TIM_PWM_Start(sound_htim, sound_channel);
		soundTim = HAL_GetTick(); // music updateに進めるように、とりあえず代入
		count = 0;
		musicState = true; // 再生中にする
	}
}

bool PwmSounds::updateSounds(){
	if( musicState == true && soundTim <= HAL_GetTick() ){ // もし再生中で、かつその音を出す時間を過ぎていたら
		if( count < number_of_sound/*音の数*/ ){ // もし次の音があれば
			// music update
			__HAL_TIM_SET_AUTORELOAD( sound_htim, 64000000/*HAL_RCC_GetHCLKFreq()*/ / (sound_htim->Init.Prescaler) / sounds[count].musicScale ); // counter period変更 → 周波数変更
			__HAL_TIM_SET_COMPARE(sound_htim, sound_channel, (sound_htim->Init.Period)*0.5); // デューティー比を50%にする
			soundTim = HAL_GetTick() + sounds[count].musicTime; // soundTim 更新
		}else{
			// music stop
			HAL_TIM_PWM_Stop(sound_htim, sound_channel);
			musicState = false; // 停止状態にする
		}
		count++; // 次の音に移行
	}
	return musicState; // 今何音目かを返す
}
