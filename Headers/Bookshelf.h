#include "Library.h"

class Bookshelf : public Library {
public:
    void store(const Book& book) override;

    void writeToFile(const string& file);

    static string lineFromBook(const Book& book);
};