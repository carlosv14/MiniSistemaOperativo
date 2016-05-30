void processCommand(char *);
int instructionCompare(char*,char*,int);

int main(){
	
	while(1){
		char buffer[80];
		buffer[0] = 0x00;
		syscall_printString("Terminal:>");
		syscall_readString(buffer);
		processCommand(buffer);
	}
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

void processCommand(char * instruction){
	int i;
	int firstArgumentPos=0;
	char command[30];

	for(i=0; i<80;i++){
		if(instruction[i]!=' ' && i<30)
			command[i]= instruction[i];
		else{	
		command[i] = '\0';
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
	}else if(instructionCompare(command,"execute",30)){
	 syscall_executeProgram(instruction+firstArgumentPos+1, 0x2000);
	}else if(instructionCompare(command,"cls",30)){
	  syscall_clearScreen();
	}else{
		 syscall_printString("Bad Command\r\n");
	}


}
