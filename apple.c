#include<stdio.h>

//file
char* readFile(char* fileName);
FILE* getFile(char* fileName, char* path);

int main(){
	FILE* player;
	char 
	
	player = getFile("Player.txt", "");
	if(player != NULL){
		
	}
}

char* readFile(char* fileName){
	FILE* file;
	char *str = malloc (sizeof (char) * 1024);
	
	file = fopen(fileName, "r");
	if(file == NULL) return 0;
	else{
		fgets(str, 1024, file);
		fclose(file);
		return str;
	}
}

FILE* getFile(char* fileName, char* path){
	FILE* file;
	char str[256] = {0, };
	
	if(checkFileName(fileName, path) == 0){
		sprintf(str, "%s\\%s", path, fileName);
		file = fopen(str, "r+");
		if(file != NULL) return file;
	}
	
	return NULL;
}
