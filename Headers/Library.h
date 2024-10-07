#pragma once
#include <vector>
#include "Book.h"
#include "GenericRepo.h"
using std::vector;

class RepoError : public std::exception {
private:
    string msg;

public:
    explicit RepoError(string msg);
    string getMessage();
};

class Library : public Repo<Book> {
public:
    /** library constructor */
    Library();

    /** destroy all copies of this object */
    Library(const Library& other) = delete;

    /** @return all the books that are stored in the library */
    const vector<Book>& getAll() override;

    /** @return the number of books in the library */
    size_t size() override;

    /**
     * tries to store a new book in the library
     * @param b - a valid book
     * @throws RepoError - b already exists
     */
    void store(const Book& b) override;

    /**
     * searches for the book with matching title
     * @param title - string
     * @return reference to the book
     * @throws RepoError - title not found
     */
    Book& searchByTitle(const string& title) override;

    /**
     * tries to update the old book from the library
     * @param old_book - a book from the library
     * @param new_book - a valid book
     */
    void update(const Book& old_book, const Book& new_book) override;

    /**
     * tries to delete the given book from the library
     * @param book - a valid book
     */
    void erase(Book& book) override;

    /** deletes all the books in the library */
    void clearContents() override;

    /** @return the book at given index */
    [[nodiscard]] Book& getBook(int index);
};

void testStore();

void testUpdate();

void testErase();