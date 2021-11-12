# PWM Sound

## サンプルプログラム

```C++
Music music[3] = { {SoundScale::hC, 200}, {SoundScale::hE, 200}, {SoundScale::hG, 250} }; // 音階と各音の時間を指定
PwmSounds pwm_sounds(&htim1, TIM_CHANNEL_1);

void init(void){
	pwm_sounds.set_sounds(music,3);
}

void loop(void){
	pwm_sounds.start_sounds();
	pwm_sounds.update_sounds();
}
```
