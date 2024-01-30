#include <iostream>
#include <windows.h>
#include <list>
#include <conio.h>
//#include "Coord.h"
//#include "Point.h"
//#include "SuperObject.h"
#include "Source.cpp"


using namespace std;
char emptyChar = '.';

ostream& operator<<(ostream& out, Point& obj)
{
    return out << obj.icon;
}

void displayClearField()
{
    for (int i = 0; i < HIGH; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            display[i][j].clear(j, i);
        };
    }
}
void displayFill()
{
    for (int i = 0; i < HIGH; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            if (!display[i][j].into) display[i][j].icon = emptyChar;
            else { display[i][j].icon = display[i][j].into->icon; }
        };
    }
}

void displayOut()
{
    for (int i = 0; i < HIGH; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            cout << display[i][j];
        };
        cout << "\n";
    }
}


class Entity : public SuperObject
{
public:
    int life = 1;

    Entity() : SuperObject() {}
    Entity(Point* placeP, char iconP, int lifeP, int speedP = 0, int directP = 0) : SuperObject(placeP, speedP, directP, iconP), life{ lifeP } {}
    virtual int collision_hanlder(SuperObject* obj)
    {
        if (typeid(obj) == typeid(Entity)) //EXAMPLE
        {
            obj = dynamic_cast<Entity*>(obj);
            life = 0;
        }
        return 1;
    }
};

class Item : public SuperObject
{
public:
    int temp = 2;

    Item() : SuperObject() {}
    Item(Point* placeP, char iconP, int tempP, int speedP = 0, int directP = 0) :
        SuperObject(placeP, speedP, directP, iconP), temp{ tempP } {}

    virtual int collision_hanlder(SuperObject* obj)
    {
        //your code here
        return 1;
    }
};

int main()
{
    Entity player;
    player.link(&display[5][5]);
    player.icon = '@';
    player.life = 10;
    Entity enemy(&display[5][7], '$', 15);
    Item sword(&display[3][3], '!', 2);

    //добавление объектов в список
    objects.push_back(&player);
    objects.push_back(&enemy);
    objects.push_back(&sword);

    Coord tempCoord(0, 0);

    while (main_flag)
    {
        // -----------STEP 1: input-----------
        // keyboard reciver
        keyboardPress = _getch();
        switch (keyboardPress)
        {
        case 'w':
            player.getCoord().y -= 1;
            break;
        case 'd':
            player.getCoord().x += 1;
            break;
        case 's':
            player.getCoord().y += 1;
            break;
        case 'a':
            player.getCoord().x -= 1;
            break;
        case ' ':
            break;
        case 27:
            main_flag = false;  //exit
            break;
        }

        // environment motor
        // здесь будет логика всех объектов: 
        // исполнение каких то паттернов движения, появление, применение свойств итд
        // в общем все, что должно произойти за этот такт



        // ---------STEP 2: processing---------
        // здесь же примененные действия обрабатываются, в частности - в блоке коллизии

        for (SuperObject* curObj : objects)
        {
            if (curObj->speed != 0)
            {
                tempCoord = curObj->move();
                //проверка на то, есть ли что-то в этой точке (into=nulptr - false - пустота)
                if (display[tempCoord.y][tempCoord.x].into)
                {
                    display[tempCoord.y][tempCoord.x].into->collision_hanlder(curObj);
                    curObj->collision_hanlder(display[tempCoord.y][tempCoord.x].into);
                }
                else
                {
                    curObj->link(&display[tempCoord.y][tempCoord.x]);
                }
            }
        }

        // -----------STEP 3: output-----------
        // вывод сцены на экран
        // очистка сцены и наполнение ее
        system("cls");
        displayFill();
        // добавление всех объектов на сцену
        // вывод сцены на экран
        displayOut();
        cout << keyboardPress << endl;
        Sleep(latency);
    }
}