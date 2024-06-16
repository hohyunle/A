#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_RESTAURANT_SIZE 60 //가중치 갯수 53, 건물 갯수 39이므로 최대 53으로 해도 돌아갈 것으로 보임
#define INF 10000
FILE* userFile;
FILE* restaurantFile;
FILE* BookmarkFile;
FILE* rest_locationFile;

typedef struct account {
    char name[20];
    char phone_number[30];
    char id[20];
    char pw[20];
}ac;
char check_id[20];
int check = -1;

//ac user[50]; //파일 불러오는 용도의 구조체

typedef struct bookmark {
    char id[20];
    char bookmark[100];
}Bookmark;

typedef struct Membership {
    ac b;
    Bookmark bm[20];
}MS;
MS user[50];

typedef struct restaurant {
    char group[4];
    char name[100];
    char loca[100];
    char number[20];
    char open[10];
    char close[10];
    char CT[10];
}re;
re rest[30];

typedef struct address {
    char province[20];
    char city[20];
    char district[20];
    char detail[60];
} ad;

typedef struct us_location {
    char type[4];
    char departure[40]; //시작지점
    char arrival[40];   //도착지점
    int distance;
} us;

//학교 가중치 갯수
#define MAX_SCHOOL_DIS 20
//식당 가중치 갯수
#define MAX_REST_DIS 18
//편의점 가중치 갯수
#define MAX_STORE_DIS 10
//카페 가중치 갯수
#define MAX_CAFE_DIS 10

//학교 건물 갯수 (학교 11)
#define MAX_SCHOOL 11
//식당 건물 갯수 (식당 16 + 학교 2) //제1자연관, 제2학생회관
#define MAX_REST 18
//편의점 건물 갯수 (편의점 6 + 학교 3) //제2학생회관, 목민관, 제1자연관
#define MAX_STORE 9
//카페 건물 갯수 (카페 7 + 학교 3) //목민관, 제2학생회관, 제1자연관
#define MAX_CAFE 10

us store[MAX_RESTAURANT_SIZE]; //전체 가중치 저장 구조체
us school[MAX_SCHOOL_DIS]; // 학교 가중치 저장 구조체 -> 학교 다익스트라에서 사용
us re_loc[MAX_REST_DIS]; // 식당 가중치 저장 구조체
us st_loc[MAX_STORE_DIS]; // 편의점 가중치 저장 구조체
us ca_loc[MAX_CAFE_DIS]; // 카페 가중치 저장 구조체

int maxIndex = -1;
typedef struct djk {
    int resta[MAX_RESTAURANT_SIZE];        // 다익스트라 저장할 배열
    int v;                                 // 방문한 노드
    int d;                                 // 거리 저장할 배열
} DJK;

DJK dj_school[MAX_SCHOOL]; //학교 다익스트라 11*11 배열
DJK dj_rest[MAX_REST]; //식당 다익스트라 17*17 배열
DJK dj_store[MAX_STORE]; //편의점 다익스트라 9*9 배열
DJK dj_cafe[MAX_CAFE]; //카페 다익스트라 11*11 배열

//다익스트라 -> 다익스트라

char user_now_location[30];

//총 건물 이름 39개
char Aname[MAX_RESTAURANT_SIZE][MAX_RESTAURANT_SIZE] = { "미래창조관", "제2자연관", "행정관","제2학생회관","사범대관","예술관","인문사회관","제1학생회관", "목민관","바이오융합관","제1자연관",
                                                         "노량진컵밥","오믈오믈","김홍걸해장국","맘스터치서원대점","면식당","다다덮밥","형제쭈꾸미","돼지가좋은날","셀러브리티포케","롯데리아",
                                                         "속시원한칼국수","왕가네짬뽕","임마누엘", "럭키마라탕", "스마일마라탕", "믹스토랑",
                                                         "GS25서원대후문점","세븐일레븐서원대점","CU청주서원대점","GS25서원대학사점","CU청주서원대스타점","GS25모충행복점",
                                                         "빽다방","메가커피","카페이루다","토프레소","재재","컴포즈커피","이디야커피" };

//학교 건물 이름 11개
char SCname[MAX_SCHOOL][MAX_RESTAURANT_SIZE] = { "미래창조관", "제2자연관", "행정관","제2학생회관",
                                                  "사범대관","예술관","인문사회관","제1학생회관",
                                                  "목민관","바이오융합관","제1자연관", };

//식당 건물 이름 17개
char Rname[MAX_REST][MAX_RESTAURANT_SIZE] = { "제2학생회관","제1자연관","노량진컵밥","오믈오믈",
                                              "김홍걸해장국","맘스터치서원대점","면식당","다다덮밥",
                                              "형제쭈꾸미","돼지가좋은날","셀러브리티포케","롯데리아",
                                              "속시원한칼국수","왕가네짬뽕","임마누엘", "럭키마라탕",
                                              "스마일마라탕", "믹스토랑" };

//편의점 건물 이름 9개
char STname[MAX_STORE][MAX_RESTAURANT_SIZE] = { "제2학생회관","목민관","제1자연관","GS25서원대후문점",
                                                "세븐일레븐서원대점","CU청주서원대점","GS25서원대학사점","CU청주서원대스타점","GS25모충행복점" };

//카페 건물 이름 11개
char Cname[MAX_CAFE][MAX_RESTAURANT_SIZE] = { "제2학생회관","목민관","제1자연관",
                                              "빽다방","메가커피","카페이루다","토프레소",
                                              "재재","컴포즈커피","이디야커피" };



int now_lo = 0; // 유저 선택 현재 위치에서 인덱스로 사용 될 예정

void goto_xy(int x, int y);
void square(int width, int height, int x, int y);
void square_char(int width, int height, int x, int y, char* msg);
void square3(int width, int height, int x, int y);
void print_rogo();

int getSmallIndex(DJK DJ, int num);
void daikstra(int start, DJK DJ, int num);

void UserFileOpen(void);                //사용자 정보 파일 여는 함수
void restaurantFileOpen(void);          //레스토랑 정보 파일
void BookmarkFileOpen(void);            //북마크 파일 
void rest_locationFileOpen(void);       //식당 위치 정보 파일

void menu(int menu);                    //메뉴 함수(로그인, 회원가입)
void menu_print();                      //메인 화면 UI
void login();                           //로그인 화면 UI
void sign_up();                         //회원가입 화면

void user_ui();                         //사용자 개인 화면 UI(북마크, 식당 추천)

void bookmark_ui();                     //북마크 화면 UI
void user_location();                   //사용자 위치 입력 받는 UI

void bookmark_menu();                   //북마크 조회
void bookmark_add();                    //북마크 추가
void bookmark_delete();                 //북마크 삭제

void recommend();      //식당 추천 받는 UI(식당, 술집, 카페, 편의점 추천)
void recommend_rest();                  //식당 추천 받기
void recommend_store();                   //술집 추천 받기
void recommend_cafe();                  //카페 추천 받기

void restarnt_information(int index);

int getSchooLIndex(const char* buildName); //학교 건물의 이름을 전달받으면 건물에 해당하는 숫자 리턴
int getRestIndex(const char* buildName); ////식당 건물의 이름을 전달받으면 건물에 해당하는 숫자 리턴
int getStoreIndex(const char* buildName); //편의점 건물의 이름을 전달받으면 건물에 해당하는 숫자 리턴
int getCafeIndex(const char* buildName); //카페 건물의 이름을 전달받으면 건물에 해당하는 숫자 리턴

//좌표만 이동하는 것
void goto_xy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}
//상자 그리는 함수(가로, 세로, 왼쪽 모서리 점 위치, )
void square(int width, int height, int x, int y)
{
    goto_xy(x, y);
    for (int i = 0; i < width; i++) {   //첫 가로 라인 그리기
        if (i == 0) printf("┏");
        else if (i == width - 1) printf("┓\n");
        else  printf("━");
    }
    goto_xy(x, y + 1);
    for (int j = 0; j < height; j++) {  //세로 라인 그리기
        printf("┃");
        for (int k = 0; k < width - 2; k++) {
            printf(" ");
        }
        printf("┃\n");
        goto_xy(x, y + (j + 1));
    }
    for (int m = 0; m < width; m++) {   //마지막 가로 라인 그리기
        if (m == 0) printf("┗");
        else if (m == width - 1) printf("┛\n");
        else printf("━");
    }
}
//상자 그리는 함수(가로, 세로, x좌표, y좌표, 글자 쓰면 박스 가운데 오도록 해주는 함수)
void square_char(int width, int height, int x, int y, char* msg)
{
    goto_xy(x, y);
    for (int i = 0; i < width; i++) {
        if (i == 0) printf("┏");
        else if (i == width - 1) printf("┓\n");
        else  printf("━");
    }
    goto_xy(x, y + 1);
    for (int j = 0; j < height; j++) {
        printf("┃");
        for (int k = 0; k < width - 2; k++) {
            if (j == height / 2 && k == ((width)-strlen(msg)) / 2) {
                goto_xy(x + ((width)-strlen(msg)) / 2 + 1, y + height / 2);
                printf("%s", msg);
                goto_xy(x + width - 1, y + j);
            }
            else if (j != height / 2) printf(" ");
        }

        printf("┃");
        goto_xy(x, y + (j + 1));
    }
    for (int m = 0; m < width; m++) {
        if (m == 0) printf("┗");
        else if (m == width - 1) printf("┛\n");
        else printf("━");
    }
}
//박스 두 개를 겹쳤을 때 겹치는 선이 없도록, 박스 안에 박스를 넣어주는 함수
void square3(int width, int height, int x, int y)
{
    goto_xy(x, y);
    for (int i = 0; i < width; i++) {
        if (i == 0) printf("┣");
        else if (i == width - 1) printf("┨\n");
        else  printf("━");
    }
    goto_xy(x, y + 1);
    for (int j = 0; j < height; j++) {
        printf("┃");
        for (int k = 0; k < width - 2; k++) {
            printf(" ");
        }
        printf("┃\n");
        goto_xy(x, y + (j + 1));
    }
    for (int m = 0; m < width; m++) {
        if (m == 0) printf("┗");
        else if (m == width - 1) printf("┛\n");
        else printf("━");
    }
}
void print_rogo()
{
    goto_xy(19, 5);
    printf("     ■■      ■  ■■  ■\n");
    goto_xy(19, 6);
    printf("   ■■■■    ■  ■■  ■\n");
    goto_xy(19, 7);
    printf(" ■■■■■■  ■  ■■  ■\n");
    goto_xy(19, 8);
    printf(" ■■■■■■  ■■■■■■\n");
    goto_xy(19, 9);
    printf(" ■■■■■■    ■■■■\n");
    goto_xy(19, 10);
    printf("   ■■■■        ■■\n");
    goto_xy(19, 11);
    printf("     ■■          ■■\n");
    goto_xy(19, 12);
    printf("     ■■          ■■\n");
    goto_xy(19, 13);
    printf("     ■■          ■■\n");
    goto_xy(19, 14);
    printf("     ■■          ■■\n");
    goto_xy(19, 15);
    printf("     ■■          ■■\n");
}

//회원 파일 열고 읽기
void UserFileOpen(void)
{
    userFile = fopen("user.txt", "r");
    if (userFile == NULL) {
        printf("파일을 열 수 없습니다.\n");
    } //파일이 존재하지 않을 경우에 출력되는 출력문

    int i = 0;
    while (fscanf(userFile, "%s %s %s %s", user[i].b.id, user[i].b.pw, user[i].b.name, user[i].b.phone_number) != EOF) {
        maxIndex = i;
        i++;
        if (i >= 50) break;
    } //파일에서 읽을 것이 없을 때까지 or 50개 이상일 경우 while문을 빠져나온다.
    //scnaf는 띄어쓰기는 읽을 수 없기에 띄어쓰기로 구분하여 파일에 있는 값을 변수에 저장

    fclose(userFile); //구조체에 따로 저장했기에 파일을 굳이 열어둘 필요가 없다.
}
//식당 파일 열고 읽기
void restaurantFileOpen(void) {
    restaurantFile = fopen("restaurant.txt", "r");
    if (restaurantFile == NULL) {
        printf("파일을 열 수 없습니다.\n");
    } //파일이 존재하지 않을 경우에 출력되는 출력문

    int i = 0;
    while (fscanf(restaurantFile, "%s %s %s %s %s %s %s", rest[i].group, rest[i].name, rest[i].loca, rest[i].number, rest[i].open, rest[i].close, rest[i].CT) != EOF) {
        maxIndex = i;
        i++;
        if (i >= 50) break;
    } //파일에서 읽을 것이 없을 때까지 or 50개 이상일 경우 while문을 빠져나온다.
    //scnaf는 띄어쓰기는 읽을 수 없기에 띄어쓰기로 구분하여 파일에 있는 값을 변수에 저장

    fclose(restaurantFile); //구조체에 따로 저장했기에 파일을 굳이 열어둘 필요가 없다.
}
//북마크 파일 열고 읽기
void BookmarkFileOpen(void) {
    FILE* BookmarkFile;
    char id[20];
    char bookmark[100];

    BookmarkFile = fopen("Bookmark.txt", "r");
    if (BookmarkFile == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }

    int i = check; // 로그인한 사용자의 인덱스
    int j = 0;
    // 로그인한 사용자의 북마크 배열 초기화
    memset(user[i].bm, 0, sizeof(user[i].bm));

    while (fscanf(BookmarkFile, "%s %s", id, bookmark) != EOF) {
        if (strcmp(id, check_id) == 0) {
            strcpy(user[i].bm[j].id, id);
            strcpy(user[i].bm[j].bookmark, bookmark);
            j++;
            if (j >= 20) break; // 한 사용자의 최대 북마크 수를 넘지 않도록 제한
        }
    }

    fclose(BookmarkFile);
}

//가중치 파일 열고 읽기
void rest_locationFileOpen(void) {
    rest_locationFile = fopen("rest_location.txt", "r");
    if (rest_locationFile == NULL) {
        printf("파일을 열 수 없습니다.\n");
    }

    //printf("파일을 열었습니다.\n");

    int i = 0, j = 0;
    while (fscanf(rest_locationFile, "%s %s %s %d", store[i].type, store[i].departure, store[i].arrival, &store[i].distance) != EOF) { //여기 주소연산자 적어야돼용!
        //maxIndex = i;
        i++;
        if (i >= MAX_RESTAURANT_SIZE) break;
    }

    //printf("파일을 읽었습니다.\n");

    //전체 가중치 파일에서 필요한 부분으로 재배치 해주는 코드
    // ************************************************************************************ 아마 한 번 읽고 안 쓸 것으로 보이나, 건물 변동이 있을 경우 수정 필요

    //각 건물 다익스틀 간의 저장 인덱스
    //학교 가중치 갯수 20 / 식당 가중치 갯수 15 / 편의점 가중치 갯수 9 / 카페 가중치 갯수 9 만큼 저장될 것
    int sch_index = 0, re_index = 0, st_index = 0, ca_index = 0;
    for (int i = 0; i < MAX_RESTAURANT_SIZE; i++) {
        if (strcmp(store[i].type, "학") == 0) {
            strcpy(school[sch_index].type, store[i].type);                  //strcpy(원본 문자열, 복사될 문자열)으로 판단한 후 작성, 틀릴 경우 수정 필요
            strcpy(school[sch_index].departure, store[i].departure);
            strcpy(school[sch_index].arrival, store[i].arrival);
            school[sch_index].distance = store[i].distance;
            sch_index++;
        }
        if (strcmp(store[i].type, "식") == 0) {
            strcpy(re_loc[re_index].type, store[i].type);
            strcpy(re_loc[re_index].departure, store[i].departure);
            strcpy(re_loc[re_index].arrival, store[i].arrival);
            re_loc[re_index].distance = store[i].distance;
            re_index++;
        }
        else if (strcmp(store[i].type, "편") == 0) {
            strcpy(st_loc[st_index].type, store[i].type);
            strcpy(st_loc[st_index].departure, store[i].departure);
            strcpy(st_loc[st_index].arrival, store[i].arrival);
            st_loc[st_index].distance = store[i].distance;
            st_index++;
        }
        else if (strcmp(store[i].type, "카") == 0) {
            strcpy(ca_loc[ca_index].type, store[i].type);
            strcpy(ca_loc[ca_index].departure, store[i].departure);
            strcpy(ca_loc[ca_index].arrival, store[i].arrival);
            ca_loc[ca_index].distance = store[i].distance;
            ca_index++;
        }
    }

    //printf("구조체를 분리하였습니다.\n");

    fclose(rest_locationFile);

    //가중치 파일 읽고 저장해주는 함수가 없소요............. ㅜㅜㅜㅜ

    for (i = 0; i < MAX_SCHOOL; i++) {
        for (j = 0; j < MAX_SCHOOL; j++) {
            dj_school[i].resta[j] = INF;
        }
        dj_school[i].resta[i] = 0;
    } //학교 다익스트라 2차원 배열을 INF(무한대)로 초기화 / 자기 자신과의 이동 가중치는 0으로 설정

    for (i = 0; i < MAX_REST; i++) {
        for (j = 0; j < MAX_REST; j++) {
            dj_rest[i].resta[j] = INF;
        }
        dj_rest[i].resta[i] = 0;
    } //식당 다익스트라 2차원 배열을 INF(무한대)로 초기화 / 자기 자신과의 이동 가중치는 0으로 설정
    //int SCH1_Index = getSchooLIndex("제2학생회관"); //배열의 인덱스로 사용하기 위해 정수값을 리턴해주는 함수 사용
    //int Re1_Index = getRestIndex("제2학생회관");
    //int SCH2_Index = getSchooLIndex("제1자연관");
    //int Re2_Index = getRestIndex("제1자연관");
    //dj_rest[Re1_Index].resta[Re1_Index] = dj_school[SCH1_Index].resta[SCH1_Index];
    //dj_rest[Re2_Index].resta[Re2_Index] = dj_school[SCH2_Index].resta[SCH2_Index];
    //굳이 안 필요한 것 같기도 하고....?

    for (i = 0; i < MAX_STORE; i++) {
        for (j = 0; j < MAX_STORE; j++) {
            dj_store[i].resta[j] = INF;
        }
        dj_store[i].resta[i] = 0;
    } //편의점 다익스트라 2차원 배열을 INF(무한대)로 초기화 / 자기 자신과의 이동 가중치는 0으로 설정

    for (i = 0; i < MAX_CAFE; i++) {
        for (j = 0; j < MAX_CAFE; j++) {
            dj_cafe[i].resta[j] = INF;
        }
        dj_cafe[i].resta[i] = 0;
    } //카페 다익스트라 2차원 배열을 INF(무한대)로 초기화 / 자기 자신과의 이동 가중치는 0으로 설정

    ////학교 가중치 갯수
    //#define MAX_SCHOOL_DIS 20
    ////식당 가중치 갯수
    //#define MAX_REST_DIS 15
    ////편의점 가중치 갯수
    //#define MAX_STORE_DIS 9
    ////카페 가중치 갯수
    //#define MAX_CAFE_DIS 9

    //us store[MAX_RESTAURANT_SIZE]; //전체 가중치 저장 구조체
    //us school[MAX_SCHOOL_DIS]; // 학교 가중치 저장 구조체 -> 학교 다익스트라에서 사용
    //us re_loc[MAX_REST_DIS]; // 식당 가중치 저장 구조체
    //us st_loc[MAX_STORE_DIS]; // 편의점 가중치 저장 구조체
    //us ca_loc[MAX_CAFE_DIS]; // 카페 가중치 저장 구조체

    for (int i = 0; i < MAX_SCHOOL_DIS; i++) {
        int firstIndex = getSchooLIndex(school[i].departure); //배열의 인덱스로 사용하기 위해 정수값을 리턴해주는 함수 사용
        int finalIndex = getSchooLIndex(school[i].arrival);

        if (firstIndex != -1 && finalIndex != -1) { //두 인덱스 값이 정상적으로 있을 때
            dj_school[firstIndex].resta[finalIndex] = school[i].distance;
            dj_school[finalIndex].resta[firstIndex] = school[i].distance;
        }
    }
    for (int i = 0; i < MAX_REST_DIS; i++) {
        int firstIndex = getRestIndex(re_loc[i].departure); //배열의 인덱스로 사용하기 위해 정수값을 리턴해주는 함수 사용
        int finalIndex = getRestIndex(re_loc[i].arrival);

        if (firstIndex != -1 && finalIndex != -1) { //두 인덱스 값이 정상적으로 있을 때
            dj_rest[firstIndex].resta[finalIndex] = re_loc[i].distance;
            dj_rest[finalIndex].resta[firstIndex] = re_loc[i].distance;
        }
    }
    for (int i = 0; i < MAX_STORE_DIS; i++) {
        int firstIndex = getStoreIndex(st_loc[i].departure); //배열의 인덱스로 사용하기 위해 정수값을 리턴해주는 함수 사용
        int finalIndex = getStoreIndex(st_loc[i].arrival);

        if (firstIndex != -1 && finalIndex != -1) { //두 인덱스 값이 정상적으로 있을 때
            dj_store[firstIndex].resta[finalIndex] = st_loc[i].distance;
            dj_store[finalIndex].resta[firstIndex] = st_loc[i].distance;
        }
    }
    for (int i = 0; i < MAX_CAFE_DIS; i++) {
        int firstIndex = getCafeIndex(ca_loc[i].departure); //배열의 인덱스로 사용하기 위해 정수값을 리턴해주는 함수 사용
        int finalIndex = getCafeIndex(ca_loc[i].arrival);

        if (firstIndex != -1 && finalIndex != -1) { //두 인덱스 값이 정상적으로 있을 때
            dj_cafe[firstIndex].resta[finalIndex] = ca_loc[i].distance;
            dj_cafe[finalIndex].resta[firstIndex] = ca_loc[i].distance;
        }
    }
}

////편의점 건물 이름 9개
//char STname[MAX_STORE][MAX_RESTAURANT_SIZE] = { "제2학생회관","목민관","제1자연관","GS25서원대후문점",
//                                                "세븐일레븐서원대점","CU청주서원대점","GS25서원대학사점","CU청주서원대스타점","GS25모충행복점" };
//
////카페 건물 이름 11개
//char Cname[MAX_CAFE][MAX_RESTAURANT_SIZE] = { "제2학생회관","인문사회관","목민관","제1자연관",
//                                              "빽다방","메가커피","카페이루다","토프레소",
//                                              "재재","컴포즈커피","이디야커피" };

//카페 건물의 이름을 전달받으면 건물에 해당하는 숫자 리턴
int getCafeIndex(const char* buildName) {
    if (strcmp(buildName, "제2학생회관") == 0)
        return 0;
    else if (strcmp(buildName, "목민관") == 0)
        return 1;
    else if (strcmp(buildName, "제1자연관") == 0)
        return 2;
    else if (strcmp(buildName, "빽다방") == 0)
        return 3;
    else if (strcmp(buildName, "메가커피") == 0)
        return 4;
    else if (strcmp(buildName, "카페이루다") == 0)
        return 5;
    else if (strcmp(buildName, "토프레소") == 0)
        return 6;
    else if (strcmp(buildName, "재재") == 0)
        return 7;
    else if (strcmp(buildName, "컴포즈커피") == 0)
        return 8;
    else if (strcmp(buildName, "이디야커피") == 0)
        return 9;
    else
        return -1; // 일치하는 건물명이 없는 경우
}

//편의점 건물의 이름을 전달받으면 건물에 해당하는 숫자 리턴
int getStoreIndex(const char* buildName) {
    if (strcmp(buildName, "제2학생회관") == 0)
        return 0;
    else if (strcmp(buildName, "목민관") == 0)
        return 1;
    else if (strcmp(buildName, "제1자연관") == 0)
        return 2;
    else if (strcmp(buildName, "GS25서원대후문점") == 0)
        return 3;
    else if (strcmp(buildName, "세븐일레븐서원대점") == 0)
        return 4;
    else if (strcmp(buildName, "CU청주서원대점") == 0)
        return 5;
    else if (strcmp(buildName, "GS25서원대학사점") == 0)
        return 6;
    else if (strcmp(buildName, "CU청주서원대스타점") == 0)
        return 7;
    else if (strcmp(buildName, "GS25모충행복점") == 0)
        return 8;
    else
        return -1; // 일치하는 건물명이 없는 경우
}

//식당 건물의 이름을 전달받으면 건물에 해당하는 숫자 리턴
int getRestIndex(const char* buildName) {
    if (strcmp(buildName, "제2학생회관") == 0)
        return 0;
    else if (strcmp(buildName, "제1자연관") == 0)
        return 1;
    else if (strcmp(buildName, "노량진컵밥") == 0)
        return 2;
    else if (strcmp(buildName, "오믈오믈") == 0)
        return 3;
    else if (strcmp(buildName, "김홍걸해장국") == 0)
        return 4;
    else if (strcmp(buildName, "맘스터치서원대점") == 0)
        return 5;
    else if (strcmp(buildName, "면식당") == 0)
        return 6;
    else if (strcmp(buildName, "다다덮밥") == 0)
        return 7;
    else if (strcmp(buildName, "형제쭈꾸미") == 0)
        return 8;
    else if (strcmp(buildName, "돼지가좋은날") == 0)
        return 9;
    else if (strcmp(buildName, "셀러브리티포케") == 0)
        return 10;
    else if (strcmp(buildName, "롯데리아") == 0)
        return 11;
    else if (strcmp(buildName, "속시원한칼국수") == 0)
        return 12;
    else if (strcmp(buildName, "왕가네짬뽕") == 0)
        return 13;
    else if (strcmp(buildName, "임마누엘") == 0)
        return 14;
    else if (strcmp(buildName, "럭키마라탕") == 0)
        return 15;
    else if (strcmp(buildName, "스마일마라탕") == 0)
        return 16;
    else if (strcmp(buildName, "믹스토랑") == 0)
        return 17;
    else
        return -1; // 일치하는 건물명이 없는 경우
}

//학교 건물의 이름을 전달받으면 건물에 해당하는 숫자 리턴
int getSchooLIndex(const char* buildName) {
    if (strcmp(buildName, "미래창조관") == 0)
        return 0;
    else if (strcmp(buildName, "제2자연관") == 0)
        return 1;
    else if (strcmp(buildName, "행정관") == 0)
        return 2;
    else if (strcmp(buildName, "제2학생회관") == 0)
        return 3;
    else if (strcmp(buildName, "사범대관") == 0)
        return 4;
    else if (strcmp(buildName, "예술관") == 0)
        return 5;
    else if (strcmp(buildName, "인문사회관") == 0)
        return 6;
    else if (strcmp(buildName, "제1학생회관") == 0)
        return 7;
    else if (strcmp(buildName, "목민관") == 0)
        return 8;
    else if (strcmp(buildName, "바이오융합관") == 0)
        return 9;
    else if (strcmp(buildName, "제1자연관") == 0)
        return 10;
    else
        return -1; // 일치하는 건물명이 없는 경우
}

//가장 작은 가중치 판별 함수
int getSmallIndex(DJK DJ[], int num) {
    int min = INF; //가중치를 비교할 변수 선언, 가장 작은 값을 구할 것이기에 무한대로 초기화 
    int index = 0; //인덱스는 0부터 시작
    for (int i = 0; i < num; i++) { //i가 0부터 빌딜 갯수(14)까지 반복
        if (DJ[i].d < min && DJ[i].v != 1) { //거리가 min보다 작고, 확정된 노드가 아닐때
            min = DJ[i].d; //min에 i번째 거리 값을 집어넣고
            index = i; //index 값에 i 값을 집어넣는다
        }
    }

    return index; //index를 반환해준다.
}

//다익스트라 거리 계산
void daikstra(int start, DJK DJ[], int num) {
    for (int i = 0; i < num; i++) {
        DJ[i].d = DJ[start].resta[i];                                        //다익스트라 2차원 배열의 원소를 거리 값으로 채워준다.
        DJ[i].v = 0;                                                         //확정된 노드는 없다.
    }
    DJ[start].v = 1;                                                         //v[0] = 1로 방문하였다는 표시를 해준다.
    for (int i = 0; i < num - 2; i++) {                                      //이중 for문을 사용할 예정이기에 최종 건물 갯수에서 -2를 해준다.
        int index = getSmallIndex(DJ, num);                                  //가장 작은 인덱스를 반환해주는 함수를 호출하고 인덱스를 변수에 저장한다.
        DJ[index].v = 1;                                                     //v[인덱스]는 방문한 노드이다.
        for (int j = 0; j < num; j++) {                                      //j는 0부터 최종 건물 갯수까지 반복한다(0 ~ 13까지 반복)
            if (DJ[j].v != 1) {                                              //v[j]가 방문한 노드가 아니라면
                if (DJ[index].d + DJ[index].resta[j] < DJ[j].d) {            //거리[인덱스] + 2차원 배열[인덱스][j] < 거리[j]에 해당한다면
                    DJ[j].d = DJ[index].d + DJ[index].resta[j];              //거리[j] = 거리[인덱스] + 2차원 배열[index][j]                  //열 값 변환 (행일 수도)
                    DJ[start].resta[j] = DJ[index].d + DJ[index].resta[j];   //2차원 배열[start][j] = 거리[index] + 2차원 배열[index][j]      //행 값 변환 (열일 수도)
                }
            }
        }
    }
}

typedef struct Nayeol {
    int dis; //비교할 거리
    int count;
}Na;

Na SCH_N[MAX_SCHOOL] = { {INF,0}, {INF,0}, {INF,0}, {INF,0} };
Na R_N[MAX_REST] = { {INF,0}, {INF,0}, {INF,0}, {INF,0} };
Na ST_N[MAX_STORE] = { {INF,0}, {INF,0}, {INF,0}, {INF,0} };
Na C_N[MAX_CAFE] = { {INF,0}, {INF,0}, {INF,0}, {INF,0} };

int sc_final[MAX_SCHOOL];    //정렬된 학교 다익스트라 인덱스 순서
int r_final[MAX_REST];       //정렬된 식당 다익스트라 인덱스 순서
int st_final[MAX_STORE];     //정렬된 편의점 다익스트라 인덱스 순서
int c_final[MAX_CAFE];       //정렬된 카페 다익스트라 인덱스 순서

//다익스트라 거리 계산 후 기준을 통해 출력하기 (함수)
void Ques(char Use_name[][MAX_RESTAURANT_SIZE], char name[], int final[], Na N[], DJK DJ[], int num) {
    int i, j;
    int index = 0;
    int stmp = 0;

    for (i = 0; i < num; i++) {
        if (strcmp(Use_name[i], name) == 0) {
            index = i;
        }
    }
    goto_xy(0, 40);
    //for (i = 0; i < num; i++) {
    //    printf("%s, %d\n", Use_name[i], DJ[index].resta[i]);  //다익스트라가 진행되었는지 확인하는 프로그램 ********************************************** 나중에 지워용!
    //}
    //dis[0] = index;

    for (i = 0; i < num; i++) {
        N[i].dis = DJ[index].resta[i];
        N[i].count = i;
    }

    for (j = 0; j < num; j++) {
        for (int k = j + 1; k < num; k++) {
            if (N[k].dis < N[j].dis) {
                stmp = N[j].dis;
                N[j].dis = N[k].dis;
                N[k].dis = stmp;

                stmp = N[j].count;
                N[j].count = N[k].count;
                N[k].count = stmp;
            }
        }
        final[j] = N[j].count;
    }

    if (strcmp(SCname[0], Use_name[final[0]]) != 0) {
        int k = 0;
        for (int j = 0; j < num; j++) {
            if (!(strcmp(SCname[8], Use_name[final[j]]) == 0 || strcmp(SCname[10], Use_name[final[j]]) == 0 || strcmp(SCname[3], Use_name[final[j]]) == 0)) {
                //목민관, 제1자연관, 제2학생회관은 제외하고 출력
                goto_xy(18, 12 + k);
                printf("%2d    %s", k + 1, Use_name[final[j]]);
                k = k + 1;
            }
        }
    }
    else {
        for (int j = 0; j < num; j++) {
            goto_xy(17, 12 + j + 1);
            printf("%2d     %s", j + 1, Use_name[final[j]]);
        }
    }
}

int main(void)
{
    UserFileOpen();
    //printf("유저 파일 열기 완료\n");
    restaurantFileOpen();
    //printf("식당 파일 열기 완료\n");
    rest_locationFileOpen();
    //printf("가중치 파일 열기 완료\n");

    menu_print();

    //system("pause");

    return 0;
}

//첫 번째 창(메인화면) 화면
void menu_print()
{
    int answer;

    square(44, 32, 10, 3);

    print_rogo();

    goto_xy(23, 17);
    printf("식당 추천 프로그램");

    square_char(24, 6, 20, 19, "1. 로그인");
    square_char(24, 6, 20, 26, "2. 회원가입");

    square(44, 2, 10, 36);

    goto_xy(12, 37);

    printf("메뉴 : ");
    scanf("%d", &answer);

    menu(answer);
}

void menu(int menu)
{
    if (menu == 1) {
                    login();
    }

    else if (menu == 2) {
        sign_up();
    }
    else
        printf("잘 못 입력하셨습니다.");
}

void login() {
    int i = 0;

    char id[20];

    system("cls");

    square(44, 32, 10, 3);
    
    goto_xy(39, 34);
    printf("되돌아가기 : @");

    goto_xy(29, 10);

    printf("로그인");

    square(34, 3, 15, 17);
    goto_xy(17, 18);

    printf("ID : ");
    scanf("%s", id);

    if (strcmp(id, "@") != 0) {
        for (i = 0; i < 50; i++) {
            if (strcmp(id, user[i].b.id) == 0) {
                check = i;
                break;
            }
        }
        if (check == -1) {
            goto_xy(12, 34);
            printf("아이디를 찾을 수 없습니다.");
            Sleep(1000);
            login();
        }

        if (check != -1) {
            while (1)
            {
                int ch = 0;
                char pw[20] = { 0 };
                char str[20] = { 0 };
                int num = 0;

                square(34, 3, 15, 21);
                goto_xy(17, 22);
                printf("PW : ");

                num = 0;

                while ((ch = _getch()) != '\r') {
                    str[num] = (char)ch;
                    if (str[num] == '\b') {
                        continue;
                    }
                    printf("*");
                    num++;
                }

                for (int i = 0; i < 20; i++) {
                    pw[i] = str[i];
                }

                if (strcmp(pw, "@") == 0) {
                    menu_print();
                }
                if (strcmp(user[check].b.pw, pw) == 0) {
                    goto_xy(12, 34);
                    printf("로그인 성공");
                    for (int i=0;i<20;i++) 
                        check_id[i] = id[i];
                    Sleep(1000);
                    break;
                }
                else {
                    goto_xy(12, 34);
                    printf("비밀번호가 틀렸습니다");
                    Sleep(1000);
                    goto_xy(12, 34);
                    printf("                      ");

                }
            }
        }
    }
    else
        menu_print();

    user_ui();
}

void sign_up() {
    int ch;
    char check_pw[20], name[20], phone_number[20], id[20], pw[20];

    UserFileOpen();

    system("cls");

    square(44, 32, 10, 3);

    goto_xy(39, 34);
    printf("되돌아가기 : @");

    goto_xy(23, 10);

    printf("   회원가입 화면");

    square(32, 3, 15, 17);
    goto_xy(17, 18);
    printf("Name : ");
    scanf("%s", name);

    if (strcmp(name, "@") == 0) {
        menu_print();
    }

    square(32, 3, 15, 21);
    goto_xy(17, 22);
    printf("Phone Number : ");

    if (strcmp(id, "@") != 0) {
        scanf("%s", phone_number);
    }
    else
        menu_print();

    while (1) {
        square(32, 3, 15, 25);
        goto_xy(17, 26);
        printf("ID : ");
        scanf("%s", id);

        if (strcmp(id, "@") == 0) {
            menu_print();
            return;
        }

        int id_exists = 0;
        for (int i = 0; i < 50; i++) {
            if (strcmp(id, user[i].b.id) == 0) {
                id_exists = 1;
                goto_xy(12, 34);
                printf("이미 존재하는 아이디입니다.");
                Sleep(1000);

                goto_xy(12, 34);
                printf("                                     ");

                goto_xy(39, 34);
                printf("되돌아가기 : @");
                break;
            }
        }

        if (!id_exists) {
            break;
        }
    }

    while (1) {
        int i = 0; // 인덱스 초기화
        square(32, 3, 15, 29);
        goto_xy(17, 30);
        printf("PASSWORD : ");

        while ((ch = _getch()) != '\r') {       // Enter 키가 입력될 때까지 반복
            if (ch == '\b') {                   // Backspace 키가 입력될 경우
                if (i > 0) {
                    i--;
                    printf("\b \b");            // Backspace를 입력하여 '*'를 지우고, 커서를 한 칸 왼쪽으로 이동
                }
            }
            else {
                pw[i++] = ch;
                printf("*");                    // 입력된 문자 대신 '*'를 출력
            }
        }
        pw[i] = '\0';              // 문자열 끝에 NULL 문자 추가

        goto_xy(17, 31);
        printf("CONFIRM  : ");
        i = 0;
        while ((ch = _getch()) != '\r') {       // Enter 키가 입력될 때까지 반복
            if (ch == '\b') {                   // Backspace 키가 입력될 경우
                if (i > 0) {
                    i--;
                    printf("\b \b");            // Backspace를 입력하여 '*'를 지우고, 커서를 한 칸 왼쪽으로 이동
                }
            }
            else {
                check_pw[i++] = ch;
                printf("*");                    // 입력된 문자 대신 '*'를 출력
            }
        }
        check_pw[i] = '\0';              // 문자열 끝에 NULL 문자 추가

        // strcmp 함수를 사용하여 비밀번호가 일치하는지 확인
        if (strcmp(pw, check_pw) == 0) {
            goto_xy(17, 33);
            printf("비밀번호가 일치합니다.          ");
            Sleep(1000);
            // 파일에 회원 정보 저장
            FILE* userFile = fopen("user.txt", "a"); // "a" 모드는 파일 끝에 추가하는 모드
            if (userFile != NULL) {
                fprintf(userFile, "%s %s %s %s\n", id, pw, name, phone_number);
                fclose(userFile);

                UserFileOpen();
            }
            else {
                printf("파일을 열 수 없습니다.\n");
            }

            break;
        }
        else {
            goto_xy(17, 33);
            printf("비밀번호가 일치하지 않습니다.");
        }
    }
    goto_xy(0, 45);
    menu_print();
}

void user_ui() {
    system("cls");

    int choice = -1;

    square(44, 32, 10, 3);

    print_rogo();

    goto_xy(29, 17);
    printf("개인화면");

    goto_xy(36, 34);
    printf("프로그램 종료 : 0");

    square_char(24, 6, 20, 19, "1. BookMark");
    square_char(24, 6, 20, 26, "2. 식당 추천 받기");

    square(44, 2, 10, 36);

    goto_xy(12, 37);

    printf("메뉴 : ");
    scanf("%d", &choice);

    if (choice == 1) {
        bookmark_ui();
    }

    else if (choice == 2) {
        user_location();
    }

    else if (choice == 0) {
        exit(0);
    }
}

void bookmark_ui() {
    BookmarkFileOpen();

    char bookmark_choice[5];
    system("cls");

    square(44, 32, 10, 3);

    goto_xy(39, 34);
    printf("되돌아가기 : @");

    goto_xy(30, 4);
    printf("북마크");

    square(36, 2, 14, 7);
    goto_xy(17, 8);
    printf("NO.    가게 이름        ");

    square(36, 20, 14, 10);

    int user_index = -1;
    for (int i = 0; i < 50; i++) {
        if (strcmp(user[i].b.id, check_id) == 0) {
            user_index = i;
            break;
        }
    }

    if (user_index != -1) {
        int j = 0;
        for (int k = 0; k < 20; k++) {
            if (strlen(user[user_index].bm[k].bookmark) > 0) {
                goto_xy(18, 11 + j);
                printf("%d     %s", j + 1, user[user_index].bm[k].bookmark);
                j++;
            }
        }
    }

    goto_xy(12, 34);
    printf("1: 추가, 2: 삭제, 3: 정보");

    square(44, 2, 10, 36);

    goto_xy(12, 37);

    printf("메뉴 : ");
    scanf("%s", bookmark_choice);

    if (strcmp(bookmark_choice, "@") != 0) {
        if (strcmp(bookmark_choice, "1") == 0) {
            bookmark_add();
        }
        else if (strcmp(bookmark_choice, "2") == 0) {
            bookmark_delete();
            goto_xy(12, 37);
            printf("메뉴 : ");
            scanf("%s", bookmark_choice);
        }
        else if (strcmp(bookmark_choice, "3") == 0) {
            goto_xy(12, 37);
            printf("가게 이름 : ");
            scanf("%s", bookmark_choice);

            int index = -1; // Initialize index to an invalid value
            for (int i = 0; i < MAX_REST; i++) {
                if (strcmp(bookmark_choice, rest[i].name) == 0) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                goto_xy(12, 37);
                printf("북마크 목록에 없는 가게입니다.");
                Sleep(600);
                goto_xy(12, 34);
                printf("                     ");
            }
            else {
                restarnt_information(index);

                square(44, 2, 10, 36);
                goto_xy(12, 37);
                printf("메뉴 : ");
                scanf("%s", bookmark_choice);
            }

            if (strcmp(bookmark_choice, "@") == 0) {
                bookmark_ui();
            }
        }
    }
    else {
        user_ui();
    }
}

void bookmark_add()
{
    char add_bk[100] = { 0 };

    system("cls");

    square(44, 32, 10, 3);

    goto_xy(27, 4);
    printf("북마크 추가");

    goto_xy(39, 34);
    printf("되돌아가기 : @");

    square(36, 2, 14, 7);
    goto_xy(16, 8);
    printf("NO.    가게 이름        ");

    square(36, 21, 14, 10);

    for (int i = 0; i < 9; i++) {
        goto_xy(17, 11 + i);
        printf("%d      %s", i + 1, rest[i + 1].name);
    }
    for (int i = 9; i < 20; i++) {
        goto_xy(16, 11 + i);
        printf("%d      %s", i + 1, rest[i + 1].name);
    }

    square(44, 2, 10, 36);

    while (1) {
        goto_xy(12, 37);

        printf("가게 이름 : ");
        scanf("%s", add_bk);

        int bk_ch = -1;

        if (strcmp(add_bk, "@") != 0) {
            int found = 0;
            for (int i = 0; i < MAX_REST; i++) {
                if (strcmp(add_bk, rest[i].name) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                goto_xy(12, 34);
                printf("목록에 없는 가게입니다.");
                Sleep(1000);
                goto_xy(12, 34);
                printf("                           ");
                goto_xy(12, 37);
                printf("                              ");
                continue; // 목록에 없는 가게이므로 다시 입력 받음
            }
            for (int i = 0; i < MAX_REST; i++) {
                if (strcmp(add_bk, user[check].bm[i].bookmark) == 0) {
                    goto_xy(12, 34);
                    printf("이미 추가된 가게입니다.");
                    Sleep(1000);
                    goto_xy(12, 37);
                    printf("                             ");
                    bk_ch = 1;

                    break;
                }
            }
            if (bk_ch == -1) {
                FILE* BookmarkFile = fopen("Bookmark.txt", "a"); // "a" 모드는 파일 끝에 추가하는 모드
                if (BookmarkFile != NULL) {
                    fprintf(BookmarkFile, "%s %s\n", check_id, add_bk);
                    fclose(BookmarkFile);

                    BookmarkFileOpen();

                    goto_xy(12, 34);
                    printf("추가되었습니다.");
                    Sleep(1000);

                    bookmark_ui();

                }
                else
                    printf("파일을 열 수 없습니다.\n");
            }
        }
        else
            bookmark_ui();
    }
}

void bookmark_delete()
{
    BookmarkFileOpen();

    char del_bk[100];

    goto_xy(12, 34);
    printf("                   ");

    goto_xy(27, 4);
    printf("북마크 삭제");

    square(44, 2, 10, 36);

    while (1) {
        goto_xy(12, 37);

        printf("삭제할 가게 : ");
        scanf("%s", del_bk);

        int bk_ch = -1;

        if (strcmp(del_bk, "@") != 0) {
            // 북마크 파일을 검사하여 삭제할 가게가 존재하는지 확인
            for (int i = 0; i < MAX_REST; i++) {
                if (strcmp(del_bk, user[check].bm[i].bookmark) == 0) {
                    bk_ch = 1; // 가게가 존재함을 표시
                    break;
                }
            }
            // 북마크 파일에 삭제할 가게가 존재하지 않는 경우
            if (bk_ch == -1) {
                goto_xy(12, 34);
                printf("목록에 없는 가게입니다.");
                Sleep(1000);
                goto_xy(12, 34);
                printf("                         ");
                goto_xy(12, 37);
                printf("                            ");
            }
            else { // 북마크 파일에 삭제할 가게가 존재하는 경우
                FILE* BookmarkFile = fopen("Bookmark.txt", "r+"); // "r+" 모드는 파일을 읽기/쓰기 모드로 열기
                if (BookmarkFile != NULL) {
                    FILE* tempFile = fopen("tempBookmark.txt", "w"); // 임시 파일을 쓰기 모드로 열기
                    if (tempFile != NULL) {
                        char line[256];
                        while (fgets(line, sizeof(line), BookmarkFile)) {
                            // 파일에서 한 줄씩 읽어와서 삭제할 가게와 일치하는 줄은 건너뛰고, 나머지는 임시 파일에 쓰기
                            if (strstr(line, check_id) != NULL && strstr(line, del_bk) != NULL) {
                                continue; // 삭제할 가게와 일치하는 줄은 건너뜀
                            }
                            fputs(line, tempFile);
                        }
                        fclose(tempFile);
                        fclose(BookmarkFile);

                        // 임시 파일을 원래 파일로 복사
                        remove("Bookmark.txt");
                        rename("tempBookmark.txt", "Bookmark.txt");

                        BookmarkFileOpen();

                        goto_xy(12, 34);
                        printf("삭제되었습니다.");
                        Sleep(1000);

                        bookmark_ui();
                    }
                    else {
                        printf("임시 파일을 열 수 없습니다.\n");
                    }
                }
                else {
                    printf("파일을 열 수 없습니다.\n");
                }
            }
        }
        else {
            bookmark_ui();
        }
    }
}

void user_location() {
    system("cls");

    square(44, 32, 10, 3);

    goto_xy(39, 34);
    printf("되돌아가기 : 0");

    goto_xy(27, 8);

    printf("사용자 위치");

    square(38, 15, 13, 12);
    goto_xy(17, 16);

    square(38, 2, 13, 7);
    goto_xy(17, 8);
    printf("NO.    학교 위치        ");

    /*for (int i = 0; i < 11; i++) {
        goto_xy(17, 16 + i);
        printf("%d. %s", i + 1, SCname[i]);
    }*/

    for (int i = 0; i < MAX_SCHOOL; i++) {
        daikstra(i, dj_school, MAX_SCHOOL);        //인덱스 값이 어디인지 확인하지 못하였기에 다익스트라 2차원 배열을 전부 돌려 값을 저장해주어야함!
    }
    Ques(SCname, "미래창조관", sc_final, SCH_N, dj_school, MAX_SCHOOL);  // ************************************************************ 나중에 지우세용! 다익스트라 돌아가는 것을 확인하기 위해 사용

    square(38, 2, 13, 31);
    goto_xy(16, 32);

    printf("위치 입력 : ");
    scanf("%d", &now_lo); //정렬을 사용하여 이미 정렬된 문자열에서 출력 후 입력 받으면 쉽게 할 수 있을 것으로 보임 -> 알고리즘 1개 추가할 수 있지 않을까?

    strcpy(user_now_location, SCname[now_lo - 1]); //정렬되지 않은 문자열에서 이미 코드 본문에 선언한 건물명을 선택한 건물명에 복사

    if (now_lo != 0) {
        recommend();               //user_now_location은 전역변수이기에 넘기지 않아도 다른 함수에서 저장된 값 그대로 함수 사용이 가능
    }
    else
        user_ui();
}

void recommend() {
    system("cls");

    int choice; // 단일 문자로 수정

    square(44, 32, 10, 3);

    goto_xy(39, 34);
    printf("되돌아가기 : 0");

    goto_xy(28, 8);
    printf("가게 추천");

    square(34, 2, 15, 21);
    goto_xy(17, 22);
    printf("1. 식당 추천");

    square(34, 2, 15, 25);
    goto_xy(17, 26);
    printf("2. 편의점 추천");

    square(34, 2, 15, 29);
    goto_xy(17, 30);
    printf("3. 카페 추천");

    square(44, 2, 10, 36);

    goto_xy(12, 37);

    printf("메뉴 : ");
    scanf("%d", &choice); // 단일 문자를 받도록 수정

    if (choice == 1) {
        recommend_rest();
    }
    else if (choice == 2) {
        recommend_store();
    }
    else if (choice == 3) {
        recommend_cafe();
    }
    else if (choice == 0) {
        user_ui();
    }
}

void recommend_rest() {
    char choice[20];

    system("cls");

    square(44, 32, 10, 3);

    goto_xy(39, 34);
    printf("되돌아가기 : @");

    goto_xy(28,7);
    printf("식당 추천");

    square(38, 2, 13, 7);
    goto_xy(17, 8);
    printf("NO.    가게 이름        ");

    square(38, 20, 13, 11);
    goto_xy(17, 17);

    // 2024.06.04 >> user_now_location 변수에 저장할 때 사용하였던 SCname의 인덱스를 사용해 보기로 함
    // daikstra(now_lo-1, dj_school, MAX_SCHOOL); //다익스트라 정렬을 하기 위한 user_now_location 변수의 인덱스 값을 찾지 못함! *********** new!

    for (int i = 0; i < MAX_REST; i++) {
        daikstra(i, dj_rest, MAX_REST);        //인덱스 값이 어디인지 확인하지 못하였기에 다익스트라 2차원 배열을 전부 돌려 값을 저장해주어야함!
    }

    //무조건 식당만을 안내해주는 함수이기에 현재 위치에서 우리가 알고있는 제1자연관과 제2학생회관의 가중치를 비교하여 더 짧은 곳으로 식당을 전개해주면 될 것?
    if (dj_school[now_lo - 1].resta[3] < dj_school[now_lo - 1].resta[10]) { //제2학생회관으로 가는 것이 더 빠를 경우
        Ques(Rname, "제2학생회관", r_final, R_N, dj_rest, MAX_REST);                                                     //이렇게 진행하였을 경우 전공 건물의 경우 출력 안 할 것인지 고민해 봐야함!
        //daikstra(0, dj_rest, MAX_REST); //식당 배열에 있는 제2학생회관의 인덱스를 찾아 다익스트라를 돌릴 것임!             ************************

    }
    else { //제1자연관으로 가는 것이 더 빠를 경우 // (dj_school[now_lo - 1].resta[3] > dj_school[now_lo - 1].resta[10])
        Ques(Rname, "제1자연관", r_final, R_N, dj_rest, MAX_REST);
    }
    //두 가중치가 같을 경우는 어떻게 할지 따로 생각해보는 것이 좋아보임!  ************************************** 이건 우리 팀이 아니라 제가 맘대로 배정 안했습니당
    int index = 0;
    while (1) {
        square(44, 2, 10, 36);
        goto_xy(12, 37);
        printf("메뉴 : ");

        scanf("%s", choice);

        if (strcmp(choice, "@") == 0) {
            recommend();
        }

        for (int i = 0; i < MAX_REST; i++) {
            if (strcmp(choice, rest[i].name) == 0) {
                index = i;
                break;
            }
        }

        if (index == 0) {
            goto_xy(12, 34);
            printf("잘못 입력하셨습니다.  ");
            Sleep(600);
            goto_xy(12, 34);
            printf("                     ");
            goto_xy(18, 37);
        }
        else break;
    }
    if (strcmp(choice, "@") != 0) {
        restarnt_information(index);

        square(44, 2, 10, 36);
        goto_xy(12, 37);
        printf("메뉴 : ");

        scanf("%s", choice);

        if (strcmp(choice, "@") == 0) {
            recommend_rest();
        }
    }
}

void recommend_store() {
    char choice[20];

    system("cls");

    square(44, 32, 10, 3);
    goto_xy(28, 7);
    printf("편의점 추천");

    square(38, 2, 13, 7);
    goto_xy(17, 8);
    printf("NO.    가게 이름        ");

    goto_xy(39, 34);
    printf("되돌아가기 : @");

    square(38, 20, 13, 11);
    goto_xy(17, 17);

    // 2024.06.04 >> user_now_location 변수에 저장할 때 사용하였던 SCname의 인덱스를 사용해 보기로 함
    // daikstra(now_lo-1, dj_school, MAX_SCHOOL); //다익스트라 정렬을 하기 위한 user_now_location 변수의 인덱스 값을 찾지 못함! *********** new!

    for (int i = 0; i < MAX_STORE; i++) {
        daikstra(i, dj_store, MAX_STORE);        //인덱스 값이 어디인지 확인하지 못하였기에 다익스트라 2차원 배열을 전부 돌려 값을 저장해주어야함!
    }

    //무조건 식당만을 안내해주는 함수이기에 현재 위치에서 우리가 알고있는 제1자연관과 제2학생회관의 가중치를 비교하여 더 짧은 곳으로 식당을 전개해주면 될 것?
    if (dj_school[now_lo - 1].resta[3] < dj_school[now_lo - 1].resta[10] && dj_school[now_lo - 1].resta[3] < dj_school[now_lo - 1].resta[8]) { //제2학생회관으로 가는 것이 더 빠를 경우
        Ques(STname, "제2학생회관", st_final, ST_N, dj_store, MAX_STORE);                                                     //이렇게 진행하였을 경우 전공 건물의 경우 출력 안 할 것인지 고민해 봐야함!
        //daikstra(0, dj_rest, MAX_REST); //식당 배열에 있는 제2학생회관의 인덱스를 찾아 다익스트라를 돌릴 것임!             ************************

    }
    else if (dj_school[now_lo - 1].resta[10] < dj_school[now_lo - 1].resta[3] && dj_school[now_lo - 1].resta[10] < dj_school[now_lo - 1].resta[8]) { //제2학생회관으로 가는 것이 더 빠를 경우
        Ques(STname, "제1자연관", st_final, ST_N, dj_store, MAX_STORE);                                                     //이렇게 진행하였을 경우 전공 건물의 경우 출력 안 할 것인지 고민해 봐야함!
        //daikstra(0, dj_rest, MAX_REST); //식당 배열에 있는 제2학생회관의 인덱스를 찾아 다익스트라를 돌릴 것임!             ************************

    }
    else { //제1자연관으로 가는 것이 더 빠를 경우 // (dj_school[now_lo - 1].resta[3] > dj_school[now_lo - 1].resta[10])
        Ques(STname, "목민관", st_final, ST_N, dj_store, MAX_STORE);
    }
    //두 가중치가 같을 경우는 어떻게 할지 따로 생각해보는 것이 좋아보임!  ************************************** 이건 우리 팀이 아니라 제가 맘대로 배정 안했습니당
    int index = 0;
    while (1) {
        square(44, 2, 10, 36);
        goto_xy(12, 37);
        printf("메뉴 : ");

        scanf("%s", choice);

        if (strcmp(choice, "@") == 0) {
            recommend();
        }

        for (int i = 0; i < MAX_REST; i++) {
            if (strcmp(choice, rest[i].name) == 0) {
                index = i;
                break;
            }
        }

        if (index == 0) {
            goto_xy(12, 34);
            printf("잘못 입력하셨습니다.  ");
            Sleep(600);
            goto_xy(12, 34);
            printf("                     ");
            goto_xy(18, 37);
        }
        else break;


    }
    if (strcmp(choice, "@") != 0) {
        restarnt_information(index);

        square(44, 2, 10, 36);
        goto_xy(12, 37);
        printf("메뉴 : ");

        scanf("%s", choice);

        if (strcmp(choice, "@") == 0) {
            recommend_rest();
        }
    }
}

void recommend_cafe() {
    char choice[20];

    system("cls");

    square(44, 32, 10, 3);
    goto_xy(28, 7);
    printf("카페 추천");

    square(38, 2, 13, 7);
    goto_xy(17, 8);
    printf("NO.    가게 이름        ");

    goto_xy(39, 34);
    printf("되돌아가기 : @");

    square(38, 20, 13, 11);
    goto_xy(17, 17);

    // 2024.06.04 >> user_now_location 변수에 저장할 때 사용하였던 SCname의 인덱스를 사용해 보기로 함
    // daikstra(now_lo-1, dj_school, MAX_SCHOOL); //다익스트라 정렬을 하기 위한 user_now_location 변수의 인덱스 값을 찾지 못함! *********** new!

    for (int i = 0; i < MAX_CAFE; i++) {
        daikstra(i, dj_cafe, MAX_CAFE);        //인덱스 값이 어디인지 확인하지 못하였기에 다익스트라 2차원 배열을 전부 돌려 값을 저장해주어야함!
    }

    //무조건 식당만을 안내해주는 함수이기에 현재 위치에서 우리가 알고있는 제1자연관과 제2학생회관의 가중치를 비교하여 더 짧은 곳으로 식당을 전개해주면 될 것?
    if (dj_school[now_lo - 1].resta[3] < dj_school[now_lo - 1].resta[10] && dj_school[now_lo - 1].resta[3] < dj_school[now_lo - 1].resta[8]) { //제2학생회관으로 가는 것이 더 빠를 경우
        Ques(Cname, "제2학생회관", c_final, C_N, dj_cafe, MAX_CAFE);                                                     //이렇게 진행하였을 경우 전공 건물의 경우 출력 안 할 것인지 고민해 봐야함!
        //daikstra(0, dj_rest, MAX_REST); //식당 배열에 있는 제2학생회관의 인덱스를 찾아 다익스트라를 돌릴 것임!             ************************

    }
    else if (dj_school[now_lo - 1].resta[10] < dj_school[now_lo - 1].resta[3] && dj_school[now_lo - 1].resta[10] < dj_school[now_lo - 1].resta[8]) { //제2학생회관으로 가는 것이 더 빠를 경우
        Ques(Cname, "제1자연관", c_final, C_N, dj_cafe, MAX_CAFE);                                                     //이렇게 진행하였을 경우 전공 건물의 경우 출력 안 할 것인지 고민해 봐야함!
        //daikstra(0, dj_rest, MAX_REST); //식당 배열에 있는 제2학생회관의 인덱스를 찾아 다익스트라를 돌릴 것임!             ************************

    }
    else { //제1자연관으로 가는 것이 더 빠를 경우 // (dj_school[now_lo - 1].resta[3] > dj_school[now_lo - 1].resta[10])
        Ques(Cname, "목민관", c_final, C_N, dj_cafe, MAX_CAFE);
    }
    //두 가중치가 같을 경우는 어떻게 할지 따로 생각해보는 것이 좋아보임!  ************************************** 이건 우리 팀이 아니라 제가 맘대로 배정 안했습니당
    ////목민관 8, 제2학생회관 3, 인문사회관 6, 제1자연관 10
    int index = 0;
    while (1) {
        square(44, 2, 10, 36);
        goto_xy(12, 37);
        printf("메뉴 : ");

        scanf("%s", choice);

        if (strcmp(choice, "@") == 0) {
            recommend();
        }

        for (int i = 0; i < MAX_REST; i++) {
            if (strcmp(choice, rest[i].name) == 0) {
                index = i;
                break;
            }
        }

        if (index == 0) {
            goto_xy(12, 34);
            printf("잘못 입력하셨습니다.  ");
            Sleep(600);
            goto_xy(12, 34);
            printf("                     ");
            goto_xy(18, 37);
        }
        else break;


    }
    if (strcmp(choice, "@") != 0) {
        restarnt_information(index);

        square(44, 2, 10, 36);
        goto_xy(12, 37);
        printf("메뉴 : ");

        scanf("%s", choice);

        if (strcmp(choice, "@") == 0) {
            recommend_rest();
        }
    }
}

void restarnt_information(int index) {
    char choice[20];
    char loca[100]; // 주소를 저장할 임시 배열

    system("cls");

    square(44, 32, 10, 3);
    goto_xy(28, 6);
    printf("가게 정보");

    goto_xy(39, 34);
    printf("되돌아가기 : @");

    square(36, 20, 14, 10);

    goto_xy(16, 12);
    printf("이     름: %s", rest[index].name);

    goto_xy(16, 14);
    printf("위     치: ");

    int row = 14;
    int col = 27;
    int len = strlen(rest[index].loca);

    // 주소를 16글자씩 출력
    for (int i = 0; i < len; i += 16) {
        goto_xy(col, row);
        strncpy(loca, &rest[index].loca[i], 16);
        loca[16] = '\0'; // 문자열 끝에 NULL 문자를 추가
        printf("%s", loca);
        row++; // 다음 줄로 이동
    }

    goto_xy(16, row + 1);
    printf("전화 번호: %s", rest[index].number);

    goto_xy(16, row + 3);
    printf("오픈 시간: %s", rest[index].open);

    goto_xy(16, row + 5);
    printf("마감 시간: %s", rest[index].close);

    goto_xy(16, row + 7);
    printf("휴 무 일 : %s", rest[index].CT);
}
