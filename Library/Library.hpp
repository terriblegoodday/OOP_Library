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
    
    Book(string title, string author): title(title), author(author) {};
    
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
    
    Filter filterByBook(string book);
    Filter filterByAuthor(string author);
    
    friend ostream & operator<<(ostream & destination, const Library & source);
};

#endif /* Library_hpp */
