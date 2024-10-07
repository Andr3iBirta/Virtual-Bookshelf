#pragma once
#include <QAbstractListModel>
#include <vector>
#include "Book.h"
#include <iostream>
using std::vector;

class TableModel : public QAbstractTableModel {
private:
    vector<Book> books;
public:
    TableModel(const vector<Book>& books) : books { books } {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return books.size();
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        return 4;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole) {
            auto book = books[index.row()];
            if (index.column() == 0)
                return QString::fromStdString(book.getTitle());
            else if (index.column() == 1)
                return QString::fromStdString(book.getAuthor());
            else if (index.column() == 2)
                return QString::fromStdString(book.getGenre());
            else
                return QString::number(book.getYear());
        }
        return {};
    }

    void setBooks(const vector<Book>& books) {
        this->books = books;
        auto topLeft = createIndex(0,0);
        auto botRight = createIndex(rowCount(), columnCount());
        emit dataChanged(topLeft,botRight);
        emit layoutChanged();
    }
};