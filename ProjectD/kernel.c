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
void listFiles();
void cleanArray(char*,int);
void deleteFile(char *);
void writeFile(char *, char *, int);
int findFreeDirectory(char *);
void copyFile(char *, char *);
int mod(int , int );
int div(int , int );
void printSize(char *);
char numbers [10];
char shell[6];

#define READSECTOR 0X13
#define PRINTSTRING 0x10
#define READSTRING 0x16
#define ENTER 0xd		
#define DELETE 0x8
#define INTERRUPT21 0x21
#define DIRECTORY_SECTOR 2
#define MAP_SECTOR 1



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
		if(!isDeleteAsci(asciC) && !isEnterAsci(asciC)){
			line[lastIndex]=fromAsciToChar(asciC);
			printChar(line[lastIndex]);
			lastIndex++;
		}else if( isDeleteAsci(asciC) && lastIndex>0){
			printChar(fromAsciToChar(asciC));
			printChar(0x0);
			printChar(fromAsciToChar(asciC));
			lastIndex-=1;
		}
	}
	line[lastIndex] = fromAsciToChar(0x0);
	println();
}

void executeProgram(char* name, int segment){
	char file[13312];
	char notFound[10];
    if(readFile(name,file)){
    
   	transferToSeg(segment,  file, 13312);

    launchProgram(segment);
	}else{
    notFound[0] = 'N';
    notFound[1] = 'o';
    notFound[2] = 't';
    notFound[3] = ' ';
    notFound[4] = 'F';
    notFound[5] = 'o';
    notFound[6] = 'u';
    notFound[7] = 'n';
    notFound[8] = 'd';
    notFound[9] = '\0';
    printString(notFound);
    println();
	}
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
void cleanArray(char * arr,int size){
	int i;
	for(i=0;i<size;i++){
		arr[i] = 0;
	}
}

void printSize(char * dirname){
	int size = 0, j = 0,i=0;
	char * buffer;
	char * sizeString;

	for(i=0;i<6;i++){
		if(dirname[i]==0)
			break;
	}
	if(i<6){
		while(i<6){
			sizeString[j] = ' ';
			i++;
			j++;
		}
	}
	size = readFile(dirname,buffer);
     if(div(size,100) == 0){
     	sizeString[j] = '0';
     	j++;
     }else{
     	sizeString[j] = numbers[div(size,100)];
     	j++;
     }
     if(div(size,10) == 0){
     	sizeString[j] = '0';
     	j++;
     }else{
     	sizeString[j] = numbers[div(size,10)];
     	j++;
     }
      sizeString[j] = numbers[mod(size, 10)];
      j++;
      sizeString[j] = ' ';
      j++;
      sizeString[j] = 'S';
      j++;
      sizeString[j] = 'E';
      j++;
      sizeString[j] = 'C';
      j++;
      sizeString[j] = 'T';
      j++;
      sizeString[j] = 'O';
      j++;
      sizeString[j] = 'R';
      j++;
      sizeString[j] = 'S';
      j++;
      sizeString[j] = '\0';
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
    return 0;
}

void copyFile (char *fileName, char *secondFile){
  char buffer[13312];
  int secCount;
  secCount = readFile(fileName, buffer);
  writeFile(secondFile, buffer, secCount);
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

void terminate(){
	shell[0] ='s';
	shell[1] ='h';
	shell[2] ='e';
	shell[3] ='l';
	shell[4] ='l';
	shell[5] ='\0';
	 interrupt(0x21, 4, shell, 0x2000, 0);
}

int mod(int a, int b){
  while(a >= b)
      a = a - b;
  return a;
}

int div(int a, int b){
  int quo = 0;
  while((quo + 1)*b <=a){
    quo++;
  }
  return quo;
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
	else if(ax==6)
		writeSector((char*)bx,cx);
	else if(ax==7)
		deleteFile((char*)bx);
	else if(ax==8)
		writeFile((char*)bx,(char *)cx,dx);
	else if(ax==9)
		copyFile((char*)bx,(char *)cx);
	else if(ax == 10)
		cls();
	else if(ax == 11)
		listFiles();
	else 
		printString("Error, option not valid");
}