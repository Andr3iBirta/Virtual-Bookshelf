#include <cassert>
#include "Book.h"
#include "Validation.h"

ValidationError::ValidationError(string msg):
        msg{ std::move(msg) } {}

string ValidationError::getMessage() {
    return msg;
}

void Validator::validateBook(const Book& book) {
    string errors;
    if(book.getTitle().empty())
        errors += "  Title field can't be empty.\n";
    if(book.getAuthor().empty())
        errors += "  Author field can't be empty.\n";
    if(book.getGenre().empty())
        errors += "  Genre field can't be empty.\n";
    if(book.getYear() < 0)
        errors += "  The book must be from this age.\n";
    if(!errors.empty())
        throw ValidationError(errors);
}

void testValidator() {
    Book b {"A", "B", "C", 2000};
    Validator::validateBook(b);

    b.setTitle("");
    try {
        Validator::validateBook(b);
    } catch(ValidationError& e) {
        assert(e.getMessage() == "  Title field can't be empty.\n");
    }

    b.setAuthor("");
    try {
        Validator::validateBook(b);
    } catch(ValidationError& e) {
        assert(e.getMessage() == "  Title field can't be empty.\n  Author field can't be empty.\n");
    }

    b.setGenre("");
    try {
        Validator::validateBook(b);
    } catch(ValidationError& e) {
        assert(e.getMessage() == "  Title field can't be empty.\n  Author field can't be empty.\n  Genre field can't be empty.\n");
    }

    b.setYear(-1);
    try {
        Validator::validateBook(b);
    } catch(ValidationError& e) {
        assert(e.getMessage() == "  Title field can't be empty.\n  Author field can't be empty.\n  Genre field can't be empty.\n  The book must be from this age.\n");
    }
}