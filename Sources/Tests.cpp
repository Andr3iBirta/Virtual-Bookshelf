#include <iostream>
#include <QDebug>
#include "Book.h"
#include "Library.h"
#include "FileRepo.h"
#include "Validation.h"
#include "Controller.h"
#include "Tests.h"

using std::cout;

void Tester::runAllTests() {
    qDebug() << "\n";
    testBook();
    qDebug() << "  ~Entity tests passed!\n";
    testStore();
    testUpdate();
    testErase();
    testFileRepo();
    qDebug() << "  ~Repo tests passed!\n";
    testValidator();
    qDebug() << "  ~Validator tests passed!\n";
    testAdd();
    testUpd();
    testDel();
    testFilter();
    testSort();
    testReport();
    testBookShelf();
    testUndo();
    qDebug() << "  ~Controller tests passed!\n";
}
