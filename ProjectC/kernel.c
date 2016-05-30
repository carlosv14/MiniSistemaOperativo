int interrupt (int number, int AX, int BX, int CX, int DX); 
void readString(char *);
void printString(char *);
void printChar(char c);
int isDeleteAsci(int);
int isEnterAsci(int);
void println();
char fromAsciToChar(int);
void readSector(char *,int);
void handleInterrupt21(int , int , int , int);
void makeInterrupt21();
int readFile(char * ,char*);
void deleteChar(int);
char readChar();
int fileNameCompare(char*,char*);
void executeProgram(char* , int );
void putInMemory (int , int , char );
void launchProgram(int segment);
void terminate();
char shell[6];

#define READSECTOR 0X13
#define PRINTSTRING 0x10
#define READSTRING 0x16
#define ENTER 0xd		
#define DELETE 0x8
#define INTERRUPT21 0x21
#define DIRECTORY_SECTOR 2



int main(){
	char line[80];
	char buffer[13312];

	shell[0] ='s';
	shell[1] ='h';
	shell[2] ='e';
	shell[3] ='l';
	shell[4] ='l';
	shell[5] ='\0';
	/*printString("Enter a line:");
	readString(line);
	printString(line);*/
	//readSector(buffer,30);
	//printString(buffer);
	/*makeInterrupt21();
	interrupt(0x21,0,0,0,0);*/
	//char line[80];
	//makeInterrupt21();
	//interrupt(0x21,2,buffer,30,0);
	//interrupt(0x21,0,buffer,0,0);
	makeInterrupt21();
	interrupt(0x21, 4, shell, 0x2000, 0);
	while(1){}
return 0;
}

void println(){
	printChar('\r');
	printChar('\n');
}



void printString(char * word){
	while(*word!=0){
		printChar(*word);
		*word++;
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

void deleteChar(int asciC){
	printChar(fromAsciToChar(asciC));
	printChar(' ');
	printChar(fromAsciToChar(asciC));
}

void readString(char * line){
	int lastIndex = 0;
	int asciC=-4;
	while(!isEnterAsci(asciC)){
		asciC=readChar();
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

void executeProgram(char* name, int segment){
	char file[13312];
    readFile(name,file);
    
   	transferToSeg(segment,  file, 13312);

    launchProgram(segment);
	interrupt(0x21, 5, 0, 0, 0);
}


int fileNameCompare(char * fileName, char* directory_sector, int directory){
	int i;
	int file_entry_size = 32;
	int found = 0;
	for(i = 0;i<6;i++){
        if((fileName[i]==0 || fileName[i] == '\n') && directory_sector[file_entry_size*directory+i]==0) 
        	return 1;
        if(!found && fileName[i]!=directory_sector[file_entry_size*directory+i]) 
        	return 0;
	}
}

int readFile (char *file_name, char *buffer){
    int i,j,found,pos,iterator=0,directory,file_name_size=6;
    char directory_sector[512];
    int file_entry_size = 32;
    readSector(directory_sector, DIRECTORY_SECTOR);
    for(i=0; i<16; i++) {
    	found = 0;
        if(directory_sector[i*file_entry_size]!=0) 
        	found = fileNameCompare(file_name,directory_sector,i);
        if(found){
        	directory = i;
         	break;
        }
    } 
    if(found){
    pos = file_entry_size*directory+file_name_size;
    while(directory_sector[pos]!=0) {
        readSector(buffer, directory_sector[pos]);
        buffer+=512;
        iterator++;
        pos = file_entry_size*directory+file_name_size+iterator;
    }
    buffer[iterator*512] = 0x00;
    return iterator;

	}
    return 0;
}


void terminate(){
	shell[0] ='s';
	shell[1] ='h';
	shell[2] ='e';
	shell[3] ='l';
	shell[4] ='l';
	shell[5] ='\0';
	 interrupt(0x21, 4, shell, 0x2000, 0);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	if(ax == 0)
		printString((char *)bx);
	else if(ax==1)
		readString((char *)bx);
	else if(ax==2)
		readSector((char *)bx,cx);
	else if(ax==3)
		readFile((char *)bx, (char *)cx);
	else if(ax==4)
		executeProgram((char *)bx, cx);
	else if(ax==5)
		terminate();
	else if(ax == 10)
		cls();
	else 
		printString("Error, option not valid");
}