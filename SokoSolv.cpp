// SokoSolv.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define byte unsigned char

#include <iostream>


// 255 - стена
// 0   - пусто
// 1   - ящик
// 8   - конечное место
// 9   - ящик на конечном месте
// 128 - человек (не использую, ввел переменную позиции)
#define MAX_X 5         // включая стены по периметру!!!
#define MAX_Y 5         // включая стены по периметру!!!


typedef struct
{
    byte x, y;
} manPosType;


typedef struct _posType
{
    byte pos[MAX_Y][MAX_X];
    _posType* prev;
    _posType* next;
    manPosType manPos;
} posType;

posType startPos;

posType* createNewPos(posType* parent)
{
    parent->next = new posType;
    (parent->next)->prev = parent;
    return parent->next;
}

void deletePos(posType* current)
{
    (current->prev)->next = nullptr;
    delete current;
}

bool canManMove(posType* curPos, manPosType src, manPosType dest, char m)
{
    bool retval = false;
    manPosType a;

    if (curPos->pos[src.y][src.x] == 255) return false;
    if (curPos->pos[src.y][src.x] == 1) return false;
    if (curPos->pos[src.y][src.x] == 9) return false;
    if (curPos->pos[dest.y][dest.x] == 255) return false;
    if (curPos->pos[dest.y][dest.x] == 1) return false;
    if (curPos->pos[dest.y][dest.x] == 9) return false;
    if ((dest.x == src.x) && (dest.y == src.y)) return true;
    //left
    if (((curPos->pos[src.y][src.x - 1] == 0) || (curPos->pos[src.y][src.x - 1] == 8)) && (m != 'R'))
    {
        a.x = src.x - 1;
        a.y = src.y;
        retval = canManMove(curPos, a, dest, 'L');
        if (retval) return true;
    }
    //right
    if (((curPos->pos[src.y][src.x + 1] == 0) || (curPos->pos[src.y][src.x + 1] == 8)) && (m != 'L'))
    {
        a.x = src.x + 1;
        a.y = src.y;
        retval = canManMove(curPos, a, dest, 'R');
        if (retval) return true;
    }
    //up
    if (((curPos->pos[src.y - 1][src.x] == 0) || (curPos->pos[src.y - 1][src.x] == 8)) && (m != 'D'))
    {
        a.x = src.x;
        a.y = src.y - 1;
        retval = canManMove(curPos, a, dest, 'U');
        if (retval) return true;
    }
    //down
    if (((curPos->pos[src.y + 1][src.x] == 0) || (curPos->pos[src.y + 1][src.x] == 8)) && (m != 'U'))
    {
        a.x = src.x;
        a.y = src.y + 1;
        retval = canManMove(curPos, a, dest, 'D');
        if (retval) return true;
    }
    return(retval);
}

// x,y - координаты ящика для сдвига
bool canCrateLeft(posType* pos, byte x, byte y)
{
    manPosType a;

    a.x = x + 1;
    a.y = y;
    return(((pos->pos[y][x-1] == 0) || (pos->pos[y][x - 1] == 8)) && canManMove(pos, pos->manPos, a, '0'));
}

void createInitPos()
{

    byte dummyPos[MAX_Y][MAX_X] = { {255, 255, 255, 255, 255},
                                    {255, 0,   8,   0,   255},
                                    {255, 0,   1,   0,   255},
                                    {255, 0,   0,   0,   255},
                                    {255, 255, 255, 255, 255} };
    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++) startPos.pos[i][j] = dummyPos[i][j];
    startPos.manPos.x = 2;
    startPos.manPos.y = 3;
    startPos.next = startPos.prev = nullptr;
    
}


int main()
{
    manPosType s, d;

    std::cout << "This is a test sokovan solver\n";
    std::cout << "Working...\n";

    createInitPos();
    s.x = 2;
    s.y = 3;
    d.x = 1;
    d.y = 1;
//    std::cout << canManMove(&startPos, s, d, '0') << std::endl;
    std::cout << canCrateLeft(&startPos, 3, 1) << std::endl;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
