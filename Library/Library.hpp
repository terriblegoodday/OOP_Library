//
//  Library.hpp
//  Library
//
//  Created by Eduard Dzhumagaliev on 5/24/20.
//  Copyright © 2020 Eduard Dzhumagaliev. All rights reserved.
//

#ifndef Library_hpp
#define Library_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>

using namespace std;

struct Book {
    string title;
    string author;
    
    Book(string title, string author);
    
    friend ostream & operator<<(ostream & destination, const Book & source);
    
    // this is needed to return set of books in filter method
    friend bool operator<(const Book & left, const Book & right);
};

class Library {
    friend class Filter;
    
private:
    vector<Book> books;
    
    template <class BookCollection>
    ostream & prepareOstream(ostream & destination, const BookCollection & source) const;
public:
    class Filter {
    private:
        Library * source;
        set<Book> filteredBooks;
        
        ostream & prepareOstream(ostream & destination) const;
        
    public:
        Filter( Library * source);
        
        set<Book> getResults();
        
        Filter & orBook(string book);
        Filter & orAuthor(string author);
        Filter & andBook(string book);
        Filter & andAuthor(string author);
        
        friend ostream & operator<<(ostream & destination, const Filter & source);
    };
    
    void loadFilesFromStorage();
    
    Filter getEmptyFilter();
    Filter filterByBook(string book);
    Filter filterByAuthor(string author);
    
    friend ostream & operator<<(ostream & destination, const Library & source);
};

class Librarian {
private:
    enum Query {
        all = 0,
        book = 1,
        author = 2
    };
    
    enum Knot {
        andK = 1,
        orK = 2,
        exit = 0
    };
    
    Library::Filter currentFilter;
    Library * source;
    
    void queryOr();
    void queryAnd();

    string inputQuery();
    void queryOrBook();
    void queryOrAuthor();
    void queryAndBook();
    void queryAndAuthor();
    void queryAll();
    
    void askForKnot();
    
    void printGoodBye();
    
public:
    Librarian(Library * source);
    
    void start();
};

#endif /* Library_hpp */
