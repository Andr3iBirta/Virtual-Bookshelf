#include "Bookshelf.h"
#include <fstream>

using std::ofstream;
using std::to_string;

void Bookshelf::store(const Book& book) {
    books.push_back(book);
}

void Bookshelf::writeToFile(const string& file) {
    ofstream g(file);
    for (const Book& b : books) {
        string line = lineFromBook(b);
        g << line;
    }
    g.close();
}

string Bookshelf::lineFromBook(const Book& book) {
    return book.getTitle() + "," + book.getAuthor() + "," +
           book.getGenre() + "," + to_string(book.getYear()) + '\n';
}