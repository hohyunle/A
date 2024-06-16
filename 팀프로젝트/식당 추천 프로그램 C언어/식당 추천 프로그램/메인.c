#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_RESTAURANT_SIZE 60 //����ġ ���� 53, �ǹ� ���� 39�̹Ƿ� �ִ� 53���� �ص� ���ư� ������ ����
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

//ac user[50]; //���� �ҷ����� �뵵�� ����ü

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
    char departure[40]; //��������
    char arrival[40];   //��������
    int distance;
} us;

//�б� ����ġ ����
#define MAX_SCHOOL_DIS 20
//�Ĵ� ����ġ ����
#define MAX_REST_DIS 18
//������ ����ġ ����
#define MAX_STORE_DIS 10
//ī�� ����ġ ����
#define MAX_CAFE_DIS 10

//�б� �ǹ� ���� (�б� 11)
#define MAX_SCHOOL 11
//�Ĵ� �ǹ� ���� (�Ĵ� 16 + �б� 2) //��1�ڿ���, ��2�л�ȸ��
#define MAX_REST 18
//������ �ǹ� ���� (������ 6 + �б� 3) //��2�л�ȸ��, ��ΰ�, ��1�ڿ���
#define MAX_STORE 9
//ī�� �ǹ� ���� (ī�� 7 + �б� 3) //��ΰ�, ��2�л�ȸ��, ��1�ڿ���
#define MAX_CAFE 10

us store[MAX_RESTAURANT_SIZE]; //��ü ����ġ ���� ����ü
us school[MAX_SCHOOL_DIS]; // �б� ����ġ ���� ����ü -> �б� ���ͽ�Ʈ�󿡼� ���
us re_loc[MAX_REST_DIS]; // �Ĵ� ����ġ ���� ����ü
us st_loc[MAX_STORE_DIS]; // ������ ����ġ ���� ����ü
us ca_loc[MAX_CAFE_DIS]; // ī�� ����ġ ���� ����ü

int maxIndex = -1;
typedef struct djk {
    int resta[MAX_RESTAURANT_SIZE];        // ���ͽ�Ʈ�� ������ �迭
    int v;                                 // �湮�� ���
    int d;                                 // �Ÿ� ������ �迭
} DJK;

DJK dj_school[MAX_SCHOOL]; //�б� ���ͽ�Ʈ�� 11*11 �迭
DJK dj_rest[MAX_REST]; //�Ĵ� ���ͽ�Ʈ�� 17*17 �迭
DJK dj_store[MAX_STORE]; //������ ���ͽ�Ʈ�� 9*9 �迭
DJK dj_cafe[MAX_CAFE]; //ī�� ���ͽ�Ʈ�� 11*11 �迭

//���ͽ�Ʈ�� -> ���ͽ�Ʈ��

char user_now_location[30];

//�� �ǹ� �̸� 39��
char Aname[MAX_RESTAURANT_SIZE][MAX_RESTAURANT_SIZE] = { "�̷�â����", "��2�ڿ���", "������","��2�л�ȸ��","������","������","�ι���ȸ��","��1�л�ȸ��", "��ΰ�","���̿����հ�","��1�ڿ���",
                                                         "�뷮���Ź�","���ɿ���","��ȫ�����屹","������ġ��������","��Ĵ�","�ٴٵ���","�����޲ٹ�","������������","�����긮Ƽ����","�Ե�����",
                                                         "�ӽÿ���Į����","�հ���«��","�Ӹ�����", "��Ű������", "�����ϸ�����", "�ͽ����",
                                                         "GS25�������Ĺ���","�����Ϸ��켭������","CUû�ּ�������","GS25�������л���","CUû�ּ����뽺Ÿ��","GS25�����ູ��",
                                                         "���ٹ�","�ް�Ŀ��","ī���̷��","��������","����","������Ŀ��","�̵��Ŀ��" };

//�б� �ǹ� �̸� 11��
char SCname[MAX_SCHOOL][MAX_RESTAURANT_SIZE] = { "�̷�â����", "��2�ڿ���", "������","��2�л�ȸ��",
                                                  "������","������","�ι���ȸ��","��1�л�ȸ��",
                                                  "��ΰ�","���̿����հ�","��1�ڿ���", };

//�Ĵ� �ǹ� �̸� 17��
char Rname[MAX_REST][MAX_RESTAURANT_SIZE] = { "��2�л�ȸ��","��1�ڿ���","�뷮���Ź�","���ɿ���",
                                              "��ȫ�����屹","������ġ��������","��Ĵ�","�ٴٵ���",
                                              "�����޲ٹ�","������������","�����긮Ƽ����","�Ե�����",
                                              "�ӽÿ���Į����","�հ���«��","�Ӹ�����", "��Ű������",
                                              "�����ϸ�����", "�ͽ����" };

//������ �ǹ� �̸� 9��
char STname[MAX_STORE][MAX_RESTAURANT_SIZE] = { "��2�л�ȸ��","��ΰ�","��1�ڿ���","GS25�������Ĺ���",
                                                "�����Ϸ��켭������","CUû�ּ�������","GS25�������л���","CUû�ּ����뽺Ÿ��","GS25�����ູ��" };

//ī�� �ǹ� �̸� 11��
char Cname[MAX_CAFE][MAX_RESTAURANT_SIZE] = { "��2�л�ȸ��","��ΰ�","��1�ڿ���",
                                              "���ٹ�","�ް�Ŀ��","ī���̷��","��������",
                                              "����","������Ŀ��","�̵��Ŀ��" };



int now_lo = 0; // ���� ���� ���� ��ġ���� �ε����� ��� �� ����

void goto_xy(int x, int y);
void square(int width, int height, int x, int y);
void square_char(int width, int height, int x, int y, char* msg);
void square3(int width, int height, int x, int y);
void print_rogo();

int getSmallIndex(DJK DJ, int num);
void daikstra(int start, DJK DJ, int num);

void UserFileOpen(void);                //����� ���� ���� ���� �Լ�
void restaurantFileOpen(void);          //������� ���� ����
void BookmarkFileOpen(void);            //�ϸ�ũ ���� 
void rest_locationFileOpen(void);       //�Ĵ� ��ġ ���� ����

void menu(int menu);                    //�޴� �Լ�(�α���, ȸ������)
void menu_print();                      //���� ȭ�� UI
void login();                           //�α��� ȭ�� UI
void sign_up();                         //ȸ������ ȭ��

void user_ui();                         //����� ���� ȭ�� UI(�ϸ�ũ, �Ĵ� ��õ)

void bookmark_ui();                     //�ϸ�ũ ȭ�� UI
void user_location();                   //����� ��ġ �Է� �޴� UI

void bookmark_menu();                   //�ϸ�ũ ��ȸ
void bookmark_add();                    //�ϸ�ũ �߰�
void bookmark_delete();                 //�ϸ�ũ ����

void recommend();      //�Ĵ� ��õ �޴� UI(�Ĵ�, ����, ī��, ������ ��õ)
void recommend_rest();                  //�Ĵ� ��õ �ޱ�
void recommend_store();                   //���� ��õ �ޱ�
void recommend_cafe();                  //ī�� ��õ �ޱ�

void restarnt_information(int index);

int getSchooLIndex(const char* buildName); //�б� �ǹ��� �̸��� ���޹����� �ǹ��� �ش��ϴ� ���� ����
int getRestIndex(const char* buildName); ////�Ĵ� �ǹ��� �̸��� ���޹����� �ǹ��� �ش��ϴ� ���� ����
int getStoreIndex(const char* buildName); //������ �ǹ��� �̸��� ���޹����� �ǹ��� �ش��ϴ� ���� ����
int getCafeIndex(const char* buildName); //ī�� �ǹ��� �̸��� ���޹����� �ǹ��� �ش��ϴ� ���� ����

//��ǥ�� �̵��ϴ� ��
void goto_xy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}
//���� �׸��� �Լ�(����, ����, ���� �𼭸� �� ��ġ, )
void square(int width, int height, int x, int y)
{
    goto_xy(x, y);
    for (int i = 0; i < width; i++) {   //ù ���� ���� �׸���
        if (i == 0) printf("��");
        else if (i == width - 1) printf("��\n");
        else  printf("��");
    }
    goto_xy(x, y + 1);
    for (int j = 0; j < height; j++) {  //���� ���� �׸���
        printf("��");
        for (int k = 0; k < width - 2; k++) {
            printf(" ");
        }
        printf("��\n");
        goto_xy(x, y + (j + 1));
    }
    for (int m = 0; m < width; m++) {   //������ ���� ���� �׸���
        if (m == 0) printf("��");
        else if (m == width - 1) printf("��\n");
        else printf("��");
    }
}
//���� �׸��� �Լ�(����, ����, x��ǥ, y��ǥ, ���� ���� �ڽ� ��� ������ ���ִ� �Լ�)
void square_char(int width, int height, int x, int y, char* msg)
{
    goto_xy(x, y);
    for (int i = 0; i < width; i++) {
        if (i == 0) printf("��");
        else if (i == width - 1) printf("��\n");
        else  printf("��");
    }
    goto_xy(x, y + 1);
    for (int j = 0; j < height; j++) {
        printf("��");
        for (int k = 0; k < width - 2; k++) {
            if (j == height / 2 && k == ((width)-strlen(msg)) / 2) {
                goto_xy(x + ((width)-strlen(msg)) / 2 + 1, y + height / 2);
                printf("%s", msg);
                goto_xy(x + width - 1, y + j);
            }
            else if (j != height / 2) printf(" ");
        }

        printf("��");
        goto_xy(x, y + (j + 1));
    }
    for (int m = 0; m < width; m++) {
        if (m == 0) printf("��");
        else if (m == width - 1) printf("��\n");
        else printf("��");
    }
}
//�ڽ� �� ���� ������ �� ��ġ�� ���� ������, �ڽ� �ȿ� �ڽ��� �־��ִ� �Լ�
void square3(int width, int height, int x, int y)
{
    goto_xy(x, y);
    for (int i = 0; i < width; i++) {
        if (i == 0) printf("��");
        else if (i == width - 1) printf("��\n");
        else  printf("��");
    }
    goto_xy(x, y + 1);
    for (int j = 0; j < height; j++) {
        printf("��");
        for (int k = 0; k < width - 2; k++) {
            printf(" ");
        }
        printf("��\n");
        goto_xy(x, y + (j + 1));
    }
    for (int m = 0; m < width; m++) {
        if (m == 0) printf("��");
        else if (m == width - 1) printf("��\n");
        else printf("��");
    }
}
void print_rogo()
{
    goto_xy(19, 5);
    printf("     ���      ��  ���  ��\n");
    goto_xy(19, 6);
    printf("   �����    ��  ���  ��\n");
    goto_xy(19, 7);
    printf(" �������  ��  ���  ��\n");
    goto_xy(19, 8);
    printf(" �������  �������\n");
    goto_xy(19, 9);
    printf(" �������    �����\n");
    goto_xy(19, 10);
    printf("   �����        ���\n");
    goto_xy(19, 11);
    printf("     ���          ���\n");
    goto_xy(19, 12);
    printf("     ���          ���\n");
    goto_xy(19, 13);
    printf("     ���          ���\n");
    goto_xy(19, 14);
    printf("     ���          ���\n");
    goto_xy(19, 15);
    printf("     ���          ���\n");
}

//ȸ�� ���� ���� �б�
void UserFileOpen(void)
{
    userFile = fopen("user.txt", "r");
    if (userFile == NULL) {
        printf("������ �� �� �����ϴ�.\n");
    } //������ �������� ���� ��쿡 ��µǴ� ��¹�

    int i = 0;
    while (fscanf(userFile, "%s %s %s %s", user[i].b.id, user[i].b.pw, user[i].b.name, user[i].b.phone_number) != EOF) {
        maxIndex = i;
        i++;
        if (i >= 50) break;
    } //���Ͽ��� ���� ���� ���� ������ or 50�� �̻��� ��� while���� �������´�.
    //scnaf�� ����� ���� �� ���⿡ ����� �����Ͽ� ���Ͽ� �ִ� ���� ������ ����

    fclose(userFile); //����ü�� ���� �����߱⿡ ������ ���� ����� �ʿ䰡 ����.
}
//�Ĵ� ���� ���� �б�
void restaurantFileOpen(void) {
    restaurantFile = fopen("restaurant.txt", "r");
    if (restaurantFile == NULL) {
        printf("������ �� �� �����ϴ�.\n");
    } //������ �������� ���� ��쿡 ��µǴ� ��¹�

    int i = 0;
    while (fscanf(restaurantFile, "%s %s %s %s %s %s %s", rest[i].group, rest[i].name, rest[i].loca, rest[i].number, rest[i].open, rest[i].close, rest[i].CT) != EOF) {
        maxIndex = i;
        i++;
        if (i >= 50) break;
    } //���Ͽ��� ���� ���� ���� ������ or 50�� �̻��� ��� while���� �������´�.
    //scnaf�� ����� ���� �� ���⿡ ����� �����Ͽ� ���Ͽ� �ִ� ���� ������ ����

    fclose(restaurantFile); //����ü�� ���� �����߱⿡ ������ ���� ����� �ʿ䰡 ����.
}
//�ϸ�ũ ���� ���� �б�
void BookmarkFileOpen(void) {
    FILE* BookmarkFile;
    char id[20];
    char bookmark[100];

    BookmarkFile = fopen("Bookmark.txt", "r");
    if (BookmarkFile == NULL) {
        printf("������ �� �� �����ϴ�.\n");
        return;
    }

    int i = check; // �α����� ������� �ε���
    int j = 0;
    // �α����� ������� �ϸ�ũ �迭 �ʱ�ȭ
    memset(user[i].bm, 0, sizeof(user[i].bm));

    while (fscanf(BookmarkFile, "%s %s", id, bookmark) != EOF) {
        if (strcmp(id, check_id) == 0) {
            strcpy(user[i].bm[j].id, id);
            strcpy(user[i].bm[j].bookmark, bookmark);
            j++;
            if (j >= 20) break; // �� ������� �ִ� �ϸ�ũ ���� ���� �ʵ��� ����
        }
    }

    fclose(BookmarkFile);
}

//����ġ ���� ���� �б�
void rest_locationFileOpen(void) {
    rest_locationFile = fopen("rest_location.txt", "r");
    if (rest_locationFile == NULL) {
        printf("������ �� �� �����ϴ�.\n");
    }

    //printf("������ �������ϴ�.\n");

    int i = 0, j = 0;
    while (fscanf(rest_locationFile, "%s %s %s %d", store[i].type, store[i].departure, store[i].arrival, &store[i].distance) != EOF) { //���� �ּҿ����� ����ߵſ�!
        //maxIndex = i;
        i++;
        if (i >= MAX_RESTAURANT_SIZE) break;
    }

    //printf("������ �о����ϴ�.\n");

    //��ü ����ġ ���Ͽ��� �ʿ��� �κ����� ���ġ ���ִ� �ڵ�
    // ************************************************************************************ �Ƹ� �� �� �а� �� �� ������ ���̳�, �ǹ� ������ ���� ��� ���� �ʿ�

    //�� �ǹ� ���ͽ�Ʋ ���� ���� �ε���
    //�б� ����ġ ���� 20 / �Ĵ� ����ġ ���� 15 / ������ ����ġ ���� 9 / ī�� ����ġ ���� 9 ��ŭ ����� ��
    int sch_index = 0, re_index = 0, st_index = 0, ca_index = 0;
    for (int i = 0; i < MAX_RESTAURANT_SIZE; i++) {
        if (strcmp(store[i].type, "��") == 0) {
            strcpy(school[sch_index].type, store[i].type);                  //strcpy(���� ���ڿ�, ����� ���ڿ�)���� �Ǵ��� �� �ۼ�, Ʋ�� ��� ���� �ʿ�
            strcpy(school[sch_index].departure, store[i].departure);
            strcpy(school[sch_index].arrival, store[i].arrival);
            school[sch_index].distance = store[i].distance;
            sch_index++;
        }
        if (strcmp(store[i].type, "��") == 0) {
            strcpy(re_loc[re_index].type, store[i].type);
            strcpy(re_loc[re_index].departure, store[i].departure);
            strcpy(re_loc[re_index].arrival, store[i].arrival);
            re_loc[re_index].distance = store[i].distance;
            re_index++;
        }
        else if (strcmp(store[i].type, "��") == 0) {
            strcpy(st_loc[st_index].type, store[i].type);
            strcpy(st_loc[st_index].departure, store[i].departure);
            strcpy(st_loc[st_index].arrival, store[i].arrival);
            st_loc[st_index].distance = store[i].distance;
            st_index++;
        }
        else if (strcmp(store[i].type, "ī") == 0) {
            strcpy(ca_loc[ca_index].type, store[i].type);
            strcpy(ca_loc[ca_index].departure, store[i].departure);
            strcpy(ca_loc[ca_index].arrival, store[i].arrival);
            ca_loc[ca_index].distance = store[i].distance;
            ca_index++;
        }
    }

    //printf("����ü�� �и��Ͽ����ϴ�.\n");

    fclose(rest_locationFile);

    //����ġ ���� �а� �������ִ� �Լ��� ���ҿ�............. �̤̤̤�

    for (i = 0; i < MAX_SCHOOL; i++) {
        for (j = 0; j < MAX_SCHOOL; j++) {
            dj_school[i].resta[j] = INF;
        }
        dj_school[i].resta[i] = 0;
    } //�б� ���ͽ�Ʈ�� 2���� �迭�� INF(���Ѵ�)�� �ʱ�ȭ / �ڱ� �ڽŰ��� �̵� ����ġ�� 0���� ����

    for (i = 0; i < MAX_REST; i++) {
        for (j = 0; j < MAX_REST; j++) {
            dj_rest[i].resta[j] = INF;
        }
        dj_rest[i].resta[i] = 0;
    } //�Ĵ� ���ͽ�Ʈ�� 2���� �迭�� INF(���Ѵ�)�� �ʱ�ȭ / �ڱ� �ڽŰ��� �̵� ����ġ�� 0���� ����
    //int SCH1_Index = getSchooLIndex("��2�л�ȸ��"); //�迭�� �ε����� ����ϱ� ���� �������� �������ִ� �Լ� ���
    //int Re1_Index = getRestIndex("��2�л�ȸ��");
    //int SCH2_Index = getSchooLIndex("��1�ڿ���");
    //int Re2_Index = getRestIndex("��1�ڿ���");
    //dj_rest[Re1_Index].resta[Re1_Index] = dj_school[SCH1_Index].resta[SCH1_Index];
    //dj_rest[Re2_Index].resta[Re2_Index] = dj_school[SCH2_Index].resta[SCH2_Index];
    //���� �� �ʿ��� �� ���⵵ �ϰ�....?

    for (i = 0; i < MAX_STORE; i++) {
        for (j = 0; j < MAX_STORE; j++) {
            dj_store[i].resta[j] = INF;
        }
        dj_store[i].resta[i] = 0;
    } //������ ���ͽ�Ʈ�� 2���� �迭�� INF(���Ѵ�)�� �ʱ�ȭ / �ڱ� �ڽŰ��� �̵� ����ġ�� 0���� ����

    for (i = 0; i < MAX_CAFE; i++) {
        for (j = 0; j < MAX_CAFE; j++) {
            dj_cafe[i].resta[j] = INF;
        }
        dj_cafe[i].resta[i] = 0;
    } //ī�� ���ͽ�Ʈ�� 2���� �迭�� INF(���Ѵ�)�� �ʱ�ȭ / �ڱ� �ڽŰ��� �̵� ����ġ�� 0���� ����

    ////�б� ����ġ ����
    //#define MAX_SCHOOL_DIS 20
    ////�Ĵ� ����ġ ����
    //#define MAX_REST_DIS 15
    ////������ ����ġ ����
    //#define MAX_STORE_DIS 9
    ////ī�� ����ġ ����
    //#define MAX_CAFE_DIS 9

    //us store[MAX_RESTAURANT_SIZE]; //��ü ����ġ ���� ����ü
    //us school[MAX_SCHOOL_DIS]; // �б� ����ġ ���� ����ü -> �б� ���ͽ�Ʈ�󿡼� ���
    //us re_loc[MAX_REST_DIS]; // �Ĵ� ����ġ ���� ����ü
    //us st_loc[MAX_STORE_DIS]; // ������ ����ġ ���� ����ü
    //us ca_loc[MAX_CAFE_DIS]; // ī�� ����ġ ���� ����ü

    for (int i = 0; i < MAX_SCHOOL_DIS; i++) {
        int firstIndex = getSchooLIndex(school[i].departure); //�迭�� �ε����� ����ϱ� ���� �������� �������ִ� �Լ� ���
        int finalIndex = getSchooLIndex(school[i].arrival);

        if (firstIndex != -1 && finalIndex != -1) { //�� �ε��� ���� ���������� ���� ��
            dj_school[firstIndex].resta[finalIndex] = school[i].distance;
            dj_school[finalIndex].resta[firstIndex] = school[i].distance;
        }
    }
    for (int i = 0; i < MAX_REST_DIS; i++) {
        int firstIndex = getRestIndex(re_loc[i].departure); //�迭�� �ε����� ����ϱ� ���� �������� �������ִ� �Լ� ���
        int finalIndex = getRestIndex(re_loc[i].arrival);

        if (firstIndex != -1 && finalIndex != -1) { //�� �ε��� ���� ���������� ���� ��
            dj_rest[firstIndex].resta[finalIndex] = re_loc[i].distance;
            dj_rest[finalIndex].resta[firstIndex] = re_loc[i].distance;
        }
    }
    for (int i = 0; i < MAX_STORE_DIS; i++) {
        int firstIndex = getStoreIndex(st_loc[i].departure); //�迭�� �ε����� ����ϱ� ���� �������� �������ִ� �Լ� ���
        int finalIndex = getStoreIndex(st_loc[i].arrival);

        if (firstIndex != -1 && finalIndex != -1) { //�� �ε��� ���� ���������� ���� ��
            dj_store[firstIndex].resta[finalIndex] = st_loc[i].distance;
            dj_store[finalIndex].resta[firstIndex] = st_loc[i].distance;
        }
    }
    for (int i = 0; i < MAX_CAFE_DIS; i++) {
        int firstIndex = getCafeIndex(ca_loc[i].departure); //�迭�� �ε����� ����ϱ� ���� �������� �������ִ� �Լ� ���
        int finalIndex = getCafeIndex(ca_loc[i].arrival);

        if (firstIndex != -1 && finalIndex != -1) { //�� �ε��� ���� ���������� ���� ��
            dj_cafe[firstIndex].resta[finalIndex] = ca_loc[i].distance;
            dj_cafe[finalIndex].resta[firstIndex] = ca_loc[i].distance;
        }
    }
}

////������ �ǹ� �̸� 9��
//char STname[MAX_STORE][MAX_RESTAURANT_SIZE] = { "��2�л�ȸ��","��ΰ�","��1�ڿ���","GS25�������Ĺ���",
//                                                "�����Ϸ��켭������","CUû�ּ�������","GS25�������л���","CUû�ּ����뽺Ÿ��","GS25�����ູ��" };
//
////ī�� �ǹ� �̸� 11��
//char Cname[MAX_CAFE][MAX_RESTAURANT_SIZE] = { "��2�л�ȸ��","�ι���ȸ��","��ΰ�","��1�ڿ���",
//                                              "���ٹ�","�ް�Ŀ��","ī���̷��","��������",
//                                              "����","������Ŀ��","�̵��Ŀ��" };

//ī�� �ǹ��� �̸��� ���޹����� �ǹ��� �ش��ϴ� ���� ����
int getCafeIndex(const char* buildName) {
    if (strcmp(buildName, "��2�л�ȸ��") == 0)
        return 0;
    else if (strcmp(buildName, "��ΰ�") == 0)
        return 1;
    else if (strcmp(buildName, "��1�ڿ���") == 0)
        return 2;
    else if (strcmp(buildName, "���ٹ�") == 0)
        return 3;
    else if (strcmp(buildName, "�ް�Ŀ��") == 0)
        return 4;
    else if (strcmp(buildName, "ī���̷��") == 0)
        return 5;
    else if (strcmp(buildName, "��������") == 0)
        return 6;
    else if (strcmp(buildName, "����") == 0)
        return 7;
    else if (strcmp(buildName, "������Ŀ��") == 0)
        return 8;
    else if (strcmp(buildName, "�̵��Ŀ��") == 0)
        return 9;
    else
        return -1; // ��ġ�ϴ� �ǹ����� ���� ���
}

//������ �ǹ��� �̸��� ���޹����� �ǹ��� �ش��ϴ� ���� ����
int getStoreIndex(const char* buildName) {
    if (strcmp(buildName, "��2�л�ȸ��") == 0)
        return 0;
    else if (strcmp(buildName, "��ΰ�") == 0)
        return 1;
    else if (strcmp(buildName, "��1�ڿ���") == 0)
        return 2;
    else if (strcmp(buildName, "GS25�������Ĺ���") == 0)
        return 3;
    else if (strcmp(buildName, "�����Ϸ��켭������") == 0)
        return 4;
    else if (strcmp(buildName, "CUû�ּ�������") == 0)
        return 5;
    else if (strcmp(buildName, "GS25�������л���") == 0)
        return 6;
    else if (strcmp(buildName, "CUû�ּ����뽺Ÿ��") == 0)
        return 7;
    else if (strcmp(buildName, "GS25�����ູ��") == 0)
        return 8;
    else
        return -1; // ��ġ�ϴ� �ǹ����� ���� ���
}

//�Ĵ� �ǹ��� �̸��� ���޹����� �ǹ��� �ش��ϴ� ���� ����
int getRestIndex(const char* buildName) {
    if (strcmp(buildName, "��2�л�ȸ��") == 0)
        return 0;
    else if (strcmp(buildName, "��1�ڿ���") == 0)
        return 1;
    else if (strcmp(buildName, "�뷮���Ź�") == 0)
        return 2;
    else if (strcmp(buildName, "���ɿ���") == 0)
        return 3;
    else if (strcmp(buildName, "��ȫ�����屹") == 0)
        return 4;
    else if (strcmp(buildName, "������ġ��������") == 0)
        return 5;
    else if (strcmp(buildName, "��Ĵ�") == 0)
        return 6;
    else if (strcmp(buildName, "�ٴٵ���") == 0)
        return 7;
    else if (strcmp(buildName, "�����޲ٹ�") == 0)
        return 8;
    else if (strcmp(buildName, "������������") == 0)
        return 9;
    else if (strcmp(buildName, "�����긮Ƽ����") == 0)
        return 10;
    else if (strcmp(buildName, "�Ե�����") == 0)
        return 11;
    else if (strcmp(buildName, "�ӽÿ���Į����") == 0)
        return 12;
    else if (strcmp(buildName, "�հ���«��") == 0)
        return 13;
    else if (strcmp(buildName, "�Ӹ�����") == 0)
        return 14;
    else if (strcmp(buildName, "��Ű������") == 0)
        return 15;
    else if (strcmp(buildName, "�����ϸ�����") == 0)
        return 16;
    else if (strcmp(buildName, "�ͽ����") == 0)
        return 17;
    else
        return -1; // ��ġ�ϴ� �ǹ����� ���� ���
}

//�б� �ǹ��� �̸��� ���޹����� �ǹ��� �ش��ϴ� ���� ����
int getSchooLIndex(const char* buildName) {
    if (strcmp(buildName, "�̷�â����") == 0)
        return 0;
    else if (strcmp(buildName, "��2�ڿ���") == 0)
        return 1;
    else if (strcmp(buildName, "������") == 0)
        return 2;
    else if (strcmp(buildName, "��2�л�ȸ��") == 0)
        return 3;
    else if (strcmp(buildName, "������") == 0)
        return 4;
    else if (strcmp(buildName, "������") == 0)
        return 5;
    else if (strcmp(buildName, "�ι���ȸ��") == 0)
        return 6;
    else if (strcmp(buildName, "��1�л�ȸ��") == 0)
        return 7;
    else if (strcmp(buildName, "��ΰ�") == 0)
        return 8;
    else if (strcmp(buildName, "���̿����հ�") == 0)
        return 9;
    else if (strcmp(buildName, "��1�ڿ���") == 0)
        return 10;
    else
        return -1; // ��ġ�ϴ� �ǹ����� ���� ���
}

//���� ���� ����ġ �Ǻ� �Լ�
int getSmallIndex(DJK DJ[], int num) {
    int min = INF; //����ġ�� ���� ���� ����, ���� ���� ���� ���� ���̱⿡ ���Ѵ�� �ʱ�ȭ 
    int index = 0; //�ε����� 0���� ����
    for (int i = 0; i < num; i++) { //i�� 0���� ���� ����(14)���� �ݺ�
        if (DJ[i].d < min && DJ[i].v != 1) { //�Ÿ��� min���� �۰�, Ȯ���� ��尡 �ƴҶ�
            min = DJ[i].d; //min�� i��° �Ÿ� ���� ����ְ�
            index = i; //index ���� i ���� ����ִ´�
        }
    }

    return index; //index�� ��ȯ���ش�.
}

//���ͽ�Ʈ�� �Ÿ� ���
void daikstra(int start, DJK DJ[], int num) {
    for (int i = 0; i < num; i++) {
        DJ[i].d = DJ[start].resta[i];                                        //���ͽ�Ʈ�� 2���� �迭�� ���Ҹ� �Ÿ� ������ ä���ش�.
        DJ[i].v = 0;                                                         //Ȯ���� ���� ����.
    }
    DJ[start].v = 1;                                                         //v[0] = 1�� �湮�Ͽ��ٴ� ǥ�ø� ���ش�.
    for (int i = 0; i < num - 2; i++) {                                      //���� for���� ����� �����̱⿡ ���� �ǹ� �������� -2�� ���ش�.
        int index = getSmallIndex(DJ, num);                                  //���� ���� �ε����� ��ȯ���ִ� �Լ��� ȣ���ϰ� �ε����� ������ �����Ѵ�.
        DJ[index].v = 1;                                                     //v[�ε���]�� �湮�� ����̴�.
        for (int j = 0; j < num; j++) {                                      //j�� 0���� ���� �ǹ� �������� �ݺ��Ѵ�(0 ~ 13���� �ݺ�)
            if (DJ[j].v != 1) {                                              //v[j]�� �湮�� ��尡 �ƴ϶��
                if (DJ[index].d + DJ[index].resta[j] < DJ[j].d) {            //�Ÿ�[�ε���] + 2���� �迭[�ε���][j] < �Ÿ�[j]�� �ش��Ѵٸ�
                    DJ[j].d = DJ[index].d + DJ[index].resta[j];              //�Ÿ�[j] = �Ÿ�[�ε���] + 2���� �迭[index][j]                  //�� �� ��ȯ (���� ����)
                    DJ[start].resta[j] = DJ[index].d + DJ[index].resta[j];   //2���� �迭[start][j] = �Ÿ�[index] + 2���� �迭[index][j]      //�� �� ��ȯ (���� ����)
                }
            }
        }
    }
}

typedef struct Nayeol {
    int dis; //���� �Ÿ�
    int count;
}Na;

Na SCH_N[MAX_SCHOOL] = { {INF,0}, {INF,0}, {INF,0}, {INF,0} };
Na R_N[MAX_REST] = { {INF,0}, {INF,0}, {INF,0}, {INF,0} };
Na ST_N[MAX_STORE] = { {INF,0}, {INF,0}, {INF,0}, {INF,0} };
Na C_N[MAX_CAFE] = { {INF,0}, {INF,0}, {INF,0}, {INF,0} };

int sc_final[MAX_SCHOOL];    //���ĵ� �б� ���ͽ�Ʈ�� �ε��� ����
int r_final[MAX_REST];       //���ĵ� �Ĵ� ���ͽ�Ʈ�� �ε��� ����
int st_final[MAX_STORE];     //���ĵ� ������ ���ͽ�Ʈ�� �ε��� ����
int c_final[MAX_CAFE];       //���ĵ� ī�� ���ͽ�Ʈ�� �ε��� ����

//���ͽ�Ʈ�� �Ÿ� ��� �� ������ ���� ����ϱ� (�Լ�)
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
    //    printf("%s, %d\n", Use_name[i], DJ[index].resta[i]);  //���ͽ�Ʈ�� ����Ǿ����� Ȯ���ϴ� ���α׷� ********************************************** ���߿� ������!
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
                //��ΰ�, ��1�ڿ���, ��2�л�ȸ���� �����ϰ� ���
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
    //printf("���� ���� ���� �Ϸ�\n");
    restaurantFileOpen();
    //printf("�Ĵ� ���� ���� �Ϸ�\n");
    rest_locationFileOpen();
    //printf("����ġ ���� ���� �Ϸ�\n");

    menu_print();

    //system("pause");

    return 0;
}

//ù ��° â(����ȭ��) ȭ��
void menu_print()
{
    int answer;

    square(44, 32, 10, 3);

    print_rogo();

    goto_xy(23, 17);
    printf("�Ĵ� ��õ ���α׷�");

    square_char(24, 6, 20, 19, "1. �α���");
    square_char(24, 6, 20, 26, "2. ȸ������");

    square(44, 2, 10, 36);

    goto_xy(12, 37);

    printf("�޴� : ");
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
        printf("�� �� �Է��ϼ̽��ϴ�.");
}

void login() {
    int i = 0;

    char id[20];

    system("cls");

    square(44, 32, 10, 3);
    
    goto_xy(39, 34);
    printf("�ǵ��ư��� : @");

    goto_xy(29, 10);

    printf("�α���");

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
            printf("���̵� ã�� �� �����ϴ�.");
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
                    printf("�α��� ����");
                    for (int i=0;i<20;i++) 
                        check_id[i] = id[i];
                    Sleep(1000);
                    break;
                }
                else {
                    goto_xy(12, 34);
                    printf("��й�ȣ�� Ʋ�Ƚ��ϴ�");
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
    printf("�ǵ��ư��� : @");

    goto_xy(23, 10);

    printf("   ȸ������ ȭ��");

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
                printf("�̹� �����ϴ� ���̵��Դϴ�.");
                Sleep(1000);

                goto_xy(12, 34);
                printf("                                     ");

                goto_xy(39, 34);
                printf("�ǵ��ư��� : @");
                break;
            }
        }

        if (!id_exists) {
            break;
        }
    }

    while (1) {
        int i = 0; // �ε��� �ʱ�ȭ
        square(32, 3, 15, 29);
        goto_xy(17, 30);
        printf("PASSWORD : ");

        while ((ch = _getch()) != '\r') {       // Enter Ű�� �Էµ� ������ �ݺ�
            if (ch == '\b') {                   // Backspace Ű�� �Էµ� ���
                if (i > 0) {
                    i--;
                    printf("\b \b");            // Backspace�� �Է��Ͽ� '*'�� �����, Ŀ���� �� ĭ �������� �̵�
                }
            }
            else {
                pw[i++] = ch;
                printf("*");                    // �Էµ� ���� ��� '*'�� ���
            }
        }
        pw[i] = '\0';              // ���ڿ� ���� NULL ���� �߰�

        goto_xy(17, 31);
        printf("CONFIRM  : ");
        i = 0;
        while ((ch = _getch()) != '\r') {       // Enter Ű�� �Էµ� ������ �ݺ�
            if (ch == '\b') {                   // Backspace Ű�� �Էµ� ���
                if (i > 0) {
                    i--;
                    printf("\b \b");            // Backspace�� �Է��Ͽ� '*'�� �����, Ŀ���� �� ĭ �������� �̵�
                }
            }
            else {
                check_pw[i++] = ch;
                printf("*");                    // �Էµ� ���� ��� '*'�� ���
            }
        }
        check_pw[i] = '\0';              // ���ڿ� ���� NULL ���� �߰�

        // strcmp �Լ��� ����Ͽ� ��й�ȣ�� ��ġ�ϴ��� Ȯ��
        if (strcmp(pw, check_pw) == 0) {
            goto_xy(17, 33);
            printf("��й�ȣ�� ��ġ�մϴ�.          ");
            Sleep(1000);
            // ���Ͽ� ȸ�� ���� ����
            FILE* userFile = fopen("user.txt", "a"); // "a" ���� ���� ���� �߰��ϴ� ���
            if (userFile != NULL) {
                fprintf(userFile, "%s %s %s %s\n", id, pw, name, phone_number);
                fclose(userFile);

                UserFileOpen();
            }
            else {
                printf("������ �� �� �����ϴ�.\n");
            }

            break;
        }
        else {
            goto_xy(17, 33);
            printf("��й�ȣ�� ��ġ���� �ʽ��ϴ�.");
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
    printf("����ȭ��");

    goto_xy(36, 34);
    printf("���α׷� ���� : 0");

    square_char(24, 6, 20, 19, "1. BookMark");
    square_char(24, 6, 20, 26, "2. �Ĵ� ��õ �ޱ�");

    square(44, 2, 10, 36);

    goto_xy(12, 37);

    printf("�޴� : ");
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
    printf("�ǵ��ư��� : @");

    goto_xy(30, 4);
    printf("�ϸ�ũ");

    square(36, 2, 14, 7);
    goto_xy(17, 8);
    printf("NO.    ���� �̸�        ");

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
    printf("1: �߰�, 2: ����, 3: ����");

    square(44, 2, 10, 36);

    goto_xy(12, 37);

    printf("�޴� : ");
    scanf("%s", bookmark_choice);

    if (strcmp(bookmark_choice, "@") != 0) {
        if (strcmp(bookmark_choice, "1") == 0) {
            bookmark_add();
        }
        else if (strcmp(bookmark_choice, "2") == 0) {
            bookmark_delete();
            goto_xy(12, 37);
            printf("�޴� : ");
            scanf("%s", bookmark_choice);
        }
        else if (strcmp(bookmark_choice, "3") == 0) {
            goto_xy(12, 37);
            printf("���� �̸� : ");
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
                printf("�ϸ�ũ ��Ͽ� ���� �����Դϴ�.");
                Sleep(600);
                goto_xy(12, 34);
                printf("                     ");
            }
            else {
                restarnt_information(index);

                square(44, 2, 10, 36);
                goto_xy(12, 37);
                printf("�޴� : ");
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
    printf("�ϸ�ũ �߰�");

    goto_xy(39, 34);
    printf("�ǵ��ư��� : @");

    square(36, 2, 14, 7);
    goto_xy(16, 8);
    printf("NO.    ���� �̸�        ");

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

        printf("���� �̸� : ");
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
                printf("��Ͽ� ���� �����Դϴ�.");
                Sleep(1000);
                goto_xy(12, 34);
                printf("                           ");
                goto_xy(12, 37);
                printf("                              ");
                continue; // ��Ͽ� ���� �����̹Ƿ� �ٽ� �Է� ����
            }
            for (int i = 0; i < MAX_REST; i++) {
                if (strcmp(add_bk, user[check].bm[i].bookmark) == 0) {
                    goto_xy(12, 34);
                    printf("�̹� �߰��� �����Դϴ�.");
                    Sleep(1000);
                    goto_xy(12, 37);
                    printf("                             ");
                    bk_ch = 1;

                    break;
                }
            }
            if (bk_ch == -1) {
                FILE* BookmarkFile = fopen("Bookmark.txt", "a"); // "a" ���� ���� ���� �߰��ϴ� ���
                if (BookmarkFile != NULL) {
                    fprintf(BookmarkFile, "%s %s\n", check_id, add_bk);
                    fclose(BookmarkFile);

                    BookmarkFileOpen();

                    goto_xy(12, 34);
                    printf("�߰��Ǿ����ϴ�.");
                    Sleep(1000);

                    bookmark_ui();

                }
                else
                    printf("������ �� �� �����ϴ�.\n");
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
    printf("�ϸ�ũ ����");

    square(44, 2, 10, 36);

    while (1) {
        goto_xy(12, 37);

        printf("������ ���� : ");
        scanf("%s", del_bk);

        int bk_ch = -1;

        if (strcmp(del_bk, "@") != 0) {
            // �ϸ�ũ ������ �˻��Ͽ� ������ ���԰� �����ϴ��� Ȯ��
            for (int i = 0; i < MAX_REST; i++) {
                if (strcmp(del_bk, user[check].bm[i].bookmark) == 0) {
                    bk_ch = 1; // ���԰� �������� ǥ��
                    break;
                }
            }
            // �ϸ�ũ ���Ͽ� ������ ���԰� �������� �ʴ� ���
            if (bk_ch == -1) {
                goto_xy(12, 34);
                printf("��Ͽ� ���� �����Դϴ�.");
                Sleep(1000);
                goto_xy(12, 34);
                printf("                         ");
                goto_xy(12, 37);
                printf("                            ");
            }
            else { // �ϸ�ũ ���Ͽ� ������ ���԰� �����ϴ� ���
                FILE* BookmarkFile = fopen("Bookmark.txt", "r+"); // "r+" ���� ������ �б�/���� ���� ����
                if (BookmarkFile != NULL) {
                    FILE* tempFile = fopen("tempBookmark.txt", "w"); // �ӽ� ������ ���� ���� ����
                    if (tempFile != NULL) {
                        char line[256];
                        while (fgets(line, sizeof(line), BookmarkFile)) {
                            // ���Ͽ��� �� �پ� �о�ͼ� ������ ���Կ� ��ġ�ϴ� ���� �ǳʶٰ�, �������� �ӽ� ���Ͽ� ����
                            if (strstr(line, check_id) != NULL && strstr(line, del_bk) != NULL) {
                                continue; // ������ ���Կ� ��ġ�ϴ� ���� �ǳʶ�
                            }
                            fputs(line, tempFile);
                        }
                        fclose(tempFile);
                        fclose(BookmarkFile);

                        // �ӽ� ������ ���� ���Ϸ� ����
                        remove("Bookmark.txt");
                        rename("tempBookmark.txt", "Bookmark.txt");

                        BookmarkFileOpen();

                        goto_xy(12, 34);
                        printf("�����Ǿ����ϴ�.");
                        Sleep(1000);

                        bookmark_ui();
                    }
                    else {
                        printf("�ӽ� ������ �� �� �����ϴ�.\n");
                    }
                }
                else {
                    printf("������ �� �� �����ϴ�.\n");
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
    printf("�ǵ��ư��� : 0");

    goto_xy(27, 8);

    printf("����� ��ġ");

    square(38, 15, 13, 12);
    goto_xy(17, 16);

    square(38, 2, 13, 7);
    goto_xy(17, 8);
    printf("NO.    �б� ��ġ        ");

    /*for (int i = 0; i < 11; i++) {
        goto_xy(17, 16 + i);
        printf("%d. %s", i + 1, SCname[i]);
    }*/

    for (int i = 0; i < MAX_SCHOOL; i++) {
        daikstra(i, dj_school, MAX_SCHOOL);        //�ε��� ���� ������� Ȯ������ ���Ͽ��⿡ ���ͽ�Ʈ�� 2���� �迭�� ���� ���� ���� �������־����!
    }
    Ques(SCname, "�̷�â����", sc_final, SCH_N, dj_school, MAX_SCHOOL);  // ************************************************************ ���߿� ���켼��! ���ͽ�Ʈ�� ���ư��� ���� Ȯ���ϱ� ���� ���

    square(38, 2, 13, 31);
    goto_xy(16, 32);

    printf("��ġ �Է� : ");
    scanf("%d", &now_lo); //������ ����Ͽ� �̹� ���ĵ� ���ڿ����� ��� �� �Է� ������ ���� �� �� ���� ������ ���� -> �˰��� 1�� �߰��� �� ���� ������?

    strcpy(user_now_location, SCname[now_lo - 1]); //���ĵ��� ���� ���ڿ����� �̹� �ڵ� ������ ������ �ǹ����� ������ �ǹ��� ����

    if (now_lo != 0) {
        recommend();               //user_now_location�� ���������̱⿡ �ѱ��� �ʾƵ� �ٸ� �Լ����� ����� �� �״�� �Լ� ����� ����
    }
    else
        user_ui();
}

void recommend() {
    system("cls");

    int choice; // ���� ���ڷ� ����

    square(44, 32, 10, 3);

    goto_xy(39, 34);
    printf("�ǵ��ư��� : 0");

    goto_xy(28, 8);
    printf("���� ��õ");

    square(34, 2, 15, 21);
    goto_xy(17, 22);
    printf("1. �Ĵ� ��õ");

    square(34, 2, 15, 25);
    goto_xy(17, 26);
    printf("2. ������ ��õ");

    square(34, 2, 15, 29);
    goto_xy(17, 30);
    printf("3. ī�� ��õ");

    square(44, 2, 10, 36);

    goto_xy(12, 37);

    printf("�޴� : ");
    scanf("%d", &choice); // ���� ���ڸ� �޵��� ����

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
    printf("�ǵ��ư��� : @");

    goto_xy(28,7);
    printf("�Ĵ� ��õ");

    square(38, 2, 13, 7);
    goto_xy(17, 8);
    printf("NO.    ���� �̸�        ");

    square(38, 20, 13, 11);
    goto_xy(17, 17);

    // 2024.06.04 >> user_now_location ������ ������ �� ����Ͽ��� SCname�� �ε����� ����� ����� ��
    // daikstra(now_lo-1, dj_school, MAX_SCHOOL); //���ͽ�Ʈ�� ������ �ϱ� ���� user_now_location ������ �ε��� ���� ã�� ����! *********** new!

    for (int i = 0; i < MAX_REST; i++) {
        daikstra(i, dj_rest, MAX_REST);        //�ε��� ���� ������� Ȯ������ ���Ͽ��⿡ ���ͽ�Ʈ�� 2���� �迭�� ���� ���� ���� �������־����!
    }

    //������ �Ĵ縸�� �ȳ����ִ� �Լ��̱⿡ ���� ��ġ���� �츮�� �˰��ִ� ��1�ڿ����� ��2�л�ȸ���� ����ġ�� ���Ͽ� �� ª�� ������ �Ĵ��� �������ָ� �� ��?
    if (dj_school[now_lo - 1].resta[3] < dj_school[now_lo - 1].resta[10]) { //��2�л�ȸ������ ���� ���� �� ���� ���
        Ques(Rname, "��2�л�ȸ��", r_final, R_N, dj_rest, MAX_REST);                                                     //�̷��� �����Ͽ��� ��� ���� �ǹ��� ��� ��� �� �� ������ ����� ������!
        //daikstra(0, dj_rest, MAX_REST); //�Ĵ� �迭�� �ִ� ��2�л�ȸ���� �ε����� ã�� ���ͽ�Ʈ�� ���� ����!             ************************

    }
    else { //��1�ڿ������� ���� ���� �� ���� ��� // (dj_school[now_lo - 1].resta[3] > dj_school[now_lo - 1].resta[10])
        Ques(Rname, "��1�ڿ���", r_final, R_N, dj_rest, MAX_REST);
    }
    //�� ����ġ�� ���� ���� ��� ���� ���� �����غ��� ���� ���ƺ���!  ************************************** �̰� �츮 ���� �ƴ϶� ���� ����� ���� ���߽��ϴ�
    int index = 0;
    while (1) {
        square(44, 2, 10, 36);
        goto_xy(12, 37);
        printf("�޴� : ");

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
            printf("�߸� �Է��ϼ̽��ϴ�.  ");
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
        printf("�޴� : ");

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
    printf("������ ��õ");

    square(38, 2, 13, 7);
    goto_xy(17, 8);
    printf("NO.    ���� �̸�        ");

    goto_xy(39, 34);
    printf("�ǵ��ư��� : @");

    square(38, 20, 13, 11);
    goto_xy(17, 17);

    // 2024.06.04 >> user_now_location ������ ������ �� ����Ͽ��� SCname�� �ε����� ����� ����� ��
    // daikstra(now_lo-1, dj_school, MAX_SCHOOL); //���ͽ�Ʈ�� ������ �ϱ� ���� user_now_location ������ �ε��� ���� ã�� ����! *********** new!

    for (int i = 0; i < MAX_STORE; i++) {
        daikstra(i, dj_store, MAX_STORE);        //�ε��� ���� ������� Ȯ������ ���Ͽ��⿡ ���ͽ�Ʈ�� 2���� �迭�� ���� ���� ���� �������־����!
    }

    //������ �Ĵ縸�� �ȳ����ִ� �Լ��̱⿡ ���� ��ġ���� �츮�� �˰��ִ� ��1�ڿ����� ��2�л�ȸ���� ����ġ�� ���Ͽ� �� ª�� ������ �Ĵ��� �������ָ� �� ��?
    if (dj_school[now_lo - 1].resta[3] < dj_school[now_lo - 1].resta[10] && dj_school[now_lo - 1].resta[3] < dj_school[now_lo - 1].resta[8]) { //��2�л�ȸ������ ���� ���� �� ���� ���
        Ques(STname, "��2�л�ȸ��", st_final, ST_N, dj_store, MAX_STORE);                                                     //�̷��� �����Ͽ��� ��� ���� �ǹ��� ��� ��� �� �� ������ ����� ������!
        //daikstra(0, dj_rest, MAX_REST); //�Ĵ� �迭�� �ִ� ��2�л�ȸ���� �ε����� ã�� ���ͽ�Ʈ�� ���� ����!             ************************

    }
    else if (dj_school[now_lo - 1].resta[10] < dj_school[now_lo - 1].resta[3] && dj_school[now_lo - 1].resta[10] < dj_school[now_lo - 1].resta[8]) { //��2�л�ȸ������ ���� ���� �� ���� ���
        Ques(STname, "��1�ڿ���", st_final, ST_N, dj_store, MAX_STORE);                                                     //�̷��� �����Ͽ��� ��� ���� �ǹ��� ��� ��� �� �� ������ ����� ������!
        //daikstra(0, dj_rest, MAX_REST); //�Ĵ� �迭�� �ִ� ��2�л�ȸ���� �ε����� ã�� ���ͽ�Ʈ�� ���� ����!             ************************

    }
    else { //��1�ڿ������� ���� ���� �� ���� ��� // (dj_school[now_lo - 1].resta[3] > dj_school[now_lo - 1].resta[10])
        Ques(STname, "��ΰ�", st_final, ST_N, dj_store, MAX_STORE);
    }
    //�� ����ġ�� ���� ���� ��� ���� ���� �����غ��� ���� ���ƺ���!  ************************************** �̰� �츮 ���� �ƴ϶� ���� ����� ���� ���߽��ϴ�
    int index = 0;
    while (1) {
        square(44, 2, 10, 36);
        goto_xy(12, 37);
        printf("�޴� : ");

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
            printf("�߸� �Է��ϼ̽��ϴ�.  ");
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
        printf("�޴� : ");

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
    printf("ī�� ��õ");

    square(38, 2, 13, 7);
    goto_xy(17, 8);
    printf("NO.    ���� �̸�        ");

    goto_xy(39, 34);
    printf("�ǵ��ư��� : @");

    square(38, 20, 13, 11);
    goto_xy(17, 17);

    // 2024.06.04 >> user_now_location ������ ������ �� ����Ͽ��� SCname�� �ε����� ����� ����� ��
    // daikstra(now_lo-1, dj_school, MAX_SCHOOL); //���ͽ�Ʈ�� ������ �ϱ� ���� user_now_location ������ �ε��� ���� ã�� ����! *********** new!

    for (int i = 0; i < MAX_CAFE; i++) {
        daikstra(i, dj_cafe, MAX_CAFE);        //�ε��� ���� ������� Ȯ������ ���Ͽ��⿡ ���ͽ�Ʈ�� 2���� �迭�� ���� ���� ���� �������־����!
    }

    //������ �Ĵ縸�� �ȳ����ִ� �Լ��̱⿡ ���� ��ġ���� �츮�� �˰��ִ� ��1�ڿ����� ��2�л�ȸ���� ����ġ�� ���Ͽ� �� ª�� ������ �Ĵ��� �������ָ� �� ��?
    if (dj_school[now_lo - 1].resta[3] < dj_school[now_lo - 1].resta[10] && dj_school[now_lo - 1].resta[3] < dj_school[now_lo - 1].resta[8]) { //��2�л�ȸ������ ���� ���� �� ���� ���
        Ques(Cname, "��2�л�ȸ��", c_final, C_N, dj_cafe, MAX_CAFE);                                                     //�̷��� �����Ͽ��� ��� ���� �ǹ��� ��� ��� �� �� ������ ����� ������!
        //daikstra(0, dj_rest, MAX_REST); //�Ĵ� �迭�� �ִ� ��2�л�ȸ���� �ε����� ã�� ���ͽ�Ʈ�� ���� ����!             ************************

    }
    else if (dj_school[now_lo - 1].resta[10] < dj_school[now_lo - 1].resta[3] && dj_school[now_lo - 1].resta[10] < dj_school[now_lo - 1].resta[8]) { //��2�л�ȸ������ ���� ���� �� ���� ���
        Ques(Cname, "��1�ڿ���", c_final, C_N, dj_cafe, MAX_CAFE);                                                     //�̷��� �����Ͽ��� ��� ���� �ǹ��� ��� ��� �� �� ������ ����� ������!
        //daikstra(0, dj_rest, MAX_REST); //�Ĵ� �迭�� �ִ� ��2�л�ȸ���� �ε����� ã�� ���ͽ�Ʈ�� ���� ����!             ************************

    }
    else { //��1�ڿ������� ���� ���� �� ���� ��� // (dj_school[now_lo - 1].resta[3] > dj_school[now_lo - 1].resta[10])
        Ques(Cname, "��ΰ�", c_final, C_N, dj_cafe, MAX_CAFE);
    }
    //�� ����ġ�� ���� ���� ��� ���� ���� �����غ��� ���� ���ƺ���!  ************************************** �̰� �츮 ���� �ƴ϶� ���� ����� ���� ���߽��ϴ�
    ////��ΰ� 8, ��2�л�ȸ�� 3, �ι���ȸ�� 6, ��1�ڿ��� 10
    int index = 0;
    while (1) {
        square(44, 2, 10, 36);
        goto_xy(12, 37);
        printf("�޴� : ");

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
            printf("�߸� �Է��ϼ̽��ϴ�.  ");
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
        printf("�޴� : ");

        scanf("%s", choice);

        if (strcmp(choice, "@") == 0) {
            recommend_rest();
        }
    }
}

void restarnt_information(int index) {
    char choice[20];
    char loca[100]; // �ּҸ� ������ �ӽ� �迭

    system("cls");

    square(44, 32, 10, 3);
    goto_xy(28, 6);
    printf("���� ����");

    goto_xy(39, 34);
    printf("�ǵ��ư��� : @");

    square(36, 20, 14, 10);

    goto_xy(16, 12);
    printf("��     ��: %s", rest[index].name);

    goto_xy(16, 14);
    printf("��     ġ: ");

    int row = 14;
    int col = 27;
    int len = strlen(rest[index].loca);

    // �ּҸ� 16���ھ� ���
    for (int i = 0; i < len; i += 16) {
        goto_xy(col, row);
        strncpy(loca, &rest[index].loca[i], 16);
        loca[16] = '\0'; // ���ڿ� ���� NULL ���ڸ� �߰�
        printf("%s", loca);
        row++; // ���� �ٷ� �̵�
    }

    goto_xy(16, row + 1);
    printf("��ȭ ��ȣ: %s", rest[index].number);

    goto_xy(16, row + 3);
    printf("���� �ð�: %s", rest[index].open);

    goto_xy(16, row + 5);
    printf("���� �ð�: %s", rest[index].close);

    goto_xy(16, row + 7);
    printf("�� �� �� : %s", rest[index].CT);
}
