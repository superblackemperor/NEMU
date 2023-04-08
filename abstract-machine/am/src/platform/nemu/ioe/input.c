#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
	uint32_t tmp=inl(KBD_ADDR);
		if((tmp&KEYDOWN_MASK)==0)
		kbd->keydown=0;
		else
		{kbd->keydown=1;
		tmp&=~KEYDOWN_MASK;}
		kbd->keycode=tmp;

}
