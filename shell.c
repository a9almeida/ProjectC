//ProjectC: Aliya Goncalves Almeida & Maya Elysse

void main()
{

char buffer[13312];
char line[80];
char fileName[6];
char command[4];
int i;
int sectorsread;

	while(1){

		syscall(0, "SHELL>");
		syscall(1, line);
		syscall(3,buffer,30);

		if(line[0] == 't' && line[1] == 'y' && line[2] == 'p' && line[3] == 'e'){
			syscall(3,line+5,buffer);
			if(buffer[0] =='\0'){
				syscall(0, "File not found");

			}
			else{
				syscall(0,buffer);
				syscall(0, "\r\n");
			}

		}
		else if(line[0] == 'e' && line[1] == 'x' && line[2] == 'e' && line[3] == 'c'){

			for(i = 5; i < 11 && line[i] != ' ' && line[i] != '\0'; i++){
				fileName[i - 5] = line[i];
			}
			fileName[i - 5] = '\0';
			//syscall(0, fileName);
			syscall(4, fileName);
			syscall(0, "Error");
		}
		else{
			syscall(4,"Bad command");
		}

	}

}
