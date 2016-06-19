void processCommand(char *);
int instructionCompare(char*,char*,int);
void writeFileInstruction (char *);
void copyFileInstruction (char *);
void printInt(int number);


struct PCB {
unsigned int status;
unsigned int sp;
unsigned int segment;
struct PCB *waiter;
}ProcessTable[8];

int main(){
	
	while(1){
		char buffer[80];
		buffer[0] = 0x00;
		syscall_printString("Terminal:>");
		syscall_readString(buffer);
		processCommand(buffer);
	}
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
int instructionCompare(char * a, char *b, int sizeofarray){
	int i;
	for(i=0;i<sizeofarray;i++){
		if(a[i]!=0 && b[i]!=0){
			if(a[i]!=b[i] )
				return 0;
		}else {
		if(a[i]==0 && b[i]==0)
			return 1;
		return 0;
		}
	}

}
void printInt(int number){
	int i = 0,j=0;	
	char buffer[10];	
    for(j=0;j<10;j++){
        buffer[j]=0x0;
    }
	while(number!=0){
		buffer[i] = (char)mod(number,10)+48;
		number=div(number,10);
		i++;	
	}	
	for(i = 9; i != -1; i--)
		if(buffer[i]!=0)
			syscall_printChar(buffer[i]);
	 syscall_printString("\r\n");

}

void writeFileInstruction (char * args){
 	char * firstParam;
 	char * secondParam;
 	int i;
 	for(i= 0; i<50;i++){
 		if(*args!=' '){
 			firstParam[i] = *args;
 		}
 		else if(*args==' '){
 			firstParam[i]= 0;
 			break;
 		}
 		*args++;	
 	}
 	syscall_writeFile(firstParam,args+1);
 }

 void copyFileInstruction (char * args){
 	char * firstParam;
 	char * secondParam;
 	int i;
 	for(i= 0; i<50;i++){
 		if(*args!=' '){
 			firstParam[i] = *args;
 		}
 		else if(*args==' '){
 			firstParam[i]= 0;
 			break;
 		}
 		*args++;	
 	}
 	syscall_copyFile(firstParam,args+1);
 }

void processCommand(char * instruction){
	int i;
	int firstArgumentPos=0;
	char command[30];

	for(i=0; i<80;i++){
		if(instruction[i]!=' ' && i<30)
			command[i]= instruction[i];
		else{	
		command[i] = 0;
		firstArgumentPos = i;
		break;
		}
	}
	
	if(instructionCompare(command,"type",30)){
	 char buffer[13312];
	 int i = 0;
	 for(i= 0 ;i< 13312;i++){
	 	buffer[i] = 0;
	 } 
 	 syscall_readFile(instruction+firstArgumentPos+1, buffer);
     syscall_printString(buffer);
     syscall_printString("\r\n");
	}else if(instructionCompare(command,"execute",30)){
	 syscall_executeProgram(instruction+firstArgumentPos+1, 0x2000);
	}else if(instructionCompare(command,"cls",30)){
	  syscall_clearScreen();
	}else if(instructionCompare(command,"ls",30)){
	   syscall_listFiles();
	 }else if(instructionCompare(command,"create",30)){
	 	char completeFileContent[13312];
	 	char line[512];
	 	int i=0, cantSectors = 1,j=0, currpos=0,empty=0;
	 	for(empty=0; empty<13312;empty++){
	 		completeFileContent[empty] = 0x0;
	 	}
	 	for(empty=0;empty<512;empty++){
	 		line[0]=0x0;
	 	}
	    while(1){
	    	syscall_readString(line);
	    	if(i>511){
	    		cantSectors++;
	    		i=0;
	    	}
	    	if(line[0] == 0x0)
	    		break;
	    	for(j = 0; j<512; j++){
	    		completeFileContent[currpos] = line[j];
	    		currpos++;
	    		i++;
	    		if(line[j] == 0x0){
	    			completeFileContent[currpos-1] = '\r';
	    			completeFileContent[currpos] = '\n'; 
	    			currpos++;
	    			i++;
	    			break;
	    		}
	    	}
	    	
	    	
	    }
	   syscall_writeFile(instruction+firstArgumentPos+1,completeFileContent,cantSectors);
	 }else if(instructionCompare(command,"del",30)){
	   syscall_deleteFile(instruction+firstArgumentPos+1);
	 }else if(instructionCompare(command,"copy",30)){
	 	copyFileInstruction(instruction+firstArgumentPos+1);
	 }else if(instructionCompare(command,"kill",30)){
	 	syscall_kill(*(instruction+firstArgumentPos+1)-'0');
	 }else if(instructionCompare(command,"execw",30)){
	 	syscall_execw(instruction+firstArgumentPos+1);
	 }else if(instructionCompare(command,"ps",30)){
	 	int active,i;
	 	active = syscall_ps(ProcessTable);
	 	printInt(active);
	 	for(i=0;i<8;i++){
	 		if(ProcessTable[i].status!=4){
	 		syscall_printString("Id: ");
	 		printInt(i+1);
	 		syscall_printString("status: ");
	 		printInt(ProcessTable[i].status);
	 		syscall_printString("segment: ");
	 		printInt(ProcessTable[i].segment);
	 	}

	 	}
	}else{
		 syscall_printString("Bad Command\r\n");
	}


}
