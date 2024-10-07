#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include "FileRepo.h"

using std::string;
using std::to_string;
using std::ifstream;
using std::ofstream;

FileRepo::FileRepo(const string& file) : file { file } {
    loadFromFile();
}

void FileRepo::loadFromFile() {
    ifstream f(file);
    string line;
    while (std::getline(f, line)) {
        if (line.empty())
            break;
        Book b = bookFromLine(line);
        Library::store(b);
    }
    f.close();
}

void FileRepo::writeToFile() {
    ofstream g(file);
    for (const Book& b : books) {
        string line = lineFromBook(b);
        g << line;
    }
    g.close();
}

Book FileRepo::bookFromLine(string& line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(iss, token, ',')) {
        tokens.push_back(token);
    }
    Book bk;
    bk.setTitle(tokens[0]);
    bk.setAuthor(tokens[1]);
    bk.setGenre(tokens[2]);
    bk.setYear(std::stoi(tokens[3]));
    return bk;
}

string FileRepo::lineFromBook(const Book& book) {
    return book.getTitle() + "," + book.getAuthor() + "," +
           book.getGenre() + "," + to_string(book.getYear()) + '\n';
}

void FileRepo::emptyFile() const {
    ofstream f(file);
    f.close();
}

void FileRepo::store(const Book &book) {
    Library::store(book);
    ofstream f (file, std::ios::app);
    f << lineFromBook(book);
    f.close();
}

void FileRepo::erase(Book &book) {
    Library::erase(book);
    writeToFile();
}

void FileRepo::update(const Book &old_book, const Book &new_book) {
    Library::update(old_book, new_book);
    writeToFile();
}

void testFileRepo() {
    FileRepo fr1 { "test.txt" };
    fr1.store(Book {"a", "b", "c", 2000});

    FileRepo fr2 { "test.txt" };
    assert(fr2.size() == 1);

    fr1.erase(fr1.searchByTitle("a"));
    FileRepo fr3 { "test.txt" };
    assert(fr3.size() == 0);

    fr2.update(fr2.searchByTitle("a"), Book {"A", "B", "C", 2001});
    fr1.loadFromFile();
    const auto it = fr1.getAll().begin();
    assert(it->getTitle() == "A");
    assert(it->getAuthor() == "B");
    assert(it->getGenre() == "C");
    assert(it->getYear() == 2001);

    fr1.emptyFile();
}