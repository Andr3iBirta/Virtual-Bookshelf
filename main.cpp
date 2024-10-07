#include <QApplication>
#include "FileRepo.h"
#include "GUI.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    FileRepo fileRepo { "books.txt" };
    Bookshelf shelf;
    Controller controller { fileRepo, shelf };
    LibraryGUI gui { controller };

    gui.resize(1250, 500);
    gui.show();

    return QApplication::exec();
}
