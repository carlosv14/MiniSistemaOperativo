void executeProgram(char* programName);
void executeWait(char* programName);
int fileNameCompare(char*,char*);
int div(int a, int b);
int mod(int a, int b);
void writeFile(char* name, char* buffer, int  numberOfSectors);
void printString(char* string);
void readString(char * buffer);
void killProcess(int id);
void backSpace();
void listFiles();
void deleteFile(char* name);
void handleInterrupt21(int ax, int bx, int cx, int dx);
void println();
void printSize(char * dirname);
int findFreeDirectory(char * directory_sector);
void cleanArray(char * arr,int size);
int activeProcess(int);
int secondCont;
char shell[5];

char numbers [10];

struct PCB {
unsigned int status;
unsigned int sp;
unsigned int segment;
struct PCB *waiter;
}ProcessTable[8];



struct regs {
unsigned int es;
unsigned int ds;
unsigned int ax;
unsigned int bp;
unsigned int di;
unsigned int si;
unsigned int dx;
unsigned int cx;
unsigned int bx;
unsigned int ip;
unsigned int cs;
unsigned int flags;
};


#define READSECTOR 0X13
#define PRINTSTRING 0x10
#define READSTRING 0x16
#define ENTER 0xd		
#define DELETE 0x8
#define INTERRUPT21 0x21
#define DIRECTORY_SECTOR 2
#define MAP_SECTOR 1

struct PCB *currentProcess;

int main(){
   int i=0;
   secondCont=0;
   for(i=0;i<8;i++){
       ProcessTable[i].status=4;
       ProcessTable[i].sp=0xff00;
       ProcessTable[i].segment=(i+2)*0x1000;
       ProcessTable[i].waiter=0;
   }
   currentProcess=0;
   makeInterrupt21(); 
   shell[0] = 's';
   shell[1] = 'h';
   shell[2] = 'e';
   shell[3] = 'l';
   shell[4] = 'l';
   shell[5] = '\0';
   executeProgram(shell);
   irqInstallHandler();
   setTimerPhase(100); 
   while(1);
   return 0;
}


int div(int a, int b){
  int quo = 0;
  while((quo + 1)*b <=a){
    quo++;
  }
  return quo;
}

int mod(int a, int b){
  while(a >= b)
      a = a - b;
  return a;
}

void cleanArray(char * arr,int size){
	int i;
	for(i=0;i<size;i++){
		arr[i] = 0;
	}
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

void readString(char* buffer){ 
  char currentChar; 
  int i=0;
  buffer[0]=0x00;
  while(currentChar!=0xD && i<79 ){
    currentChar=readChar(); 
    if(currentChar==0x8 && i>0){
      backSpace(currentChar);
      i--;
    }else if(currentChar!=0x8 && currentChar!=0xD){ 
      buffer[i]=currentChar;      
      printChar(buffer[i]);
      i++;
    }
  }
	buffer[i]=0x0;
  println();
}

void backSpace(char chr){
  printChar(chr);
  printChar(0x0);
  printChar(chr);
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
	 buffer[0] = 0x00;
    return -1;
}

int activeProcess(int procTable[]){
	int i,counter=0;
	setKernelDataSegment();
	for(i=0;i<8;i++){
		if(ProcessTable[i].status!=4)
			counter++;
	}
	moveToSegment(currentProcess->segment,procTable,&ProcessTable,100);
	restoreDataSegment();

	return counter;
}

void deleteFile(char * file_name){
	 int i,j,found,pos,iterator=0,directory,file_name_size=6;
    char directory_sector[512];
    char map_sector[512];
    int file_entry_size = 32;
    readSector(directory_sector, DIRECTORY_SECTOR);
    readSector(map_sector, MAP_SECTOR);
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
	    directory_sector[file_entry_size*directory] = 0x00;
	    pos = file_entry_size*directory+file_name_size;
	    while(directory_sector[pos]!=0) {
	        map_sector[(int)directory_sector[pos]] = 0x00;	
	        iterator++;
	        pos = file_entry_size*directory+file_name_size+iterator;
	    }
	    writeSector(directory_sector,DIRECTORY_SECTOR);
	    writeSector(map_sector,MAP_SECTOR);
	    return;
	}
}


void copyFile (char *fileName, char *secondFile){
  char buffer[13312];
  int secCount;
  secCount = readFile(fileName, buffer);
  writeFile(secondFile, buffer, secCount);
}

int findFreeDirectory(char * directory_sector){
	int i;
	int file_entry_size = 32;
	for(i= 0; i<16;i++){
		if(directory_sector[i*file_entry_size]==0)
			return i*file_entry_size;
	}
	return -1;
}


void writeFile(char * name , char * buffer, int numberOfSectors){
 	int file_name_size=6,i;
    char directory_sector[512];
    int k;
    char sec[512];
    char map_sector[512];
    int freeDirectory = -1;
    int file_entry_size = 32;
    int lastFileDirectory = 0;
    int countOfDispSectors = 0;

    readSector(directory_sector, DIRECTORY_SECTOR);
    readSector(map_sector, MAP_SECTOR);
     freeDirectory = findFreeDirectory(directory_sector);
    
     for(i= 0 ; i<512 ; i++){
     	if(map_sector[i]==0x00){
     		countOfDispSectors++;
     		if(countOfDispSectors==numberOfSectors)
     			break;
     	}
     }
     if(countOfDispSectors!=numberOfSectors)
     	return;
    i=0;
    while(*name!=0){
    	directory_sector[freeDirectory+i] = *name;
    	*name++;
    	i++;
    }
    while(i<6){
        directory_sector[freeDirectory+i] = 0x00;
        i++;
    }

  
    for(i= 0 ; i<512 ; i++){
    	if(map_sector[i]==0x00){
    		directory_sector[freeDirectory+6+lastFileDirectory] = i;
    		map_sector[i] = 0xFF;
    		for(k = 0; k < 512; k++){
    			sec[k] = buffer[lastFileDirectory*512+k];
    		}
    		writeSector(sec,i);
    		lastFileDirectory++;
    	}
    	if(lastFileDirectory==numberOfSectors)
    		break;
    }
    for(i = 0; i<26-numberOfSectors;i++){
    	directory_sector[freeDirectory+ 6 + numberOfSectors + i] = 0x00;
    }

		writeSector(directory_sector,DIRECTORY_SECTOR);
	    writeSector(map_sector,MAP_SECTOR);
	    return;
}



void printSize(char * dirname){
	int size = 0, j = 0,i=0;
	char * buffer;
	char * sizeString;

	size = readFile(dirname,buffer);
     if(div(size,10) == 0){
     	sizeString[j] = '0';
     	j++;
     }else{
     	sizeString[j] = numbers[div(size,10)];
     	j++;
     }
      sizeString[j] = numbers[mod(size, 10)];
    
      printString(sizeString);
}

void listFiles(){
int i,j;
char directory_sector[512];
char filename[7];
int file_entry_size = 32;
char * headers;
numbers[0] ='0';
numbers[1] ='1';
numbers[2] ='2';
numbers[3] ='3';
numbers[4] ='4';
numbers[5] ='5';
numbers[6] ='6';
numbers[7] ='7';
numbers[8] ='8';
numbers[9] ='9';
  readSector(directory_sector, DIRECTORY_SECTOR);
	for(i=0; i<16;i++){
 		if(directory_sector[i*file_entry_size]!=0){
 			cleanArray(filename,7);
			for(j = 0;j<6;j++){
				filename[j] = directory_sector[i*file_entry_size+j];
			}
			filename[7] = '\0';
			printString(filename);
			printChar(' ');
			printSize(filename);
			println();
 		}
	}

}



void terminate(){
    setKernelDataSegment();
    currentProcess->status=4;
    currentProcess->sp=0xff00;
	if(currentProcess->waiter!=0)
		currentProcess->waiter->status=1;
	restoreDataSegment();
    #asm
	sti
	#endasm
    while(1);
}


void initializeProgram(int segment){
    struct regs registers;
	registers.ds = segment;
	registers.es = segment;
	registers.ax = 0;
	registers.bp = 0;
	registers.di = 0;
	registers.si = 0;
	registers.dx = 0;
	registers.cx = 0;
	registers.bx = 0;
	registers.ip = 0;
    registers.cs = segment;
	registers.flags = 0x0200;
	moveToSegment(segment,0xff00,&registers,24);
}

void executeProgram(char* programName){
    char buffer[13312];
    int i=0,segment=0;
    setKernelDataSegment();
    for(i=0;i<8;i++){
        if(ProcessTable[i].status==4){
            ProcessTable[i].status=1;         
            segment=ProcessTable[i].segment;
            break;
        }
    }
    restoreDataSegment();
    if(readFile(programName, buffer)!=-1){ 
        moveToSegment(segment,0, buffer, 13312); 
        initializeProgram(segment);
    }
}

void executeWait(char* programName){
    char buffer[13312];
    int i=0,segment=0;
    setKernelDataSegment();
    for(i=0;i<8;i++){
        if(ProcessTable[i].status==4){
            ProcessTable[i].status=1;
            ProcessTable[i].waiter=currentProcess;
            segment=ProcessTable[i].segment;
            currentProcess->status=2;
            break;
        } 
    }
    restoreDataSegment();
    if(readFile(programName, buffer)!=-1){ 
        moveToSegment(segment,0, buffer, 13312); 
        initializeProgram(segment);
    }

}

void killProcess(int id){
  setKernelDataSegment();
  ProcessTable[id-1].status = 4;
  ProcessTable[id-1].sp = 0xff00;
  if(ProcessTable[id-1].waiter!=0)
		ProcessTable[id-1].waiter->status=1;
  restoreDataSegment();
}


void scheduleProcess(int currentProcessSP){
	int tmpCurrent=-1;
	setKernelDataSegment();
	if(currentProcess!=0){
		if(currentProcess->status!=4){
			currentProcess->sp = currentProcessSP;
			if(currentProcess->status==3)
				currentProcess->status = 1;
			tmpCurrent = div(currentProcess->segment,0x1000)-2;
		}
	}
    while(1){
        tmpCurrent++;
        if(tmpCurrent==7)
            tmpCurrent=0;          
        if(ProcessTable[tmpCurrent].status==1){
			currentProcess = &(ProcessTable[tmpCurrent]);
			currentProcess->status=3;
            break;
		}
    }
	restoreDataSegment();
}


void handleInterrupt21(int ax, int bx, int cx, int dx){
  switch(ax){
      case 0:
         printString((char*)bx);
         break;
      case 1:
         readString((char*)bx);
         break;
      case 2:
         readSector((char*)bx,cx);
         break;
      case 3:
         readFile((char*)bx,(char*)cx);
         break;
      case 4:
         executeProgram((char*)bx);
         break;
      case 5:
         terminate();
         break;
      case 6:
         writeSector((char*)bx,cx);
         break;
      case 7:
         deleteFile((char*)bx);
         break;
      case 8:
         writeFile((char*)bx,(char*)cx,dx);
         break;
      case 9:
         copyFile((char*)bx,(char *)cx);
         break;
      case 10:
         clearScreen();
         break;
      case 11:
          listFiles();
         break;
      case 12:
         killProcess(bx);
         break;
      case 13:
         executeWait((char*)bx);
         break;
     case 14:
         ax = activeProcess(bx);
         break;
     case 15:
     	printChar((char)bx);
     	break;
		
	default:
		printString("Error, option not valid");
		break;  
	}
}
