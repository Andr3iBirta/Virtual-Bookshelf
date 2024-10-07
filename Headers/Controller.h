#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Library.h"
#include "Bookshelf.h"
#include "UNDO.h"
#include "Observer.h"

using std::string;
using std::vector;
using std::map;
using std::unique_ptr;

class Controller : public Observable {
private:
    Library& library;
    Bookshelf& bookshelf;
    vector<unique_ptr<undoAction>> undoList;

public:
    /** controller constructor */
    Controller(Library& lib, Bookshelf& shelf);

    /** destroy all copies */
    Controller(const Controller& other) = delete;

    /**
     * creates a new book and tries to save it in the library
     * @param title - string
     * @param author - string
     * @param genre - string
     * @param year - int
     * @throws ValidationError - one of the fields is invalid
     * @throws RepoError - a book with the same fields already exists
     */
    void add(string title, string author, string genre, int year);

    /**
     * updates the book with the given title
     * @param title - string
     * @param new_title - string
     * @param new_author - string
     * @param new_genre - string
     * @param new_year - int
     * @throws RepoError - no books in library or title not found
     * @throws ValidationError - new fields are invalid
     */
    void update(const string& title, const string& new_title,
                const string& new_author, const string& new_genre, int new_year);

    /**
     * deletes the book with the given title
     * @param title - string
     * @throws RepoError - no books in library or title not found
     */
    void erase(const string& title);

    /**
     * returns a list of books that have the given title
     * @param title - string
     * @throws RepoError - no books in library or no books have the given title
     */
    [[nodiscard]] vector<Book> filterByTitle(const string& title) const;

    /**
     * returns a list of books that have been released since the given year
     * @param year - int
     * @throws RepoError - no books in library or no books have the given title
     */
    [[nodiscard]] vector<Book> filterByYear(int year) const;

    /**
     * returns the books in the library, sorted by title, author, year and genre
     * @throws RepoError - no books in library
     */
    [[nodiscard]] vector<Book> sortBooks() const;

    /** undoes the last action that modified the library */
    void undo();

    /** @return all the books that are stored in the library */
    [[nodiscard]] const vector<Book>& getAll() const;

    /** @return a map with a list for every genre */
    [[nodiscard]] map<string, vector<Book>> genreReport() const;

    /// bookshelf methods
    /** takes all books off the shelf
      * @throws RepoError - bookshelf already empty
      */
    void b_emptyShelf() const;

    /** places the book with the given title on the shelf
      * @param title - string
      * @throws RepoError - library empty or title not found
      */
    void b_placeBook(const string& title) const;

    /** randomly generates a given number of books and fills up the shelf
      * @param nr - int
      */
    void b_generateShelf(int nr) const;

    /** returns number of books on the shelf */
    [[nodiscard]] int b_size() const;

    /** saves all the books on the bookshelf in a file
      * @param file - a valid filename
      */
    void b_export(const string& file) const;

    /** returns all the books in the bookshelf */
    [[nodiscard]] const vector<Book>& b_getAll() const;
};

bool compare(const Book& elem, const Book& other);

void testAdd();

void testUpd();

void testDel();

void testFilter();

void testSort();

void testReport();

void testBookShelf();

void testUndo();
