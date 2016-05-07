int interrupt (int number, int AX, int BX, int CX, int DX); 
void readString(char line[80]);
void printString(char *);
void printChar(char c);
int isDeleteAsci(int);
int isEnterAsci(int);
void println();
char fromAsciToChar(int);
int mod(int , int );
int div(int , int );
void readSector(char,int);
void handleInterrupt21(int , int , int , int);
void makeInterrupt21();

#define READSECTOR 0X13
#define PRINTSTRING 0x10
#define READSTRING 0x16
#define ENTER 0xd		
#define DELETE 0x8
#define INTERRUPT21 0x21


int main(){
	char line[80];
	//char buffer[512];
	/*printString("Enter a line:");
	readString(line);
	printString(line);*/
	/*readSector(buffer,30);
	printString(buffer);*/
	/*makeInterrupt21();
	interrupt(0x21,0,0,0,0);*/
	//char line[80];
	makeInterrupt21();
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
	while(1){}
return 0;
}

void println(){
	printChar('\r');
	printChar('\n');
}

void printChar(char c){
	char al = c;
	char ah = 0xe;
	int ax = ah * 256 + al;
	interrupt(PRINTSTRING, ax, 0, 0, 0);
}
void printString(char * word){
	int i;
	for(i=0;word[i]!='\0';i++){
		printChar(word[i]);
	}
}
int isDeleteAsci(int asci){
	if(DELETE==asci)
		return 1;
	return 0;
}
int isEnterAsci(int asci){
	if(ENTER==asci)
		return 1;
	return 0;
}

char fromAsciToChar(int asci){
	return(char)asci;
}

void readString(char line[80]){
	int lastIndex = 0;
	int asciC=-4;
	while(!isEnterAsci(asciC) && lastIndex<78){
		asciC=interrupt(READSTRING,0,0,0,0);
		if(!isDeleteAsci(asciC)){
			line[lastIndex]=fromAsciToChar(asciC);
			printChar(line[lastIndex]);
			lastIndex++;
		}else if( isDeleteAsci(asciC) && lastIndex>0){
			printChar(fromAsciToChar(asciC));
			printChar(' ');
			printChar(fromAsciToChar(asciC));
			lastIndex-=1;
		}
	}
	line[lastIndex] = fromAsciToChar(0xa);
	line[lastIndex+1] = fromAsciToChar(0x0);
	println();
}

int mod(int a,int b){
	while (a>=b){
			a = a-b;
	}
	return a;
}	

int div(int a,int b){
	int q= 1;
	while((q*b<=a)){
		q=q+1;	
	}
	return q;
}

void readSector(char * buffer,int sector){
	int ah =2;
	int al = 1;
	int ch = div(sector,36); 
	int cl = (mod(sector,18)+1);
	int dh =(mod(div(sector,18),2));
	int dl = 0;
	int ax = ah*256+al;
	int cx = ch*256+cl;
	int dx= dh*256+dl;
	interrupt(READSECTOR,ax,buffer,cx,dx);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	if(ax == 0)
		printString((char *)bx);
	else if(ax==1)
		readString((char *)bx);
	else if(ax==2)
		readSector((char *)bx,cx);
	else 
		printString("Error, option not valid");
}