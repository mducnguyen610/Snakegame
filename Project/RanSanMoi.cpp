
#include <iostream>
#include <winbgim.h>
#include <string.h>
#include <stdio.h>

// dinh nghia cac gia tri su dung cho viec thiet ke do hoa game
#define 	MINX 		20
#define 	MINY 		20
#define 	MAXX 		770
#define 	MAXY 		435
#define 	BACKGROUND	7

class Point {
public:
	int x,y;
	int x0,y0;
};

class HighScore {
public:
	int score;
	char name[30];
};

int level; // level cua game , di nhanh hay di cham 
bool endGame; // Luu trang thai cua game: ket thuc hay chua
int snakeLength; //  do dai hien tai cua ran
Point snake[100]; // con ran 
Point direction; // huong di cua ran.
Point food; // doi tuong thuc an
const int DIRECTION = 10; // khoang cach di chuyen
HighScore highscore[5];	
char* score_str = new char[20]; // Convert score to string

void changeDirecton (int x);
void moveSnake ();
void showText (int x,int y,char *str); //Ham hien thi text
void showTextBackground (int x,int y,char *str,int color); //Ham con hien thi Text Background 
bool checkPoint ();
void drawPoint (int x,int y,int r);
void drawFood ();
void drawSnake ();
void drawMap ();
void initGame ();
void mainLoop (void (*gloop)());
void showHighScore ();
void getHighScore ();
void checkHighScore(int score);
void run ();


void changeDirecton (int x){
	switch(x){
                case 72: 
                    if (direction.y != DIRECTION) { 		//dieu kien de khong cho di nguoc
            			direction.y = -DIRECTION; 
						direction.x = 0;
        			}
                    break;
                case 80: 
                	if (direction.y != -DIRECTION) {
            			direction.y = DIRECTION; 
						direction.x = 0;
        			}
                   break;
                case 77:
                    if (direction.x != -DIRECTION) {
            			direction.x = DIRECTION; 
						direction.y = 0;
        			}
                    break;
                case 75:
                    if (direction.x != DIRECTION) {
            			direction.x = -DIRECTION; 
						direction.y = 0;
        			}
                    break;
                case 27:
                	endGame = true;
					break;
            }
}

// Snake is moving
void moveSnake(){
	for (int i = 0;i < snakeLength;i++){
		if (i == 0){
			snake[0].x0 = snake[0].x;
			snake[0].y0 = snake[0].y;		//Luu toa do cu cua dau ran
			snake[0].x += direction.x;
			snake[0].y += direction.y;			//toa do moi cua dau ran
		}
		else {
			snake[i].x0 = snake[i].x;		//Luu toa do cua cac dot than i
			snake[i].y0 = snake[i].y;
			snake[i].x = snake[i-1].x0;		//lay toa do cua dot than truoc no i-1
			snake[i].y = snake[i-1].y0;
		}
		if (snake[0].x < MINX+5 || snake[0].y < MINY+5 || snake[0].x > MAXX-5 || snake[0].y > MAXY - 5 )
			endGame = true;
		if (i != 0 && snake[0].x == snake[i].x && snake[0].y == snake[i].y)	 endGame = true;
	}
	if (snake[0].x == food.x && snake[0].y == food.y){  
		snake[snakeLength].x = snake[snakeLength-1].x0;snake[snakeLength].y = snake[snakeLength-1].y0; //toa do food thanh toa do dau ran
		snakeLength++;
		//Random food again
		do{
        	food.x = (rand() % (73) + 3)*10;
    		food.y = (rand() % (41) + 3)*10;
		}while (checkPoint() == false);
	}
}

void showTextBackground(int x,int y,char *str,int color){
	int bk = getbkcolor();
	setbkcolor(color);
	outtextxy(x,y,str);
	delay(200);
	setbkcolor(bk);
}

void showText(int x,int y,char *str){
	setcolor(2);
	outtextxy(x+3,y,str);
	setcolor(2);
	delay(50);
}

bool checkPoint (){   //Kiem tra trung lap de dam bao khong tao food trung voi Ran
	for (int i = 0;i < snakeLength;i++){
		if (food.x == snake[i].x && food.y == snake[i].y)
		return false;
	}
	return true;
}

void drawPoint (int x,int y,int r){  //Ham hien thi do hoa doi tuong Point
	circle (x,y,r);
	floodfill (x,y,getcolor());
}

void drawFood (){				//Ham ve Food
	int x = getcolor();
	int c = rand() % 14 + 1;
	setcolor(c);
	setfillstyle (1, c);
	drawPoint(food.x, food.y,5);
	setcolor(x); //lay lai mau ve
}

void drawSnake (){
	int x = getcolor();
	for (int i = 0;i < snakeLength;i++){
		if (i == 0){
			setcolor(4);
			setfillstyle (1, 4);
			drawPoint(snake[0].x, snake[0].y,5);
			setcolor(x);
		}
		else{
			setfillstyle (1,2);
		}	drawPoint(snake[i].x, snake[i].y,5);
		}

	//delete point when snake move
	setfillstyle (1,BACKGROUND); //to vong tron mau bk
	setcolor (BACKGROUND); // to vien mau bk
	drawPoint(snake[snakeLength-1].x0,snake[snakeLength-1].y0,5);
	setcolor(x);
}

//Ve toan bo giao dien do hoa choi game
void drawMap (){
	drawSnake();
	drawFood();
	int x = getcolor ();
	setcolor (4);
	sprintf(score_str,"%d",snakeLength*level*10-level*30);    //show score tren man hinh choi game
    settextstyle(8,0,4);
    outtextxy(515,455,"SCORE:");
    setcolor(4);
    outtextxy(670,455,score_str);
	setcolor (x);
 }

void initGame(){  //khoi tao khung game
	setbkcolor (BACKGROUND);
	cleardevice ();
	setwindowtitle ("Playing");
	endGame = false;
	setfillstyle (1,2);
	// ve khung do hoa game
	bar (MINX,MINY,MAXX,MINY+5);
	bar (MAXX,MINY,MAXX-5,MAXY);
	bar (MAXX,MAXY,MINX,MAXY-5);
	bar (MINX,MINY,MINX+5,MAXY);
    setcolor (0);
    int bk = getbkcolor();
	settextstyle(8,0,3);
	setbkcolor(15);
	outtextxy(60,455,"SPACE");
	setbkcolor(bk);
	setcolor(0);
	settextstyle (3,0,3);
	outtextxy(200,455,"Pause/Resume");
	settextstyle(8,0,3);
	setbkcolor(15);
	outtextxy(80,505,"ESC");
	setbkcolor(bk);
	setcolor(0);
	settextstyle (3,0,3);
	outtextxy(200,505,"End Game");

	snake[0].x = 60;snake[0].y = 50;
	snake[1].x = 50;snake[1].y = 50;
	snake[2].x = 40;snake[2].y = 50;
	snakeLength = 3;
	//Init snake
	direction.x = 10;
	direction.y = 0; 
	setcolor (2);
	//Init food
    do{
        food.x = (rand() % (73) + 3)*10;
    	food.y = (rand() % (41) + 3)*10;
	}while (checkPoint() == false);
}

void mainLoop (void (*p)()){
	int x = getcolor();
	if (!kbhit()){
		p(); //chay ham con tro p tro toi
	}
	else{
		char ch;
        ch = getch();
		if (ch == -32) ch = getch(); // cac phim mui ten
		if ( ch == 32){		//nhan phim space
		//hien thi khi nhan spacebar
			setcolor(0);
			outtextxy (290,215,"Game Pause");
			setcolor(x);
			ch = getch();
			while (ch != 32){ 			
				ch = getch();
				delay(0);
			}
			//xaa hien thi 
			setcolor (BACKGROUND);
			outtextxy (290,215,"Game Pause");
			setcolor (x);
			delay(100);
		}
        changeDirecton(ch);
        mainLoop (p);
    }
}

void showHighScore(){
	FILE *f;
	f = fopen("highscore.txt", "r");
	char ch[20];
	settextstyle (3,0,5);
	setcolor (15);
	outtextxy (150,50,"HIGH SCORE");
	settextstyle (3,0,4);
	int y = 150,count = 0;
	while (!feof(f)){
		if (count == 10) break;
		count++;
		fscanf(f, "%s", ch);
   		if (count%2 == 1){
   			setcolor (10);
   			outtextxy (180,y,ch);
   			y+=50;
		}
		else{
			setcolor (12);
			outtextxy (500,y-50,ch);	   	
		}
	}
fclose(f);
}

void getHighScore (){
	FILE *f;
	f = fopen("highscore.txt", "w");
	for (int i = 0;i < 5;i++){
		fputs(highscore[i].name,f);
		fputs(" ",f);
		fprintf(f,"%d",highscore[i].score);
		fputs("\n",f);
	}
fclose(f);
}

void checkHighScore (int _score){
	char _name[20]={""};
	for (int i = 0;i < 5;i++){
		if (_score > highscore[i].score){
			settextstyle(3,0,3);
				for (int j = 0;j < 50;j++){
					if (j%2 == 0){
						setcolor (14);
						if (i == 0)
						outtextxy(480,110,"BEST SCORE");
						else
						outtextxy(460,100,"HIGH SCORE");
						delay(100);
					}
					else{
						setcolor (9);
						if (i == 0)
						outtextxy(480,110,"BEST SCORE");
						else
						outtextxy(460,100,"HIGH SCORE");
						delay(100);
					}
				}
			settextstyle(3,0,2);
			setcolor (4);outtextxy(430,150,"Player:");
			delay(100);
			char ch1;
			int x = 0;
			char str[2];
			str[1] = 0;
			while (ch1 != 13 && x < 10){
				do{
					ch1 = getch();
				}
				while (ch1 < 65 && ch1 >90 || ch1 < 97 && ch1 > 132);
					x++;
					str[0] = ch1;
					strcat(_name,str);
					outtextxy(540,150,_name);
			}
			for (int j = 4;j > i;j--){
				strcpy(highscore[j].name,highscore[j-1].name);
				highscore[j].score = highscore[j-1].score;
			}
			strcpy(highscore[i].name,_name);
			highscore[i].score = _score;
			break;
		}
	}
getHighScore();
}

// khoi tao ung dung
void run () { 
	initwindow (800,600); //Khoi tao cua so ung dung
	int x = 0;
	char *s = new char[5];
	while (true){
		x1: //Menu cua game
		int bk = getbkcolor();
		cleardevice();
		setbkcolor(BACKGROUND);
		cleardevice();
		clearmouseclick(WM_LBUTTONDOWN);
		while (!ismouseclick(WM_LBUTTONDOWN) || mousex() < 200 || mousey() > 465 || mousey() < 190 || mousex() > 580 ){
			settextstyle(3,0,8);
			setcolor (10);
			outtextxy(150,60,"GAME MENU");
			settextstyle(3,0,5);
			showText (220,200,"NEW GAME");
			showText (220,270,"HIGH SCORE");
			showText (220,340,"QUIT GAME");
			showText (220,410,"ABOUT");
		}
		clearmouseclick(WM_LBUTTONDOWN);
		if (mousey() <= 255){
			showTextBackground(220,200,"NEW GAME",15);
			x2: //Chay new game
			setbkcolor(BACKGROUND);
			cleardevice();
			int nameFunction;
			clearmouseclick(WM_LBUTTONDOWN);
			setbkcolor(BACKGROUND);
			cleardevice();
			setcolor(2);
			settextstyle(3,0,6);
			outtextxy(175,150,"CHOOSE LEVEL");
			setcolor(2);
			settextstyle(3,0,5);
			outtextxy(50,300,"LEVEL:");
			int xx = 330;
			char str1[2];
			clearmouseclick(WM_LBUTTONDOWN);
			while (!ismouseclick(WM_LBUTTONDOWN) || mousex() < 300 || mousex() > 790 || mousey() < 280 || mousey() > 350) 
				for(int i = 1;i <= 5;i++){
				sprintf(str1,"%d",i);
				settextstyle(3,0,5);
				showTextBackground(xx,300,str1,BACKGROUND); 
				xx+= 100;
			}
			if (ismouseclick(WM_LBUTTONDOWN) && mousex() >= 300 && mousex() <= 790 && mousey() >= 280 && mousey() <= 350){
				if(mousex() <= 390) level = 1;
				else if(mousex() <= 490) level = 2;
				else if(mousex() <= 590) level = 3;
				else if(mousex() <= 690) level = 4;
				else level = 5;
			}
	
			initGame();
			while (!endGame){
			//Change delay follow level
				delay (250 - (level*4)*40);
				mainLoop(moveSnake); // main loop ve ham moveSnake
				drawMap();
			}
		
			settextstyle (3,0,5);
			setcolor (0);
			outtextxy (90,110,"GAME OVER!");
			setcolor (8);
			settextstyle (1,0,3);
			outtextxy (100,170,"Your Score: ");
			setcolor(10);
			outtextxy (300,170,score_str);
			//Here is check high score
			checkHighScore(snakeLength*level*10-level*30);
			setcolor(10);
			settextstyle(3,0,5);
			outtextxy (100,210,"Play Again?");
			settextstyle(4,0,3);
			setcolor(1);
			clearmouseclick(WM_LBUTTONDOWN);
			while (!ismouseclick(WM_LBUTTONDOWN)){
				showTextBackground(150,280,"YES",7);
				showTextBackground(300,280,"NO",7);
			}
			if(ismouseclick(WM_LBUTTONDOWN)){
				if (mousex() >= 140 && mousex() <= 230 && mousey() >= 270 && mousey() <= 320){
					showTextBackground(150,280,"YES",7);
					delay(100);
					goto x2; //ch?y l?i game
				}
			else if(mousex() >= 290 && mousex() <= 360 && mousey() >= 270 && mousey() <= 320){
					showTextBackground(300,280,"NO",7);
					delay(100);
					goto x1; //quay v? menu
				}
			}
    	}
		else if (mousey() <= 325){
			showTextBackground(220,270,"HIGH SCORE",15);
			cleardevice();
			showHighScore();
			while (!ismouseclick(WM_LBUTTONDOWN)){
				delay(0);
			}
			goto x1;
		}
		else if(mousey() <= 395){
			showTextBackground (220,340,"QUIT GAME",15);
			exit(0);
		}
		else{
			cleardevice();
			settextstyle (1,0,4);
			setcolor (BACKGROUND);
			int x = getcolor();
			while(!ismouseclick(WM_LBUTTONDOWN)){
				settextstyle(3,0,6);setcolor(15);
				outtextxy(290,20,"ABOUT");
				settextstyle(3,0,4);;setcolor(9);
				outtextxy(100,100,"TRUONG DAI HOC BACH KHOA HA NOI");
				settextstyle(3,0,4);
				showText(320,150,"DE TAI");
				settextstyle(3,0,4);
				showText(200,200,"TRO CHOI \"RAN SAN MOI\"");
				settextstyle(3,0,4);
				showText(200,270,"SINH VIEN THUC HIEN");
				settextstyle(3,0,4);
				showText(150,320,"NGUYEN MINH DUC - 20203895");
				settextstyle(3,0,4);
				showText(200,390,"GIANG VIEN HUONG DAN");
				settextstyle(3,0,4);
				showText(200,440,"THAY HOANG QUANG HUY");
			}
			setcolor(x);
			settextstyle (1,0,2);
			setcolor (4);
			goto x1;
		}
	}
}
int main (){

	run();
	//Giai phong bo nho con tro;
	delete score_str;
	getch();
	closegraph ();
    return 0;
}

