#include <type_traits>
#include <QRandomGenerator>
#include <QPainterPath>
#include "GUI.h"
#include "Tests.h"
using std::to_string;
using std::is_integral;

LibraryGUI::LibraryGUI(Controller& ctr) : controller { ctr } {
    initGUI();
    loadData(controller.getAll());
}

void LibraryGUI::initGUI() {
    Tester::runAllTests();

    cosmeticizeWindow();

    mainLayout = new QHBoxLayout {};
    setLayout(mainLayout);

    list = new QListWidget;
    mainLayout->addWidget(list);

    rhsLayout = new QVBoxLayout;
    mainLayout->addLayout(rhsLayout);

    formLayout = new QFormLayout;
    rhsLayout->addLayout(formLayout);
    title = new QLineEdit;
    author = new QLineEdit;
    genre = new QLineEdit;
    year = new QLineEdit;
    titleFilter = new QLineEdit;
    yearFilter = new QLineEdit;

    auto *spacer = new QSpacerItem(0, 15);
    formLayout->addItem(spacer);

    formLayout->addRow("Title", title);
    formLayout->addRow("Author", author);
    formLayout->addRow("Genre", genre);
    formLayout->addRow("Year", year);

    spacer = new QSpacerItem(0, 60);
    formLayout->addItem(spacer);

    formLayout->addRow("Title Filter", titleFilter);
    formLayout->addRow("Year Filter", yearFilter);

    bookshelfReadOnly = new QPushButton {"&Visualize bookshelf"};
    rhsLayout->addWidget(bookshelfReadOnly);

    buttonLayout = new QHBoxLayout {};
    rhsLayout->addLayout(buttonLayout);

    buttonLayout1 = new QVBoxLayout {};
    buttonLayout->addLayout(buttonLayout1);
    buttonLayout2 = new QVBoxLayout {};
    buttonLayout->addLayout(buttonLayout2);
    buttonLayout3 = new QVBoxLayout {};
    buttonLayout->addLayout(buttonLayout3);

    addButton = new QPushButton {"&Add" };
    buttonLayout1->addWidget(addButton);

    deleteButton = new QPushButton {"&Delete" };
    buttonLayout1->addWidget(deleteButton);

    updateButton = new QPushButton {"&Update" };
    buttonLayout1->addWidget(updateButton);

    filterButton1 = new QPushButton {"&Filter by Title" };
    buttonLayout2->addWidget(filterButton1);

    filterButton2 = new QPushButton {"&Filter by Year" };
    buttonLayout2->addWidget(filterButton2);

    sortButton = new QPushButton {"&Sort" };
    buttonLayout2->addWidget(sortButton);

    undoButton = new QPushButton {"&Undo" };
    buttonLayout3->addWidget(undoButton);

    bookshelfButton = new QPushButton {"&Bookshelf" };
    buttonLayout3->addWidget(bookshelfButton);

    exitButton = new QPushButton {"&Exit" };
    buttonLayout3->addWidget(exitButton);

    genreButtons = new QVBoxLayout {};
    mainLayout->addLayout(genreButtons);

    connectButtons();
}

void LibraryGUI::loadData(const vector<Book>& repo) {
    list->clear();
    for (const auto &book: repo) {
        auto* item = new QListWidgetItem(QString::fromStdString(book.toString()));
        QVariantList data;
        data.push_back(QString::fromStdString(book.getTitle()));
        data.push_back(QString::fromStdString(book.getAuthor()));
        data.push_back(QString::fromStdString(book.getGenre()));
        data.push_back(QString::fromStdString(to_string(book.getYear())));
        item->setData(Qt::UserRole, data);
        list->addItem(item);
    }
}

void LibraryGUI::connectButtons() {
    QObject::connect(list,&QListWidget::itemSelectionChanged,this,&LibraryGUI::changedSelection);
    QObject::connect(addButton,&QPushButton::clicked,this,&LibraryGUI::clickedAdd);
    QObject::connect(deleteButton,&QPushButton::clicked,this,&LibraryGUI::clickedDelete);
    QObject::connect(updateButton,&QPushButton::clicked,this,&LibraryGUI::clickedUpdate);
    QObject::connect(exitButton,&QPushButton::clicked,this,&LibraryGUI::clickedExit);
    QObject::connect(filterButton1,&QPushButton::clicked,this,&LibraryGUI::applyFilterTitle);
    QObject::connect(filterButton2,&QPushButton::clicked,this,&LibraryGUI::applyFilterYear);
    QObject::connect(sortButton,&QPushButton::clicked,this,&LibraryGUI::applySort);
    QObject::connect(undoButton,&QPushButton::clicked,this,&LibraryGUI::clickedUndo);

    QObject::connect(bookshelfButton,&QPushButton::clicked,this,[this](){
        auto* bookshelfWindow = new BookshelfGUI { controller };
        bookshelfWindow->resize(1000, 500);
        bookshelfWindow->move(50, 125);
        bookshelfWindow->show();
    });

    QObject::connect(bookshelfReadOnly,&QPushButton::clicked,this,[this](){
        auto* bookshelfWindow = new BookshelfReadOnly { controller };
        bookshelfWindow->resize(400, 500);
        bookshelfWindow->move(1100, 125);
        bookshelfWindow->show();
    });

    for (const auto& pair : controller.genreReport()) {
        auto button = new QPushButton { QString::fromStdString(pair.first) };
        genreButtons->addWidget(button);
        QObject::connect(button,&QPushButton::clicked,this,[this,pair](){
            loadData(pair.second);
        });
    }
}

void LibraryGUI::changedSelection() {
    auto selected = list->selectedItems();
    if (selected.empty()) {
        title->setText("");
        author->setText("");
        genre->setText("");
        year->setText("");
    }
    else {
        auto selectedItem = selected.at(0);
        QVariantList dataList = selectedItem->data(Qt::UserRole).toList();
        title->setText(dataList.at(0).toString());
        genre->setText(dataList.at(2).toString());
        author->setText(dataList.at(1).toString());
        year->setText(dataList.at(3).toString());
    }
}

void LibraryGUI::applyFilterTitle() {
    try {
        vector<Book> filtered_books = controller.filterByTitle(titleFilter->text().toStdString());
        loadData(filtered_books);
    } catch(RepoError& e) {
        createMessageBox("Error", QString::fromStdString(e.getMessage()));
    }
}

void LibraryGUI::applyFilterYear() {
    try {
        string string_filter = yearFilter->text().toStdString();
        bool isInteger = true;
        if (string_filter.empty() || ((!isdigit(string_filter[0])) && (string_filter[0] != '-') && (string_filter[0] != '+')))
            isInteger = false;
        char* p;
        strtol(string_filter.c_str(), &p, 10);
        if (*p != 0)
            isInteger = false;
        if (!isInteger) {
            createMessageBox("Error", "Year Filter must be an integer");
            return;
        }
        vector<Book> filtered_books = controller.filterByYear(yearFilter->text().toInt());
        loadData(filtered_books);
    } catch(RepoError& e) {
        createMessageBox("Error", QString::fromStdString(e.getMessage()));
    }
}

void LibraryGUI::applySort() {
    try {
        vector<Book> sorted_books = controller.sortBooks();
        loadData(sorted_books);
    } catch(RepoError& e) {
        createMessageBox("Error", QString::fromStdString(e.getMessage()));
    }
}

void LibraryGUI::clickedAdd() {
    try {
        controller.add(title->text().toStdString(), author->text().toStdString(), genre->text().toStdString(), year->text().toInt());
        createMessageBox("Info", "Book added");
        loadData(controller.getAll());
    } catch(RepoError& e) {
        createMessageBox("Error", "Book already exists");
    } catch(ValidationError& e) {
        createMessageBox("Invalid book", QString::fromStdString(e.getMessage()));
    }
}

void LibraryGUI::clickedDelete() {
    try {
        auto selected = list->selectedItems();
        if (selected.empty()) {
            createMessageBox("Error", "Please select a book to delete");
            return;
        }
        controller.erase(title->text().toStdString());
        createMessageBox("Error", "Book deleted");
        loadData(controller.getAll());
    } catch(RepoError& e) {
        createMessageBox("Error", QString::fromStdString(e.getMessage()));
    }
}

void LibraryGUI::clickedUpdate() {
    try {
        auto selected = list->selectedItems();
        if (selected.empty()) {
            createMessageBox("Error", "Please select a book to update");
            return;
        }
        auto selectedItem = selected.at(0);
        QVariantList dataList = selectedItem->data(Qt::UserRole).toList();
        string new_title = dataList.at(0).toString().toStdString();
        controller.update(new_title, title->text().toStdString(), author->text().toStdString(), genre->text().toStdString(), year->text().toInt());
        createMessageBox("Info", "Book updated!");
        loadData(controller.getAll());
    } catch(RepoError& e) {
        createMessageBox("Error", QString::fromStdString(e.getMessage()));
    }
}

void LibraryGUI::clickedExit() {
    close();
}

void LibraryGUI::clickedUndo() {
    try {
        controller.undo();
        createMessageBox("Info", "Last action undone");
        loadData(controller.getAll());
    } catch(RepoError& e) {
        createMessageBox("Error", QString::fromStdString(e.getMessage()));
    }
}

void LibraryGUI::cosmeticizeWindow() {
    setStyleSheet(
        "QListWidget {"
        "   border: 2.5px solid #6969ff;"
        "   background-color: #0d1a42;"
        "   font-size: 15px;"
        "   font-style: italic bold;"
        "}"
        "QListWidget::item {"
        "   color: #ffffff;"
        "   padding: 10px;"
        "   border: 1px solid #6969ff;"
        "}"
        "QFormLayout {"
        "   margin: 10px;"
        "   padding: 5px;"
        "}"
        "QLabel {"
        "   font-size: 20px;"
        "   color: #ffffff;"
        "}"
        "QLineEdit {"
        "   color: #2efff1;"
        "   background-color: #0d1a42;"
        "   font-size: 20px;"
        "   text-align: center;"
        "   border: 1px solid #6969ff;"
        "}"
        "QPushButton {"
        "   background-color: #4848ab;"
        "   color: white;"
        "   font-size: 15px;"
        "   padding: 5px 10px;"
        "   border-radius: 3px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3d3d91;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2c2c69;"
        "}"
    );
}

void LibraryGUI::createMessageBox(const QString& windowTitle, const QString& text) {
    QMessageBox msgBox;
    msgBox.setStyleSheet(
        "QMessageBox {"
        "   background-color: #2d2d2d;"
        "   color: white;"
        "   font-size: 15px;"
        "}"
        "QMessageBox QLabel {"
        "   color: white;"
        "}"
        "QMessageBox QPushButton {"
        "   background-color: #4848ab;"
        "   color: white;"
        "   border: none;"
        "   font-size: 15px;"
        "   padding: 5px 10px;"
        "   border-radius: 3px;"
        "}"
        "QMessageBox QPushButton:hover {"
        "   background-color: #3d3d91;"
        "}"
        "QMessageBox QPushButton:pressed {"
        "   background-color: #2c2c69;"
        "}"
    );
    msgBox.setWindowTitle(windowTitle);
    msgBox.setText(text);
    msgBox.exec();
}

/// Bookshelf CRUD
BookshelfGUI::BookshelfGUI(Controller &ctr) : controller { ctr } {
    initGUI();
    loadData(controller.b_getAll());
}

void BookshelfGUI::initGUI() {
    cosmeticizeWindow();

    mainLayout = new QHBoxLayout;
    setLayout(mainLayout);

    table = new QTableView;
    tableModel = new TableModel(controller.b_getAll());
    table->setModel(tableModel);
    mainLayout->addWidget(table);

    rhsLayout = new QVBoxLayout;
    mainLayout->addLayout(rhsLayout);

    formLayout = new QFormLayout;
    rhsLayout->addLayout(formLayout);
    title = new QLineEdit;
    number = new QLineEdit;
    filename = new QLineEdit;

    formLayout->addRow("Book you want to place", title);
    formLayout->addRow("Number of generated books", number);
    formLayout->addRow("File to export", filename);

    buttonLayout = new QHBoxLayout;
    rhsLayout->addLayout(buttonLayout);

    emptyButton = new QPushButton {"&Empty Bookshelf" };
    buttonLayout->addWidget(emptyButton);

    placeButton = new QPushButton {"&Place book" };
    buttonLayout->addWidget(placeButton);

    generateButton = new QPushButton {"&Generate books" };
    buttonLayout->addWidget(generateButton);

    exportButton = new QPushButton {"&Export" };
    buttonLayout->addWidget(exportButton);

    exitButton = new QPushButton {"&Exit" };
    buttonLayout->addWidget(exitButton);

    connectButtons();
}

void BookshelfGUI::loadData(const vector<Book>& repo) {
    tableModel->setBooks(repo);
}

void BookshelfGUI::cosmeticizeWindow() {
    setStyleSheet(
            "QTableWidget {"
            "   border: 2.5px solid #6969ff;"
            "   background-color: #0d1a42;"
            "   font-size: 15px;"
            "   font-style: italic bold;"
            "}"
            "QTableWidget::item {"
            "   color: #ffffff;"
            "   padding: 10px;"
            "   border: 1px solid #6969ff;"
            "}"
            "QFormLayout {"
            "   margin: 10px;"
            "   padding: 5px;"
            "}"
            "QLabel {"
            "   font-size: 15px;"
            "   color: #ffffff;"
            "}"
            "QLineEdit {"
            "   color: #2efff1;"
            "   background-color: #0d1a42;"
            "   font-size: 15px;"
            "   text-align: center;"
            "   border: 1px solid #6969ff;"
            "}"
            "QPushButton {"
            "   background-color: #4848ab;"
            "   color: white;"
            "   font-size: 15px;"
            "   padding: 5px 10px;"
            "   border-radius: 3px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #3d3d91;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #2c2c69;"
            "}"
    );
}

void BookshelfGUI::connectButtons() {
    controller.addObserver(this);
    QObject::connect(emptyButton,&QPushButton::clicked,this,&BookshelfGUI::clickedEmpty);
    QObject::connect(placeButton,&QPushButton::clicked,this,&BookshelfGUI::clickedPlace);
    QObject::connect(generateButton,&QPushButton::clicked,this,&BookshelfGUI::clickedGenerate);
    QObject::connect(exportButton,&QPushButton::clicked,this,&BookshelfGUI::clickedExport);
    QObject::connect(exitButton,&QPushButton::clicked,this,&BookshelfGUI::clickedExit);
}

void BookshelfGUI::clickedEmpty() {
    try {
        controller.b_emptyShelf();
        loadData(controller.b_getAll());
    } catch(RepoError& e) {
        createMessageBox("Error", QString::fromStdString(e.getMessage()));
    }
}

void BookshelfGUI::clickedPlace() {
    try {
        controller.b_placeBook(title->text().toStdString());
        loadData(controller.b_getAll());
    } catch(RepoError& e) {
        createMessageBox("Error", QString::fromStdString(e.getMessage()));
    }
}

void BookshelfGUI::clickedGenerate() {
    string string_nr = number->text().toStdString();
    bool isInteger = true;
    if (string_nr.empty() || ((!isdigit(string_nr[0])) && (string_nr[0] != '+')))
        isInteger = false;
    char* p;
    strtol(string_nr.c_str(), &p, 10);
    if (*p != 0)
        isInteger = false;
    if (!isInteger) {
        createMessageBox("Error", "Number field must be an integer");
        return;
    }
    controller.b_generateShelf(number->text().toInt());
    loadData(controller.b_getAll());
}

void BookshelfGUI::clickedExport() {
    string file = filename->text().toStdString();
    if (file.empty()) {
        createMessageBox("Error", "Filename invalid");
        return;
    }
    controller.b_export(file);
    createMessageBox("Info", "Bookshelf exported");
}

void BookshelfGUI::clickedExit() {
    close();
}

void BookshelfGUI::createMessageBox(const QString& windowTitle, const QString& text) {
    QMessageBox msgBox;
    msgBox.setStyleSheet(
            "QMessageBox {"
            "   background-color: #2d2d2d;"
            "   color: white;"
            "   font-size: 15px;"
            "}"
            "QMessageBox QLabel {"
            "   color: white;"
            "}"
            "QMessageBox QPushButton {"
            "   background-color: #4848ab;"
            "   color: white;"
            "   border: none;"
            "   font-size: 15px;"
            "   padding: 5px 10px;"
            "   border-radius: 3px;"
            "}"
            "QMessageBox QPushButton:hover {"
            "   background-color: #3d3d91;"
            "}"
            "QMessageBox QPushButton:pressed {"
            "   background-color: #2c2c69;"
            "}"
    );
    msgBox.setWindowTitle(windowTitle);
    msgBox.setText(text);
    msgBox.exec();
}

void BookshelfGUI::update() {
    loadData(controller.b_getAll());
}

/// Bookshelf read-only
BookshelfReadOnly::BookshelfReadOnly(Controller& ctr) : controller { ctr } {
    controller.addObserver(this);
}

void BookshelfReadOnly::paintEvent(QPaintEvent* ev) {
    QPainter painter { this };
    for (int i = 0; i < controller.b_size(); i++) {
        int x = QRandomGenerator::global()->bounded(width());
        int y = QRandomGenerator::global()->bounded(height());
        drawShape(&painter, x, y, 50);
    }
}

void BookshelfReadOnly::drawShape(QPainter* painter, int x, int y, int size) {
    painter->setPen(QColor("#0d1a42"));
    painter->setBrush(QColor("#6969ff"));

    painter->drawEllipse(x, y, size, size);

    painter->setBrush(QColor("#0d1a42"));
    painter->drawEllipse(x + size / 5, y + size / 3, size / 10, size / 10);
    painter->drawEllipse(x + (3 * size / 5), y + size / 3, size / 10, size / 10);

    QPainterPath mouthPath;
    mouthPath.moveTo(x + size / 5, y + 2 * size / 3);
    mouthPath.arcTo(x + size / 5, y + size / 2, size / 2, size / 4, 0, -180);
    painter->drawPath(mouthPath);
}

void BookshelfReadOnly::update() {
    repaint();
}