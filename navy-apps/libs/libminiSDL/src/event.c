#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
	SDL_Event*event=ev;
  	char tmp[20]={'\0'};
	int ud=NDL_PollEvent(tmp,sizeof(tmp));
	if(strcmp(tmp,keyname[0])==0){
	event->key.type=SDL_KEYUP;
	event->key.keysym.sym=0;
	return 0;
	}
	for(int i=1;i<sizeof(keyname);i++)
	if(strcmp(tmp,keyname[i])==0){
	//printf("%s\n",keyname[i]);
	event->key.keysym.sym=i;
	if(ud)
	event->key.type=SDL_KEYDOWN;
	else
	event->key.type=SDL_KEYUP;
	break;}
	return 1;
}

int SDL_WaitEvent(SDL_Event *event) {
	while(SDL_PollEvent(event)==0);
	return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
