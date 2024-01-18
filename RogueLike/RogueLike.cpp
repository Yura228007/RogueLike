#include <iostream> 
#include <Windows.h> 
#include <vector> 
#include <conio.h>

using namespace std;

class SuperObject
{
public:
    char icon;
    int x;
    int y;
    SuperObject(int xP, int yP, const char iconP) : x{ xP }, y{ yP }, icon { iconP } {}
    SuperObject() : SuperObject(rand() % 20 + 1, rand() % 20 + 1, '!') {};
};

class Box {
public:
    int x;
    int y;
    int speed;
    int into;

};

class Entity : SuperObject
{
public:
    int health;
    int damage;
    int protection;
    Entity(int xP, int yP, const char iconP, int healthP, int damageP, int protectionP) : SuperObject(xP, yP, iconP), health{ healthP }, damage{ damageP }, protection{ protectionP } {}
    Entity() : SuperObject(), health{ 100 }, damage{ 10 }, protection{ 5 } {}
};


class Item : SuperObject {
public:
    string title;
    int AddHealth;
    int AddDamage;
    int AddProtection;
    Item(int xP, int yP, const char iconP, int addHealth, int addProtection, int addDamage)
        : SuperObject(xP, yP, iconP), AddHealth(addHealth), AddProtection(addProtection), AddDamage(addDamage) {
    }
    Item() : SuperObject(rand() % 20 + 1, rand() % 20 + 1, '$'), AddHealth{ 0 }, AddProtection{ 0 }, AddDamage{ 0 } {}
};

class Box {
public:
    int x;
    int y;
    int speed;
    SuperObject* into;

    void put(SuperObject* obj) {
        x = obj->x;
        y = obj->y;
        into = obj;
    }

    SuperObject* get() {
        return into;
    }
};

class Player : Entity {
public:
    vector <Item> inventory;
    Player(int xP, int yP, int healthP, int damageP, int protectionP) : Entity(xP, yP, '@', healthP, damageP, protectionP) {};
    Player() : Entity() {};
};

int fps = 100;
int latency = 1000 / fps;

const int HIGHT = 22;
const int WIDTH = 22;

bool move_flag = true;

char display[HIGHT][WIDTH];
vector <SuperObject> pointArr = {};

void convert()
{
    display[0][0] = ' '; display[HIGHT - 1][0] = ' ';
    for (int i = 1; i < HIGHT; i++) { display[i][0] = '|'; display[i][WIDTH - 1] = '|'; }
    for (int j = 1; j < WIDTH - 1; j++) { display[0][j] = '_'; display[HIGHT - 1][j] = '_'; }

    for (int i = 1; i < HIGHT - 1; i++)
    {
        for (int j = 1; j < WIDTH - 1; j++) {
            display[i][j] = '.';
        }
    }

    for (int i = 0; i < pointArr.size(); i++) {
        display[pointArr[i].y][pointArr[i].x] = pointArr[i].icon;
    }
}

void print() {
    for (int i = 0; i < HIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << display[i][j];
        }
        cout << "\n";
    }
}

//Коллизия с BOOL игрока при нахождении рядом
bool collision() {
    for (int i = 1; i < pointArr.size(); i++) {
        if (pointArr[i].x == pointArr[0].x && pointArr[i].y == pointArr[0].y \
            || pointArr[i].x == pointArr[0].x + 1 && pointArr[i].y == pointArr[0].y \
            || pointArr[i].x == pointArr[0].x - 1 && pointArr[i].y == pointArr[0].y \
            || pointArr[i].x == pointArr[0].x && pointArr[i].y == pointArr[0].y + 1 \
            || pointArr[i].x == pointArr[0].x && pointArr[i].y == pointArr[0].y - 1)
        {
            return true;
        }
    }
    return false;
}

void movePlayer(char move) {
    switch (move) {
    case('w'):
        if (pointArr[0].y - 1 != 0) { pointArr[0].y--; };
        break;
    case('s'):
        if (pointArr[0].y + 1 != HIGHT - 1) { pointArr[0].y++; };
        break;
    case('a'):
        if (pointArr[0].x - 1 != 0) { pointArr[0].x--; };
        break;
    case('d'):
        if (pointArr[0].x + 1 != WIDTH - 1) { pointArr[0].x++; };
        break;
    case('e'):
        if (collision()) {
            cout << "Successfully!";
            break;
        }
    default:
        move_flag = false;
        break;
    }
}

void collision_handler(Player& player, Box& box) {
    SuperObject* obj = box.get();
    if (obj != nullptr) {
        // Обработка столкновения
        if (dynamic_cast<Item*>(obj) != nullptr) {
            player.inventory.push_back(*dynamic_cast<Item*>(obj));
        }
        box.put(nullptr);
    }
}
//int main()
//{
//    int getch();
//    char move;
//    Sleep(1000);
//    system("cls");
//    SuperObject me{ 1,1,'@' };
//    pointArr.push_back(me);
//    //Добавление элементов помимо игрока
//    for (int i = 0; i < 5; i++) {
//        SuperObject temp{ (rand() % HIGHT - 2) + 1, (rand() % WIDTH - 2) + 1, '$' };
//        pointArr.push_back(temp);
//    }
//    bool main_flag = true;
//
//    while (main_flag)
//    {
//    };
//}