#include<iostream>
#include<time.h>
#include<conio.h>
#include<Windows.h>

using namespace std;

enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 };

bool isFinished = false;
bool quit = false;

int menumain;

class Object
{
	protected :
		int x, y;
		int originalX, originalY;

	public :
		virtual void Reset() = 0;
		inline int getX() { return x; }
		inline int getY() { return y; }
};

class Ball : public Object
{
	private:
		eDir direction;
	public:
		Ball(int posX, int posY)
		{
			originalX = posX;
			originalY = posY;
			x = posX; 
			y = posY;
			direction = STOP;
		}
		void Reset()
		{
			x = originalX; 
			y = originalY;
			direction = STOP;
		}
		void changeDirection(eDir d)
		{
			direction = d;
		}
		void randomDirection()
		{
			direction = (eDir)((rand() % 6) + 1);
		}
		inline eDir getDirection() { return direction; }
		void Move()
		{
			switch (direction)
			{
				case STOP:
					break;
				case LEFT:
					x--;
					break;
				case RIGHT:
					x++;
					break;
				case UPLEFT:
					x--; y--;
					break;
				case DOWNLEFT:
					x--; y++;
					break;
				case UPRIGHT:
					x++; y--;
					break;
				case DOWNRIGHT:
					x++; y++;
					break;
				default:
					break;
			}
		}
};

class Paddle : public Object
{
	protected:
		int score;
		int min, max;
	public:
		Paddle()
		{
			x = y = 0;
		}
		Paddle(int posX, int posY) 
		{
			originalX = posX;
			originalY = posY;
			x = posX;
			y = posY;
		}
		int getMin()
		{
			return min;
		}
		int getMax()
		{
			return max;
		}
		void addMin()
		{
			this->min += 1;
		}
		void addMax()
		{
			this->max += 1;
		}
		
		inline void Reset() { x = originalX; y = originalY; }
		inline void moveUp() { y--; }
		inline void moveDown() { y++; }
};

class NormalPaddle : public Paddle
{
	public:
		NormalPaddle(int posX, int posY)
		{
			originalX = posX;
			originalY = posY;
			x = posX;
			y = posY;
			min = 0;
			max = 6;

		}
};

class GameManager
{
	protected:
		int menu;
		int width, height;
		int score1, score2;
		char up1, down1, up2, down2;
		char ans;
		char current;
		Ball *ball;
		Paddle *player1, *obstacle1;
		Paddle *player2, *obstacle2;
	public:
		GameManager()
		{
		}
		GameManager(int width, int height/*int menu*/)
		{
			
			srand(time(NULL));
			up1 = 'w'; up2 = 'i';
			down1 = 's'; down2 = 'k';
			score1 = score2 = 0;
			this->width = width;
			this->height = height;
			//this->menu=menu;
			ball = new Ball(width / 1, height / 2);
		//	if(menu == 1 || menu == 2 || menu == 3 || menu == 4)
		//	{
				player1 = new NormalPaddle(1, height / 2 - 3);
				obstacle1 = new NormalPaddle(2, height /3 - 4);
				player2 = new NormalPaddle(width - 2, height / 2 - 3);
				obstacle2 = new NormalPaddle(width / 2, height / 2);
		//	}
		}
		
		~GameManager()
		{
			delete ball, player1, player2;
		}
		void gotoxy(int x, int y)
		{
			COORD c = {x, y};
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
		}
		void scoreUp(Paddle *player)
		{
			if (player == player1) score1++;
			else if (player == player2) score2++;

			ball->Reset();
			player1->Reset();
			player2->Reset();
		}
		void returnMenu()
		{
			cout << "Do you want to play again? (y/n) ";
			cin >> ans;
			if(ans == 'y')
			{
				system("cls");
				score1 = score2 = 0;
			}
			else
			{
				cout << "Back To Main Menu ? (y/n) ";
				cin >> ans;

				if(ans == 'y')
				{
					system("cls");
					quit = true;
				}
				else
				{
					quit = true;
					isFinished = true;
				}
			}
		}
		void validateScore()
		{
			if(score1 == 5)
			{
				cout << endl << "Player 1... YOU ARE THE WINNER" << endl;
				returnMenu();
			}
			
			else if(score2 == 5)
			{
				cout << endl << "Player 2... YOU ARE THE WINNER" << endl;
				returnMenu();
			}
		}
		void validateExit()
		{
			if(current == 'q')
			{
				quit = true;
				isFinished = true;
			}
		}	
		
		void draw(int drawType)
		{
			gotoxy(0, 0);

			for (int i = 0; i < width + 2; i++) cout << "\xDB";
			cout << endl;

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					int ballx = ball->getX();
					int bally = ball->getY();
					int player1x = player1->getX();
					int obstacle1x = obstacle1->getX();
					int obstacle1y = obstacle1->getY();
					int obstacle2x = obstacle2->getX();
					int obstacle2y = obstacle2->getY();
					int player1y = player1->getY();
					int player2x = player2->getX();
					int player2y = player2->getY();
					int minP1 = player1->getMin();
					int maxP1 = player1->getMax();
					int maxO1 = obstacle1->getMax();
					int minO1 = obstacle1->getMin();
					int minP2 = player2->getMin();
					int maxP2 = player2->getMax();
					int maxO2 = obstacle2->getMax();
					int minO2 = obstacle2->getMin();
						
				
					if (drawType == 1) //normal draw
					{
						if (j == 0) cout << "\xDB";

						if (ballx == j && bally == i) cout << "O"; //ball
						
//						else if (player1x == j && player1y == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
//						else if (player1x == j && player1y + 1 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
//						else if (player1x == j && player1y + 2 == i&&minP1<maxP1) cout << "\xDB";
//						else if (player1x == j && player1y + 3 == i&&minP1<maxP1) cout << "\xDB";
//						else if (player1x == j && player1y + 4 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
//						else if (player1x == j && player1y + 5 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
//						
						else if (obstacle1x == j+1 && obstacle1y == i+5&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (obstacle1x == j+1 && obstacle1y  + 1 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (obstacle1x == j+1 && obstacle1y  + 2 == i&&minP1<maxP1) cout << "\xDB";
						else if (obstacle1x == j+1 && obstacle1y  + 3 == i&&minP1<maxP1) cout << "\xDB";
						else if (obstacle1x == j+1 && obstacle1y  + 4 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						else if (obstacle1x == j+1 && obstacle1y  + 5 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
	
//						else if (player2x == 2 && player2y == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
//						else if (player2x == 21 && player2y + 1 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
//						else if (player2x == 2 && player2y + 2 == i&&minP2<maxP2) cout << "\xDB";
//						else if (player2x == 2 && player2y + 3 == i&&minP2<maxP2) cout << "\xDB";
//						else if (player2x == 2 && player2y + 4 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
//						else if (player2x == 2 && player2y + 5 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
//						
						else if (obstacle2x == j && obstacle2y == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (obstacle2x == j && obstacle2y  + 1 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (obstacle2x == j && obstacle2y  + 2 == i&&minP1<maxP1) cout << "\xDB";
						else if (obstacle2x == j && obstacle2y  + 3 == i&&minP1<maxP1) cout << "\xDB";
						else if (obstacle2x == j && obstacle2y  + 4 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						else if (obstacle2x == j && obstacle2y  + 5 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						
						else cout << " ";
						
						minP1++;
						minP2++;
						minO1++;
						minO2++;
						
						if (j == width - 1) cout << "\xDB";
					}
					
					else if (drawType == 2) //paddle 1 bolong
					{
						if (j == 0) cout << "\xDB";

						if (ballx == j && bally == i) cout << "O"; //ball
						
						else if (player1x == j && player1y == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (player1x == j && player1y + 1 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						//else if (player1x == j && player1y + 2 == i&&minP1<maxP1) cout << "\xDB";
						//else if (player1x == j && player1y + 3 == i&&minP1<maxP1) cout << "\xDB";
						else if (player1x == j && player1y + 4 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						else if (player1x == j && player1y + 5 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						
						else if (obstacle1x == j && obstacle1y == i+1&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (obstacle1x == j && obstacle1y  + 1 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (obstacle1x == j && obstacle1y  + 2 == i&&minP1<maxP1) cout << "\xDB";
						else if (obstacle1x == j && obstacle1y  + 3 == i&&minP1<maxP1) cout << "\xDB";
						else if (obstacle1x == j && obstacle1y  + 4 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						else if (obstacle1x == j && obstacle1y  + 5 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
	
						else if (player2x == j && player2y == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						else if (player2x == j && player2y + 1 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						else if (player2x == j && player2y + 2 == i&&minP2<maxP2) cout << "\xDB";
						else if (player2x == j && player2y + 3 == i&&minP2<maxP2) cout << "\xDB";
						else if (player2x == j && player2y + 4 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						else if (player2x == j && player2y + 5 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						
						else cout << " ";
						
						minP1++;
						minP2++;
						
						if (j == width - 1) cout << "\xDB";
					}
					
					else if (drawType == 3) //paddle 2 bolong
					{
						if (j == 0) cout << "\xDB";

						if (ballx == j && bally == i) cout << "O"; //ball
						
						else if (player1x == j && player1y == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (player1x == j && player1y + 1 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (player1x == j && player1y + 2 == i&&minP1<maxP1) cout << "\xDB";
						else if (player1x == j && player1y + 3 == i&&minP1<maxP1) cout << "\xDB";
						else if (player1x == j && player1y + 4 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						else if (player1x == j && player1y + 5 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						
						else if (obstacle1x == j && obstacle1y == i+1&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (obstacle1x == j && obstacle1y  + 1 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (obstacle1x == j && obstacle1y  + 2 == i&&minP1<maxP1) cout << "\xDB";
						else if (obstacle1x == j && obstacle1y  + 3 == i&&minP1<maxP1) cout << "\xDB";
						else if (obstacle1x == j && obstacle1y  + 4 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						else if (obstacle1x == j && obstacle1y  + 5 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
	
						else if (player2x == j && player2y == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						else if (player2x == j && player2y + 1 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						//else if (player2x == j && player2y + 2 == i&&minP2<maxP2) cout << "\xDB";
						//else if (player2x == j && player2y + 3 == i&&minP2<maxP2) cout << "\xDB";
						else if (player2x == j && player2y + 4 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						else if (player2x == j && player2y + 5 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						
						else cout << " ";
						
						minP1++;
						minP2++;
						
						if (j == width - 1) cout << "\xDB";
					}
					
					else if (drawType == 4) //paddle 2 bolong
					{
						if (j == 0) cout << "\xDB";

						if (ballx == j && bally == i) cout << "O"; //ball
						
						else if (player1x == j && player1y == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (player1x == j && player1y + 1 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						//else if (player1x == j && player1y + 2 == i&&minP1<maxP1) cout << "\xDB";
						//else if (player1x == j && player1y + 3 == i&&minP1<maxP1) cout << "\xDB";
						else if (player1x == j && player1y + 4 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						else if (player1x == j && player1y + 5 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						
						else if (obstacle1x == j && obstacle1y == i+1&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (obstacle1x == j && obstacle1y  + 1 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB"; 
						else if (obstacle1x == j && obstacle1y  + 2 == i&&minP1<maxP1) cout << "\xDB";
						else if (obstacle1x == j && obstacle1y  + 3 == i&&minP1<maxP1) cout << "\xDB";
						else if (obstacle1x == j && obstacle1y  + 4 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
						else if (obstacle1x == j && obstacle1y  + 5 == i&&minP1<maxP1&&menu!=2&menu!=4) cout << "\xDB";
	
						else if (player2x == j && player2y == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						else if (player2x == j && player2y + 1 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						//else if (player2x == j && player2y + 2 == i&&minP2<maxP2) cout << "\xDB";
						//else if (player2x == j && player2y + 3 == i&&minP2<maxP2) cout << "\xDB";
						else if (player2x == j && player2y + 4 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						else if (player2x == j && player2y + 5 == i&&minP2<maxP2&&menu!=3&menu!=4) cout << "\xDB";
						
						else cout << " ";
						
						minP1++;
						minP2++;
						
						if (j == width - 1) cout << "\xDB";
					}
				}
				cout << endl;
			}
			for (int i = 0; i < width + 2; i++) cout << "\xDB";
			cout << endl;

			cout << "Score 1: " << score1 << endl << "Score 2: " << score2 << endl << "You'll win if your score is 5" << endl << "Press 'q' to exit" << endl;	
		}
		
		void validateMove(int drawType)
		{
			ball->Move();

			int ballx = ball->getX();
			int bally = ball->getY();
			int player1x = player1->getX();
			int player2x = player2->getX();
			int player1y = player1->getY();
			int player2y = player2->getY();
			int min1,min2,max1,max2, minO1, minO2, maxO1, maxO2;
			if(drawType==1){
				min1 = 0;
				min2 = 0;
				max1 = 6;
				max2 = 6;
				minO1 = 0;
				minO2 = 0;
				maxO1 = 6;
				maxO2 = 6;
			}
			if (_kbhit())
			{
				current = _getch();
				if (current == up1) {if (player1y + min1 > 0) player1->moveUp();}
				if (current == up2) {if (player2y + min2 > 0) player2->moveUp();}
				if (current == down1) {if (player1y + max1 < height) player1->moveDown();}
				if (current == down2) {if (player2y + max2 < height) player2->moveDown();}
				if (ball->getDirection() == STOP) ball->randomDirection();
			}
			
			if (drawType == 1) //normal paddle
			{
				for (int i = min1; i < max1; i++) //left paddle
					if (ballx == player1x + 1)
						if (bally == player1y + i)
								ball->changeDirection((eDir)((rand() % 3) + 4));
				
				for (int i = min2; i < max2; i++) //right paddle
					if (ballx == player2x - 1)
						if (bally == player2y + i)
							ball->changeDirection((eDir)((rand() % 3) + 1));	
			}	
			
			else if (drawType == 2) //left paddle bolong
			{
				for (int i = min1; i < max1; i++)
					if (ballx == player1x + 1)
						if (player1y + 3 != bally)
							if (player1x == 1 && player1y + 2 != bally)
								if (bally == player1y + i)
									ball->changeDirection((eDir)((rand() % 3) + 4));
									
				for (int i = min2; i < max2; i++) //right paddle
					if (ballx == player2x - 1)
						if (bally == player2y + i)
							ball->changeDirection((eDir)((rand() % 3) + 1));	
			}
			
			else if (drawType == 3) //right pdadle bolong
			{
				for (int i = min1; i < max1; i++) //left paddle
					if (ballx == player1x + 1)
						if (bally == player1y + i)
								ball->changeDirection((eDir)((rand() % 3) + 4));
				
				for (int i = min2; i < max2; i++)
					if (ballx == player2x - 1)
						if (player2y + 3 != bally)
							if (player2y + 2 != bally)
								if (bally == player2y + i)
									ball->changeDirection((eDir)((rand() % 3) + 1));
			}
			
			else if (drawType == 4) //both paddle bolong
			{
				for (int i = min1; i < max1; i++)
					if (ballx == player1x + 1)
						if (player1y + 3 != bally)
							if (player1x == 1 && player1y + 2 != bally)
								if (bally == player1y + i)
									ball->changeDirection((eDir)((rand() % 3) + 4));
									
				for (int i = min2; i < max2; i++)
					if (ballx == player2x - 1)
						if (player2y + 3 != bally)
							if (player2y + 2 != bally)
								if (bally == player2y + i)
									ball->changeDirection((eDir)((rand() % 3) + 1));	
			}
			
			//bottom wall
			if (bally == height - 1) ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
			//top wall
			if (bally == 0) ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
			//right wall
			if (ballx == width - 1) scoreUp(player1);
			//left wall
			if (ballx == 0) scoreUp(player2);
	}
		
		void run(int drawType)
		{
			while (!quit)
			{
				draw(menumain);
				validateMove(menumain);
				validateExit();
				validateScore();
			}
		}
};
	
int main()
{
	do
	{
		cout << "Welcome to PONG!" << endl;
		cout << "================" << endl;
		cout << "1. Normal" << endl;
		cout << "2. Unfair Player 1" << endl;
		cout << "3. Unfair Player 2" << endl;
		cout << "4. Unfair Both" << endl;
		cout << "5. Exit" << endl;

		cout << "Select game mode: ";
		cin >> menumain; cin.get();

		GameManager *gameManager;

		gameManager = new GameManager(100, 100);
		gameManager->run(menumain);
		break;

	} while(isFinished == false);
	return 0;
}			

