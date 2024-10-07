#pragma once
#include <string>
using std::string;

class Book {
private:
    string title;
    string author;
    string genre;
    int year = 0;

public:
    [[nodiscard]] string getTitle() const;
    [[nodiscard]] string getAuthor() const;
    [[nodiscard]] string getGenre() const;
    [[nodiscard]] int getYear() const;

    void setTitle(const string& new_title);
    void setAuthor(const string& new_author);
    void setGenre(const string& new_genre);
    void setYear(int new_year);

    /** book constructor */
    Book(string title, string author, string genre, int year);
    Book() = default;

    /** copy constructor */
    Book(const Book& other);

    string toString() const;

    /** overloaded equality operator */
    bool operator == (const Book &other) const {
        return title == other.title && author == other.author && genre == other.genre && year == other.year;
    }
};

void testBook();