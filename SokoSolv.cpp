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
#define MAX_X  7         // включая стены по периметру!!!
#define MAX_Y  7         // включая стены по периметру!!!

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
    manPosType craftWasMoved;
    char dir;   //L,R,U,D
} posType;

typedef struct _manHistType
{
    byte x, y;
    _manHistType* next;
    _manHistType* prev;
} manHistType;

manHistType manHist;

void clearManHist()
{
    manHistType* addr;

//    if (manHist.next == nullptr) exit;
    while (manHist.next != nullptr)
    {
        addr = manHist.next;
        while (addr->next != nullptr) addr = addr->next;
        (addr->prev)->next = nullptr;
        delete addr;
    }
}

manHistType* addNewManHist(manHistType* aaa)
{
    aaa->next = new manHistType;
    (aaa->next)->next = nullptr;
    (aaa->next)->prev = aaa;
    return aaa->next;
}

void deleteLastManHist()
{
    manHistType* addr;

    if (manHist.next == nullptr) return;
    addr = manHist.next;
    while (addr->next != nullptr) addr = addr->next;
    (addr->prev)->next = nullptr;
    delete addr;
}

//true - если нет повторений
bool checkManHist(manPosType pos)
{
    manHistType* addr;

    addr = &manHist;
    if ((addr->x == pos.x) && (addr->y == pos.y)) return false;
    while (addr->next != nullptr)
    {
        if ((addr->x == pos.x) && (addr->y == pos.y)) return false;
        addr = addr->next;
    }

    return true;
}

// стартовая позиция
posType startPos;
// каждая новая позиция содержит координаты ящика, который двигали, и направление сдвига, человек при этом в месте для требуемого сдвига ящика

posType* createNewPos(posType* parent)
{
    parent->next = new posType;
    (parent->next)->next = nullptr;
    (parent->next)->prev = parent;
    //копирование массива данных
    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++)
            (parent->next)->pos[i][j] = parent->pos[i][j];
    return parent->next;
}

void deletePos(posType* current)
{
    (current->prev)->next = nullptr;
    delete current;
}

void clearPos()
{
    posType* addr;

    //    if (manHist.next == nullptr) exit;
    while (startPos.next != nullptr)
    {
        addr = startPos.next;
        while (addr->next != nullptr) addr = addr->next;
        (addr->prev)->next = nullptr;
        delete addr;
    }
}

// m - направление предыдущего сдвига, только для внутренней рекурсии (чтобы избежать зацикливания)
// '0' - самое начало
// не совсем помогает - нужно также держать историю, и отказывать, если приходит на ту же позицию
bool canManMove(posType* curPos, manPosType src, manPosType dest, char m, manHistType* mh)
{
    static int qqq = 0;
    bool retval = false;
    manPosType a;

    if (m == '0')
    {
        qqq++;
        if (qqq == 35)
        {
            std::cout << "here stack overflow" << std::endl;
        }
        clearManHist();
        manHist.x = src.x;
        manHist.y = src.y;
    };

//    std::cout << qqq++ << std::endl;
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
           if (checkManHist(a))
           {
               addNewManHist(mh);
               (mh->next)->x = a.x;
               (mh->next)->y = a.y;
               (mh->next)->prev = mh;
               retval = canManMove(curPos, a, dest, 'L', mh->next);
               if (retval) return true;
           }
           else
           {
               deleteLastManHist();
           }
    }
    //right
    if (((curPos->pos[src.y][src.x + 1] == 0) || (curPos->pos[src.y][src.x + 1] == 8)) && (m != 'L'))
    {
        a.x = src.x + 1;
        a.y = src.y;
        if (checkManHist(a))
        {
            addNewManHist(mh);
            (mh->next)->x = a.x;
            (mh->next)->y = a.y;
            (mh->next)->prev = mh;
            retval = canManMove(curPos, a, dest, 'R', mh->next);
            if (retval) return true;
        }
        else
        {
            deleteLastManHist();
        }
    }
    //up
    if (((curPos->pos[src.y - 1][src.x] == 0) || (curPos->pos[src.y - 1][src.x] == 8)) && (m != 'D'))
    {
        a.x = src.x;
        a.y = src.y - 1;
        if (checkManHist(a))
        {
            addNewManHist(mh);
            (mh->next)->x = a.x;
            (mh->next)->y = a.y;
            (mh->next)->prev = mh;
            retval = canManMove(curPos, a, dest, 'U', mh->next);
            if (retval) return true;
        }
        else
        {
            deleteLastManHist();
        }
    }
    //down
    if (((curPos->pos[src.y + 1][src.x] == 0) || (curPos->pos[src.y + 1][src.x] == 8)) && (m != 'U'))
    {
        a.x = src.x;
        a.y = src.y + 1;
        if (checkManHist(a))
        {
            addNewManHist(mh);
            (mh->next)->x = a.x;
            (mh->next)->y = a.y;
            (mh->next)->prev = mh;
            retval = canManMove(curPos, a, dest, 'D', mh->next);
            if (retval) return true;
        }
        else
        {
            deleteLastManHist();
        }
    }
    return(retval);
}

// x,y - координаты ящика для сдвига
bool canCrateLeft(posType* pos, byte x, byte y)
{
    manPosType a;

    a.x = x + 1;
    a.y = y;
    return(((pos->pos[y][x-1] == 0) || (pos->pos[y][x - 1] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}
bool canCrateRight(posType* pos, byte x, byte y)
{
manPosType a;

a.x = x - 1;
a.y = y;
return(((pos->pos[y][x + 1] == 0) || (pos->pos[y][x + 1] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}
bool canCrateUp(posType* pos, byte x, byte y)
{
    manPosType a;

    a.x = x;
    a.y = y + 1;
    return(((pos->pos[y - 1][x] == 0) || (pos->pos[y - 1][x] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}
bool canCrateDown(posType* pos, byte x, byte y)
{
    manPosType a;

    a.x = x;
    a.y = y - 1;
    return(((pos->pos[y + 1][x] == 0) || (pos->pos[y + 1][x] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}

bool ifPosFinal(posType* pos)
{
    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++)
            if (pos->pos[i][j] == 1) return false;
    return true;
}

bool ifPosIdentifical(posType* a1, posType* a2)
{
    bool retval = true;

    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++)
        {
            if (a1->pos[i][j] != a2->pos[i][j]) retval = false;
            break;
        }
    if (retval)
    {
        //все ящики стоят идентично, надо проверить положение человека
        if ((a1->manPos.x != a2->manPos.x) || (a1->manPos.y != a2->manPos.y)) retval = false;
    }
    return retval;
}

// проверка, не содержится ли данная позиция уже в списке (чтобы избежать зацикливания)
// эта позиция должна быть последней в списке
bool ifPosExisting(posType* pos)
{
    bool retval = false;
    posType* addr;

    if (pos == &startPos) return false;
    addr = startPos.next;
    while (addr != pos)
    {
        retval = ifPosIdentifical(addr, pos);
        if (retval) break;
        addr = addr->next;
    }
    return retval;
}

void Solver(posType* pos)
{
    posType* addr;
    bool canMove;

    //пришла новая позиция, проверить идентичность, если да - то удалить и выйти
    if (ifPosExisting(pos))
    {
        deletePos(pos);
        return;
    }
    //проверить на финальную позицию, пока вывести в терминал путь
    // затем удалить и выйти - чтобы найти возможно более короткий путь
    if (ifPosFinal(pos))
    {
        std::cout << "path found:" << std::endl;
        addr = startPos.next;
        while (addr != pos)
        {
            std::cout << (int)addr->craftWasMoved.x << "," << (int)addr->craftWasMoved.y << "," << addr->dir << std::endl;
            addr = addr->next;
        }
        std::cout << (int)addr->craftWasMoved.x << "," << (int)addr->craftWasMoved.y << "," << addr->dir << std::endl;
        deletePos(pos);
        return;
    }
    //цикл по всем ящикам
    //проверить на возможные движения, если можно - создать новую позицию, заполнить поля ящика и направления, рекурсия
    //если ни одного движения нет - удалить, выход ??????
    // возможно - после всех движений всех ящиков удалить и выйти???
    canMove = false;
    for (int i = 1; i < MAX_Y-1; i++)
        for (int j = 1; j < MAX_X-1; j++)
        {
            if ((pos->pos[i][j] == 1) || (pos->pos[i][j] == 9))
            {
                std::cout << "try left" << std::endl;
                if (canCrateLeft(pos, j, i))
                {
                    addr = createNewPos(pos);
                    addr->pos[i][j - 1] = addr->pos[i][j-1] ^0x01; //новое место
                    addr->pos[i][j] = addr->pos[i][j] ^0x01; //старое место
                    addr->manPos.x = j + 1;
                    addr->manPos.y = i;
                    addr->dir = 'L';
                    addr->craftWasMoved.x = j;
                    addr->craftWasMoved.y = i;
                    Solver(addr);
                }
                std::cout << "try right" << std::endl;
                if (canCrateRight(pos, j, i))
                {
                    addr = createNewPos(pos);
                    addr->pos[i][j + 1] = addr->pos[i][j + 1] ^ 0x01; //новое место
                    addr->pos[i][j] = addr->pos[i][j] ^ 0x01; //старое место
                    addr->manPos.x = j - 1;
                    addr->manPos.y = i;
                    addr->dir = 'R';
                    addr->craftWasMoved.x = j;
                    addr->craftWasMoved.y = i;
                    Solver(addr);
                }
                std::cout << "try up" << std::endl;
                if (canCrateUp(pos, j, i))
                {
                    addr = createNewPos(pos);
                    addr->pos[i-1][j] = addr->pos[i-1][j] ^ 0x01; //новое место
                    addr->pos[i][j] = addr->pos[i][j] ^ 0x01; //старое место
                    addr->manPos.x = j;
                    addr->manPos.y = i+1;
                    addr->dir = 'U';
                    addr->craftWasMoved.x = j;
                    addr->craftWasMoved.y = i;
                    Solver(addr);
                }
                std::cout << "try down" << std::endl;
                if (canCrateDown(pos, j, i))
                {
                    addr = createNewPos(pos);
                    addr->pos[i + 1][j] = addr->pos[i + 1][j] ^ 0x01; //новое место
                    addr->pos[i][j] = addr->pos[i][j] ^ 0x01; //старое место
                    addr->manPos.x = j;
                    addr->manPos.y = i - 1;
                    addr->dir = 'D';
                    addr->craftWasMoved.x = j;
                    addr->craftWasMoved.y = i;
                    Solver(addr);
                }
            }
        }

    if(pos != &startPos) deletePos(pos);
}

void createInitPos()
{
    byte dummyPos[MAX_Y][MAX_X] = { {255, 255, 255, 255, 255, 255, 255},
                                    {255, 0,   0,   0,   0,   255, 255},
                                    {255, 1,   1,   9,   0,   0,   255},
                                    {255, 8,   8,   0,   8,   8,   255},
                                    {255, 0,   0,   9,   1,   1,   255},
                                    {255, 255, 0,   0,   0,   0,   255},
                                    {255, 255, 255, 255, 255, 255, 255} };
    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++) startPos.pos[i][j] = dummyPos[i][j];
    startPos.manPos.x = 3;
    startPos.manPos.y = 3;
    startPos.next = startPos.prev = nullptr;
    manHist.next = manHist.prev = nullptr;

}


int main()
{
    manPosType s, d;

    std::cout << "This is a test sokovan solver\n";
    std::cout << "Working...\n";

    createInitPos();
    s.x = 4;
    s.y = 3;
    d.x = 1;
    d.y = 3;
//    std::cout << canManMove(&startPos, s, d, '0', &manHist) << std::endl;
//    std::cout << canCrateLeft(&startPos, 3, 1) << std::endl;
    Solver(&startPos);

    clearManHist();
    clearPos();
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
