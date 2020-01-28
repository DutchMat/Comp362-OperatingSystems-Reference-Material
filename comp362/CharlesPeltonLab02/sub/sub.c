// Charles Pelton, Lab02 Sub
#include "sub.h"
#define BUFFER 1024
int main(int argc, char *argv[]){
	int param = 0;
	int position = 0;
	char* find;
	char* replace;
	char flag = 0;
	if(argc > 1){
while(param < argc){
switch(param){
	case 0:
	param++;
	break;
	default:
 position = 0;
	switch(argv[param][position]){
		case '-':
			position++;
			switch(argv[param][position]){
			case 'h':
				printf("USAGE:\n\nsub [ -h | --fromChars -+toChars [-i inputFile] [-o outputFile] ]\nDESCRIPTION:\nThis utility copies text from an input stream to an output stream replacing every\ninstance of a specific character in fromChars with a corresponding (position-wise)\ncharacter from toChars. Any characters in fromChars and in toChars that do not\nhave corresponding counterparts in the other one are ignored. If an input file is\nprovided, the content is read from that file; otherwise, the standard input is used.\nIf an output file is provided, then the modified content is written to that file;\notherwise, thestandard output is used.\n\nOPTIONS:\n\n--(followed by a string without separating space)\nindicates all characters that will be replaced in the processed text\n\n-+(followed by a string without separating space)\nindicates the characters that will be used to replace corresponding\n(position-wise) characters from fromChars in the processed text\n\n-i (followed by input file name)\nuse the provided file as an input stream instead of standard input\n\n-o (followed by output file name)\nuse the provided file as an output stream instead of standard output\n\n-h prints this help message\n");
			        exit(0);
				break;	
			case '-':
				switch(flag){
					case 0:
						flag = 1;
						break;
					case 1:
						break;
					case 2: flag = 3;
						break;
				}
				find = &(argv[param][position+1]);
				break;
			case '+':
				 switch(flag){
                                        case 0:
                                                flag = 2;
                                                break;
                                        case 1: flag = 3;
                                                break;
                                        case 2:
                                                break;
                                }

				replace = &(argv[param][position+1]);
				break; 
			case 'o':
				param++;
                                freopen(argv[param],"w+",stdout);
			        break;
			case 'i':
				param++;
				freopen(argv[param],"r",stdin);
			        break;	
			default:
				printf("Invalid command; type sub -h for more help");
				exit(1);
			}
			break;
		case '>':
			param++;
			break;
		case '<':
			param++;
			break;
		default:
		printf("Invalid operation; type sub -h for more help");
		exit(1);
	}
	param++;
	break;
}
}
if(flag != 3){
	printf("No + or -; type sub -h for more help");
	exit(1);
}
else{
int f = strlen(find);
int r = strlen(replace);
if(r < f){
process(find, replace, r);
}
else{
process(find, replace, f);
}
}
}
else{
 printf("Not enough arguments; type sub -h for more help");
 exit(1);
}
}

void process(char* find, char* replace , int size){
char c = getchar();
int check = 0;
while(c != EOF){
while(check < size){
if(c == find[check]){
c = replace[check];
break;
}
check++;
}
check = 0;
printf("%c",c);
c = getchar();
}
}
