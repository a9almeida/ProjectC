//ProjectC by Aliya Goncalves Almeida & Maya Elysse


void printString(char *);
void printChar(char);
void readString(char *);
void readSector(char *, int);
void handleInterrupt21(int, int, int, int);
void readFile(char *, char *, char *);
void executeProgram(char*);
void terminate();


void main()
{

/*	char line[80];
	makeInterrupt21();
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
*/

//	char buffer[13312]; //this is the maximum size of a file
//	int sectorsRead;
	makeInterrupt21();
/*	interrupt(0x21, 3, "messag", buffer, &sectorsRead); //read the file into buffer
	if(sectorsRead > 0)
		interrupt(0x21, 0, buffer, 0, 0); //print out the file
	else
		interrupt(0x21, 0, "messag not found\r\n", 0, 0); //no sectors read? then print an error

	interrupt(0x21,4,"tstpr1",0,0);
*/
	interrupt(0x21,4,"shell",0,0);

	while(1);
}
void printChar(char chars)
{
		interrupt(0x10, 0xe*256+chars,0,0,0);
}


void printString(char* chars)
{
	char al = '\n';
	while(*chars != '\0'){

		al = *chars;
		interrupt(0x10, 0xe * 256 + al, 0, 0, 0);
		chars++;
	}
}

void readString(char* str)
{
	int i =0;
	int length = 80;
	int enterKey = 0xd;
	int lineFeed = 0xa;
	int endString = 0x0;
	int backSpace = 0x8;

	while(i != length)
	{
		str[i] = interrupt(0x16,0,0,0);
		printChar(str[i]);

		if(str[i] == enterKey){
			str[i+1] = lineFeed;
			printChar(lineFeed);
			str[i+2] = endString;
			break;
		}

		else if(str[i] == backSpace){
			if(i > 0){

				printChar(' ');
				printChar(backSpace);
				i--;
			}
		}

		else{
			i++;
		}
	}
}


void readSector(char *buffer, int sector)
{
	int ax = 2*256+1;
	int cx = 0*256+sector+1;
	int dx = 0*256+0x80;

	interrupt(0x13, ax, buffer, cx, dx);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if(ax == 0){
		printString(bx);
	}

	else if(ax == 1){
		readString(bx);
	}

	else if(ax == 2){
		readSector(bx, cx);
	}

	else if(ax == 3){
		readFile(bx, cx, dx);
	}

	else if(ax == 4){
		executeProgram(bx);
	}

	else if(ax == 5){
		terminate();
	}

	else{
		printString("An error occured when ax is greather than 5");
	}

}

void readFile(char *filename, char *buffer,int *sectorsread)
{

	char dir[512];
	int fileentry;
	int sectors;
	int foundfile = 0;
	int entry;
	int i;
	*sectorsread = 0;


	readSector(dir, 2);

		for(entry=0; entry<512; entry+=32){
			foundfile=1;
			for(fileentry=0; fileentry<6; fileentry++){

			//	printChar(dir[fileentry+entry]);	//
			//	printChar(filename[fileentry]);		//

				if(filename[fileentry]!=dir[fileentry+entry]){
					foundfile=0;
					break;
				}
			}
			if(foundfile==1){
				break;
			}
		}
		if(foundfile==0){
			*sectorsread = 0;
			return;
		}

		for(i=6; i<26; i++){
			sectors = dir[entry+i];
			if(sectors==0)
			break;

			readSector(buffer, sectors);
			buffer += 512;
			*sectorsread += 1;
		}
}

void executeProgram(char* name)
{
	char buffer[13312];
	int i=0;
	int sectorsread;
	int segment = 0x2000;
	readFile(name, buffer, &sectorsread);
	if(sectorsread == 0){
		return;
	}

	while(i<13312){
		putInMemory(segment, i, buffer[i]);
		i++;
	}

	launchProgram(segment);

}

void terminate()
{
	char shellname[6];
	shellname[0] = 's';
	shellname[1] = 'h';
	shellname[2] = 'e';
	shellname[3] = 'l';
	shellname[4] = 'l';
	shellname[5] = '\0';

	executeProgram(shellname);
}
