#include <iostream>
#include <cassert>
#include <algorithm>
#include <random>
#include <vector>
#include <memory>
#include <fstream>
#include "Book.h"
#include "Validation.h"
#include "Controller.h"

using std::sort;
using std::copy_if;
using std::back_inserter;
using std::pair;
using std::vector;
using std::make_unique;
using std::ifstream;
using std::getline;

Controller::Controller(Library& lib, Bookshelf& shelf) :
        library { lib }, bookshelf { shelf } {}

void Controller::add(string title, string author, string genre, int year) {
    Book b{ std::move(title), std::move(author), std::move(genre), year };
    Validator::validateBook(b);
    library.store(b);
    undoList.push_back(make_unique<undoAdd>(library,b));
}

void Controller::update(const string& title, const string& new_title,
                        const string& new_author, const string& new_genre, int new_year) {
    const Book& old_book = library.searchByTitle(title);
    Book new_book { new_title, new_author,
                    new_genre, new_year };
    Validator::validateBook(new_book);
    if (old_book == new_book)
        throw RepoError("Book has the same fields.");
    undoList.push_back(make_unique<undoUpdate>(library,old_book, new_book));
    library.update(old_book, new_book);
}

void Controller::erase(const string& title) {
    Book& book = library.searchByTitle(title);
    undoList.push_back(make_unique<undoDelete>(library, book));
    library.erase(book);
}

const vector<Book>& Controller::getAll() const {
    return library.getAll();
}

vector<Book> Controller::filterByTitle(const string& title) const {
    vector<Book> list;

    if (library.getAll().empty())
        throw RepoError("  No books recorded.\n");

    copy_if(library.getAll().begin(), library.getAll().end(),
            back_inserter(list), [&title](const Book& el) {
                return el.getTitle() == title;
            });

    if (list.empty())
        throw RepoError("  No books found.\n");

    return list;
}

vector<Book> Controller::filterByYear(int year) const {
    vector<Book> list;

    if (library.getAll().empty())
        throw RepoError("  No books recorded.\n");

    copy_if(library.getAll().begin(), library.getAll().end(),
            back_inserter(list), [year](const Book& el) {
                return el.getYear() >= year;
            });

    if (list.empty())
        throw RepoError("  No books found.\n");

    return list;
}

vector<Book> Controller::sortBooks() const {
    vector<Book> list = library.getAll();

    if (list.empty())
        throw RepoError("  No books recorded.\n");

    sort(list.begin(), list.end(), compare);

    return list;
}

void Controller::undo() {
    if (undoList.empty())
        throw RepoError("  No action to undo.\n");
    undoList.back()->doUndo();
    undoList.pop_back();
}

map<string, vector<Book>> Controller::genreReport() const {
    vector<Book> list = library.getAll();
    map<string, vector<Book>> report;
    for (const auto& book : list) {
        bool found = false;
        for (const auto& pair : report)
            if (pair.first == book.getGenre()) {
                found = true;
                break;
            }
        if (!found)
            report[book.getGenre()] = vector<Book>();
        report[book.getGenre()].push_back(book);
    }
    return report;
}

bool compare(const Book& elem, const Book& other) {
    if (elem.getTitle() < other.getTitle())
        return true;
    if (elem.getTitle() == other.getTitle()) {
        if (elem.getAuthor() < other.getAuthor())
            return true;
        if (elem.getAuthor() == other.getAuthor()) {
            if (elem.getYear() < other.getYear())
                return true;
            if (elem.getYear() == other.getYear())
                if (elem.getGenre() <= other.getGenre())
                    return true;
        }
    }
    return false;
}

void Controller::b_emptyShelf() const {
    if (bookshelf.getAll().empty())
        throw RepoError("  Bookshelf already empty.\n");
    bookshelf.clearContents();
    notify();
}

void Controller::b_placeBook(const string& title) const {
    Book& book = library.searchByTitle(title);
    bookshelf.store(book);
    notify();
}

void Controller::b_generateShelf(int nr) const {
    std::mt19937 mt { std::random_device{}() };
    std::uniform_int_distribution<> dist(0, library.getAll().size() - 1);
    for (int i = 0; i < nr; i++) {
        int randNr = dist(mt);
        bookshelf.store(library.getBook(randNr));
    }
    notify();
}

int Controller::b_size() const {
    return bookshelf.getAll().size();
}

void Controller::b_export(const string& file) const {
    bookshelf.writeToFile(file);
}

const vector<Book>& Controller::b_getAll() const {
    return bookshelf.getAll();
}

void testAdd() {
    Library lib;
    Bookshelf shelf;
    Controller ctr{lib, shelf};
    ctr.add("A", "B", "C", 2000);
    assert(ctr.getAll().size() == 1);
    try {
        ctr.add("", "B", "C", 2000);
    } catch(ValidationError& e) {}
    try {
        ctr.add("A", "B", "C", 2000);
    } catch(RepoError& e) {}
}

void testUpd() {
    Library lib;
    Bookshelf shelf;

    Controller ctr{lib, shelf};
    try {
        ctr.update("A", "a", "b", "c", 2005);
    } catch(RepoError& e) {}
    ctr.add("A", "B", "C", 2000);
    ctr.update("A", "a", "b", "c", 2005);
    try {
        ctr.update("A", "a", "b", "c", 2005);
    } catch(RepoError& e) {}
}

void testDel() {
    Library lib;
    Bookshelf shelf;

    Controller ctr{lib, shelf};
    try {
        ctr.erase("a");
    } catch(RepoError& e) {}
    ctr.add("A", "B", "C", 2000);
    ctr.erase("A");
    assert(ctr.getAll().empty());
    try {
        ctr.erase("A");
    } catch(RepoError& e) {}
}

void testFilter() {
    Library lib;
    Bookshelf shelf;
    Controller ctr{lib, shelf};

    try {
        ctr.filterByTitle("A");
    } catch(RepoError& e) {}

    try {
        ctr.filterByYear(2000);
    } catch(RepoError& e) {}

    ctr.add("A", "B", "C", 1999);
    ctr.add("A", "B", "C", 2000);
    ctr.add("A", "B", "C", 2005);
    ctr.add("B", "B", "C", 2006);
    ctr.add("A", "B", "C", 2010);

    /// filter by title
    vector<Book> list = ctr.filterByTitle("A");
    assert(list.size() == 4);

    list = ctr.filterByTitle("B");
    assert(list.size() == 1);

    try {
        ctr.filterByTitle("C");
    } catch(RepoError& e) {}

    /// filter by year
    list = ctr.filterByYear(2000);
    assert(list.size() == 4);

    list = ctr.filterByYear(2010);
    assert(list.size() == 1);

    try {
        ctr.filterByYear(2011);
    } catch(RepoError& e) {}
}

void testSort() {
    Library lib;
    Bookshelf shelf;
    Controller ctr{lib, shelf};

    try {
        ctr.sortBooks();
    } catch(RepoError& e) {}

    ctr.add("b", "B", "C", 1999);
    ctr.add("a", "B", "C", 2000);
    ctr.add("c", "B", "C", 2005);
    ctr.add("e", "B", "C", 2006);
    ctr.add("d", "B", "C", 2010);

    auto list = ctr.sortBooks();

    assert(list[0].getTitle() == "a");
    assert(list[1].getTitle() == "b");
    assert(list[2].getTitle() == "c");
    assert(list[3].getTitle() == "d");
    assert(list[4].getTitle() == "e");
}

void testReport() {
    Library lib;
    Bookshelf shelf;
    Controller ctr{lib, shelf};

    ctr.add("b", "B", "C", 1999);
    ctr.add("a", "B", "A", 2000);
    ctr.add("c", "B", "C", 2005);
    ctr.add("e", "B", "B", 2006);
    ctr.add("d", "B", "B", 2010);

    map<string, vector<Book>> report = ctr.genreReport();

    auto pair = report.begin();
    assert(pair->first == "A");
    assert(pair->second.size() == 1);

    ++pair;
    assert(pair->first == "B");
    assert(pair->second.size() == 2);

    ++pair;
    assert(pair->first == "C");
    assert(pair->second.size() == 2);
}

void testBookShelf() {
    Library lib;
    Bookshelf shelf;
    Controller ctr{lib, shelf};
    ctr.add("A", "B", "C", 2000);
    ctr.add("B", "B", "C", 2000);
    ctr.add("C", "B", "C", 2000);
    try {
        ctr.b_emptyShelf();
        assert(false);
    } catch(RepoError& e) {}
    try {
        ctr.b_placeBook("D");
        assert(false);
    } catch(RepoError& e) {}
    ctr.b_placeBook("A");
    ctr.b_placeBook("A");
    assert(ctr.b_size() == 2);
    ctr.b_emptyShelf();
    assert(ctr.b_size() == 0);
    ctr.b_generateShelf(10);
    assert(ctr.b_size() == 10);

    /// Test export
    ctr.b_export("testbookshelf.txt");
    ifstream f("testbookshelf.txt");
    int lines = 0;
    string line;
    while (getline(f, line))
        if (!line.empty())
            lines++;
    f.close();
    assert(ctr.b_size() == lines);
}

void testUndo() {
    Library lib;
    Bookshelf shelf;
    Controller ctr{lib, shelf};
    try {
        ctr.undo();
    } catch(RepoError& e) {}
    ctr.add("A", "B", "C", 2000);
    ctr.update("A", "a", "b", "c", 2005);
    assert(ctr.getAll()[0].getTitle() == "a");
    ctr.undo();
    assert(ctr.getAll()[0].getTitle() == "A");
    ctr.undo();
    assert(ctr.getAll().empty());
    ctr.add("A", "B", "C", 2000);
    ctr.erase("A");
    assert(ctr.getAll().empty());
    ctr.undo();
    assert(ctr.getAll().size() == 1);
}