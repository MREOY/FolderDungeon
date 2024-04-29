#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define APPLE_APPEAR_PER 30
#define IMAGE_SIZE 100
#define EXE_SIZE 500

//file
char* readFile(char* fileName);
FILE* createFile(char* fileName, char* path);
FILE* getFile(char* fileName, char* path);
void removeFile(char* fileName, char* path);
void getImage(char* imageName, char imageSource[]);
void createImage(char* imageName, char imageSource[], char* path);

int checkFileName(char* name, char* path);
int checkFileNameOnRoot(char* name);

//content
int createDungeon(void);
void createLayer(int layer,char* folderName, char* path);
void createItem(int layer,char* folderName, char* path);
void createDoor(int layer,char* folderName, char* path);
void createApple(int layer,char* folderName, char* path);

//utill
int atoi_ten(char* str);
int randomInt(int size);

void userAct(char *input);

void actEat(void);
void actEnter(void);


void init(void);

char* APPLE_IMAGE[IMAGE_SIZE][IMAGE_SIZE];
char* DOOR_IMAGE[IMAGE_SIZE][IMAGE_SIZE];

FILE *buffImage;

char inventoryRoot[50] = "Dungeon\\Inven";

char dungeonCreate = 0;
char gameRunning = 0;
char userInput[256];

uint8_t i;
uint8_t deep;
uint8_t nowLayer = 1;

int main(void){
	char *str;

	init();
	if(checkFileNameOnRoot("Dungeon.txt")){
        str = readFile("Dungeon.txt");
        dungeonCreate++;

        deep = atoi_ten(str);
        createDungeon();
	}
    else printf("Dungeon.txt 파일이 없습니다.\n");

    if(dungeonCreate == 2) {
        printf("던전이 생성되었습니다!\n");
        printf("eat - 음식을 먹는다\nquit - 종료\nenter - 문 열기\n\n");
        gameRunning = 1;
    }
    else printf("던전이 생성되지 못했습니다!\n");

    while(1){
        if(!gameRunning) break;

        printf("행동을 입력해주세요.\n-");
        scanf("%s", userInput);
        puts("\n");
        if(strcmp(userInput, "quit") == 0) break;
        else userAct(userInput);

    }

    system("pause");

	return 0;
}

void init(void){
    srand(time(NULL));

    getImage("apple.jpg", APPLE_IMAGE);
    getImage("door.jpg", DOOR_IMAGE);
}

void userAct(char *input){
    if(strcmp(userInput, "eat") == 0) actEat();
    else if(strcmp(userInput, "enter") == 0) actEnter();
}

void actEat(void){
    printf("먹을 음식을 입력해주세요(아이템의 이름을 적어주세요)\n-");
    scanf("%s", userInput);
    if(checkFileName(userInput, inventoryRoot)){
        removeFile(userInput, inventoryRoot);
        //switch

        printf("음식을 먹었습니다.\n\n");
    }else printf("음식을 찾을 수 없습니다.\n\n");

}

void actEnter(void){
    char path[256];
    char str[256];

    printf("경로를 복사 붙여넣어 주세요\n-");
    scanf("%s", path);

    strcpy(str, path);
    strcat(str, "\\");

    printf("문의 이름을 입력해주세요.\n-");
    scanf("%s", userInput);

    strcat(str, userInput);

    if(remove(str) == 0){
        nowLayer++;;
        sscanf(userInput, "%[^.jpg]", str);
        createLayer(nowLayer, str, path);
        printf("문이 열렸습니다.\n\n");
    }else
        printf("해당 경로에 문이 없습니다.\n\n");

}

/////////////////////////////////

int createDungeon(void){
	FILE* charact = NULL;
	DIR* dir = NULL;

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

    createLayer(nowLayer, "Door", "Dungeon");

    dir = opendir("Dungeon");
    closedir(dir);

    dungeonCreate++;
}


void createLayer(int layer,char* folderName, char* path){
    char buff[256];
    sprintf(buff, "%s\\%s", path, folderName);

	mkdir(buff);
    createItem(layer, folderName, path);
}

void createItem(int layer,char* folderName, char* path){
    createDoor(layer, folderName, path);
	createApple(layer, folderName, path);

}

void createDoor(int layer,char* folderName, char* path){
	char buffName[50];
	char buff[256];
	int a;

    sprintf(buff, "%s\\%s", path, folderName);

	a = (rand() % 3) + 1;

	for(i = 0; i < a; i++){
        sprintf(buffName, "door(%d).jpg", (i + 1));
        createImage(buffName, DOOR_IMAGE, buff);
	}
}

void createApple(int layer,char* folderName, char* path){
    char buff[256];
    sprintf(buff, "%s\\%s", path, folderName);


    if((rand() % 100) < APPLE_APPEAR_PER){
        createImage("apple.jpg", APPLE_IMAGE, buff);
    }
}

////////////////////////////////

int randomInt(int size){
	int random = 0;
	random = rand() % size;
	return random;
}


int atoi_ten(char* str){
	int turn = 0;

	for(i = (strlen(str) - 1); i > 0; i--){
		turn = 10 * i * atoi(&str[i]);
	}
	turn += atoi(&str[0]);

	return turn;
}

///////////////////////////////////

void getImage(char* imageName, char imageSource[]){
    char path[100];

    sprintf(path, "Resources\\%s", imageName);

    buffImage = fopen(path, "rb");
    fread(imageSource, sizeof(char), IMAGE_SIZE * IMAGE_SIZE, buffImage);
    fclose(buffImage);

}

void createImage(char* imageName, char imageSource[], char* path){
    char buffPath[100];

    sprintf(buffPath, "%s\\%s", path, imageName);

    buffImage = fopen(buffPath, "wb");
    fwrite(imageSource, sizeof(char), IMAGE_SIZE * IMAGE_SIZE, buffImage);
    fclose(buffImage);

}

/////////////////////////////////////

FILE* createFile(char* fileName, char* path){
	FILE* file;
	char str[256] = {0, };

	if(!checkFileName(fileName, path)){
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

	if(checkFileName(fileName, path)){
		sprintf(str, "%s\\%s", path, fileName);
		file = fopen(str, "r+");
		if(file != NULL) return file;
	}

	return NULL;
}

void removeFile(char* fileName, char* path){
    char str[256] = {0, };

	if(checkFileName(fileName, path)){
		sprintf(str, "%s\\%s", path, fileName);
		remove(str);
	}
}

int checkFileName(char* name, char* path){
	int check = 0;

	FILE* file;
	char str[256] = {0, };

	sprintf(str, "%s\\%s", path, name);

	file = fopen(str, "r");
	fclose(file);
	if(file == NULL) return 0;
	else return 1;
}

int checkFileNameOnRoot(char* name){
	int check = 0;

	FILE* file;
	char str[256] = {0, };

	file = fopen(name, "r");
	fclose(file);
	if(file == NULL) return 0;
	else return 1;
}









