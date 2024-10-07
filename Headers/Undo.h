#pragma once

#include "Book.h"
#include "Library.h"

class undoAction {
public:
    virtual void doUndo() = 0;
    virtual ~undoAction() = default;
};

class undoAdd : public undoAction {
private:
    Book added_book;
    Library& library;
public:
    undoAdd(Library& lib, const Book& book);
    void doUndo() override;
};

class undoDelete : public undoAction {
private:
    Book deleted_book;
    Library& library;
public:
    undoDelete(Library& lib, const Book& book);
    void doUndo() override;
};

class undoUpdate : public undoAction {
private:
    Book old_book;
    Book new_book;
    Library& library;
public:
    undoUpdate(Library& lib, const Book& ob, const Book& nb);
    void doUndo() override;
};