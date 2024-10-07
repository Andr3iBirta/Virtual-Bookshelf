#include <iostream>
#include <cassert>
#include <algorithm>
#include "Library.h"
using std::cout;

RepoError::RepoError(string msg):
        msg{ std::move(msg) } {}

string RepoError::getMessage() {
    return msg;
}

Library::Library() {
    books = vector<Book>();
}

const vector<Book>& Library::getAll() {
    return books;
}

size_t Library::size() {
    return books.size();
}

void Library::store(const Book& book) {
    for(const auto& _book : books)
        if(book == _book)
            throw RepoError("  This book already exists.\n");
    books.push_back(book);
}

Book& Library::getBook(int index) {
    return books[index];
}

Book& Library::searchByTitle(const string& title) {
    if (books.empty())
        throw RepoError("  No books recorded.\n");
    for (auto& book : books)
        if (book.getTitle() == title)
            return book;
    throw RepoError("  Book not found.\n");
}

void Library::update(const Book& old_book, const Book& new_book) {
    for (auto& book : books)
        if (book == old_book) {
            book = new_book;
            break;
        }
}

void Library::erase(Book& book) {
    auto iterator = std::find_if(books.begin(), books.end(), [&book](const Book& el){
        return el.getTitle() == book.getTitle();
    });
    books.erase(iterator);
}

void Library::clearContents() {
    books.clear();
}

void testStore() {
    Library L;
    Book b1 {"A", "B", "C", 2000};
    L.store(b1);
    auto bookshelf = L.getAll();
    assert(bookshelf.size() == 1);
    try {
        L.store(b1);
    } catch(RepoError &e) {}
}

void testUpdate() {
    Library L;
    Book b1 {"A", "B", "C", 2000};

    try {
        L.searchByTitle("A");
    } catch(RepoError &e) {
        assert(e.getMessage() == "  No books recorded.\n");
    }

    L.store(b1);

    try {
        L.searchByTitle("a");
    } catch(RepoError &e) {
        assert(e.getMessage() == "  Book not found.\n");
    }

    Book b2 = {"a", "b", "c", 2005};
    L.update(b1, b2);

    assert(L.searchByTitle("a") == b2);

    assert(L.getBook(0).getTitle() == "a");
    assert(L.getBook(0).getAuthor() == "b");
    assert(L.getBook(0).getGenre() == "c");
    assert(L.getBook(0).getYear() == 2005);
}

void testErase() {
    Library L;
    Book b1 {"A", "B", "C", 2000};
    L.store(b1);
    assert(L.size() == 1);
    L.erase(b1);
    assert(L.size() == 0);
}