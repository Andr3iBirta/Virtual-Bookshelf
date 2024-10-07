#include <cassert>
#include "Book.h"

using std::to_string;

Book::Book(string title, string author, string genre, int year):
        title{ std::move(title) },
        author{ std::move(author) },
        genre{ std::move(genre) },
        year { year } {}

Book::Book(const Book& other) {
    title = other.title;
    author = other.author;
    genre = other.genre;
    year = other.year;
}

string Book::getTitle() const {
    return title;
}

string Book::getAuthor() const {
    return author;
}

string Book::getGenre() const {
    return genre;
}

int Book::getYear() const {
    return year;
}

void Book::setTitle(const string& new_title) {
    title = new_title;
}

void Book::setAuthor(const string& new_author) {
    author = new_author;
}

void Book::setGenre(const string& new_genre) {
    genre = new_genre;
}

void Book::setYear(int new_year){
    year = new_year;
}

string Book::toString() const {
    return getTitle() + " ~ " + getAuthor() + " ~ " + getGenre() + " ~ " + to_string(getYear());
}

void testBook() {
    Book b{"A", "B", "C", 2000 };

    assert(b.getTitle() == "A");
    assert(b.getAuthor() == "B");
    assert(b.getGenre() == "C");
    assert(b.getYear() == 2000);

    b.setTitle("a");
    b.setAuthor("b");
    b.setGenre("c");
    b.setYear(2005);

    assert(b.getTitle() == "a");
    assert(b.getAuthor() == "b");
    assert(b.getGenre() == "c");
    assert(b.getYear() == 2005);

    assert(b.toString() == "a ~ b ~ c ~ 2005");
}
