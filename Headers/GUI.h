#pragma once
#include <qwidget.h>
#include <QApplication>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qlineedit.h>
#include <qformlayout.h>
#include <qlistwidget.h>
#include <qmessagebox.h>
#include <string>
#include <qtablewidget.h>
#include <qpainter.h>
#include "Validation.h"
#include "Controller.h"
#include "Observer.h"
#include "ModelView.h"
using std::string;

class LibraryGUI : public QWidget {
public:
    explicit LibraryGUI(Controller& ctr);
private:
    Controller& controller;
    QHBoxLayout* mainLayout;
    QHBoxLayout* buttonLayout;
    QVBoxLayout* rhsLayout;
    QVBoxLayout* buttonLayout1;
    QVBoxLayout* buttonLayout2;
    QVBoxLayout* buttonLayout3;
    QVBoxLayout* genreButtons;
    QFormLayout* formLayout;
    QLineEdit* title;
    QLineEdit* author;
    QLineEdit* genre;
    QLineEdit* year;
    QLineEdit* titleFilter;
    QLineEdit* yearFilter;
    QListWidget* list;
    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* updateButton;
    QPushButton* exitButton;
    QPushButton* filterButton1;
    QPushButton* filterButton2;
    QPushButton* sortButton;
    QPushButton* bookshelfButton;
    QPushButton* bookshelfReadOnly;
    QPushButton* undoButton;

    void initGUI();
    void loadData(const vector<Book>& repo);
    void cosmeticizeWindow();
    void connectButtons();
    static void createMessageBox(const QString& windowTitle, const QString& text);

private slots:
    void changedSelection();
    void applyFilterTitle();
    void applyFilterYear();
    void applySort();
    void clickedAdd();
    void clickedDelete();
    void clickedUpdate();
    void clickedExit();
    void clickedUndo();
};

class BookshelfGUI : public QWidget, public Observer {
public:
    explicit BookshelfGUI(Controller& ctr);
private:
    Controller& controller;
    QHBoxLayout* mainLayout;
    QHBoxLayout* buttonLayout;
    QVBoxLayout* rhsLayout;
    QFormLayout* formLayout;
    QLineEdit* title;
    QLineEdit* number;
    QLineEdit* filename;
    QTableView* table;
    TableModel* tableModel;
    QPushButton* emptyButton;
    QPushButton* placeButton;
    QPushButton* generateButton;
    QPushButton* exportButton;
    QPushButton* exitButton;

    void initGUI();
    void loadData(const vector<Book>& repo);
    void cosmeticizeWindow();
    void connectButtons();
    static void createMessageBox(const QString& windowTitle, const QString& text);
    void update() override;

private slots:
    void clickedEmpty();
    void clickedPlace();
    void clickedGenerate();
    void clickedExport();
    void clickedExit();
};

class BookshelfReadOnly : public QWidget, public Observer {
public:
    explicit BookshelfReadOnly(Controller& ctr);
private:
    Controller& controller;

    void update() override;
    void paintEvent(QPaintEvent* ev) override;
    static void drawShape(QPainter* painter, int x, int y, int size);
};