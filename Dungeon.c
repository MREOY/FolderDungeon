#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define APPLE_APPEAR_PER 30
#define IMAGE_SIZE 100

//file
char* readFile(char* fileName);
FILE* createFile(char* fileName, char* path);
FILE* getFile(char* fileName, char* path);
char* getImage(char* imageName);
void createImage(char* imageName, char* imageSource, char* path);

//f name check
int checkFileName(char* name, char* path);

//content
int createDungeon(int deep);
void createLayer(int deep);
void createItem(int layer);
void createApple(int layer);

//utill
int atoi_ten(char* str);
int randomInt(int size);

void init(void);

char* APPLE_IMAGE;
FILE *buffImage;
char buffImageData[IMAGE_SIZE][IMAGE_SIZE];

char dungeonCreate  = 0;


int main(){
	char *str;
	int deep;

	init();
    str = readFile("Dungeon.txt");
    deep = atoi_ten(str);

    createDungeon(deep);

    if(dungeonCreate == 1) printf("던전이 생성되었습니다!\n");
    else printf("던전이 생성되지 못했습니다!\n");
	system("pause");

	return 0;
}

void init(void){
    srand(time(NULL));

    APPLE_IMAGE = getImage("apple.jpg");
}

int createDungeon(int deep){
	FILE* charact = NULL;
	int layer = deep;

    mkdir("Dungeon");
    mkdir("Dungeon\\Inven");

    charact = createFile("Player.txt", "Dungeon");
    if(charact == NULL){
        charact = getFile("Player.txt", "Dungeon");
    }else {
        fputs("HP:20\n", charact);
        fputs("AT:5\n", charact);
    }
        fclose(charact);

        createLayer(layer);
        dungeonCreate = 1;
}


void createLayer(int deep){
	char str[256] = "Dungeon";
	int i;

	for(i = 0; i <= deep; i++){
		strcat(str, "\\Door");
		mkdir(str);
		if(i != 0)
			createItem(i);
	}
}

void createItem(int layer){

	createApple(layer);

}

void createApple(int layer){
	char path[256] = "Dungeon";
	int i;

	for(i = 0; i < layer; i++){
		strcat(path, "\\Door");
	}
    if((rand() % 100) < APPLE_APPEAR_PER){
        createImage("apple.jpg", APPLE_IMAGE, path);
    }
}

int randomInt(int size){
	int random = 0;
	random = rand() % size;
	return random;
}


int atoi_ten(char* str){
	int turn = 0;
	int i = 0;

	for(i = (strlen(str) - 1); i > 0; i--){
		turn = 10 * i * atoi(&str[i]);
	}
	turn += atoi(&str[0]);

	return turn;
}

char* getImage(char* imageName){
    char path[100];

    sprintf(path, "Resources\\%s", imageName);

    buffImage = fopen(path, "rb");
    fread(buffImageData, sizeof(char), IMAGE_SIZE * IMAGE_SIZE, buffImage);
    fclose(buffImage);

    return buffImageData;
}

void createImage(char* imageName, char* imageSource, char* path){
    char buffPath[100];

    sprintf(buffPath, "%s\\%s", path, imageName);

    buffImage = fopen(buffPath, "wb");
    fwrite(imageSource, sizeof(char), IMAGE_SIZE * IMAGE_SIZE, buffImage);
    fclose(buffImage);
}

FILE* createFile(char* fileName, char* path){
	FILE* file;
	char str[256] = {0, };

	if(checkFileName(fileName, path) == 1){
		sprintf(str, "%s\\%s", path, fileName);
		file = fopen(str, "w");
		if(file != NULL) return file;
	}

	return NULL;
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

int checkFileName(char* name, char* path){
	int check = 0;

	FILE* file;
	char str[256] = {0, };

	sprintf(str, "%s\\%s", path, name);

	file = fopen(str, "r");
	if(file == NULL) return 1;
	else{
		return 0;
	}

}









