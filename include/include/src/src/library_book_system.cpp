#include <iostream>
#include <vector>
#include <string>
#include "Book.h"

int main() {
    std::cout.setf(std::ios::boolalpha);

    // Seed books to match your Phase 2 transcript:
    // - "Design Patterns" is already borrowed -> first attempt should print "Error: Book is already borrowed."
    // - "Clean Code" is available -> second attempt succeeds and prints the book details with "Availability: Borrowed"
    // - "The Pragmatic Programmer" is available (not used in sample IO but present for completeness)
    std::vector<Book> books = {
        Book("Clean Code", "Robert C. Martin", "9780132350884", /*available*/ true),
        Book("The Pragmatic Programmer", "Andrew Hunt, David Thomas", "9780201616224", /*available*/ true),
        Book("Design Patterns", "Erich Gamma et al.", "9780201633610", /*available*/ false) // already borrowed
    };

    auto findByIsbn = [&](const std::string& key) -> Book* {
        for (auto& b : books) if (b.isbn == key) return &b;
        return nullptr;
    };

    std::cout << "Library ready. Enter ISBN to borrow (0 to exit):\n";

    while (true) {
        std::cout << "> ISBN: ";
        std::string in;
        if (!std::getline(std::cin, in)) break;

        if (in == "0") {
            std::cout << "Program terminated.\n";
            break;
        }
        if (in.empty()) continue;

        Book* b = findByIsbn(in);
        if (!b) {
            std::cout << "Error: ISBN not found.\n";
            continue;
        }

        if (!b->borrowBook()) {
            std::cout << "Error: Book is already borrowed.\n";
        } else {
            std::cout << "Success: Book borrowed.\n";
            b->display();
        }
    }

    return 0;
}
