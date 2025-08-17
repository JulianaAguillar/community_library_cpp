#include <iostream>
#include <string>
#include <algorithm>
#include "Book.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

void showMenu();
void showAllBooks();
void borrowBooks();
void returnBooks();
void sortBooks();
void addNewBooks();

// Increase if you want to store more books
static const int kCapacity = 300;
Book arrayBook[kCapacity];
int totalBooks = 0; // how many valid books do we currently have?

// Seed with a few books to make demo easier
void seedInitialBooks() {
    Book b1; b1.setBookDetails("Clean Code", "9780132350884", "Robert C. Martin", false);
    Book b2; b2.setBookDetails("The Pragmatic Programmer", "9780201616224", "Andrew Hunt, David Thomas", true);
    Book b3; b3.setBookDetails("Design Patterns", "9780201633610", "Erich Gamma et al.", true);

    arrayBook[0] = b1;
    arrayBook[1] = b2;
    arrayBook[2] = b3;
    totalBooks = 3;
}

int main() {
    cout << std::boolalpha; // show "true/false" instead of 1/0
    seedInitialBooks();

    int option = 0;
    while (true) {
        showMenu();
        cout << "Select an option (1-6): ";
        if (!(cin >> option)) {
            cout << "Invalid input. Exiting." << endl;
            return 0;
        }
        if (option == 1) {
            showAllBooks();
        } else if (option == 2) {
            borrowBooks();
        } else if (option == 3) {
            returnBooks();
        } else if (option == 4) {
            sortBooks();
        } else if (option == 5) {
            addNewBooks();
        } else if (option == 6) {
            cout << "BYE! Hope to see you again!" << endl;
            break;
        } else {
            cout << "Invalid Option, please select 1..6" << endl;
        }
    }
    return 0;
}

void showMenu() {
    cout << "\n========================================\n";
    cout << "=             LIBRARY MENU             =\n";
    cout << "========================================\n";
    cout << "= 1: Show all books                    =\n";
    cout << "= 2: Borrow Books                      =\n";
    cout << "= 3: Return a Book                     =\n";
    cout << "= 4: Sort Books                        =\n";
    cout << "= 5: Add New Books                     =\n";
    cout << "= 6: QUIT                              =\n";
    cout << "========================================\n\n";
}

void showAllBooks() {
    cout << "========================================\n";
    cout << "=          Displaying all Books        =\n";
    cout << "========================================\n";
    int shown = 0;
    for (int i = 0; i < totalBooks; ++i) {
        if (!arrayBook[i].isbn.empty() && !arrayBook[i].title.empty()) {
            arrayBook[i].displayBookDetails();
            shown++;
        }
    }
    if (shown == 0) cout << "==> There are NO Books registered yet!\n";
    cout << endl;
}

void borrowBooks() {
    cout << "========================================\n";
    cout << "=             Borrowing Books          =\n";
    cout << "========================================\n";
    string isbnToBorrow;
    cout << "==> Input the book's ISBN to borrow (0 to quit): ";
    while (cin >> isbnToBorrow && isbnToBorrow != "0") {
        bool found = false;
        for (int i = 0; i < totalBooks; ++i) {
            if (arrayBook[i].isbn == isbnToBorrow) {
                found = true;
                if (arrayBook[i].borrowBook()) {
                    cout << "==> You borrowed: " << arrayBook[i].title << endl;
                } else {
                    cout << "==> The book is unavailable (already borrowed)\n";
                }
                break;
            }
        }
        if (!found) cout << "==> Book NOT FOUND\n";
        cout << "==> Input another ISBN (0 to quit): ";
    }
    cout << endl;
}

void returnBooks() {
    cout << "========================================\n";
    cout << "=            Returning Books           =\n";
    cout << "========================================\n";
    string isbnToReturn;
    cout << "==> Input the ISBN of the book to return (0 to quit): ";
    while (cin >> isbnToReturn && isbnToReturn != "0") {
        bool found = false;
        for (int i = 0; i < totalBooks; ++i) {
            if (arrayBook[i].isbn == isbnToReturn) {
                found = true;
                arrayBook[i].returnBook();
                cout << "==> Returned: " << arrayBook[i].title << endl;
                break;
            }
        }
        if (!found) cout << "==> Book NOT FOUND\n";
        cout << "==> Input another ISBN (0 to quit): ";
    }
    cout << endl;
}

void sortBooks() {
    cout << "========================================\n";
    cout << "=             Sorting Books            =\n";
    cout << "========================================\n";
    // Simple alphabetical sort by title (ascending)
    for (int i = 0; i < totalBooks - 1; ++i) {
        for (int j = i + 1; j < totalBooks; ++j) {
            if (arrayBook[i].title > arrayBook[j].title) {
                std::swap(arrayBook[i], arrayBook[j]);
            }
        }
    }
    cout << "==> The books have been sorted! Select 1 to display them.\n\n";
}

void addNewBooks() {
    cout << "========================================\n";
    cout << "=            Adding New Books          =\n";
    cout << "========================================\n";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear line

    char again = 'Y';
    while ((again == 'Y' || again == 'y') && totalBooks < kCapacity) {
        string title, isbn, author;

        cout << "==> Book's title: ";
        std::getline(cin, title);

        cout << "==> Book's ISBN: ";
        std::getline(cin, isbn);

        cout << "==> Book's author: ";
        std::getline(cin, author);

        if (title.empty() || isbn.empty()) {
            cout << "==> Title and ISBN are required. Book NOT added.\n";
        } else {
            Book b; b.setBookDetails(title, isbn, author, true);
            arrayBook[totalBooks++] = b;
            cout << "==> Book added: " << b.title << endl;
        }

        cout << "==> Add another book (Y/N)? ";
        cin >> again;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear trailing newline
    }
    cout << endl;
}
