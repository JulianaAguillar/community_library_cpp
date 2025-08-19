// tests/test_app.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>

#if __has_include("include/Book.h")
  #include "include/Book.h"
#elif __has_include("Book.h")
  #include "Book.h"
#elif __has_include("include/include/Book.h")
  #include "include/include/Book.h"
#else
  #error "Book.h not found. Adjust include path in tests/test_app.cpp."
#endif

// ---------- string helpers ----------
static std::string toLower(std::string s) {
    for (char& ch : s) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return s;
}

static bool looksNumeric(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

static bool isValidIsbn13(const std::string& s) { return looksNumeric(s) && s.size() == 13; }

static bool isValidDate(const std::string& ymd) {
    if (ymd.size() != 10 || ymd[4] != '-' || ymd[7] != '-') return false;
    int y = std::stoi(ymd.substr(0,4));
    int m = std::stoi(ymd.substr(5,2));
    int d = std::stoi(ymd.substr(8,2));
    if (y < 1900 || y > 2100) return false;
    static const int mdays[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (m < 1 || m > 12) return false;
    int dm = mdays[m];
    bool leap = (y%4==0 && (y%100!=0 || y%400==0));
    if (m==2 && leap) dm = 29;
    return d >= 1 && d <= dm;
}

// ---------- map getters here if your names differ ----------
static std::string getTitle(const Book& b)    { return b.getTitle(); }
static std::string getAuthor(const Book& b)   { return b.getAuthor(); }
static std::string getIsbn(const Book& b)     { return b.getIsbn(); }
// Availability varies per implementation; for logging only:
static std::string getAvailability(const Book& /*b*/) { return "<n/a>"; }
static std::string getDateAdded(const Book& b) { return b.getDateAdded(); }

// ---------- comparator: title ascending, case-insensitive ----------
static bool byTitleAsc(const Book& a, const Book& b) {
    return toLower(getTitle(a)) < toLower(getTitle(b));
}

// ---------- factory: uses constructor OR setters (-DUSE_SETTERS=1) ----------
static Book makeBookOrThrow(
    const std::string& title,
    const std::string& author,
    const std::string& isbn,
    const std::string& availability,
    const std::string& dateAdd
) {
    if (title.empty()) throw std::invalid_argument("title is empty");
    if (!isValidIsbn13(isbn)) throw std::invalid_argument("isbn invalid (non-numeric or wrong length)");
    if (!isValidDate(dateAdd)) throw std::invalid_argument("dateAdd invalid (YYYY-MM-DD)");

#ifndef USE_SETTERS
    // expected constructor: Book(title, author, isbn, availability, dateAdd)
    return Book(title, author, isbn, availability, dateAdd);
#else
    // fallback if you only have a default ctor + setters (adjust names if needed)
    Book b;
    b.setTitle(title);
    b.setAuthor(author);
    b.setIsbn(isbn);
    b.setAvailability(availability);
    b.setDateAdded(dateAdd);
    return b;
#endif
}

static void printBook(const Book& b, const std::string& prefix="[OK] ") {
    std::cout << prefix
              << "\"" << getTitle(b) << "\" | "
              << getAuthor(b) << " | "
              << getIsbn(b) << " | "
              << getAvailability(b) << " | "
              << getDateAdded(b) << "\n";
}

static void showList(const std::vector<Book>& v, const std::string& header) {
    std::cout << header << "\n";
    int i = 1;
    for (const auto& b : v) {
        std::cout << i++ << ") " << getTitle(b) << "\n";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "=== Instantiation: valid data ===\n";
    std::vector<Book> valid;
    try {
        valid.push_back(makeBookOrThrow(
            "Clean Code","Robert C. Martin","9780132350884","Borrowed","2024-01-10"));
        printBook(valid.back());
        valid.push_back(makeBookOrThrow(
            "The Pragmatic Programmer","Andrew Hunt, David Thomas","9780201616224","Available","2023-11-20"));
        printBook(valid.back());
        valid.push_back(makeBookOrThrow(
            "Design Patterns","Erich Gamma et al.","9780201633610","Available","2022-05-01"));
        printBook(valid.back());
    } catch (const std::exception& ex) {
        std::cerr << "[FAIL] Unexpected exception creating valid books: " << ex.what() << "\n";
        return 1;
    }

    std::cout << "\n=== Instantiation: invalid data (expect errors) ===\n";
    auto tryInvalid = [](const std::string& t,const std::string& a,const std::string& i,const std::string& av,const std::string& d){
        try {
            auto b = makeBookOrThrow(t,a,i,av,d);
            printBook(b, "[UNEXPECTED OK] ");
        } catch (const std::exception& ex) {
            std::cout << "[ERROR] " << ex.what() << "\n";
        }
    };
    tryInvalid("","Unknown","abc","Available","2025-02-30");
    tryInvalid("Bad ISBN Length","Test","123","Borrowed","2024-12-12");
    tryInvalid("Negative Availability (if your model uses int)","Test","9789999999999","-1","2024-09-09");

    // arrays for sorting (use only valid books)
    std::vector<Book> arrAsc   = { valid[0], valid[2], valid[1] }; // Clean Code, Design Patterns, Pragmatic
    std::vector<Book> arrDesc  = { valid[1], valid[2], valid[0] }; // Pragmatic, Design Patterns, Clean Code
    std::vector<Book> arrMixed = { valid[2], valid[1], valid[0] }; // Design Patterns, Pragmatic, Clean Code

    showList(arrAsc,  "\nBefore sort (ASC added):");
    showList(arrDesc, "Before sort (DESC added):");
    showList(arrMixed,"Before sort (MIXED added):");

    std::sort(arrAsc.begin(),  arrAsc.end(),  byTitleAsc);
    std::sort(arrDesc.begin(), arrDesc.end(), byTitleAsc);
    std::sort(arrMixed.begin(),arrMixed.end(),byTitleAsc);

    showList(arrAsc,  "After sort (ASC -> ascending):");
    showList(arrDesc, "After sort (DESC -> ascending):");
    showList(arrMixed,"After sort (MIXED -> ascending):");

    std::cout << "=== Done ===\n";
    return 0;
}
