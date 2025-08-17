#pragma once
#include <string>
#include <iostream>

class Book {
public:
    std::string title;
    std::string author;
    std::string isbn;
    bool available{true};

    // Sets all book details
    void setBookDetails(const std::string& title_,
                        const std::string& isbn_,
                        const std::string& author_,
                        bool available_);

    // Displays a single book (one-line summary)
    void displayBookDetails() const;

    // Borrow/return operations
    bool borrowBook();   // returns true if borrowed successfully
    void returnBook();   // marks as available
};
