void putInMemory (int segment, int address, char character);
void writeWord(char * helloWorld,int row, int column,int color);
void writeLetter(char c, int position,int color);
void clearScreen();
void clearRow(int row);
int calculatePosition(int row,int column);
int time = 0;
int main(){
  int i = 0;
   clearScreen();
  
   writeWord("SISTEMAS OPERATIVOS I",1,28,0x7);

   writeWord("UNIVERSIDAD TECNOLOGICA CENTROAMERICANA",2,16,0x7);
   writeWord("(UNITEC)",2,55,0x7);

   writeWord("21341140-CARLOS VARELA",3,28,0x7);

   writeWord("MI PRIMER SISTEMA OPERATIVO",4,26,0x8);
   writeWord("------------------------------",5,0,0x7);
   writeWord("------------------------------",5,30,0x7);
   writeWord("--------------------",5,60,0x7);

   writeWord("********************",6,0,0x6);
   writeWord("************",6,20,0x6);
   writeWord("HELLO WORLD",6,35,0x6);
   writeWord("************",6,49,0x6);
   writeWord("*******************",6,61,0x6);

   writeWord("********************",7,0,0x4);
   writeWord("************",7,20,0x4);
   writeWord("HOLA MUNDO",7,35,0x4);
   writeWord("************",7,49,0x4);
   writeWord("*******************",7,61,0x4);
   //----------------ALEMAN-----------------------
   writeWord("********************",8,0,0x5);
   writeWord("************",8,20,0x5);
   writeWord("HALLO WELT",8,35,0x5);
   writeWord("************",8,49,0x5);
   writeWord("*******************",8,61,0x5);
   //----------------CORSO-----------------------
   writeWord("********************",9,0,0x3);
   writeWord("************",9,20,0x3);
   writeWord("CIAO MONDU",9,35,0x3);
   writeWord("************",9,49,0x3);
   writeWord("*******************",9,61,0X3);
   //------------------CROATA-------------------
   writeWord("********************",10,0,0x8);
   writeWord("************",10,20,0x8);
   writeWord("BOK SVIJETE",10,35,0x8);
   writeWord("************",10,49,0x8);
   writeWord("*******************",10,61,0X8);
    //------------------MALTES-------------------
   writeWord("********************",11,0,0xA);
   writeWord("************",11,20,0xA);
   writeWord("HELLO DINJA",11,35,0xA);
   writeWord("************",11,49,0xA);
   writeWord("*******************",11,61,0XA);
   //------------------EUSKERA-----------------
   writeWord("********************",12,0,0xB);
   writeWord("************",12,20,0xB);
   writeWord("KAIXO MUNDUA",12,35,0xB);
   writeWord("************",12,49,0xB);
   writeWord("*******************",12,61,0XB);

  
      //------------------INDONESIO-----------------
   writeWord("********************",13,0,0x9);
   writeWord("************",13,20,0x9);
   writeWord("HALO DUNIA",13,35,0x9);
   writeWord("************",13,49,0x9);
   writeWord("*******************",13,61,0X9);
    //------------------RUMANO-------------------
   writeWord("********************",14,0,0xC);
   writeWord("************",14,20,0xC);
   writeWord("SALUT LUME",14,35,0xC);
   writeWord("************",14,49,0xC);
   writeWord("*******************",14,61,0XC);
   //------------------NEERLANDES-----------------
   writeWord("********************",15,0,0xD);
   writeWord("************",15,20,0xD);
   writeWord("HALLO WERELD",15,35,0xD);
   writeWord("************",15,49,0xD);
   writeWord("*******************",15,61,0XD);
   //--------------------HAWAIANO---------------
   writeWord("********************",16,0,0xE);
   writeWord("************",16,20,0xE);
   writeWord("HOLA WORLD",16,35,0xE);
   writeWord("************",16,49,0xE);
   writeWord("*******************",16,61,0XE);

    //------------------NORUEGO-----------------
   writeWord("********************",17,0,0xF);
   writeWord("************",17,20,0xF);
   writeWord("HEI VERDEN",17,35,0xF);
   writeWord("************",17,49,0xF);
   writeWord("*******************",17,61,0XF);
   //------------------LETON-----------------
   writeWord("********************",18,0,0x1);
   writeWord("************",18,20,0x1);
   writeWord("SVEIKA PASAULE",18,34,0x1);
   writeWord(" ************",18,48,0x1);
   writeWord("*******************",18,61,0X1);
   //--------------------LATIN---------------
   writeWord("********************",19,0,0x2);
   writeWord("************",19,20,0x2);
   writeWord("SALVE MUNDI",19,35,0x2);
   writeWord("************",19,49,0x2);
   writeWord("*******************",19,61,0X2);
  
   //------------------NORUEGO-----------------
   writeWord("********************",20,0,0x3);
   writeWord("************",20,20,0x3);
   writeWord("NDEWO UWA",20,35,0x3);
   writeWord("************",20,49,0x3);
   writeWord("*******************",20,61,0X3);
   //------------------SUAJILI-----------------
   writeWord("********************",21,0,0x6);
   writeWord("************",21,20,0x6);
   writeWord("SALAMU, DUNIA",21,34,0x6);
   writeWord(" ************",21,48,0x6);
   writeWord("*******************",21,61,0X6);
   //--------------------LATIN---------------
   writeWord("********************",22,0,0x8);
   writeWord("************",22,20,0x8);
   writeWord("SANNU DUNIYA",22,34,0x8);
   writeWord(" ************",22,48,0x8);
   writeWord("*******************",22,61,0X8);
     //--------------------GALES---------------
   writeWord("********************",23,0,0xA);
   writeWord("************",23,20,0xA);
   writeWord("HELO BYD",23,34,0xA);
   writeWord(" ************",23,48,0xA);
   writeWord("*******************",23,61,0XA);

   writeWord("********************",24,0,0x5);
   writeWord("************",24,20,0x5);
   writeWord("HALLO HEIMUR",24,34,0x5);
   writeWord(" ************",24,48,0x5);
   writeWord("*******************",24,61,0X5);

 //--------------------ESTONIO---------------
   writeWord("********************",25,0,0x2);
   writeWord("************",25,20,0x2);
   writeWord("TERE, MAAILM",25,34,0x2);
   writeWord(" ************",25,48,0x2);
   writeWord("*******************",25,61,0X2);



return 0;
}


int calculatePosition(int row,int column){
  int position = (row-1)*80;
  position = position*2;
  position = position+(column*2);
  return 0x8000+position;
}

void writeLetter(char c, int position, int color){
  putInMemory(0xB000, position, c);  
  putInMemory(0xB000,position+1,color);
}


void clearScreen(){
  int i = 0;
  for(i=0;i<12;i++){
     clearRow(i); 
  }
}

void clearRow(int row){
   int i = 0;
   for(i=0;i<80;i++){
    writeLetter(' ',calculatePosition(row+1,i),0x7);
   }
}

void writeWord(char * helloWorld,int row, int column,int color){
int i = 0;
int col = column;
for(i = 0; i<helloWorld[i]!='\0'; i++) {
    writeLetter(helloWorld[i],calculatePosition(row,col),color);
    col = col+1; 
  }
}