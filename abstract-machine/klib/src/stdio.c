#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
#include <stdint.h>
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void int2strD(char*str,int32_t num){                                                                                                                         
        if(num==0){strcpy(str,"0");return ;}                                                                                                                            
       	if(num==-2147483648){strcpy(str,"-2147483648");return ;}
	// char*tmp=(char*)malloc(50);//字符stack                                                                                                                
        char tmp[50]={'\0'};                                                                                                                                     
        char cat[2]={'\0'};  
	bool negative=false;                                                                                                                                 
        while(num!=0){
	if(num<0)
	{num=-1*num;                                                                      
	negative=true;	}
	int32_t mod=num%10;                                                                                                                                  
	cat[0]=(char)(mod+48);                                                                                                                                
	strcat(tmp,cat);                                                                                                                                      
        num-=mod;                                                                                                                                             
        num/=10;                                                                                                                                              
        }       
	if(negative)
	strcat(tmp,"-");     
        //双指针字符串反转                                                                                                                                    
        int len=strlen(tmp);                                                                                                                                  
        for(int i=0,j=len-1;i<j;i++,j--)                                                                                                                      
        {                                                                                                                                                     
                char x=tmp[i];                                                                                                                                
                tmp[i]=tmp[j];                                                                                                                                
                tmp[j]=x;                                                                                                                                     
        }                                                                                                                                                     
        strcpy(str,tmp);                                                                                                                                      
        //free(tmp);                                                                                                                                           
}    

/*int printf(const char *fmt, ...) {
  panic("Not implemented");
}*/

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
 // panic("Not implemented");
	strcpy(out,"");
	va_list ap;
	int d;
	char*s;
	int count=0;
	va_start(ap,fmt);
	while(fmt[count]){
		char tmpcat[20]={'\0'}; 
		if(fmt[count]=='%'){
		count++;
		switch(fmt[count]){
		case 'd':
			d=va_arg(ap,int);
		int2strD(tmpcat,d);		
			break;
		case 's':
			s=va_arg(ap,char*);
			strcpy(tmpcat,s);
			break;
		default:
			return 0;
		}
	}else
	tmpcat[0]=fmt[count];
	strcat(out,tmpcat);
	count++; 
	}
	return 1;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
