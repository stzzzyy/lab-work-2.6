#include <iostream>
#include <iomanip>
#include <new>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Константи програми (Лабораторна робота 2.6, Варіант 3)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const int INSERT_COUNT = 6;   // Кількість операцій запису до списку
const int REMOVE_COUNT = 7;   // Кількість операцій видалення (з перевіркою порожності)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Описи власних типів даних та Абстрактного Типу Даних (АТД)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum ListEnd
{
    END_FRONT = 0,   // Початок двоспрямованого списку (голова)
    END_BACK  = 1    // Кінець  двоспрямованого списку (хвіст)
};

// Елемент двоспрямованого списку у зв'язаному представленні (дійсний тип)
struct DoublyNode
{
    double      dData;   // Інформаційне поле (дійсне значення)
    DoublyNode* pPrev;   // Покажчик на попередній вузол
    DoublyNode* pNext;   // Покажчик на наступний вузол
};

// АТД «Двоспрямований список»: дані та покажчики на операції обробки
struct DoublyListAdt
{
    // Дані структури
    DoublyNode* pHead;
    DoublyNode* pTail;
    int         nSize;

    // Конструктори АТД (ініціалізація, запис, видалення, очищення)
    void (*pInit)  (DoublyListAdt& list);
    bool (*pInsert)(DoublyListAdt& list, ListEnd eEnd, double dValue);
    bool (*pRemove)(DoublyListAdt& list, ListEnd eEnd, double& dOutValue);
    void (*pClear) (DoublyListAdt& list);

    // Селектори АТД (перевірка стану та читання крайнього значення)
    bool (*pIsEmpty)(const DoublyListAdt& list);
    int  (*pGetSize)(const DoublyListAdt& list);
    bool (*pPeekEnd)(const DoublyListAdt& list, ListEnd eEnd, double& dOutValue);

    // Ітератори АТД (обхід елементів у прямому та зворотному напрямках)
    void (*pIterateForward) (const DoublyListAdt& list, void (*pVisit)(double dValue, int nIndex));
    void (*pIterateBackward)(const DoublyListAdt& list, void (*pVisit)(double dValue, int nIndex));
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Відкрита підпрограма (inline)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline const char* getEndLabel(ListEnd eEnd)
{
    return (eEnd == END_FRONT) ? "FRONT" : "BACK ";
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Закриті підпрограми АТД: Конструктори
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void initDoublyList(DoublyListAdt& list)
{
    list.pHead = nullptr;
    list.pTail = nullptr;
    list.nSize = 0;
}

bool pushListEnd(DoublyListAdt& list, ListEnd eEnd, double dValue)
{
    DoublyNode* pNewNode = new (std::nothrow) DoublyNode;
    if (pNewNode == nullptr)
    {
        return false;
    }

    pNewNode->dData = dValue;
    pNewNode->pPrev = nullptr;
    pNewNode->pNext = nullptr;

    if (list.pHead == nullptr)
    {
        list.pHead = pNewNode;
        list.pTail = pNewNode;
    }
    else if (eEnd == END_FRONT)
    {
        pNewNode->pNext   = list.pHead;
        list.pHead->pPrev = pNewNode;
        list.pHead        = pNewNode;
    }
    else
    {
        pNewNode->pPrev   = list.pTail;
        list.pTail->pNext = pNewNode;
        list.pTail        = pNewNode;
    }

    ++list.nSize;
    return true;
}

bool popListEnd(DoublyListAdt& list, ListEnd eEnd, double& dOutValue)
{
    if (list.pHead == nullptr)
    {
        return false;
    }

    DoublyNode* pTarget = nullptr;

    if (list.pHead == list.pTail)
    {
        pTarget    = list.pHead;
        dOutValue  = pTarget->dData;
        list.pHead = nullptr;
        list.pTail = nullptr;
    }
    else if (eEnd == END_FRONT)
    {
        pTarget           = list.pHead;
        dOutValue         = pTarget->dData;
        list.pHead        = pTarget->pNext;
        list.pHead->pPrev = nullptr;
    }
    else
    {
        pTarget           = list.pTail;
        dOutValue         = pTarget->dData;
        list.pTail        = pTarget->pPrev;
        list.pTail->pNext = nullptr;
    }

    delete pTarget;
    pTarget = nullptr;
    --list.nSize;

    return true;
}

void clearDoublyList(DoublyListAdt& list)
{
    DoublyNode* pCurrent = list.pHead;
    while (pCurrent != nullptr)
    {
        DoublyNode* pNextNode = pCurrent->pNext;
        delete pCurrent;
        pCurrent = pNextNode;
    }

    list.pHead = nullptr;
    list.pTail = nullptr;
    list.nSize = 0;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Закриті підпрограми АТД: Селектори
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool isListEmpty(const DoublyListAdt& list)
{
    return (list.pHead == nullptr);
}

int getListSize(const DoublyListAdt& list)
{
    return list.nSize;
}

bool peekListEnd(const DoublyListAdt& list, ListEnd eEnd, double& dOutValue)
{
    bool bFound = false;

    if (list.pHead != nullptr)
    {
        dOutValue = (eEnd == END_FRONT) ? list.pHead->dData : list.pTail->dData;
        bFound    = true;
    }

    return bFound;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Закриті підпрограми АТД: Ітератори
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void iterateListForward(const DoublyListAdt& list, void (*pVisit)(double dValue, int nIndex))
{
    if (pVisit == nullptr) return;

    const DoublyNode* pCurrent = list.pHead;
    int               nIndex   = 0;

    while (pCurrent != nullptr)
    {
        pVisit(pCurrent->dData, nIndex);
        pCurrent = pCurrent->pNext;
        ++nIndex;
    }
}

void iterateListBackward(const DoublyListAdt& list, void (*pVisit)(double dValue, int nIndex))
{
    if (pVisit == nullptr) return;

    const DoublyNode* pCurrent = list.pTail;
    int               nIndex   = 0;

    while (pCurrent != nullptr)
    {
        pVisit(pCurrent->dData, nIndex);
        pCurrent = pCurrent->pPrev;
        ++nIndex;
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Закрита підпрограма зв'язування покажчиків на функції АТД
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DoublyListAdt createDoublyListAdt()
{
    DoublyListAdt list = {};

    list.pHead            = nullptr;
    list.pTail            = nullptr;
    list.nSize            = 0;
    list.pInit            = initDoublyList;
    list.pInsert          = pushListEnd;
    list.pRemove          = popListEnd;
    list.pClear           = clearDoublyList;
    list.pIsEmpty         = isListEmpty;
    list.pGetSize         = getListSize;
    list.pPeekEnd         = peekListEnd;
    list.pIterateForward  = iterateListForward;
    list.pIterateBackward = iterateListBackward;

    return list;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Допоміжні підпрограми виведення стану та результатів обробки АТД
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void printNodeRow(double dValue, int nIndex)
{
    std::cout << "  | " << std::right << std::setw(4) << nIndex
              << " | "  << std::right << std::setw(12) << std::fixed << std::setprecision(2) << dValue
              << " |\n";
}

void printListTraversal(const DoublyListAdt& list, const char* pTitle, bool bReverse)
{
    std::cout << pTitle << "\n";

    if (list.pIsEmpty(list))
    {
        std::cout << "  [EMPTY]    Doubly linked list has no elements.\n\n";
        return;
    }

    std::cout << "  +------+--------------+\n";
    std::cout << "  | Step |        Value |\n";
    std::cout << "  +------+--------------+\n";

    if (bReverse)
    {
        list.pIterateBackward(list, printNodeRow);
    }
    else
    {
        list.pIterateForward(list, printNodeRow);
    }

    std::cout << "  +------+--------------+\n\n";
}

void printListSummary(const DoublyListAdt& list)
{
    double dFrontVal = 0.0;
    double dBackVal  = 0.0;

    std::cout << "ADT State Summary:\n";
    std::cout << "  - Size:    "         << list.pGetSize(list)                  << "\n";
    std::cout << "  - Empty:   "         << (list.pIsEmpty(list) ? "Yes" : "No") << "\n";

    if (list.pPeekEnd(list, END_FRONT, dFrontVal))
    {
        std::cout << "  - Front:   " << std::fixed << std::setprecision(2) << dFrontVal << "\n";
    }
    else
    {
        std::cout << "  - Front:   N/A\n";
    }

    if (list.pPeekEnd(list, END_BACK, dBackVal))
    {
        std::cout << "  - Back:    " << std::fixed << std::setprecision(2) << dBackVal << "\n\n";
    }
    else
    {
        std::cout << "  - Back:    N/A\n\n";
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Сценарій демонстрації операцій АТД (конструктори, селектори, ітератори)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void executeAdtScenario()
{
    const double  arrInsertValues[INSERT_COUNT] = { 12.50, -3.75, 48.20, 0.00, 19.40, -8.60 };
    const ListEnd arrInsertEnds[INSERT_COUNT]   = { END_BACK, END_FRONT, END_BACK, END_FRONT, END_BACK, END_FRONT };
    const ListEnd arrRemoveEnds[REMOVE_COUNT]   = { END_FRONT, END_BACK, END_FRONT, END_BACK, END_FRONT, END_BACK, END_FRONT };

    DoublyListAdt list = createDoublyListAdt();

    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << "                       Part 1: Initialization of ADT (Constructor) \n";
    std::cout << "-----------------------------------------------------------------------------------------\n\n";
    list.pInit(list);
    std::cout << "  [INIT]     Doubly linked list initialized (pHead = nullptr, pTail = nullptr).\n\n";

    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << "             Part 2: Operation 1 - Inserting values at either end (Constructor) \n";
    std::cout << "-----------------------------------------------------------------------------------------\n\n";
    for (int i = 0; i < INSERT_COUNT; ++i)
    {
        bool bInserted = list.pInsert(list, arrInsertEnds[i], arrInsertValues[i]);
        if (bInserted)
        {
            std::cout << "  [INSERT]   End = " << getEndLabel(arrInsertEnds[i])
                      << " | Value = "         << std::right << std::setw(7) << std::fixed << std::setprecision(2) << arrInsertValues[i]
                      << " | New size = "      << list.pGetSize(list) << "\n";
        }
        else
        {
            std::cout << "  [OVERFLOW] Failed to allocate node for value "
                      << std::fixed << std::setprecision(2) << arrInsertValues[i] << ".\n";
        }
    }
    std::cout << "\n";

    printListSummary(list);
    printListTraversal(list, "Forward Traversal (Head -> Tail, Iterator):", false);
    printListTraversal(list, "Backward Traversal (Tail -> Head, Iterator):", true);

    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << "     Part 3: Operation 2 - Removing and reading values from either end (Constructor) \n";
    std::cout << "-----------------------------------------------------------------------------------------\n\n";
    for (int i = 0; i < REMOVE_COUNT; ++i)
    {
        double dRemovedValue = 0.0;
        bool   bRemoved      = list.pRemove(list, arrRemoveEnds[i], dRemovedValue);

        if (bRemoved)
        {
            std::cout << "  [REMOVE]   End = " << getEndLabel(arrRemoveEnds[i])
                      << " | Value = "         << std::right << std::setw(7) << std::fixed << std::setprecision(2) << dRemovedValue
                      << " | Remaining = "     << list.pGetSize(list) << "\n";
        }
        else
        {
            std::cout << "  [EMPTY]    End = " << getEndLabel(arrRemoveEnds[i])
                      << " | Cannot remove: list is already empty.\n";
        }
    }
    std::cout << "\n";

    printListSummary(list);

    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << "                          Part 4: Cleanup of ADT (Constructor) \n";
    std::cout << "-----------------------------------------------------------------------------------------\n\n";
    list.pClear(list);
    std::cout << "  [CLEAR]    All remaining dynamic nodes released (Size = " << list.pGetSize(list) << ").\n\n";
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Головна функція main
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main()
{
    std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    std::cout << "          LABORATORY WORK 2.6: ABSTRACT DATA TYPES - DOUBLY LINKED LIST (VARIANT 3)\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

    executeAdtScenario();

    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    std::cout << "                         Laboratory work 2.6 completed successfully.\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    return 0;
}
