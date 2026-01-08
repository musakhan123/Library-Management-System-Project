#include "LibraryManager.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>

bool isValidStudentID(int id)
{
    return id >= 10000 && id <= 99999;
}


// Constructor - Initialize all data structures
LibraryManager::LibraryManager() : nextTransactionID(1) {
    bookCatalog = new BST();
    students = new LinkedList();
    requestQueue = new Queue();
    transactionHistory = new Stack();
}

// Destructor - Clean up all data structures
LibraryManager::~LibraryManager() {
    delete bookCatalog;
    delete students;
    delete requestQueue;
    delete transactionHistory;
}

/**
 * GetCurrentDate - Helper to get current date in YYYY-MM-DD format
 */
std::string LibraryManager::getCurrentDate() const {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    std::stringstream ss;
    ss << (1900 + ltm->tm_year) << "-" 
       << std::setfill('0') << std::setw(2) << (1 + ltm->tm_mon) << "-"
       << std::setfill('0') << std::setw(2) << ltm->tm_mday;
    
    return ss.str();
}

/**
 * FindStudent - Helper to find student (used internally)
 */
Student* LibraryManager::findStudent(int studentID) {
    return students->find(studentID);
}

// ============== BOOK MANAGEMENT ==============

/**
 * AddBook - Add a new book to the catalog
 * Returns false if book with same ID already exists
 */
bool LibraryManager::addBook(int id,
                             const std::string& title,
                             const std::string& author,
                             int copies)

{
		   if (id <= 0 || copies <= 0)
		{
		    lastError = "Invalid book ID or copies";
		    return false;
		}
		
		if (searchBookByID(id) != nullptr)
		{
		    lastError = "Book ID already exists";
		    return false;
		}


    Book newBook(id, title, author, copies);
    getBookList()->insert(newBook);

    return true;
}


/**
 * SearchBook - Find a book by ID
 * Returns nullptr if not found
 */
Book* LibraryManager::searchBook(int bookID) {
    return bookCatalog->search(bookID);
}

/**
 * DisplayAllBooks - Show all books in sorted order
 */
void LibraryManager::displayAllBooks() const {
    bookCatalog->displayInOrder();
}

// ============== STUDENT MANAGEMENT ==============

/**
 * RegisterStudent - Add a new student
 * Returns false if student with same ID already exists
 */
bool LibraryManager::registerStudent(int studentID,
                                     const std::string& name,
                                     const std::string& department)
{
    // 🔴 FORMAT CHECK
		if (!isValidStudentID(studentID))
	{
	    lastError = "Student ID must be 5 digits (10000–99999)";
	    return false;
	}
	
	if (students->find(studentID) != nullptr)
	{
	    lastError = "Student ID already exists";
	    return false;
	}
	

    Student newStudent(studentID, name, department);
	students->append(newStudent);

	lastError.clear(); 
	return true;

}


/**
 * SearchStudent - Find a student by ID
 * Returns nullptr if not found
 */
Student* LibraryManager::searchStudent(int studentID) {
    return students->find(studentID);
}

/**
 * DisplayAllStudents - Show all registered students
 */
void LibraryManager::displayAllStudents() const {
    students->display();
}

// ============== BORROW/RETURN OPERATIONS ==============

/**
 * BorrowBook - Student borrows a book
 * Returns false if operation fails
 */
bool LibraryManager::borrowBook(int studentID, int bookID)
{
	Student* student = findStudent(studentID);
	if (!student)
	{
    lastError = "Student not found";
    return false;
    
}

    // ❌ Borrow limit check (MAX 3)
if (student->getBorrowedCount() >= 3) {
    std::cout << "Error: Borrow limit reached (Max 3 books)." << std::endl;
    return false;
}


    Book* book = searchBook(bookID);
	if (!book)
	{
    lastError = "Book not found";
    return false;
}


    // 🔴 ALREADY BORROWED
    if (student->hasBorrowedBook(bookID))
	{
    lastError = "Student already borrowed this book";
    return false;
	}


    // 🔴 MAX LIMIT (feature 4)
    if (student->getBorrowedCount() >= 3)
        return false;

    // 🔴 NO COPIES AVAILABLE
    if (book->getAvailableCopies() <= 0)
	{
    requestQueue->enqueue(studentID);
    lastError = "Book unavailable. Student added to request queue";
    return false;
	}


    // ✅ Safe borrow
    book->borrowCopy();
    student->addBorrowedBook(bookID);

    Transaction t(nextTransactionID++, studentID, bookID,
                  TransactionType::BORROW, getCurrentDate());

    transactionHistory->push(t);
    saveTransactionsToCSV("data/transactions.csv");

    return true;
}


/**
 * ReturnBook - Student returns a book
 * Returns false if operation fails
 */
bool LibraryManager::returnBook(int studentID, int bookID) {
    // Find student
    Student* student = findStudent(studentID);
	   if (!student)
	{
	    lastError = "Student not found";
	    return false;
	}


    
    // Find book
    Book* book = searchBookByID(bookID);

	    if (!book)
	{
	    lastError = "Book not found";
	    return false;
	}
	
    
    // Check if student has this book
	  if (!student->hasBorrowedBook(bookID))
	{
	    lastError = "Student has not borrowed this book";
	    return false;
	}

    // Return the book
    book->returnCopy();
    student->removeBorrowedBook(bookID);
    
    // Create transaction record
    Transaction t(nextTransactionID++, studentID, bookID, 
              TransactionType::RETURN, getCurrentDate());

transactionHistory->push(t);
saveTransactionsToCSV("data/transactions.csv");   // ? ADD THIS

std::cout << "Book returned successfully!" << std::endl;
return true;

}

// ============== REQUEST QUEUE MANAGEMENT ==============

/**
 * AddToRequestQueue - Add student to waiting list
 */
void LibraryManager::addToRequestQueue(int studentID) {
    requestQueue->enqueue(studentID);
    std::cout << "Student " << studentID << " added to request queue." << std::endl;
}

/**
 * ProcessNextRequest - Process the next student in queue
 * Returns -1 if queue is empty
 */
int LibraryManager::processNextRequest() {
    if (requestQueue->isEmpty()) {
        std::cout << "Request queue is empty." << std::endl;
        return -1;
    }
    
    int studentID = requestQueue->dequeue();
    std::cout << "Processing request for Student ID: " << studentID << std::endl;
    return studentID;
}

/**
 * DisplayRequestQueue - Show all waiting students
 */
void LibraryManager::displayRequestQueue() const {
    requestQueue->display();
}

// ============== TRANSACTION HISTORY ==============

/**
 * DisplayTransactionHistory - Show all transactions
 */
void LibraryManager::displayTransactionHistory() const {
    transactionHistory->display();
}

/**
 * DisplayRecentTransactions - Show last N transactions
 */
void LibraryManager::displayRecentTransactions(int count) const {
    if (transactionHistory->isEmpty()) {
        std::cout << "No transactions yet." << std::endl;
        return;
    }
    
    std::cout << "\n=== Recent Transactions (Last " << count << ") ===" << std::endl;
    
    // This is a simplified version - in real implementation,
    // you'd need to peek without popping
    Stack tempStack;
    int displayed = 0;
    
    // Copy transactions to temp stack
    while (!transactionHistory->isEmpty() && displayed < count) {
        Transaction t = transactionHistory->pop();
        t.display();
        tempStack.push(t);
        displayed++;
    }
    
    // Restore original stack
    while (!tempStack.isEmpty()) {
        transactionHistory->push(tempStack.pop());
    }
}
Stack* LibraryManager::getTransactionHistory() const
{
    return transactionHistory;
}

/*bool LibraryManager::saveToFile(const std::string& filename) {
    std::cout << "Save to file: " << filename << " (Not implemented yet)" << std::endl;
    return false;
}

bool LibraryManager::loadFromFile(const std::string& filename) {
    std::cout << "Load from file: " << filename << " (Not implemented yet)" << std::endl;
    return false;
}*/
//new add
BST* LibraryManager::getBookList() const
{
    return bookCatalog;
}

LinkedList* LibraryManager::getStudentsList() const
{
    return students;
}
//new add / ================= LOAD BOOKS =================
bool LibraryManager::loadBooksFromCSV(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open " << filename << std::endl;
        return false;
    }

    std::string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, title, author, copies;

        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, copies, ',');

        addBook(
            std::stoi(id),
            title,
            author,
            std::stoi(copies)
        );
    }

    file.close();
    return true;
}

// ================= LOAD STUDENTS =================
bool LibraryManager::loadStudentsFromCSV(const std::string& filename)
{
	delete students;
	students = new LinkedList();

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open " << filename << std::endl;
        return false;
    }

    std::string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, name, dept;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, dept, ',');

        int sid = std::stoi(id);
if (students->find(sid) == nullptr) {
    students->append(Student(sid, name, dept));
}

    }

    file.close();
    return true;
}

// ================= SAVE BOOKS =================
bool LibraryManager::saveBooksToCSV(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "bookID,title,author,totalCopies\n";

    bookCatalog->inOrderForEach([&](Book* b) {
        file << b->getBookID() << ","
             << b->getTitle() << ","
             << b->getAuthor() << ","
             << b->getTotalCopies() << "\n";
    });

    file.close();
    return true;
}

// ================= SAVE STUDENTS =================
bool LibraryManager::saveStudentsToCSV(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "studentID,name,department\n";

    students->forEach([&](void* data) {
        Student* s = static_cast<Student*>(data);
        file << s->getStudentID() << ","
             << s->getName() << ","
             << s->getDepartment() << "\n";
    });

    file.close();
    return true;
}
bool LibraryManager::loadTransactionsFromCSV(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    std::string line;
    getline(file, line); // skip header

    while (getline(file, line))
    {
        std::stringstream ss(line);
        std::string tid, sid, bid, type, date;

        getline(ss, tid, ',');
        getline(ss, sid, ',');
        getline(ss, bid, ',');
        getline(ss, type, ',');
        getline(ss, date, ',');

        TransactionType t =
            (type == "BORROW") ? TransactionType::BORROW : TransactionType::RETURN;

        Transaction tr(
            std::stoi(tid),
            std::stoi(sid),
            std::stoi(bid),
            t,
            date
        );

        transactionHistory->push(tr);

        // keep transaction IDs consistent
        nextTransactionID = std::max(nextTransactionID, std::stoi(tid) + 1);
    }

    file.close();
    return true;
}


//seacrh / ID search already exists
Book* LibraryManager::searchBookByID(int bookID)
{
    return searchBook(bookID);
}

// TITLE SEARCH
std::vector<Book*> LibraryManager::searchBookByTitle(const std::string& title)
{
    std::vector<Book*> results;

    bookCatalog->inOrderForEach([&](Book* b) {
        if (b->getTitle().find(title) != std::string::npos)
            results.push_back(b);
    });

    return results;
}

// AUTHOR SEARCH
std::vector<Book*> LibraryManager::searchBookByAuthor(const std::string& author)
{
    std::vector<Book*> results;

    bookCatalog->inOrderForEach([&](Book* b) {
        if (b->getAuthor().find(author) != std::string::npos)
            results.push_back(b);
    });

    return results;
}
//UNDO 
	bool LibraryManager::undoLastTransaction()
{
    if (transactionHistory->isEmpty())
    {
        std::cout << "No transaction to undo." << std::endl;
        return false;
    }

    // Get last transaction
    Transaction last = transactionHistory->pop();

    int studentID = last.getStudentID();
    int bookID = last.getBookID();

    Student* student = findStudent(studentID);
    Book* book = searchBook(bookID);

    if (!student || !book)
    {
        std::cout << "Undo failed: invalid data." << std::endl;
        return false;
    }

    // Reverse the transaction
    if (last.getType() == TransactionType::BORROW)
    {
        // Undo borrow ? return book
        book->returnCopy();
        student->removeBorrowedBook(bookID);
    }
    else // RETURN
    {
        // Undo return ? borrow again
        if (!book->borrowCopy())
        {
            std::cout << "Undo failed: book unavailable." << std::endl;
            return false;
        }
        student->addBorrowedBook(bookID);
    }

    std::cout << "Last transaction undone successfully." << std::endl;
	saveTransactionsToCSV("data/transactions.csv");   
		return true;

}
bool LibraryManager::saveTransactionsToCSV(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "transactionID,studentID,bookID,type,date\n";

    Stack temp;
    while (!transactionHistory->isEmpty())
    {
        Transaction t = transactionHistory->pop();
        temp.push(t);

        file << t.getTransactionID() << ","
             << t.getStudentID() << ","
             << t.getBookID() << ","
             << t.getTypeString() << ","
             << t.getDate() << "\n";
    }

    // restore stack
    while (!temp.isEmpty())
        transactionHistory->push(temp.pop());

    file.close();
    return true;
}
Queue* LibraryManager::getRequestQueue() const
{
    return requestQueue;
}

Student* LibraryManager::getStudentByID(int studentID)
{
    return findStudent(studentID); // calls private helper
}
void LibraryManager::rebuildBorrowedLists()
{
     // 1️⃣ Reset all books to full availability
   getBookList()->inOrderForEach([](Book* b) {
    b->resetAvailable();
});


    // 2️⃣ Clear borrowed books for every student
   getStudentsList()->forEach([](void* data) {
    Student* s = static_cast<Student*>(data);
    s->clearBorrowedBooks();
});

    // 3. Replay all transactions
    Stack temp;
    while (!transactionHistory->isEmpty())
    {
        Transaction t = transactionHistory->pop();
        temp.push(t);

        Student* s = getStudentByID(t.getStudentID());
        Book* b = searchBookByID(t.getBookID());

        if (!s || !b) continue;

        if (t.getType() == TransactionType::BORROW)
        {
            s->addBorrowedBook(b->getBookID());
            b->decreaseAvailable();
        }
        else if (t.getType() == TransactionType::RETURN)
        {
            s->removeBorrowedBook(b->getBookID());
            b->increaseAvailable();
        }
    }

    // 4. Restore stack
    while (!temp.isEmpty())
        transactionHistory->push(temp.pop());
        
    
}

std::string LibraryManager::getLastError() const
{
    return lastError;
}



