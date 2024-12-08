#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define kbhit _kbhit
#define getch _getch

#define LEFT 75   // 왼쪽 화살표 키보드 값
#define RIGHT 77  // 오른쪽 화살표 키
#define UP 72     // 위로 화살표 키 , 블록 회전 시키기
#define DOWN 80   // 아래로 화살표 키, 더 빨리 아래로 이동
#define SPACE 32  // 스페이스 바, 아래로 떨어 뜨리기
#define p 112     // 일시 정지
#define P 80      // 일시 정지
#define ESC 27    // 게임 중간에 취소 하기

#define false 0
#define true 1

#define ACTIVE_BLOCK -2  // 게임판 배열에 저장될 블록의 상태들
#define CEILLING -1      // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현
#define EMPTY 0          // 블록이 이동할 수 없는 공간은 양수로 표현
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값

#define MAIN_X 11    //게임판 가로 크기
#define MAIN_Y 23    //게임판 세로 크기
#define MAIN_X_ADJ 3 //게임판 위치 조정
#define MAIN_Y_ADJ 1 //게임판 위치 조정

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 // 게임 정보 표시

int STATUS_Y_GOAL;  //GOAL 정보 표시 위치Y 좌표 저장
int STATUS_Y_LEVEL; //LEVEL 정보 표시 위치Y 좌표 저장
int STATUS_Y_SCORE; //SCORE 정보 표시 위치Y 좌표 저장

int blocks[7][4][4][4] = {
        {{0,0,0,0,
                 0,1,1,0,
                 0,1,1,0,
                 0,0,0,0},
                {0,0,0,0,
                        0,1,1,0,
                        0,1,1,0,
                        0,0,0,0},
                {0,0,0,0,
                        0,1,1,0,
                        0,1,1,0,
                        0,0,0,0},
                {0,0,0,0,
                        0,1,1,0,
                        0,1,1,0,
                        0,0,0,0}},
        {{0,0,0,0,
                 0,0,0,0,
                 1,1,1,1,
                 0,0,0,0},
                {0,1,0,0,
                        0,1,0,0,
                        0,1,0,0,
                        0,1,0,0},
                {0,0,0,0,
                        0,0,0,0,
                        1,1,1,1,
                        0,0,0,0},
                {0,1,0,0,
                        0,1,0,0,
                        0,1,0,0,
                        0,1,0,0}},
        {{0,0,0,0,
                 1,1,0,0,
                 0,1,1,0,
                 0,0,0,0},
                {0,0,0,0,
                        0,0,1,0,
                        0,1,1,0,
                        0,1,0,0},
                {0,0,0,0,
                        1,1,0,0,
                        0,1,1,0,
                        0,0,0,0},
                {0,0,0,0,
                        0,0,1,0,
                        0,1,1,0,
                        0,1,0,0}},
        {{0,0,0,0,
                 0,1,1,0,
                 1,1,0,0,
                 0,0,0,0},
                {0,0,0,0,
                        1,0,0,0,
                        1,1,0,0,
                        0,1,0,0},
                {0,0,0,0,
                        0,1,1,0,
                        1,1,0,0,
                        0,0,0,0},
                {0,0,0,0,
                        1,0,0,0,
                        1,1,0,0,
                        0,1,0,0}},
        {{0,0,0,0,
                 0,0,1,0,
                 1,1,1,0,
                 0,0,0,0},
                {0,0,0,0,
                        1,1,0,0,
                        0,1,0,0,
                        0,1,0,0},
                {0,0,0,0,
                        0,0,0,0,
                        1,1,1,0,
                        1,0,0,0},
                {0,0,0,0,
                        0,1,0,0,
                        0,1,0,0,
                        0,1,1,0}},
        {{0,0,0,0,
                 1,0,0,0,
                 1,1,1,0,
                 0,0,0,0},
                {0,0,0,0,
                        0,1,0,0,
                        0,1,0,0,
                        1,1,0,0},
                {0,0,0,0,
                        0,0,0,0,
                        1,1,1,0,
                        0,0,1,0},
                {0,0,0,0,
                        0,1,1,0,
                        0,1,0,0,
                        0,1,0,0}},
        {{0,0,0,0,
                 0,1,0,0,
                 1,1,1,0,
                 0,0,0,0},
                {0,0,0,0,
                        0,1,0,0,
                        0,1,1,0,
                        0,1,0,0},
                {0,0,0,0,
                        0,0,0,0,
                        1,1,1,0,
                        0,1,0,0},
                {0,0,0,0,
                        0,1,0,0,
                        1,1,0,0,
                        0,1,0,0}}
};

int b_type;      // 블록 종류
int b_rotation;  // 블록 회전값
int b_type_next; // 다음 블록값

int main_org[MAIN_Y][MAIN_X]; // 게임판의 정보를 저장하는 배열 모니터에 표시 후에 main_cpy로 복사됨
int main_cpy[MAIN_Y][MAIN_X]; // 즉 maincpy는 게임판이 모니터에 표시되기 전의 정보를 가지고 있음
// main의 전체를 계속 모니터에 표시 하지 않고(이렇게 할 경우 모니터가 깜빡 거림)
// main_cpy와 배열을 비교해 값이 달라진 곳만 모니터에 고침

int bx, by; // 이동 중인 블록의 게임판 상의 x,y 좌표를 저장
int key;    // 키보드로 입력 받은 키값을 저장

int speed;          // 게임 진행 속도
int level;          // 현재 level
int level_goal;     // 다음 레벨로 넘어 가기 위한 목표 점수
int cnt;            // 현재 레벨에서 제거한 줄 수를 저장
int score;          // 현재 점수
int last_score = 0; // 마지막 게임 점수
int best_score = 0; // 최고 게임 점수

int new_block_on = 0; // 새로운 블럭이 필요함을 알리는 flag
int crush_on = 0;     // 현재 이동 중인 블록이 충돌 상태 인지 알려 주는 flag
int level_up_on = 0;  // 다음 레벨로 진행(현재 레벨 목표가 완료 되었음을) 알리는 flag
int space_key_on = 0; // hard drop 상태임을 알려 주는 flag

void title(void);                              // 게임 시작 화면
void reset(void);                              // 게임판 초기화
void reset_main(void);                         // 메인 게임판(main_org[][]를 초기화)
void reset_main_cpy(void);                     // copy 게임판(main_cpy[][]를 초기화)
void draw_map(void);                           // 게임 전체 인터페이스 표시
void draw_main(void);                          // 게임판을 그림
void new_block(void);                          // 새로운 블록을 하나 만듦
void check_key(void);                          // 키보드로 키를 입력 받음
void drop_block(void);                         // 블록을 아래로 떨어트림
int check_crush(int bx, int by, int rotation); // bx, by 위치에 rotation 회전값을 같는 경우 충돌 판단
void move_block(int dir);                      // dir 방향으로 블록을 움직임
void check_line(void);                         // 줄이 가득 찼는지를 판단 하고 지움
void check_level_up(void);                     // 레벨 목표가 달성 되었는지를 판단 하고 levelup 시킴
void check_game_over(void);                    // 게임 오버 인지 판단 하고 게임 오버를 진행
void pause(void);                              // 게임을 일시 정지 시킴

void gotoxy(int x, int y) {
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
void setcursortype(CURSOR_TYPE c) {
    CONSOLE_CURSOR_INFO CurInfo;

    switch (c) {
        case NOCURSOR:
            CurInfo.dwSize = 1;
            CurInfo.bVisible = FALSE;
            break;
        case SOLIDCURSOR:
            CurInfo.dwSize = 100;
            CurInfo.bVisible = TRUE;
            break;
        case NORMALCURSOR:
            CurInfo.dwSize = 20;
            CurInfo.bVisible = TRUE;
            break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

int main() {
    int i;

    srand((unsigned)time(NULL));
    setcursortype(NOCURSOR); // 커서 숨김
    title();
    reset();

    while (1) {
        for (i = 0; i < 5; i++) { // 블록이 한칸 떨어지는 동안 키 입력을 5번 받을 수 있음
            check_key(); // 키 입력 확인
            draw_main(); // 화면을 그림
            Sleep(speed); // 게임 속도 조절
            if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
            // 블록이 충돌 중인 경우 추가로 이동및 회전할 시간을 갖음
            if (space_key_on == 1) { // 스페이스 바를 누른 경우(hard drop) 추가로 이동및 회전할 수 없음 break;
                space_key_on = 0;
                break;
            }
        }
        drop_block();       // 블록을 한칸 내림
        check_level_up();   // 레벨업 체크
        check_game_over();  // 게임 오버 체크
        if (new_block_on == 1) new_block(); // 뉴 블럭 flag가 있는 경우 새로운 블럭 생성
    }
}

void title(void) {
    int x = 5;  // 타이틀 화면이 표시 되는 x좌표
    int y = 4;  // 타이틀 화면이 표시 되는 y좌표
    int cnt;    // 타이틀 프레임을 세는 변수

    gotoxy(x, y + 0); printf("■□□□■■■□□■■□□■■"); Sleep(100);
    gotoxy(x, y + 1); printf("■■■□  ■□□    ■■□□■"); Sleep(100);
    gotoxy(x, y + 2); printf("□□□■              □■  ■"); Sleep(100);
    gotoxy(x, y + 3); printf("■■□■■  □  ■  □□■□□"); Sleep(100);
    gotoxy(x, y + 4); printf("■■  ■□□□■■■□■■□□"); Sleep(100);
    gotoxy(x + 5, y + 2); printf("T E T R I S"); Sleep(100);
    gotoxy(x, y + 7); printf("Please Enter Any Key to Start..");
    gotoxy(x, y + 9); printf("  △   : Shift");
    gotoxy(x, y + 10); printf("◁  ▷ : Left / Right");
    gotoxy(x, y + 11); printf("  ▽   : Soft Drop");
    gotoxy(x, y + 12); printf(" SPACE : Hard Drop");
    gotoxy(x, y + 13); printf("   P   : Pause");
    gotoxy(x, y + 14); printf("  ESC  : Quit");

    for (cnt = 0;; cnt++) { // cnt를 1씩 증가 시키면서 계속 반복, 별이 반짝이는 애니메이션 효과
        if (kbhit()) break; // 키 입력이 있으면 무한 루프 종료
        if (cnt % 200 == 0) { gotoxy(x + 4, y + 1); printf("★"); }
        // cnt가 200으로 나누어 떨어질 때 별을 표시
        if ((cnt % 200 - 100) == 0) { gotoxy(x + 4, y + 1); printf("  "); }
        // 위 카운트에서 100카운트 간격으로 별을 지움
        if ((cnt % 350) == 0) { gotoxy(x + 13, y + 2); printf("☆"); }
        // 윗별과 같지만 시간 차를 뒀음
        if ((cnt % 350 - 100) == 0) { gotoxy(x + 13, y + 2); printf("  "); }
        Sleep(10); // 0.01초 멈춤
    }

    while (kbhit()) getch(); // 버퍼에 저장된 키 값을 버림
}

void reset(void) {
    FILE* file = fopen("score.dat", "rt"); // 점수를 읽을 파일
    if (file == 0) { best_score = 0; } // 파일을 열기 못했으면 최고 점수를 0으로 초기화
    else {
        fscanf(file, "%d", &best_score); // 최고 점수를 읽음
        fclose(file);
    }

    level = 1;
    score = 0;
    level_goal = 1000;
    key = 0;
    crush_on = 0;
    cnt = 0;
    speed = 100;

    system("cls");  // 전체 화면 지움
    reset_main();             // main_org 초기화
    draw_map();               // 게임 화면을 그림
    draw_main();              // 게임판을 그림

    b_type_next = rand() % 7; // 다음 번에 나올 블록 종류를 랜덤 생성
    new_block();              // 새로운 블록을 하나 만듦
}


void reset_main(void) {                 // 게임판 초기화
    int i, j;

    for (i = 0; i < MAIN_Y; i++) {      // 게임판을 0으로 초기화
        for (j = 0; j < MAIN_X; j++) {
            main_org[i][j] = 0;
            main_cpy[i][j] = 100;
        }
    }
    for (j = 1; j < MAIN_X; j++) {      // y값이 3인 위치는 천장
        main_org[3][j] = CEILLING;
    }
    for (i = 1; i < MAIN_Y - 1; i++) {  // 좌우 벽
        main_org[i][0] = WALL;
        main_org[i][MAIN_X - 1] = WALL;
    }
    for (j = 0; j < MAIN_X; j++) {      // 바닥 벽
        main_org[MAIN_Y - 1][j] = WALL;
    }
}

void reset_main_cpy(void) {             // main_cpy 초기화
    int i, j;

    for (i = 0; i < MAIN_Y; i++) {      // 게임판에 게임에 사용 되지 않는 숫자를 넣음
        for (j = 0; j < MAIN_X; j++) {  // main_org와 같은 숫자가 없게 하기 위함
            main_cpy[i][j] = 100;
        }
    }
}

void draw_map(void) {       // 게임 상태 표시를 나타내는 함수
    int y = 3;              // level, goal, score만 게임 중에 값이 바뀔 수 있으므로 y값을 따로 저장

    gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL = y); printf(" LEVEL : %5d", level);
    gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL = y + 1); printf(" GOAL  : %5d", 10 - cnt);
    gotoxy(STATUS_X_ADJ, y + 2); printf("+-  N E X T  -+ ");
    gotoxy(STATUS_X_ADJ, y + 3); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 4); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 6); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 7); printf("+-- -  -  - --+ ");
    gotoxy(STATUS_X_ADJ, y + 8); printf(" YOUR SCORE :");
    gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 9); printf("        %6d", score);
    gotoxy(STATUS_X_ADJ, y + 10); printf(" LAST SCORE :");
    gotoxy(STATUS_X_ADJ, y + 11); printf("        %6d", last_score);
    gotoxy(STATUS_X_ADJ, y + 12); printf(" BEST SCORE :");
    gotoxy(STATUS_X_ADJ, y + 13); printf("        %6d", best_score);
    gotoxy(STATUS_X_ADJ, y + 15); printf("  ↑   : Shift        SPACE : Hard Drop");
    gotoxy(STATUS_X_ADJ, y + 16); printf("←  → : Left / Right   P   : Pause");
    gotoxy(STATUS_X_ADJ, y + 17); printf("  ↓   : Soft Drop     ESC  : Quit");
}

void draw_main(void) {
    int i, j;

    for (j = 1; j < MAIN_X - 1; j++) {  // 천장은 계속 새로운 블록이 지나가서 지워지면 새로 그림
        if (main_org[3][j] == EMPTY) main_org[3][j] = CEILLING;
    }
    for (i = 0; i < MAIN_Y; i++) {
        for (j = 0; j < MAIN_X; j++) {
            if (main_cpy[i][j] != main_org[i][j]) { // cpy와 비교해 값이 달라진 부분만 새로 그림
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
                switch (main_org[i][j]) {
                    case EMPTY:                     // 빈칸 모양
                        printf("  ");
                        break;
                    case CEILLING:                  // 천장 모양
                        printf(". ");
                        break;
                    case WALL:                      // 벽모양
                        printf("▩");
                        break;
                    case INACTIVE_BLOCK:            // 굳은 블럭 모양
                        printf("□");
                        break;
                    case ACTIVE_BLOCK:              // 움직이고 있는 블럭 모양
                        printf("■");
                        break;
                }
            }
        }
    }
    for (i = 0; i < MAIN_Y; i++) { // 게임판을 그린 후 main_cpy에 복사
        for (j = 0; j < MAIN_X; j++) {
            main_cpy[i][j] = main_org[i][j];
        }
    }
}

void new_block(void) {        // 새로운 블록 생성
    int i, j;

    bx = (MAIN_X / 2) - 1;    // 블록 생성 위치 x좌표(게임판의 가운데)
    by = 0;                   // 블록 생성 위치 y좌표(제일 위)
    b_type = b_type_next;     // 다음 블록 값을 가져옴
    b_type_next = rand() % 7; // 다음 블럭을 만듦
    b_rotation = 0;           // 회전은 0번으로 가져옴

    new_block_on = 0;         // new_block flag를 끔

    for (i = 0; i < 4; i++) { // 게임판 bx, by위치에 블록 생성
        for (j = 0; j < 4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
        }
    }
    for (i = 1; i < 3; i++) { // 게임 상태 표시에 다음에 나올 블록을 그림
        for (j = 0; j < 4; j++) {
            if (blocks[b_type_next][0][i][j] == 1) {
                gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
                printf("■");
            }
            else {
                gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
                printf("  ");
            }
        }
    }
}

void check_key(void) {
    key = 0;                // 키값 초기화

    if (kbhit()) {          // 키 입력이 있는 경우
        key = getch();      // 키값을 받음
        if (key == 224) {   // 방향 키인 경우
            do { key = getch(); } while (key == 224); // 방향키 지시값을 버림
            switch (key) {
                case LEFT:  // 왼쪽 키 눌렀을 때
                    if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
                    break;  // 왼쪽으로 갈 수 있는지 체크 후 가능하면 이동
                case RIGHT: // 오른쪽 방향키 눌렀을 때 - 위와 동일하게 처리됨
                    if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
                    break;
                case DOWN:  // 아래쪽 방향키 눌렀을 때 - 위와 동일하게 처리됨
                    if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
                    break;
                case UP:    // 위쪽 방향키 눌렀을 때
                    if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
                            // 회전할 수 있는지 체크 후 가능하면 회전
                    else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
            }               // 바닥에 닿은 경우 위쪽으로 한칸 띄워서 회전이 가능하면 그렇게 함(특수동작)
        }
        else {                               // 방향키가 아닌 경우
            switch (key) {
                case SPACE:                  // 스페이스 키 눌렀을 때
                    space_key_on = 1;        // 스페이스 키 flag를 띄움
                    while (crush_on == 0) {  // 바닥에 닿을 때까지 이동 시킴
                        drop_block();
                        score += level;      // hard drop 보너스
                        gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score); // 점수 표시
                    }
                    break;
                case P:                      // P(대문자) 눌렀을 때
                case p:                      // p(소문자) 눌렀을 때
                    pause();                 // 일시 정지
                    break;
                case ESC:                    // ESC 눌렀을 때
                    system("cls"); // 화면을 지우고
                    exit(0);          // 게임 종료
            }
        }
    }
    while (kbhit()) getch();                // 키 버퍼를 비움
}

void drop_block(void) {
    int i, j;

    if (crush_on && check_crush(bx, by + 1, b_rotation) == true) crush_on = 0;
    // 밑이 비어 있으면 crush flag 끔
    if (crush_on && check_crush(bx, by + 1, b_rotation) == false) {
        // 밑이 비어 있지 않고 crush flag가 켜저 있으면
        for (i = 0; i < MAIN_Y; i++) { // 현재 조작 중인 블럭을 굳힘
            for (j = 0; j < MAIN_X; j++) {
                if (main_org[i][j] == ACTIVE_BLOCK) main_org[i][j] = INACTIVE_BLOCK;
            }
        }
        crush_on = 0;     // flag를 끔
        check_line();     // 라인을 체크
        new_block_on = 1; // 새로운 블록 생성 flag를 켬
        return;           // 함수 종료
    }
    if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
    // 밑이 비어 있으면 밑으로 한칸 이동
    if (check_crush(bx, by + 1, b_rotation) == false) crush_on++;
    // 밑으로 이동이 안되면  crush flag를 켬
}

int check_crush(int bx, int by, int b_rotation) { // 지정된 좌표와 회전 값으로 충돌이 있는지 검사
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) { // 지정된 위치의 게임판과 블록 모양을 비교해 겹치면 false 리턴
            if (blocks[b_type][b_rotation][i][j] == 1 && main_org[by + i][bx + j] > 0) return false;
        }
    }
    return true; // 하나도 겹치지 않을 경우 true 리턴
};

void move_block(int dir) { // 블록을 이동 시킴
    int i, j;

    switch (dir) {
        case LEFT: // 왼쪽 방향
            for (i = 0; i < 4; i++) { // 현재 좌표의 블럭을 지움
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
                }
            }
            for (i = 0; i < 4; i++) { // 왼쪽 으로 한칸 가서 active block을 찍음
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j - 1] = ACTIVE_BLOCK;
                }
            }
            bx--; // 좌표값 이동
            break;

        case RIGHT:    // 오른쪽 방향. 왼쪽 방향과 같은 원리로 동작
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
                }
            }
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j + 1] = ACTIVE_BLOCK;
                }
            }
            bx++;
            break;

        case DOWN:    // 아래쪽 방향. 왼쪽 방향과 같은 원리로 동작
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
                }
            }
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i + 1][bx + j] = ACTIVE_BLOCK;
                }
            }
            by++;
            break;

        case UP: // 키보드 위쪽 눌렀을 때 회전 시킴.
            for (i = 0; i < 4; i++) { // 현재 좌표의 블럭을 지움
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
                }
            }
            b_rotation = (b_rotation + 1) % 4; // 회전 값을 1 증가 시킴(3에서 4가 되는 경우는 0으로 되돌림)
            for (i = 0; i < 4; i++) { // 회전된 블록을 찍음
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
                }
            }
            break;

        case 100: // 블록이 바닥, 혹은 다른 블록과 닿은 상태에서 한칸 위로 올려 회전이 가능한 경우 이를 동작 시키는 특수 동작
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
                }
            }
            b_rotation = (b_rotation + 1) % 4;
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i - 1][bx + j] = ACTIVE_BLOCK;
                }
            }
            by--;
            break;
    }
}

void check_line(void) {
    int i, j, k, l;

    int block_amount; // 한 줄의 블록 갯수를 저장하는 변수
    int combo = 0; // 콤보 갯수 저장하는 변수 지정 및 초기화

    for (i = MAIN_Y - 2; i > 3;) { // i=MAIN_Y-2:밑쪽벽의 윗칸부터, i>3:천장(3) 아래까지 검사
        block_amount = 0; // 블록 갯수 저장 변수 초기화
        for (j = 1; j < MAIN_X - 1; j++) { // 벽과 벽 사이의 블록 갯수를 셈
            if (main_org[i][j] > 0) block_amount++;
        }
        if (block_amount == MAIN_X - 2) { // 블록이 가득 찬 경우
            if (level_up_on == 0) { // 레벨 업 상태가 아닌 경우에(레벨 업이 되면 자동 줄 삭제가 있음)
                score += 100 * level; // 점수 추가
                cnt++; // 지운 줄 갯수 카운트 증가
                combo++; // 콤보 수 증가
            }
            for (k = i; k > 1; k--) { // 윗줄을 한칸씩 모두 내림(윗줄이 천장이 아닌 경우에만)
                for (l = 1; l < MAIN_X - 1; l++) {
                    if (main_org[k - 1][l] != CEILLING) main_org[k][l] = main_org[k - 1][l];
                    if (main_org[k - 1][l] == CEILLING) main_org[k][l] = EMPTY;
                    // 윗줄이 천장인 경우에 천장을 내릴 수 없으니 빈칸을 넣음
                }
            }
        }
        else i--;
    }
    if (combo) { // 줄 삭제가 있는 경우 점수와 레벨 목표를 새로 표시함
        if (combo > 1) { // 2 콤보 이상인 경우 경우 보너스 및 메세지를 게임판에 띄웠다가 지움
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("%d COMBO!", combo);
            Sleep(500);
            score += (combo * level * 100);
            reset_main_cpy(); // 텍스트를 지우기 위해 main_cpy 초기화.
            // (main_cpy와 main_org가 전부 다르므로 다음번 draw()호출시 게임판 전체를 새로 그리게 됨)
        }
        gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", (cnt <= 10) ? 10 - cnt : 0);
        gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
    }
}

void check_level_up(void) {
    int i, j;

    if (cnt >= 10) {     // 레벨 별로 10줄씩 없애야 함. 10줄 이상 없앤 경우
        draw_main();
        level_up_on = 1; // 레벨업 flag를 띄움
        level += 1;      // 레벨을 1 올림
        cnt = 0;         // 지운 줄수 초기화

        for (i = 0; i < 4; i++) {
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
            printf("             ");
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
            printf("             ");
            Sleep(200);

            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
            printf("☆LEVEL UP!☆");
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
            printf("☆SPEED UP!☆");
            Sleep(200);
        }
        reset_main_cpy(); // 텍스트를 지우기 위해 main_cpy 초기화
        // (main_cpy와 main_org가 전부 다르므로 다음번 draw()호출시 게임판 전체를 새로 그리게 됨)

        for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--) { // 레벨 업 보상: 각 레벨-1의 수만큼 아랫쪽 줄을 지워줌
            for (j = 1; j < MAIN_X - 1; j++) {
                main_org[i][j] = INACTIVE_BLOCK; // 줄을 블록으로 모두 채우고
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i); // 별 찍기
                printf("★");
                Sleep(20);
            }
        }
        Sleep(100); // 별 출력을 위해 delay
        check_line(); // 블록으로 모두 채운 것 지우기
        //.check_line()함수 내부에서 level up flag가 켜져있는 경우 점수는 없음
        switch (level) { // 레벨별 속도 조절
            case 2:
                speed = 50;
                break;
            case 3:
                speed = 25;
                break;
            case 4:
                speed = 10;
                break;
            case 5:
                speed = 5;
                break;
            case 6:
                speed = 4;
                break;
            case 7:
                speed = 3;
                break;
            case 8:
                speed = 2;
                break;
            case 9:
                speed = 1;
                break;
            case 10:
                speed = 0;
                break;
        }
        level_up_on = 0; // 레벨업 flag 꺼줌

        gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); // 레벨 표시
        gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10 - cnt); // 레벨 목표 표시

    }
}

void check_game_over(void) {
    int i;

    int x = 5;
    int y = 5;

    for (i = 1; i < MAIN_X - 2; i++) {
        if (main_org[3][i] > 0) { // 천장(위에서 세번째 줄)에 inactive가 생성되면 게임 오버
            gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤"); // 게임 오버 메세지
            gotoxy(x, y + 1); printf("▤                              ▤");
            gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
            gotoxy(x, y + 3); printf("▤  |  G A M E  O V E R..   |   ▤");
            gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
            gotoxy(x, y + 5); printf("▤   YOUR SCORE: %6d         ▤", score);
            gotoxy(x, y + 6); printf("▤                              ▤");
            gotoxy(x, y + 7); printf("▤  Press any key to restart..  ▤");
            gotoxy(x, y + 8); printf("▤                              ▤");
            gotoxy(x, y + 9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
            last_score = score; // 게임 점수를 옮김

            if (score > best_score) { // 최고 기록 갱신시
                FILE* file = fopen("score.dat", "wt"); // score.dat에 점수 저장

                gotoxy(x, y + 6); printf("▤  ★★★ BEST SCORE! ★★★   ▤  ");

                if (file == 0) { // 파일 에러 메세지
                    gotoxy(0, 0);
                    printf("FILE ERROR: SYSTEM CANNOT WRITE BEST SCORE ON \"SCORE.DAT\"");
                }
                else {
                    fprintf(file, "%d", score);
                    fclose(file);
                }
            }
            Sleep(1000);
            while (kbhit()) getch();
            key = getch();
            reset();
        }
    }
}


void pause(void) { // 게임 일시 정지 함수
    int i, j;

    int x = 5;
    int y = 5;

    for (i = 1; i < MAIN_X - 2; i++) { // 게임 일시 정지 메세지
        gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
        gotoxy(x, y + 1); printf("▤                              ▤");
        gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
        gotoxy(x, y + 3); printf("▤  |       P A U S E       |   ▤");
        gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
        gotoxy(x, y + 5); printf("▤  Press any key to resume..   ▤");
        gotoxy(x, y + 6); printf("▤                              ▤");
        gotoxy(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
    }
    getch(); // 키 입력 시까지 대기

    system("cls"); // 화면 지우고 새로 그림
    reset_main_cpy();
    draw_main();
    draw_map();

    for (i = 1; i < 3; i++) { // 다음 블록 그림
        for (j = 0; j < 4; j++) {
            if (blocks[b_type_next][0][i][j] == 1) {
                gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
                printf("■");
            }
            else {
                gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
                printf("  ");
            }
        }
    }
}