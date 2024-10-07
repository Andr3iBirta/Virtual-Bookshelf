#pragma once
#include <vector>

using std::vector;

template<typename Element>
class Repo {
protected:
    vector<Element> books;

public:
    /** repo constructor */
    Repo() {
        books = vector<Element>();
    };

    /** @return a list of all the stored elements */
    virtual const vector<Element>& getAll() = 0;

    /** @return the number of elements */
    virtual size_t size() = 0;

    virtual void store(const Element& b) = 0;

    virtual Element& searchByTitle(const string& title) = 0;

    virtual void update(const Element& old_book, const Element& new_book) = 0;

    virtual void erase(Element& book) = 0;

    virtual void clearContents() = 0;

    virtual ~Repo() = default;
};