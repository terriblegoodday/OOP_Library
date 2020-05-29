//
//  main.cpp
//  Library
//
//  Created by Eduard Dzhumagaliev on 5/24/20.
//  Copyright © 2020 Eduard Dzhumagaliev. All rights reserved.
//

#include <iostream>
#include "Library.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    Library library;
    library.loadFilesFromStorage();
    
    cout << library << endl;
    
    auto filteredBooks = library.filterByAuthor("James Joyce");
    cout << filteredBooks;
    auto considerPhlebas = library.filterByAuthor("Iain M. Banks").andBook("Consider Phlebas");
    cout << considerPhlebas;
    
    cout << considerPhlebas.orBook("Excession");
    
    auto librarian = Librarian(&library);
    
    librarian.start();
    
    return 0;
}
