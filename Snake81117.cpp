#include <iostream>
#include <stdlib.h> //rand
#include <conio.h> // getch, kbhit
#include <dos.h> // sleep
#include <limits> //numeric_limits
#include <Windows.h> //clearScreen uses it
#include <stdio.h> // write to file
#include <time.h> // get time for file

using namespace std;

void run();
void record();
void printMap();
void move(int directionX, int directionY);
void update();
void changeDirection(char key);
void clearScreen();
void generateFood();
char getMapValue(int value);

// Setting map dimensions
const int mapwidth = 20;
const int mapheight = 60;

const int size = mapwidth * mapheight;

// The tile values for the map
int map[size];

// Snake head position
int headXposition;
int headYposition;
int direction;
// Score of the game
int score=0;

// Amount of food the snake has in the beginning
int food = 3;

// Checking if game is running
bool running;

int main()
{
    cout<<"\t \t \t Welcome to my SNAKE game \n \n\n";

    cout<<"How to play:\n ***\t W- Go up\t S- Go down\t***\n ***\t A- Go left\t D- Go right\t***\n\n\n";
    cout<<"Instructions: \n The apple\"O\" gives you 10points \n The dollar sign\"$\" gives you 20points\n The fence\"#\" gives you -10points\n\n\n";
    run();
    return 0;
}

// Main game function
void run()
{
    // Initialize the map
     for (int x = 0; x < mapwidth; ++x) {
        map[x] = -1;
        map[x + (mapheight - 1) * mapwidth] = -1;
    }
    // Places left and right walls
    for (int y = 0; y < mapheight; y++) {
        map[0 + y * mapwidth] = -1;
        map[(mapwidth - 1) + y * mapwidth] = -1;
    }
    headXposition = mapwidth / 2;
    headYposition = mapheight / 2;
    map[headXposition + headYposition * mapwidth] = 1;
    generateFood();
    // Setting level
    int level=0;
    // Setting speed according to level
    int speed=0;
    cout<<"Set difficulty\n Press 1 for easy mode\n Press 2 for normal mode\n Type: ";
    while (!(cin>>level) || level<1 || level>2){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.  Try again: ";
    }
    switch(level){
        case 1: speed= 200; break;
        case 2: speed= 1; break;
    }
    running = true;
    while(running)
    {
        if( kbhit())
        {
            changeDirection(getch());
        }
    cout<<" \"O\" gives you 10points \n \"$\" gives you 20points\n \"#\" gives you -10points\n\n";
    cout<<"SCORE: "<<score<<"\t  ";
    update();
    clearScreen();
    printMap();
    _sleep(speed);
    }
    cout << "\n\n\n\n\n\t\t XXX GAME OVER XXX\n\n"<< "\t\t SCORE: "<<score<<endl;
    record();
}

// Refreshes the screen
void clearScreen()
{
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

// Changes snake direction according to what is pressed
void changeDirection(char key)
{

    switch (key)
    {
        case 'w': direction = 0; break;
        case 'd': direction = 1; break;
        case 's': direction = 2; break;
        case 'a': direction = 3; break;
    }
}

// Moves snake head to new location
void move(int directionX, int directionY)
{
    // determine new head position
    int newX = headXposition + directionX;
    int newY = headYposition + directionY;
    // Check if there is food at location
    if (map[newX + newY * mapwidth] == -2)
    {
        // Increasing body length and score
        food++;
        score+=20;
        // Generate new food on map
        generateFood();
    }
    else if (map[newX + newY * mapwidth] == -3)
    {
        // Increasing body length and score
        food++;
        score+=10;
        generateFood();
    }
     else if (map[newX + newY * mapwidth] == -4)
    {
        // Increasing body length and  decreasing score
        score-=10;
        generateFood();
    }

    // Check location is free
     else  if (map[newX + newY * mapwidth] == -1)
    {
      running = false;
      cout<<"You hit a wall";
    }
    else if (map[newX + newY * mapwidth] !=0)
    {
        running = false;
        cout<<"You ate yourself";
    }
    // Move head to new location
    headXposition = newX;
    headYposition = newY;
    map[headXposition + headYposition * mapwidth] = food + 1;

}

// Generates new food on map
void generateFood()
{
    int x = 0;
    int y = 0;
    do
    {
        // Generate random x and y values within the map (1- 28) (1- 58)
        x = rand() % (mapwidth - 2) + 1;
        y = rand() % (mapheight - 2) + 1;
        // If location is not free try again
    } while (map[x + y * mapwidth] != 0);
    // Place new food of type -2, -3 or -4
    map[x + y * mapwidth] =  rand()% 3 - 4;
}
// Updates the map
void update()
{
    // Move in direction indicated
    switch (direction)
    {
        case 0: move(-1, 0); break;
        case 1: move(0, 1); break;
        case 2: move(1, 0); break;
        case 3: move(0, -1); break;
    }
    // Reducing snake values on map by 1
    for (int i = 1; i < size-1; i++) {
        if (map[i] > 0) map[i]--;
    }
}
// Printing the map to console
void printMap()
{
    for (int x = 0; x < mapwidth; ++x)
    {
        for (int y = 0; y < mapheight; ++y)
        {
            // Printing the value at current x,y location
            cout << getMapValue(map[x + y * mapwidth]);
        }
        // Ends the line for next x value
        cout << endl;
    }
}
// Returning graphical symbol for display from map value
char getMapValue(int value)
{
    // part of snake body
    if (value > 0) return '*';
    switch (value)
    {
        // wall
        case -1: return '_';
        // types of  food
        case -2: return '$';
        case -3: return 'O';
        case -4: return '#';
    }
}
// Saving records in a file
void record(){

    char playerName[20],cha, c;
    FILE *info;
    info=fopen("record.txt","a+");
    cout<<"Type your name to save your score.\nType \" - \" if you don't want your progress to be saved\n";
    do
    {
        cin>>playerName;
    } while (playerName =="-");

    if(strcmp(playerName,"-")!=0)
    {
        scanf("%[^\n]",playerName);
        fprintf(info,"____________________________\n");
        fprintf(info,"Player Name :%s\n",playerName);
        //for date and time
        time_t mytime;
        mytime = time(NULL);
        fprintf(info,"Played Date:%s",ctime(&mytime));
        fprintf(info,"Score:%d\n",score);
        for(int i = 0; i <= 50; i++)
        {
            fprintf(info,"____________________________\n");
            fprintf(info,"\n");
            fclose(info);
        }
    }
    printf("To see previous saves type 's' . To exit press any other key \n");
    cha=getch();
    if(cha=='s')
    {
        info=fopen("record.txt","r");
        do
        {
            putchar(c=getc(info));
        }while(c!=EOF);
        system("PAUSE");
    }
    fclose(info);
}
