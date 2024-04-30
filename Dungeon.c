#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define APPLE_APPEAR_PER 40
#define ENEMY_APPEAR_PER 80

#define IMAGE_SIZE 100
#define EXE_SIZE 500

#define APPLE_HEAL 3
#define UPSET_STOM 50

//file
char* readFile(char* fileName, char* path);
FILE* createFile(char* fileName, char* path);
FILE* getFile(char* fileName, char* path);
void removeFile(char* fileName, char* path);
void getImage(char* imageName, char imageSource[]);
void createImage(char* imageName, char imageSource[], char* path);

int checkFileName(char* name, char* path);
int checkFileNameOnRoot(char* name);

//content
int createDungeon(void);
void createLayer(uint8_t layer,char* folderName, char* path);
void createItem(uint8_t layer,char* folderName, char* path);
void createDoor(uint8_t layer,char* folderName, char* path);
void createApple(uint8_t layer,char* folderName, char* path);
void createSlime(uint8_t layer,char* folderName, char* path);

//utill
int atoi_ten(char* str);
int randomInt(uint8_t size);

void playerHeal(uint8_t heal);
void playerDamage(uint8_t dmg);

uint8_t getPlayerHP(void);
uint8_t getPlayerDMG(void);
void setPlayerHP(uint8_t hp);
void setPlayerDMG(uint8_t dmg);

void userAct(char *input);

void actEat(void);
void actEnter(void);
void actState(void);
void actEnemy(void);

void init(void);

char* APPLE_IMAGE[IMAGE_SIZE][IMAGE_SIZE];
char* SLIME_IMAGE[IMAGE_SIZE][IMAGE_SIZE];
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
        str = readFile("Dungeon.txt", ".");
        dungeonCreate++;

        deep = atoi_ten(str);
        createDungeon();
	}
    else printf("Dungeon.txt 파일이 없습니다.\n");

    if(dungeonCreate == 2) {
        printf("던전이 생성되었습니다!\n");
        printf("eat - 음식을 먹는다\nquit - 종료\nenter - 문 열기\nstate - 현재 상태\nenemy - 적 확인\n\n");
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
    getImage("slime.jpg", SLIME_IMAGE);
}

void userAct(char *input){
    if(strcmp(userInput, "eat") == 0) actEat();
    else if(strcmp(userInput, "enter") == 0) actEnter();
    else if(strcmp(userInput, "state") == 0) actState();
    else if(strcmp(userInput, "enemy") == 0) actEnemy();
}

void actEat(void){
    char str[256];
    int ran = (rand() % 100);

    printf("먹을 음식을 입력해주세요(아이템의 이름을 적어주세요)\n-");
    scanf("%s", userInput);
    if(checkFileName(userInput, inventoryRoot)){
        removeFile(userInput, inventoryRoot);

        if(strcmp(str, "apple.jpg") == 0){
             printf("사과를 먹었다!\n");
            playerHeal(APPLE_HEAL);
        }
        else{
            if(ran <= UPSET_STOM) {
                printf("이상할걸 먹었더니 배탈이 났다...\n");
                playerDamage(1);
            }
            else {
                printf("맛이 좋지 않다...\n");
                playerHeal(1);
            }
        }
        printf("현재 채력 : %d\n\n", getPlayerHP());
    }else printf("음식을 찾을 수 없습니다.\n\n");

}

void actEnter(void){
    char path[256];
    char str[256];

    printf("경로를 복사 및 붙여넣어 주세요\n-");
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

void actState(void){
    printf("HP : %d, DMG : %d\n\n", getPlayerHP(), getPlayerHP());
}

void actEnemy(void){
    char str[256];

    printf("적의 이름을 적어주세요.\n(slime)\n-");
    scanf("%s", str);

    if(strcmp(str, "slime") == 0)
        printf("state : HP - 10 , DMG - 2");
    else
        printf("알 수 없는 적입니다.");

    printf("\n\n");
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
        fputs("HP 20  AT 5", charact);
    }
    fclose(charact);

    createLayer(nowLayer, "Door", "Dungeon");

    dir = opendir("Dungeon");
    closedir(dir);

    dungeonCreate++;
}


void createLayer(uint8_t layer,char* folderName, char* path){
    char buff[256];
    sprintf(buff, "%s\\%s", path, folderName);

	mkdir(buff);
    createItem(layer, folderName, path);
}

void createItem(uint8_t layer,char* folderName, char* path){
    createDoor(layer, folderName, path);
	createApple(layer, folderName, path);
    createSlime(layer, folderName, path);
}

void createDoor(uint8_t layer,char* folderName, char* path){
	char buffName[256];
	char buff[256];
	uint8_t ran;

    sprintf(buff, "%s\\%s", path, folderName);

    if(layer <= deep){
        ran = (rand() % 2);

        if(ran == 0){
            ran = (rand() % 3) + 1;

            for(i = 0; i < ran; i++){
            sprintf(buffName, "door(%d).jpg", (i + 1));
            createImage(buffName, DOOR_IMAGE, buff);
            }
        }
        else{
            createImage("door.jpg", DOOR_IMAGE, buff);
        }

    }
}

void createApple(uint8_t layer,char* folderName, char* path){
    char buffName[256];
    char buff[256];
    uint8_t ran;

    sprintf(buff, "%s\\%s", path, folderName);

    if(layer <= deep){
        ran = (rand() % 100);

        if(ran < APPLE_APPEAR_PER){
            ran = (rand() % 2) + 1;

            for(i = 0; i < ran; i++){
            sprintf(buffName, "apple(%d).jpg", (i + 1));
            createImage(buffName, APPLE_IMAGE, buff);
            }
        }
        else{
            createImage("apple.jpg", APPLE_IMAGE, buff);
        }

    }
}

void createSlime(uint8_t layer,char* folderName, char* path){
    char buffName[256];
    char buff[256];
    uint8_t ran;

    sprintf(buff, "%s\\%s", path, folderName);

    if(layer <= deep){
        ran = (rand() % 100);

        if(ran < ENEMY_APPEAR_PER){
            ran = (rand() % 2) + 1;

            for(i = 0; i < ran; i++){
            sprintf(buffName, "slime(%d).jpg", (i + 1));
            createImage(buffName, SLIME_IMAGE, buff);
            }
        }
        else{
            createImage("slime.jpg", SLIME_IMAGE, buff);
        }

    }
}

////////////////////////////////

int randomInt(uint8_t size){
	uint8_t random = 0;
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

uint8_t getPlayerHP(void){
    char *buff = malloc (sizeof (char) * 256);
    uint8_t nowHP;

    buff = readFile("Player.txt", "Dungeon");

    sscanf(buff, "%*s %d", &nowHP);
    return nowHP;
}

uint8_t getPlayerDMG(void){
    char *buff = malloc (sizeof (char) * 256);
    uint8_t dmg;

    buff = readFile("Player.txt", "Dungeon");

    sscanf(buff, "%*s %*d  %*s %d", &dmg);
    return dmg;
}

void setPlayerHP(uint8_t hp){
    FILE* charact = NULL;
    char *str = malloc (sizeof (char) * 256);

    charact = getFile("Player.txt", "Dungeon");
    if(charact != NULL){
        sprintf(str, "HP %d  AT %d", hp, getPlayerDMG());
        fputs(str, charact);
    }
    fclose(charact);
}

void setPlayerDMG(uint8_t dmg){
    FILE* charact = NULL;
    char *str = malloc (sizeof (char) * 256);

    charact = getFile("Player.txt", "Dungeon");
    if(charact != NULL){
        sprintf(str, "HP %d  AT %d", getPlayerHP(), dmg);
        fputs(str, charact);
    }
    fclose(charact);
}

void playerHeal(uint8_t heal){
    uint8_t nowHP = getPlayerHP();

    if(nowHP + heal >= 99){
        setPlayerHP(99);
    }else{
        setPlayerHP(nowHP + heal);
    }
}

void playerDamage(uint8_t dmg){
    uint8_t nowHP = getPlayerHP();

    if(nowHP - dmg <= 0){
        setPlayerHP(0);
    }else{
        setPlayerHP(nowHP - dmg);
    }
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

char* readFile(char* fileName, char* path){
	FILE* file;
	char buff[100] = {0,};
	char *str = malloc (sizeof (char) * 1024);

	if(checkFileName(fileName, path)){
        sprintf(buff, "%s\\%s", path, fileName);
        file = fopen(buff, "r");

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









