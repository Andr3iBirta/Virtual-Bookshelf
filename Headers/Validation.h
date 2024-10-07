#pragma once
#include "Book.h"

class ValidationError : public std::exception {
private:
    string msg;

public:
    explicit ValidationError(string msg);
    string getMessage();
};

class Validator {
public:
    /**
     * @throws ValidationError - invalid book
     */
    static void validateBook(const Book& book);
};

void testValidator();