#pragma once
#include "Library.h"

class FileRepo : public Library {
private:
    string file;
public:
    explicit FileRepo(const string& file);

    /** copies the contents of the file in memory */
    void loadFromFile();

    /** saves the recorded books to the file */
    void writeToFile();

    static Book bookFromLine(string& line);

    static string lineFromBook(const Book& book);

    void emptyFile() const;

    /** adds the book in memory and appends to the file */
    void store(const Book& book) override;

    /** updates the book in memory and in file */
    void update(const Book& old_book, const Book& new_book) override;

    /** deletes the book from memory and from file */
    void erase(Book& book) override;
};

void testFileRepo();
