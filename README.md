# 📚 Library Management System (C++ | Data Structures)

A **Digital Library Management System** developed as a **Data Structures Final Lab Project**.  
The system demonstrates the practical use of core data structures such as **Binary Search Trees, Linked Lists, Queues, and Stacks**, combined with a **graphical user interface (GUI)** built using **Dear ImGui**.

---

## 👥 Team Members

- Abia Saeed  
- Shayan Ijaz  
- Noora Imran  
- Raheel Ahmad  
- Hammad Khalil  
- Muhammad Musa (Team Lead)  
- Malak Saad Bin Rashid  
- Muhammad Zikria Shah  
- Muhammad Rahim Ullah  
- Muhammad Shameer Qureshi  

**Instructor:** Usama Afridi  
**Course:** Data Structures (Lab Project)

---

## 🎯 Project Objectives

- Implement real-world library operations using fundamental data structures  
- Efficiently manage books, students, and transactions  
- Provide a user-friendly graphical interface  
- Demonstrate understanding of data structure performance and design  

---

## 🏗️ System Architecture

The system is built around a central **`LibraryManager`** class that coordinates all operations:

| Component | Data Structure Used |
|---------|---------------------|
| Book Catalog | Binary Search Tree (BST) |
| Student Records | Linked List |
| Book Requests | Queue (FIFO) |
| Transaction History | Stack (LIFO) |
| Data Persistence | CSV File Handling |
| User Interface | Dear ImGui |

---

## ✨ Features

### 📖 Book Management
- Add new books
- Search books by ID, title, or author
- View all books in sorted order (BST inorder traversal)

### 👨‍🎓 Student Management
- Register students
- Search student records
- View currently borrowed books

### 🔄 Issue & Return
- Borrow and return books
- Borrowing limit enforcement
- Automatic availability updates

### ⏳ Request Queue
- Students are queued when books are unavailable
- FIFO-based fair request handling

### 📜 Transaction History
- Borrow/return records stored in a stack
- Undo last transaction feature
- View recent transactions

### 💾 File Handling
- Books, students, and transactions saved in CSV files
- Data automatically loaded on startup

---

## 🖥️ Graphical User Interface

The GUI is developed using **Dear ImGui** and includes:

- Book Management Tab  
- Student Management Tab  
- Issue/Return Tab  
- Request Queue Tab  
- Transaction History & Statistics Tab  

All GUI actions interact **only through the backend (`LibraryManager`)**, ensuring clean separation of concerns.

---

## 🛠️ Technologies Used

- **Language:** C++
- **GUI:** Dear ImGui + GLFW + OpenGL
- **Data Structures:** BST, Linked List, Queue, Stack
- **File Handling:** CSV (fstream)
- **Documentation:** LaTeX
- **AI Assistance:** ChatGPT, Claude AI (with permission)

---

## 🧠 AI Tools Usage

- **ChatGPT**
  - Designing class structures (`.h` / `.cpp`)
  - Implementing data structures and logic
  - Writing LaTeX documentation content

- **Claude AI**
  - Assisting with GUI layout and Dear ImGui code
  - Generating LaTeX diagrams and tables

All AI-generated content was **reviewed, modified, tested, and validated** by the team.

---

## ⚠️ Challenges Faced

- Team coordination across multiple members  
- Limited experience with GitHub and version control  
- Manual code merging due to lack of repository usage  
- AI usage limits (non-premium accounts)  

These were resolved using meetings, manual integration, and careful testing.

---
## Build (Windows)

Run:
```bat
build.bat



