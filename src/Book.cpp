#include "Book.h"

// NOTE: fixed member shadowing bug: we use different parameter names and assign to members via this->
void Book::setBookDetails(const std::string& title_,
                          const std::string& isbn_,
                          const std::string& author_,
                          bool available_) {
    this->title = title_;
    this->isbn = isbn_;
    this->author = author_;
    this->available = available_;
}

void Book::displayBookDetails() const {
    std::cout << "isbn: "  << isbn
              << ", title: " << title
              << ", author: " << author
              << ", available: " << std::boolalpha << available
              << std::endl;
}

void Book::returnBook() {
    available = true;
}

bool Book::borrowBook() {
    if (!available) {
        return false; // already borrowed
    }
    available = false;
    return true;
}
