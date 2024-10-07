#include "Undo.h"

undoAdd::undoAdd(Library& lib, const Book& book) :
        library { lib }, added_book { book } {}

void undoAdd::doUndo() {
    library.erase(added_book);
}

undoDelete::undoDelete(Library& lib, const Book& book) :
        library { lib }, deleted_book { book } {}

void undoDelete::doUndo() {
    library.store(deleted_book);
}

undoUpdate::undoUpdate(Library& lib, const Book& ob, const Book& nb) :
        library { lib }, old_book { ob }, new_book { nb } {}

void undoUpdate::doUndo() {
    library.update(new_book, old_book);
}