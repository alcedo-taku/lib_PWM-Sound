#include "PwmSounds.hpp"

PwmSounds::PwmSounds(TIM_HandleTypeDef* sound_htim, uint32_t sound_channel):
	sound_htim(sound_htim),
	sound_channel(sound_channel)
{
}

PwmSounds::PwmSounds(TIM_HandleTypeDef& sound_htim, uint32_t sound_channel):
	PwmSounds(&sound_htim, sound_channel)
{
}

void PwmSounds::set_sounds(Music* sounds, uint8_t number_of_sound){
	this->sounds = sounds;
	this->number_of_sounds = number_of_sound;
}

void PwmSounds::start_sounds(){
	if( is_playing == false ){ // もし停止状態だったら
		// music start
		__HAL_TIM_SET_AUTORELOAD( sound_htim, 99); // counter period を変更
		HAL_TIM_PWM_Start( sound_htim, sound_channel );
		end_time_of_sound = 0;
		playing_sound = 0;
		is_playing = true; // 再生中にする
	}
}

bool PwmSounds::update_sounds(){
	if( is_playing == true && end_time_of_sound <= HAL_GetTick() ){ // もし再生中で、かつその音を出す時間を過ぎていたら
		if( playing_sound < number_of_sounds/*音の数*/ ){ // もし次の音があれば
			// music update
			if(sounds[playing_sound].music_scale == 0){ // もし周波数が0(休符)だったら
				__HAL_TIM_SET_COMPARE(sound_htim, sound_channel, 0); // デューティー比を0%にする
			}else{
				__HAL_TIM_SET_PRESCALER( sound_htim, 64000000/*HAL_RCC_GetHCLKFreq()*/ / (sound_htim->Init.AutoReloadPreload) / sounds[playing_sound].music_scale ); // counter period変更 → 周波数変更
				__HAL_TIM_SET_COMPARE( sound_htim, sound_channel, 50 ); // デューティー比を50%にする
			}
			end_time_of_sound = HAL_GetTick() + sounds[playing_sound].music_time; // soundTim 更新
		}else{
			// music stop
			HAL_TIM_PWM_Stop(sound_htim, sound_channel);
			is_playing = false; // 停止状態にする
		}
		playing_sound++; // 次の音に移行
	}
	return is_playing; // 今何音目かを返す
}
