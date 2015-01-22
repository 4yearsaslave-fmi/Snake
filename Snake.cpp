#include <iostream>
#include <vector>
#include <iostream>
#include <conio.h>
#include <time.h>

#include "ConsoleGaming.h"

using namespace std;

HANDLE consoleHandle;

typedef vector<GameObject>::iterator randomAccess_iterator;
typedef vector<GameObject>::const_iterator const_iterator;

// Window constants
const int WindowWidth = 70;
const int WindowHeight = 30;
// Snake
const int SnakeSpeed = 1;
const int SnakeStartingLength = 5;
const char SnakeSymbol = '*',
	//Fruit
		FruitSymbol = '+';
// Game variables
unsigned long sleepDuration = 200;
vector<GameObject> snake;
vector<GameObject> fruit;

COORD direction = { 1, 0 };

void drawFrame(){
	for (int i = 0; i < 70; i++)
	{
		cout << 'x';
	}
	cout << endl;
	for (int i = 20; i > -1; --i)
	{
		cout << 'x' << setw(69) << 'x' << endl;
	}
	for (int i = 0; i < 70; i++)
	{
		cout << 'x';
	}
}

void startGame(){
	ClearScreen(consoleHandle);
	slowFruit.clear();
	fastFruit.clear();
	wall.clear();
	sleepDuration = 100;
	direction = { 1, 0 };
	drawFrame();
	snake.clear();
	for (int i = SnakeStartingLength; i > -1; --i)
	{
		snake.push_back(GameObject(i + 1, 1, SnakeSymbol));
	}
	snake.begin()->Coordinates.X = 6;
	snake.begin()->Coordinates.Y = 1;

	while (true)
	{
		Update();
		Draw();
		Sleep(sleepDuration);
	}
}
void die(){
	ClearScreen(consoleHandle);
	cout << "Your result is: " << result << endl;
	result = 0;

	playAgain();
}

void playAgain(){
	char input;
	cout << "Do you want to play again?" << '\t' << "Y / N" << '\t';
	cout << "Press M to go back to the menu.";
	cin >> input;

	switch (input)
	{
	case 'y':
		startGame();
		break;
	case 'n':
		exit(0);
		break;
	case 'm':
		mainMenu();
		break;
	
	};

}
void Update()
{
	// Save the tail, we might need it later.
	GameObject tail = *(snake.end() - 1);

	for (randomAccess_iterator i = snake.end() - 1; i != snake.begin(); --i)
	{
		GameObject next = *(i - 1);
		i->UpdateCoordinates(next.Coordinates);
	}

	if (kbhit())
	{
		char key = getch();
		switch (key)
		{
		case 'a':
			direction.X = -SnakeSpeed;
			direction.Y = 0;
			break;
		case 'w':
			direction.X = 0;
			direction.Y = -SnakeSpeed;
			break;
		case 'd':
			direction.X = SnakeSpeed;
			direction.Y = 0;
			break;
		case 's':
			direction.X = 0;
			direction.Y = SnakeSpeed;
			break;
		};
	}

	snake.begin()->Coordinates.X += direction.X;
	snake.begin()->Coordinates.Y += direction.Y;

	GameObject head = *snake.begin();
	for (randomAccess_iterator i = fruit.begin(); i != fruit.end(); ++i)
	{
		if (head.Coordinates.X == i->Coordinates.X && head.Coordinates.Y == i->Coordinates.Y)
		{
			// Remove the old fruit, increase the snake's size
			fruit.erase(i);
			snake.push_back(tail);

			// Add a new fruit
			int x = rand() % WindowWidth;
			int y = rand() % WindowHeight;
			fruit.push_back(GameObject(x, y, FruitSymbol));

			sleepDuration *= (sleepDuration > 50) * 0.1 + 0.9;
			// Break, since you can't eat more than one fruit at the same time.
			break;
		}
	}
}

void Draw()
{
	ClearScreen(consoleHandle);

	for (const_iterator snakeBody = snake.begin(); snakeBody != snake.end(); ++snakeBody)
	{
		snakeBody->Draw(consoleHandle);
	}

	for (const_iterator singleFruit = fruit.begin(); singleFruit != fruit.end(); ++singleFruit)
	{
		singleFruit->Draw(consoleHandle);
	}

}
void mainMenu() {

	ClearScreen(consoleHandle);
	system("color 1a");
	cout << char(201);
	for (int i = 1; i <= 17; i++){
		cout << char(205);
	}
	cout << char(187);
	cout << "\n";
	cout << char(186) << " Main Menu" << setw(8) << char(186) << "\n";
	cout << char(186) << " 1 - Start game" << setw(3) << char(186) << "\n";
	cout << char(186) << " 2 - Instructions" << setw(1) << char(186) << "\n";
	cout << char(186) << " 3 - Quit" << setw(9) << char(186) << "\n";
	cout << char(186) << " Please choose:" << setw(3) << char(186) << "\n";;
	cout << char(200);
	for (int i = 1; i <= 17; i++){
		cout << char(205);
	}
	cout << char(188);
	cin >> choice;
	if (choice == 1)
	{
		startGame();
	}

	if (choice == 2)
	{
		ClearScreen(consoleHandle);
		cout << "Hi!," << endl << "Welcome to Snake,";
		cout << "You may navigate the snake using the keys: " << endl;
		cout << "W-UP\nS-DOWN\nD-RIGHT\nA-LEFT\nM - Go to the Main Menu\n";
		cout << "Power-ups:" << endl;
		cout << "\031-speed++\n\030-speed--\nx-fuck you\n" << endl;
		cout << "Press m to go to the main menu ";
		char button;
		cin >> button;
		switch (button){

		case 'm':
			mainMenu();
			break;
		};
	}
	if (choice == 3)
	{
		exit(1);
	}

}
int main()
{
	PlaySound(TEXT("panther.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	// Randomize stuff
	srand(time(NULL));
	
	int x = rand() % WindowWidth;
	int y = rand() % WindowHeight;
	fruit.push_back(GameObject(x, y, '+'));
	mainMenu();

	return 0;
}
