#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
	printf("%s\n",cmd);
/*	int argc=0;
	char argv[5][30]={0};//max 5 argv
	int i=0,j=0;
	const char*p=cmd;
	while(*p!='\n'&&j<5){
		if(*p!=' '){
		argv[j][i]=*p;
		i++;}
		else
		{
		argc++;
		argv[j][i]='\0';	
		j++;
		i=0;
		}
		p++;
	}
	argc++;
	argv[j][i]='\0';	

	char*argvv[20]={NULL};
	for(int i=0;i<argc;i++){
	argvv[i]=argv[i];
	}
	char*envp[]={NULL};
	int ret=execve(argvv[0],argvv,envp);*/
	}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
