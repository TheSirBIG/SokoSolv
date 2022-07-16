// SokoSolv.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define byte unsigned char

#include <iostream>
#include "openArray.hpp"

// 255 - стена
// 0   - пусто
// 1   - ящик
// 8   - конечное место
// 9   - ящик на конечном месте
// 128 - человек (не использую, ввел переменную позиции)
//#define MAX_X  7         // включая стены по периметру!!!
//#define MAX_Y  7         // включая стены по периметру!!!
//#define MAX_X  6         // включая стены по периметру!!!
//#define MAX_Y  6         // включая стены по периметру!!!
#define MAX_X  7         // включая стены по периметру!!!
#define MAX_Y  7         // включая стены по периметру!!!
#define NUM_OF_CRATE 6
#define MAX_POS (NUM_OF_CRATE*4)
#define MAX_ITERATIONS 100

typedef struct
{
    byte x, y;
} manPosType;

/*
typedef struct _posType
{
    byte pos[MAX_Y][MAX_X];
    _posType* prev;
    _posType* next;
    manPosType manPos;
    manPosType craftWasMoved;
    char dir;   //L,R,U,D
} posType;
*/
typedef struct _posType2
{
    byte pos[MAX_Y][MAX_X];
    _posType2* prev;
    _posType2* next[MAX_POS];
    manPosType manPos;
    manPosType craftWasMoved; //в этой записи - позоция ящика и куда сдвигали из позиции prev, чтобы прийти в эту!!!
    char dir;   //L,R,U,D
} posType2;

typedef struct _posType3
{
    byte pos[MAX_Y][MAX_X];
    _posType3* prev;
    manPosType manPos;
    manPosType craftWasMoved; //в этой записи - позоция ящика и куда сдвигали из позиции prev, чтобы прийти в эту!!!
    char dir;   //L,R,U,D
} posType3;

openArray<posType2*> arr(100);
// стартовая позиция
//posType startPos;
posType2 startPos;
posType3 startPos3;

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

// каждая новая позиция содержит координаты ящика, который двигали, и направление сдвига, человек при этом в месте для требуемого сдвига ящика

//posType* createNewPos(posType* parent)
posType2* createNewPos(posType2* parent)
{
//    parent->next = new posType;
//    (parent->next)->next = nullptr;
//    (parent->next)->prev = parent;
    posType2* curnew = new posType2;
    for (int i = 0; i < MAX_POS; i++)
        curnew->next[i] = nullptr;
    curnew->prev = parent;
    //копирование массива данных
    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++)
            //            (parent->next)->pos[i][j] = parent->pos[i][j];
            curnew->pos[i][j] = parent->pos[i][j];
    return curnew;
              
//    return parent->next;
}

posType3* createNewPos(posType3* parent)
{
    // parent->next = new posType;
    //    (parent->next)->next = nullptr;
    //    (parent->next)->prev = parent;
    posType3* curnew = new posType3;
//    for (int i = 0; i < MAX_POS; i++)
//        curnew->next[i] = nullptr;
    curnew->prev = parent;
    //копирование массива данных
    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++)
            //            (parent->next)->pos[i][j] = parent->pos[i][j];
            curnew->pos[i][j] = parent->pos[i][j];
    return curnew;

    //    return parent->next;
}

//void deletePos(posType* current)
//{
//    (current->prev)->next = nullptr;
//    delete current;
//}

//void clearPos()
void clearPos(posType2* pos)
{
//    posType* addr;

    for (int i = 0; i < MAX_POS; i++)
        if (pos->next[i] != nullptr) clearPos(pos->next[i]);
    if(pos != &startPos) delete pos;

/*
    //    if (manHist.next == nullptr) exit;
    while (startPos.next != nullptr)
    {
        addr = startPos.next;
        while (addr->next != nullptr) addr = addr->next;
        (addr->prev)->next = nullptr;
        delete addr;
    }
*/
}

// m - направление предыдущего сдвига, только для внутренней рекурсии (чтобы избежать зацикливания)
// '0' - самое начало
// не совсем помогает - нужно также держать историю, и отказывать, если приходит на существующие позиции
//bool canManMove(posType* curPos, manPosType src, manPosType dest, char m, manHistType* mh)
bool canManMove(posType2* curPos, manPosType src, manPosType dest, char m, manHistType* mh)
{
    static int qqq = 0;
    bool retval = false;
    manPosType a;

    if (m == '0')
    {
//        qqq++;
//        if (qqq == 25)
//        {
//            std::cout << "unknown fail" << std::endl;
//        }
//        if (qqq == 35)
//        {
//            std::cout << "here stack overflow" << std::endl;
//        }
        clearManHist();
        manHist.x = src.x;
        manHist.y = src.y;
    };

//    std::cout << qqq++ << std::endl;
//    if (qqq == 25)
//    {
//        std::cout << "unknown fail" << std::endl;
//    }

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
//               deleteLastManHist();
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
//            deleteLastManHist();
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
//            deleteLastManHist();
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
//            deleteLastManHist();
        }
    }
    return(retval);
}
bool canManMove(posType3* curPos, manPosType src, manPosType dest, char m, manHistType* mh)
{
    static int qqq = 0;
    bool retval = false;
    manPosType a;

    if (m == '0')
    {
        //        qqq++;
        //        if (qqq == 25)
        //        {
        //            std::cout << "unknown fail" << std::endl;
        //        }
        //        if (qqq == 35)
        //        {
        //            std::cout << "here stack overflow" << std::endl;
        //        }
        clearManHist();
        manHist.x = src.x;
        manHist.y = src.y;
    };

    //    std::cout << qqq++ << std::endl;
    //    if (qqq == 25)
    //    {
    //        std::cout << "unknown fail" << std::endl;
    //    }

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
            //               deleteLastManHist();
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
            //            deleteLastManHist();
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
            //            deleteLastManHist();
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
            //            deleteLastManHist();
        }
    }
    return(retval);
}

// x,y - координаты ящика для сдвига
bool canCrateLeft(posType2* pos, byte x, byte y)
{
    manPosType a;

    a.x = x + 1;
    a.y = y;
    return(((pos->pos[y][x-1] == 0) || (pos->pos[y][x - 1] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}
bool canCrateLeft(posType3* pos, byte x, byte y)
{
    manPosType a;

    a.x = x + 1;
    a.y = y;
    return(((pos->pos[y][x - 1] == 0) || (pos->pos[y][x - 1] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}
bool canCrateRight(posType2* pos, byte x, byte y)
{
manPosType a;

a.x = x - 1;
a.y = y;
return(((pos->pos[y][x + 1] == 0) || (pos->pos[y][x + 1] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}
bool canCrateRight(posType3* pos, byte x, byte y)
{
    manPosType a;

    a.x = x - 1;
    a.y = y;
    return(((pos->pos[y][x + 1] == 0) || (pos->pos[y][x + 1] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}
bool canCrateUp(posType2* pos, byte x, byte y)
{
    manPosType a;

    a.x = x;
    a.y = y + 1;
    return(((pos->pos[y - 1][x] == 0) || (pos->pos[y - 1][x] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}
bool canCrateUp(posType3* pos, byte x, byte y)
{
    manPosType a;

    a.x = x;
    a.y = y + 1;
    return(((pos->pos[y - 1][x] == 0) || (pos->pos[y - 1][x] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}
bool canCrateDown(posType2* pos, byte x, byte y)
{
    manPosType a;

    a.x = x;
    a.y = y - 1;
    return(((pos->pos[y + 1][x] == 0) || (pos->pos[y + 1][x] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}
bool canCrateDown(posType3* pos, byte x, byte y)
{
    manPosType a;

    a.x = x;
    a.y = y - 1;
    return(((pos->pos[y + 1][x] == 0) || (pos->pos[y + 1][x] == 8)) && canManMove(pos, pos->manPos, a, '0', &manHist));
}

bool ifPosFinal(posType2* pos)
{
    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++)
            if (pos->pos[i][j] == 1) return false;
    return true;
}
bool ifPosFinal(posType3* pos)
{
    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++)
            if (pos->pos[i][j] == 1) return false;
    return true;
}

bool ifPosIdentifical(posType2* a1, posType2* a2, bool man = true)
{
    //man - проверять ли положение человека на соответствие
    bool retval = true;

    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++)
            if (a1->pos[i][j] != a2->pos[i][j]) return false;
    if (man)
    {
        //все ящики стоят идентично, надо проверить положение человека
//        if ((a1->manPos.x != a2->manPos.x) || (a1->manPos.y != a2->manPos.y)) retval = false;
        // не сравнивать, а проверить, можно ли прийти из а1 в а2
        manPosType src, dest;
        src = a1->manPos;
        dest = a2->manPos;
        retval = canManMove(a1, src, dest, '0', &manHist);
    }
    return retval;
}
bool ifPosIdentifical(posType3* a1, posType3* a2, bool man = true)
{
    //man - проверять ли положение человека на соответствие
    bool retval = true;

    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++)
            if (a1->pos[i][j] != a2->pos[i][j]) return false;
    if (man)
    {
        //все ящики стоят идентично, надо проверить положение человека
//        if ((a1->manPos.x != a2->manPos.x) || (a1->manPos.y != a2->manPos.y)) retval = false;
        // не сравнивать, а проверить, можно ли прийти из а1 в а2
        manPosType src, dest;
        src = a1->manPos;
        dest = a2->manPos;
        retval = canManMove(a1, src, dest, '0', &manHist);
    }
    return retval;
}

// проверка, не содержится ли данная позиция уже в списке (чтобы избежать зацикливания)
// эта позиция должна быть последней в списке
//bool ifPosExisting(posType* pos)
//{
//    bool retval = false;
//    posType* addr;
//
//    if (pos == &startPos) return false;
//    addr = startPos.next;
//    while (addr != pos)
//    {
//        retval = ifPosIdentifical(addr, pos, true);
//        if (retval) break;
//        addr = addr->next;
//    }
//    return retval;
//}

//очень долго и оооочень много памяти жрет
//и что-то не очень работает, буду другую думать
/*
void Solver(posType* pos)
{
    posType* addr;
    bool canMove;
    int qqq;

//печать позиции в файл
//
//    for (int i = 0; i < MAX_Y; i++)
//    {
//        for (int j = 0; j < MAX_X; j++)
//        {
//            switch (pos->pos[i][j])
//            {
//            case 1:
//                std::cout << "1";
//                break;
//            case 8:
//                std::cout << "o";
//                break;
//            case 9:
//                std::cout << "O";
//                break;
//            case 0:
//                std::cout << " ";
//                break;
//            default:
//                std::cout << "X";
//            }
//        }
//        std::cout << std::endl;
//    }

    //пришла новая позиция, проверить идентичность, если да - то удалить и выйти
    if (ifPosExisting(pos))
    {
        deletePos(pos);
//        std::cout << "deleting" << std::endl;
        return;
    }
    //проверить на финальную позицию, пока вывести в терминал путь
    // затем удалить и выйти - чтобы найти возможно более короткий путь
    if (ifPosFinal(pos))
    {
        std::cout << "path found:" << std::endl;
        addr = startPos.next;
        qqq = 1;
        while (addr != pos)
        {
            std::cout << (int)addr->craftWasMoved.x << "," << (int)addr->craftWasMoved.y << "," << addr->dir << std::endl;
            addr = addr->next;
            qqq++;
        }
        std::cout << (int)addr->craftWasMoved.x << "," << (int)addr->craftWasMoved.y << "," << addr->dir << std::endl;
        //deletePos(pos);
        std::cout << "total " << qqq << std::endl;
//        std::cin >> qqq;
        goto commonExit;
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
                if (canCrateDown(pos, j, i) || canCrateLeft(pos, j, i) || canCrateRight(pos, j, i) || canCrateUp(pos, j, i))
                {
//                    std::cout << "try left" << std::endl;
                    if (canCrateLeft(pos, j, i))
                    {
                        addr = createNewPos(pos);
                        addr->pos[i][j - 1] = addr->pos[i][j - 1] ^ 0x01; //новое место
                        addr->pos[i][j] = addr->pos[i][j] ^ 0x01; //старое место
                        addr->manPos.x = j + 1;
                        addr->manPos.y = i;
                        addr->dir = 'L';
                        addr->craftWasMoved.x = j;
                        addr->craftWasMoved.y = i;
                        //если ящик в новом месте нельзя сдвинуть, и он не на маркере - эта позиция удаляется, и рекурсия не вызывается
                        //т.е. рекурсия - когда на маркере и нельзя сдвинуть, или можно сдвинуть
                        if (canCrateDown(addr, j - 1, i) || canCrateLeft(addr, j - 1, i) || canCrateRight(addr, j - 1, i) || canCrateUp(addr, j - 1, i))
                        {
                            Solver(addr);
                        }
                        else
                            if (addr->pos[i][j - 1] == 9)
                            {
                                Solver(addr);
                            }
                            else
                            {
                                delete addr;
                                pos->next = nullptr;
                            }
                    }
//                    std::cout << "try right" << std::endl;
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
                        //если ящик в новом месте нельзя сдвинуть, и он не на маркере - эта позиция удаляется, и рекурсия не вызывается
                        //т.е. рекурсия - когда на маркере и нельзя сдвинуть, или можно сдвинуть
                        if (canCrateDown(addr, j + 1, i) || canCrateLeft(addr, j + 1, i) || canCrateRight(addr, j + 1, i) || canCrateUp(addr, j + 1, i))
                        {
                            Solver(addr);
                        }
                        else
                            if (addr->pos[i][j + 1] == 9)
                            {
                                Solver(addr);
                            }
                            else
                            {
                                delete addr;
                                pos->next = nullptr;
                            }
                    }
//                    std::cout << "try up" << std::endl;
                    if (canCrateUp(pos, j, i))
                    {
                        addr = createNewPos(pos);
                        addr->pos[i - 1][j] = addr->pos[i - 1][j] ^ 0x01; //новое место
                        addr->pos[i][j] = addr->pos[i][j] ^ 0x01; //старое место
                        addr->manPos.x = j;
                        addr->manPos.y = i + 1;
                        addr->dir = 'U';
                        addr->craftWasMoved.x = j;
                        addr->craftWasMoved.y = i;
                        //если ящик в новом месте нельзя сдвинуть, и он не на маркере - эта позиция удаляется, и рекурсия не вызывается
                        //т.е. рекурсия - когда на маркере и нельзя сдвинуть, или можно сдвинуть
                        if (canCrateDown(addr, j, i - 1) || canCrateLeft(addr, j, i - 1) || canCrateRight(addr, j, i - 1) || canCrateUp(addr, j, i - 1))
                        {
                            Solver(addr);
                        }
                        else
                            if (addr->pos[i-1][j] == 9)
                            {
                                Solver(addr);
                            }
                            else
                            {
                                delete addr;
                                pos->next = nullptr;
                            }
                    }
//                    std::cout << "try down" << std::endl;
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
                        //если ящик в новом месте нельзя сдвинуть, и он не на маркере - эта позиция удаляется, и рекурсия не вызывается
                        //т.е. рекурсия - когда на маркере и нельзя сдвинуть, или можно сдвинуть
                        if (canCrateDown(addr, j, i+1) || canCrateLeft(addr, j, i+1) || canCrateRight(addr, j, i+1) || canCrateUp(addr, j, i+1))
                        {
                            Solver(addr);
                        }
                        else
                            if (addr->pos[i+1][j] == 9)
                            {
                                Solver(addr);
                            }
                            else
                            {
                                delete addr;
                                pos->next = nullptr;
                            }
                    }
                }
                else
                    if (pos->pos[i][j] != 9)
                    {
                        goto commonExit;
                    }
            }
        }
commonExit:
    if(pos != &startPos) deletePos(pos);
}
*/
static const int tableFact[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};

/*
void solver2GeneratePositions()
{
    //создание всех возможных позиций, вывод их чиста
    //убирание явно глухих позиций, вывод числа оставшихся
    int numSq;
    int numPos;
    byte** buf;

    //подсчет числа клеток, где можно разместить ящик
    numSq = 0;
    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++)
            if (startPos.pos[i][j] != 255) numSq++;
    //подсчет колиества вариантов расположения
    numPos = tableFact[1];
    //заготовка массива шаблонов
    buf = new byte * [1];
}
*/
void prepareSolver2()
{
    arr.put(&startPos);
}

void Solver2(posType2* pos, int iteration)
{
    int posFound = 0;

    //если финальная позиция - сообщаем
    if (ifPosFinal(pos))
    {
        //пока что просто нашли
        std::cout << "final found!!!" << std::endl;
        std::cout << "solver from back to begin:" << std::endl;
        posType2* qqq = pos;
        posType2* qqq1;

        while (qqq != &startPos)
        {
            std::cout << (int)qqq->craftWasMoved.x << "," << (int)qqq->craftWasMoved.y << "," << qqq->dir << std::endl;
            qqq1 = qqq->prev;
            for(int i=0; i<MAX_POS; i++)
                if (qqq1->next[i] == qqq)
                {
                    qqq = qqq1->next[i];
                    break;
                }
        }
        std::cout << (int)qqq->craftWasMoved.x << "," << (int)qqq->craftWasMoved.y << "," << qqq->dir << std::endl;
        //далее можно продолжать, чтобы найти более короткий путь, если есть
        return;
    }
    //если итерация >= MAX_ITERATION - прекращение данной ветки
    if (iteration >= MAX_ITERATIONS)
    {
        std::cout << "max iteration reached" << std::endl;
        return;
    }
    //генерация всех возможных следующих комбинаций из текущей, цикл по количеству ящиков
    // сразу проверка на существующую позицию, если такой нет - то добавить в arr и в текущую позицию (создать массив для этого?????)
    // также проверка на финальную стадию
    for(int i=0; i<MAX_Y; i++)
        for(int j=0; j<MAX_X; j++)
            if ((pos->pos[i][j] == 1) || (pos->pos[i][j] == 9))
            {
                //ящик найден
                //проверка на возможные движения
            }

    //цикл по всем записанным позициям, рекурсия с увеличением итерации
}

bool ifInConer(posType3* pos, int x, int y)
{
    bool u, d, l, r;

    u = (pos->pos[y - 1][x] == 255);
    d = (pos->pos[y + 1][x] == 255);
    l = (pos->pos[y][x - 1] == 255);
    r = (pos->pos[y][x + 1] == 255);

    return ((l && u) || (u && r) || (r && d) || (d && l));
}

//идея - без рекурсии, сначала построить все позиции от стартовой, проверить их на финальные, удалить имеющиеся (или просто не добавлять)
// затем цикл по всем этим позициям, строить кучу новых, проверка их на финальные, удалить имеющиеся (или просто не добавлять)
// и т.д.
// плюс - сразу будет найден самый короткий путь
// минус - много памяти потребует??? - вроде, значительно меньше, чем 2-й вариант
void Solver3()
{
    posType3* pos;
    posType3* newPos;
    openArray<posType3*> commonArr(200);
    int startIdx, posCount;
    int posFounded;
    int posDropped;
    bool mustDelete;
    bool found;
    int perc;

    commonArr.put(&startPos3);
    startIdx = 0;
    posCount = 1;
    //main cycle
    for (int ccc = 0; ccc < MAX_ITERATIONS; ccc++)
    {
        //цикл по последним позициям
        std::cout << "Ineration - " << ccc << std::endl;
        std::cout << "[ 0% ]";
//        for (int pc = startIdx; pc < commonArr.getNum(); pc++)
        newPos = nullptr;
        posFounded = 0;
        posDropped = 0;
        for (int pc = startIdx; pc < startIdx + posCount; pc++)
        {
            perc = (pc - startIdx) / (posCount / 100.0);
            std::cout << (char)0x08 << (char)0x08 << (char)0x08 << (char)0x08 << (char)0x08 << (char)0x08 << (char)0x08;
            std::cout << "[ " << perc << "% ]";
            pos = commonArr.get(pc);
            //цикл по количеству ящиков
            //т.к. все крайние ячейки - это стены, в цикле иду с 1 до MAX-1
            for (int i = 1; i < MAX_Y-1; i++)
                for (int j = 1; j < MAX_X-1; j++)
                    if ((pos->pos[i][j] == 1) || (pos->pos[i][j] == 9))
                    {
                        //ящик найден
                        //проверка на возможные движения
                        if (canCrateLeft(pos, j, i))
                        {
                            mustDelete = false;
                            found = false;
                            newPos = createNewPos(pos);
                            newPos->pos[i][j - 1] = newPos->pos[i][j - 1] ^ 0x01; //новое место ящика
                            newPos->pos[i][j] = newPos->pos[i][j] ^ 0x01; //старое место ящика
                            newPos->manPos.x = j + 1;
                            newPos->manPos.y = i;
                            newPos->dir = 'L';
                            newPos->craftWasMoved.x = j;
                            newPos->craftWasMoved.y = i;
                            /*
                            //если ящик в новом месте нельзя сдвинуть, и он не на маркере - эта позиция удаляется
                            if (!((canCrateDown(newPos, j - 1, i) || canCrateLeft(newPos, j - 1, i) || canCrateRight(newPos, j - 1, i) || canCrateUp(newPos, j - 1, i))))
                                if (newPos->pos[i][j - 1] != 9)
                                    mustDelete = true;*/
                            //так не пойдет, может отсечь нормальные ситуации
                            // теперь удалять только если в углу (оттуда 100% нельзя сдвинуть никуда) и не на маркере
                            if(ifInConer(newPos,j-1,i))
                                if (newPos->pos[i][j - 1] != 9)
                                    mustDelete = true; 
                            //проверка на финальную позицию
                            if(!mustDelete)
                                if (ifPosFinal(newPos))
                                {
                                    std::cout << "found!!!" << std::endl;
                                    found = true;
                                    goto endsolver3;
                                }
                            //проверка на существующую позицию, поиск по всем предыдущим
                            //новые, возможно, не надо включать в поиск??? пока не очевидно
                            if(!mustDelete)
                                for (int k = 0; k < startIdx + posCount + posFounded; k++)
                                    if (ifPosIdentifical(commonArr.get(k), newPos))
                                    {
                                        mustDelete = true;
                                        break;
                                    }
                            if (mustDelete) posDropped++;
                            if (mustDelete) delete newPos; else
                            {
                                //добавляем в массив
                                commonArr.put(newPos);
                                posFounded++;
                            }
                        }
                        if (canCrateRight(pos, j, i))
                        {
                            mustDelete = false;
                            found = false;
                            newPos = createNewPos(pos);
                            newPos->pos[i][j + 1] = newPos->pos[i][j + 1] ^ 0x01; //новое место ящика
                            newPos->pos[i][j] = newPos->pos[i][j] ^ 0x01; //старое место ящика
                            newPos->manPos.x = j - 1;
                            newPos->manPos.y = i;
                            newPos->dir = 'R';
                            newPos->craftWasMoved.x = j;
                            newPos->craftWasMoved.y = i;
                            /*
                            //если ящик в новом месте нельзя сдвинуть, и он не на маркере - эта позиция удаляется
                            if (!((canCrateDown(newPos, j + 1, i) || canCrateLeft(newPos, j + 1, i) || canCrateRight(newPos, j + 1, i) || canCrateUp(newPos, j + 1, i))))
                                if (newPos->pos[i][j + 1] != 9)
                                    mustDelete = true;*/
                            if (ifInConer(newPos, j + 1, i))
                                if (newPos->pos[i][j + 1] != 9)
                                    mustDelete = true;
                            //проверка на финальную позицию
                            if (!mustDelete)
                                if (ifPosFinal(newPos))
                                {
                                    std::cout << "found!!!" << std::endl;
                                    found = true;
                                    goto endsolver3;
                                }
                            //проверка на существующую позицию, поиск по всем предыдущим
                            //новые, возможно, не надо включать в поиск??? пока не очевидно
                            if (!mustDelete)
                                for (int k = 0; k < startIdx + posCount + posFounded; k++)
                                    if (ifPosIdentifical(commonArr.get(k), newPos))
                                    {
                                        mustDelete = true;
                                        break;
                                    }
                            if (mustDelete) posDropped++;
                            if (mustDelete) delete newPos; else
                            {
                                //добавляем в массив
                                commonArr.put(newPos);
                                posFounded++;
                            }
                        }
                        if (canCrateUp(pos, j, i))
                        {
                            mustDelete = false;
                            found = false;
                            newPos = createNewPos(pos);
                            newPos->pos[i - 1][j] = newPos->pos[i - 1][j] ^ 0x01; //новое место ящика
                            newPos->pos[i][j] = newPos->pos[i][j] ^ 0x01; //старое место ящика
                            newPos->manPos.x = j;
                            newPos->manPos.y = i + 1;
                            newPos->dir = 'U';
                            newPos->craftWasMoved.x = j;
                            newPos->craftWasMoved.y = i;
                            /*
                            //если ящик в новом месте нельзя сдвинуть, и он не на маркере - эта позиция удаляется
                            if (!((canCrateDown(newPos, j, i - 1) || canCrateLeft(newPos, j, i - 1) || canCrateRight(newPos, j, i - 1) || canCrateUp(newPos, j, i - 1))))
                                if (newPos->pos[i - 1][j] != 9)
                                    mustDelete = true;*/
                            if (ifInConer(newPos, j, i - 1))
                                if (newPos->pos[i - 1][j] != 9)
                                    mustDelete = true;
                            //проверка на финальную позицию
                            if (!mustDelete)
                                if (ifPosFinal(newPos))
                                {
                                    std::cout << "found!!!" << std::endl;
                                    found = true;
                                    goto endsolver3;
                                }
                            //проверка на существующую позицию, поиск по всем предыдущим
                            //новые, возможно, не надо включать в поиск??? пока не очевидно
                            if (!mustDelete)
                                for (int k = 0; k < startIdx + posCount + posFounded; k++)
                                    if (ifPosIdentifical(commonArr.get(k), newPos))
                                    {
                                        mustDelete = true;
                                        break;
                                    }
                            if (mustDelete) posDropped++;
                            if (mustDelete) delete newPos; else
                            {
                                //добавляем в массив
                                commonArr.put(newPos);
                                posFounded++;
                            }
                        }
                        if (canCrateDown(pos, j, i))
                        {
                            mustDelete = false;
                            found = false;
                            newPos = createNewPos(pos);
                            newPos->pos[i + 1][j] = newPos->pos[i + 1][j] ^ 0x01; //новое место ящика
                            newPos->pos[i][j] = newPos->pos[i][j] ^ 0x01; //старое место ящика
                            newPos->manPos.x = j;
                            newPos->manPos.y = i - 1;
                            newPos->dir = 'D';
                            newPos->craftWasMoved.x = j;
                            newPos->craftWasMoved.y = i;
                            /*
                            //если ящик в новом месте нельзя сдвинуть, и он не на маркере - эта позиция удаляется
                            if (!((canCrateDown(newPos, j, i + 1) || canCrateLeft(newPos, j, i + 1) || canCrateRight(newPos, j, i + 1) || canCrateUp(newPos, j, i + 1))))
                                if (newPos->pos[i + 1][j] != 9)
                                    mustDelete = true;*/
                            if (ifInConer(newPos, j, i + 1))
                                if (newPos->pos[i + 1][j] != 9)
                                    mustDelete = true;
                            //проверка на финальную позицию
                            if (!mustDelete)
                                if (ifPosFinal(newPos))
                                {
                                    std::cout << "found!!!" << std::endl;
                                    found = true;
                                    goto endsolver3;
                                }
                            //проверка на существующую позицию, поиск по всем предыдущим
                            //новые, возможно, не надо включать в поиск??? пока не очевидно
                            if (!mustDelete)
                                for (int k = 0; k < startIdx + posCount + posFounded; k++)
                                    if (ifPosIdentifical(commonArr.get(k), newPos))
                                    {
                                        mustDelete = true;
                                        break;
                                    }
                            if (mustDelete) posDropped++;
                            if (mustDelete) delete newPos; else
                            {
                                //добавляем в массив
                                commonArr.put(newPos);
                                posFounded++;
                            }
                        }
                    }
            //прошли цикл по всем полям в поисках ящиков
            //заполнили новые позиции
        }
        std::cout << std::endl;
        //прошли все новые позиции, там все заполнили, что не надо - не заполнили ))
        //переходим на следущую итерацию, т.е. вот на эти все новые
        //надо модифицировать стартовые переменные
        startIdx = startIdx + posCount;
        posCount = posFounded;
        std::cout << "new: pos founded - " << posFounded << ", dropped - " << posDropped << ", tolal - " << posFounded+posDropped << std::endl;
    }
    std::cout << "Can't found for " << (int)MAX_ITERATIONS << " iterations" << std::endl;
endsolver3:
    if (found)
    {
        posType3* pt;

        pt = newPos;
        std::cout << "back solver:" << std::endl;
        while (pt != &startPos3)
        {
            std::cout << (int)pt->craftWasMoved.x+1 << "," << (int)pt->craftWasMoved.y+1 << "," << pt->dir << std::endl;
            pt = pt->prev;
        }

        delete newPos;
    }
    //удалить все созданные позиции
    for (int i = 1; i < commonArr.getNum(); i++)
        delete commonArr.get(i);
}
void createInitPos()
{
    byte dummyPos[MAX_Y][MAX_X] =  {{255, 255, 255, 255, 255, 255, 255},
                                    {255, 0,   0,   0,   0,   255, 255},
                                    {255, 1,   1,   9,   0,   0,   255},
                                    {255, 8,   8,   0,   8,   8,   255},
                                    {255, 0,   0,   9,   1,   1,   255},
                                    {255, 255, 0,   0,   0,   0,   255},
                                    {255, 255, 255, 255, 255, 255, 255} };
/*    byte dummyPos[MAX_Y][MAX_X] = {{255, 255, 255, 255, 255, 255, 255},
                                    {255, 8,   8,   1,   8,   8,   255},
                                    {255, 8,   8,   255, 8,   8,   255},
                                    {255, 0,   1,   1,   1,   0,   255},
                                    {255, 0,   0,   1,   0,   0,   255},
                                    {255, 0,   1,   1,   1,   0,   255},
                                    {255, 0,   0,   255, 0,   0,   255},
                                    {255, 255, 255, 255, 255, 255, 255} }; //7,8,8,4,6   */
                                
    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++) startPos.pos[i][j] = dummyPos[i][j];
    startPos.manPos.x = 3;
    startPos.manPos.y = 3;
//    startPos.next = startPos.prev = nullptr;
    manHist.next = manHist.prev = nullptr;
    for (int i = 0; i < MAX_POS; i++)
        startPos.next[i] = nullptr;
    startPos.prev = nullptr;

    for (int i = 0; i < MAX_Y; i++)
        for (int j = 0; j < MAX_X; j++) startPos3.pos[i][j] = dummyPos[i][j];
    startPos3.manPos.x = 3;
    startPos3.manPos.y = 3;
    startPos3.prev = nullptr;
}


int main()
{
    manPosType s, d;
    openArray<byte> qq(200);
    qq.put(0);
    openArray<int> a;
    a.put(0);

    std::cout << "This is a test sokovan solver\n";
    std::cout << "Working...\n";

    createInitPos();
    s.x = 4;
    s.y = 3;
    d.x = 1;
    d.y = 3;
//    std::cout << canManMove(&startPos, s, d, '0', &manHist) << std::endl;
//    std::cout << canCrateLeft(&startPos, 3, 1) << std::endl;
//    Solver(&startPos);
//    solver2GeneratePositions();

//    prepareSolver2();
//    Solver2(&startPos, 0);

    Solver3();

    clearManHist();
//    clearPos(&startPos);
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
