#include <iostream>﻿
#include <windows.h>
#include <list>
#include <conio.h>
#include <vector>
#include <random>

using namespace std;
char emptyChar = '.';

class Coord
{
public:
    int x = 0;
    int y = 0;
    Coord(int xP, int yP) :
        x{ xP }, y{ yP } {};
    Coord& operator()(int xP, int yP)
    {
        x = xP; y = yP;
        return *this;
    };
};

class SuperObject;
class Point
{
public:
    char icon = emptyChar;
    Coord coord;
    SuperObject* into;
    Point() : icon{ emptyChar }, coord(0, 0), into{ nullptr } {};
    Point(Coord coordP) : coord{ coordP }, into{ nullptr } {};
    Point(Coord coordP, SuperObject* intoP, char iconP = emptyChar) :
        icon{ iconP }, coord(coordP), into{ intoP } {};

    Point* operator()(char iconP)
    {
        icon = iconP;
        return this;
    }

    void clear(int xP = 0, int yP = 0)
    {
        coord(0, 0);
        icon = emptyChar;
        into = nullptr;

    }
};

ostream& operator<<(ostream& out, Point& obj)
{
    return out << obj.icon;
}

class SuperObject
{
public:
    Point* place;
    char icon;
    int speed{ 1 }; // квантификатор скорости
    bool ismov{ false }; // движется объект или нет
    int direct{ 0 };
    SuperObject() :
        place{ nullptr }, speed{ 0 }, direct{ 0 }, icon{ emptyChar } {}
    SuperObject(Point* placeP, int speedP = 0, int directP = 0, char iconP = emptyChar) :
        speed{ speedP }, direct{ directP }, icon{ iconP }
    {
        link(placeP);
    }

    virtual Coord* getCoord()
    {
        return &place->coord;
    }

    virtual void link(Point* p)
    {
        if (place != nullptr)
            place->into = nullptr;

        if (p->into != nullptr) {}
        else
        {
            place = p;
            p->into = this;
        }
    }
    virtual int collision_hanlder(SuperObject* obj)
    {
        return 0;
    };
    virtual Coord move() //возвращает координаты следующего местоположения. не меняет текущие параметры
    {
        Coord tcoord(place->coord);
        tcoord = place->coord;
        switch (direct) {
        case(0):
            break;
        case(1):
            tcoord.y -= 1;
            break;
        case(2):
            tcoord.x += 1;
            break;
        case(3):
            tcoord.y += 1;
            break;
        case(4):
            tcoord.x -= 1;

            break;
        }
        return tcoord;
    }
};

//----- env var -----

const int HIGH = 15;
const int WIDTH = 25;
//char emptyChar = '.';

int fps = 4;
int latency = 1000 / fps;

Point display[HIGH][WIDTH]{};
vector<SuperObject*> objects;

char keyboardPress;
bool main_flag = true;

char heart = 3;
char nothing = 219;
string inventory_info;

int circle_move[4] = { 1,2,3,4 };
//===== env var =====
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

class Item : public SuperObject
{
public:
    int temp = 2;

    Item() : SuperObject() {}
    Item(Point* placeP, char iconP, int tempP, int speedP = 0, int directP = 0) :
        SuperObject(placeP, speedP, directP, iconP), temp{ tempP } {}

    //virtual int collision_hanlder(SuperObject* obj)
    //{
    //    //your code here
    //    return 1;
    //}
};

class Case : public SuperObject
{
public:
    vector <Item*> inventory;
    Case() : SuperObject(), inventory{ 10 } {};
    Case(Point* placeP, char iconP, int sizeInt, int speedP = 0, int directP = 0) : SuperObject(placeP, speedP, directP, iconP), inventory(sizeInt) {};
};

class Instrument : public Item
{
public:
    int damagePlus;
    int life;
    int ammo;
    Instrument() : Item(), damagePlus{ 1 }, life{ 5 }, ammo{ 10 } {}
    Instrument(Point* placeP, char iconP, int tempP, int damagePlusP, int lifeP, int ammoP, int speedP = 0, int directP = 0) :
        Item(placeP, iconP, tempP), damagePlus{ damagePlusP }, life{ lifeP }, ammo{ ammoP } {}
};

class Coin : public Item
{
public:
    int volume;
    Coin() : Item(), volume{ 1 } {}
    Coin(Point* placeP, char iconP, int tempP, int volumeP, int speedP = 0, int directP = 0) :
        Item(placeP, speedP, directP, iconP, tempP), volume{ volumeP } {}
};

class Entity : public SuperObject
{
public:
    int life = 1;
    int damage;
    vector <Item*> inventory;

    Entity() : SuperObject(), inventory(1), damage(1) {}
    Entity(Point* placeP, char iconP, int lifeP, int damageP, int speedP = 0, int directP = 0, int sizeInt = 1) :
        SuperObject(placeP, speedP, directP, iconP), life{ lifeP }, inventory(sizeInt), damage{ damageP } {}
    virtual int collision_hanlder(SuperObject* obj)
    {
        if (typeid(obj) == typeid(Case)) //EXAMPLE
        {
            obj = dynamic_cast <Case*>(obj);
            link(obj->place);
            ismov = false;
        }
        return 1;
    }
};

class Human : public Entity
{
public:

    Human() : Entity() {};
    Human(Point* placeP, char iconP, int lifeP, int damageP, int speedP = 0, int directP = 0, int sizeInt = 5) :
        Entity(placeP, speedP, directP, iconP, lifeP, damageP, sizeInt) {}
    //int collision_hanlder(SuperObject* obj)
    //{
    //    Entity::collision_hanlder(obj);
    //}
};
class Monster : public Entity
{
public:
    bool friendly;
    int scorePlus;
    Monster() : Entity(), friendly(false), scorePlus(0) {};
    Monster(Point* placeP, char iconP, int lifeP, int damageP, bool friendlyP, int scorePlusP, int speedP = 0, int directP = 0, int sizeInt = 2) :
        Entity(placeP, iconP, lifeP, damageP, sizeInt), friendly{ friendlyP }, scorePlus{ scorePlusP } {}
    virtual int collision_hanlder(SuperObject* obj);
};

void deleting(SuperObject* obj) {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i] == obj) {
            display[objects[i]->getCoord()->y][objects[i]->getCoord()->x].into = nullptr;
            objects.erase(objects.cbegin() + i);
            break;
        }
    }
}

void stopObject(SuperObject* obj) {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i] == obj) {
            objects.erase(objects.cbegin() + i);
            break;
        }
    }
}

class Player : public Entity
{
public:
    int score;
    Player() : Entity(), score(0) {};
    Player(Point* placeP, char iconP, int lifeP, int damageP, int speedP = 0, int directP = 0, int sizeInt = 1) :
        Entity(placeP, iconP, lifeP, damageP, speedP, directP, sizeInt), score(0) {}
    virtual int collision_hanlder(SuperObject* obj)
    {
        if (typeid(*obj) == typeid(Case))
        {
            Case* caseObj = dynamic_cast<Case*>(obj);
            if (caseObj)
            {
                caseObj->icon = 'O';
                link(caseObj->place);
                link(this->place);
            }
            inventory_info = "";  // очищение предыдущего результата
            for (int i = 0; i < caseObj->inventory.size(); i++) {
                if (caseObj->inventory[i] != nullptr) inventory_info += string(1, caseObj->inventory[i]->icon) + "|";
                else inventory_info += " |";
            }
            inventory_info += "\n";
        }
        else if (typeid(*obj) == typeid(Instrument))
        {
            Instrument* instrumentObj = dynamic_cast<Instrument*>(obj);
            for (int i = 0; i < this->inventory.size(); i++) {
                if (this->inventory[i] == nullptr) {
                    this->inventory[i] = instrumentObj;
                    this->damage += instrumentObj->damagePlus;                                       
                    deleting(instrumentObj);
                    break;
                }
                else if (typeid(*this->inventory[i]) == typeid(Instrument)) {
                    Instrument* myObj = dynamic_cast<Instrument*>(inventory[i]);
                    this->damage -= myObj->damagePlus;
                    this->inventory[i] = instrumentObj;
                    this->damage += instrumentObj->damagePlus;
                    deleting(instrumentObj);
                    break;
                }
            }
        }
        else if (typeid(*obj) == typeid(Monster))
        {
            Monster* monsterObj = dynamic_cast<Monster*>(obj);
            if (monsterObj->life > 0) 
            {
                monsterObj->life -= this->damage;
                if (monsterObj->life <= 0) {
                    monsterObj->ismov = false;
                    stopObject(monsterObj);
                    monsterObj->icon = 197;
                    this->score += monsterObj->scorePlus;
                }                
            }
            else if (monsterObj->life <= 0) 
            {
                inventory_info = "";  // очищение предыдущего результата
                for (int i = 0; i < monsterObj->inventory.size(); i++) {
                    if (monsterObj->inventory[i] != nullptr) inventory_info += string(1, monsterObj->inventory[i]->icon) + "|";
                    else inventory_info += " |";
                }
                inventory_info += "\n";
            }
        }
        return 1;
    }
};

int Monster::collision_hanlder(SuperObject* obj) {
    if (typeid(*obj) == typeid(Player))
    {
        Player* playerObj = dynamic_cast<Player*>(obj);
        if (this->life > 0)
        {
            playerObj->life -= this->damage;
        }
    }
    return 1;
}


int enemyMoved[4] = { 1,2,3,4 };
char animated[5] = { '.',',',';','\"' };
int main()
{
    for (int i = 0; i < HIGH; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            display[i][j].coord(j, i);
        }
    };
    Player player(&display[5][5], '@', 10, 1);
    Monster enemy(&display[5][9], 'M', 5, 2, false, 10);
    Monster enemy2(&display[10][20], 'M', 5, 2, false, 10);
    Monster enemy3(&display[9][9], 'M', 5, 2, false, 10);
    Instrument pistol{ &display[2][3], 'S', 2, 20, 5, 20 };
    Instrument sword{ &display[3][3], '!', 2, 8, 2, 0 };
    Case bag(&display[8][9], 'B', 10);
    bag.inventory[0] = &sword;
    bag.inventory[1] = &sword;

    //добавление объектов в список
    objects.push_back(&player);
    objects.push_back(&enemy);
    objects.push_back(&enemy2);
    objects.push_back(&enemy3);
    objects.push_back(&pistol);
    objects.push_back(&sword); 
    objects.push_back(&bag);

    Coord tempCoord(0, 0);
    int i = 0;
    cout << "Press any key to start";
    while (main_flag)
    {
        keyboardPress = _getch();
        // -----------STEP 1: input-----------
        // keyboard reciver
        switch (keyboardPress)
        {
            // объекты двигаются через direct. если direct=0 - объект стоит. 
            // speed - это как модификатор передвижения, который показывает 
            // расстояние передвижения за один такт. по умолчанию speed=1
        case 'w':
            player.ismov = true;
            player.direct = 1;
            player.icon = 30;
            break;
        case 'd':
            player.ismov = true;
            player.direct = 2;
            player.icon = 16;
            break;
        case 's':
            player.ismov = true;
            player.direct = 3;
            player.icon = 31;
            break;
        case 'a':
            player.ismov = true;
            player.direct = 4;
            player.icon = 17;
            break;
        case ' ':
            break;
        case 27:
            main_flag = false;  //exit
            break;
        }

        // environment motor
        // здесь будет логика всех объектов: 
        // исполнение каких то паттернов движения, появление, применение свойств и тд
        // в общем все, что должно произойти за этот такт

        /*if (i > 3) i = 0;
        enemy.ismov = true;
        enemy.direct = enemyMoved[i];
        i++;*/
        
        for (int i = 0; i < objects.size(); i++)
        {
            if (typeid(*objects[i]) == typeid(Monster)) 
            {
                Monster *monsterObj = dynamic_cast<Monster*>(objects[i]);
                if (rand() % 3 > 0)
                {
                    monsterObj->ismov = true;
                    if (abs(player.getCoord()->x - monsterObj->getCoord()->x) > abs(player.getCoord()->y - monsterObj->getCoord()->y))
                    {
                        if ((player.getCoord()->x - monsterObj->getCoord()->x) > 0)
                        {
                            monsterObj->direct = 2;
                        }
                        else
                        {
                            monsterObj->direct = 4;
                        }
                    }
                    else
                    {
                        if ((player.getCoord()->y - monsterObj->getCoord()->y) > 0)
                        {
                            monsterObj->direct = 3;
                        }
                        else
                        {
                            monsterObj->direct = 1;
                        }
                    }
                }
            }
        }

        enemy3.direct = circle_move[i];
        i++;
        if (i == 4) i = 0;

        // ---------STEP 2: processing---------
        // здесь же примененные действия обрабатываются, в частности - в блоке коллизии

        for (int i = 0; i < objects.size(); i++)
        {
            SuperObject* curObj = objects[i];
            //здесь можно изменять CurObj->icon (сделать анимацию)
            if (curObj->ismov)
            {
                tempCoord = curObj->move();
                //проверка на то, есть ли что-то в этой точке (into=nulptr - false - пустота)
                if ((tempCoord.y >= 0 && tempCoord.y < HIGH) and (tempCoord.x >= 0 && tempCoord.x < WIDTH))
                {
                    if (display[tempCoord.y][tempCoord.x].into)
                    {
                        //display[tempCoord.y][tempCoord.x].into->collision_hanlder(curObj);
                        curObj->collision_hanlder(display[tempCoord.y][tempCoord.x].into);
                    }
                    else
                    {
                        curObj->link(&display[tempCoord.y][tempCoord.x]);
                        curObj->ismov = false;
                    }
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
        cout << "Life: ";
        for (int i = 0; i < player.life; i++) {
            cout << heart;
        }
        cout << endl << "Score: " << player.score;
        cout << endl << "Damage: " << player.damage;
        cout << endl << "Inventory: ";
        for (int i = 0; i < player.inventory.size(); i++) {
            if (player.inventory[i] != nullptr) { cout << player.inventory[i]->icon << "|"; }
            else  cout << nothing <<"|";
        }
        cout << endl;
        cout << inventory_info;
        inventory_info = "";

        //for (int i = 0; i < objects.size(); i++) {
        //    cout << objects[i] << "\n";
        //}

        if (player.life <= 0) {
            main_flag = false;
            system("cls");
            cout << "You lose!";
        }
    }
}
