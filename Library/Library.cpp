//
//  Library.cpp
//  Library
//
//  Created by Eduard Dzhumagaliev on 5/24/20.
//  Copyright © 2020 Eduard Dzhumagaliev. All rights reserved.
//

#include "Library.hpp"

void Library::loadFilesFromStorage() {
    ifstream inputFile;
    inputFile.open("books.txt");
    
    string currentLine;
    while (getline(inputFile, currentLine)) {
        auto firstDelimiterOccurence = currentLine.find("-");
        string title = currentLine.substr(0, firstDelimiterOccurence - 1);
        string author = currentLine.substr(firstDelimiterOccurence + 2);
        Book book(title, author);
        books.push_back(book);
    }
}

ostream & operator<<(ostream & destination, const Book & source) {
    destination << source.title << " by " << source.author;
    
    return destination;
}

template <class BookCollection>
ostream & Library::prepareOstream(ostream & destination, const BookCollection & source) const {
    for (const Book & book: source) {
        destination << book << endl;
    }
    return destination;
}

ostream & operator<<(ostream & destination, const Library & source) {
    return source.prepareOstream(destination, source.books);
}

ostream & Library::Filter::prepareOstream(ostream & destination) const {
    return source->prepareOstream(destination, filteredBooks);
}

ostream & operator<<(ostream & destination, const Library::Filter & filter) {
    destination << "Filtered results below" << endl;
    return filter.prepareOstream(destination);
}

bool operator<(const Book & left, const Book & right) {
    return (left.title + left.author < right.title + right.author);
}

Library::Filter::Filter(Library * source) {
    this->source = source;
}

Library::Filter Library::filterByBook(string book) {
    Library::Filter filter(this);
    return filter.orBook(book);
}

Library::Filter Library::filterByAuthor(string author) {
    Library::Filter filter(this);
    return filter.orAuthor(author);
}

set<Book> Library::Filter::getResults() {
    return filteredBooks;
}

Library::Filter & Library::Filter::orBook(string book) {
    copy_if(source->books.begin(), source->books.end(), inserter(filteredBooks, filteredBooks.end()), [book](const Book & currentBook) {
        return (currentBook.title == book);
    });
    return *this;
}

Library::Filter & Library::Filter::orAuthor(string author) {
    copy_if(source->books.begin(), source->books.end(), inserter(filteredBooks, filteredBooks.end()), [author](const Book & currentBook) {
        return (currentBook.author == author);
    });
    return *this;
}

Library::Filter & Library::Filter::andBook(string book) {
    Filter filterByTitle(this->source);
    
    set<Book> booksByTitle = filterByTitle.orBook(book).getResults();
    set<Book> filterIntersection;
   
    set_intersection(filteredBooks.begin(), filteredBooks.end(), booksByTitle.begin(), booksByTitle.end(), inserter(filterIntersection, filterIntersection.end()));
    
    filteredBooks = filterIntersection;
    
    return *this;
}

Library::Filter & Library::Filter::andAuthor(string author) {
    Filter filterByAuthor(this->source);
    
    set<Book> booksByAuthor = filterByAuthor.orAuthor(author).getResults();
    set<Book> filterIntersection;
    
    set_intersection(filteredBooks.begin(), filteredBooks.end(), booksByAuthor.begin(), booksByAuthor.end(), inserter(filterIntersection, filterIntersection.end()));
    
    filteredBooks = filterIntersection;
    
    return *this;
}

Library::Filter Library::getEmptyFilter() {
    Filter filter(this);
    return filter;
}

Librarian::Librarian(Library * source): currentFilter(source->getEmptyFilter()) {
    this->source = source;
}

void Librarian::queryAll() {
    cout << "Querying all books" << endl;
    cout << *source << endl;
}

void Librarian::askForKnot() {
    cout << "Now choose whether you want to continue building your query" << endl;
    cout << "Available knots: and " << Knot::andK << ", or " << Knot::orK << "; You can exit by typing " << Knot::exit << endl;
    int query;
    cin >> query;
    auto knot = Knot(query);
    switch (knot) {
        case andK:
            queryAnd();
            break;
        case orK:
            queryOr();
            break;
        default:
            printGoodBye();
            break;
    }
}

void Librarian::queryOr() {
    cout << "book title " << Query::book << " / author " << Query::author << " / exit " << Query::all << "?" << endl;
    int query;
    cin >> query;
    auto type = Query(query);
    switch (type) {
        case book:
            queryOrBook();
            break;
        case author:
            queryOrAuthor();
            break;
        default:
            printGoodBye();
            break;
    }
}

void Librarian::queryAnd() {
    cout << "book title " << Query::book << " / author " << Query::author << " / exit " << Query::all << "?" << endl;
    int query;
    cin >> query;
    auto type = Query(query);
    switch (type) {
        case book:
            queryAndBook();
            break;
        case author:
            queryAndAuthor();
            break;
        default:
            printGoodBye();
            break;
    }
}

string Librarian::inputQuery() {
    string query;
    cin.ignore();
    getline(cin, query);
    return query;
}

void Librarian::queryOrBook() {
    cout << "Please input your book title" << endl;
    auto query = inputQuery();
    cout << currentFilter.orBook(query);
    askForKnot();
}

void Librarian::queryOrAuthor() {
    cout << "Please input your book author" << endl;
    auto query = inputQuery();
    cout << currentFilter.orAuthor(query);
    askForKnot();
}

void Librarian::queryAndBook() {
    cout << "Please input your book title" << endl;
    auto query = inputQuery();
    cout << currentFilter.andBook(query);
    askForKnot();
}

void Librarian::queryAndAuthor() {
    cout << "Please input your book author" << endl;
    auto query = inputQuery();
    cout << currentFilter.andAuthor(query);
    askForKnot();
}

void Librarian::start() {
    cout << "Hi Library User! How would you like your query to be formed?" << endl;
    cout << "Available commands:" << endl;
    cout << Query::all << ": query all" << endl;
    cout << Query::book << ": query books by title" << endl;
    cout << Query::author << ": query books by author" << endl;
    int query;
    cin >> query;
    auto type = Query(query);
    
    switch (type) {
        case all:
            queryAll();
            break;
        case book:
            queryOrBook();
            break;
        case author:
            queryOrAuthor();
            break;
        default:
            printGoodBye();
            break;
    }
}

void Librarian::printGoodBye() {
    cout << "Thanks bye!" << endl;
}
