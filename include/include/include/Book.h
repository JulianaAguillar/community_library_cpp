#pragma once
#include <string>
#include <iostream>

class Book {
public:
    std::string title;
    std::string author;
    std::string isbn;
    bool available; // true = Available, false = Borrowed

    Book() : available(true) {}

    Book(const std::string& t,
         const std::string& a,
         const std::string& i,
         bool avail)
        : title(t), author(a), isbn(i), available(avail) {}

    void display() const {
        std::cout << "Title: " << title << "\n"
                  << "Author: " << author << "\n"
                  << "ISBN: " << isbn << "\n"
                  << "Availability: " << (available ? "Available" : "Borrowed") << "\n";
    }

    bool borrowBook() {
        if (!available) return false;   // already borrowed
        available = false;              // mark as borrowed
        return true;
    }
};
