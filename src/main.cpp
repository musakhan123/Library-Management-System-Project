#include "LibraryManager.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"


#include "imgui_impl_opengl3.h"
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

// =====================================================
// GLOBAL STATE
// =====================================================
LibraryManager library;
//Font
static ImFont* statusFont = nullptr;

// ---------- Book ----------
static int bookId = 0;
static int bookCopies = 1;
static char bookTitle[128] = "";
static char bookAuthor[128] = "";
static char bookISBN[64] = "";

// ---------- Student ----------
static int studentId = 0;
static char studentName[128] = "";
static char studentDept[128] = "";

// ---------- Student Search ----------
static int searchStudentID = 0;
static Student* foundStudent = nullptr;

// ---------- Book Search ----------
static int searchMode = 0;
static int searchBookID = 0;
static char searchText[128] = "";
static vector<Book*> searchResults;
static bool showSearchResults = false;

// ---------- Issue / Return ----------
static int issueStudentID = 0;
static int issueBookID = 0;
static int returnStudentID = 0;
static int returnBookID = 0;

// ---------- UI Message ----------
static char statusMsg[256] = "";
static ImVec4 statusColor = ImVec4(0.2f, 0.8f, 0.3f, 1.0f);

static bool isFullscreen = false;
static int windowedX = 100, windowedY = 100;
static int windowedW = 1400, windowedH = 900;
	static char issueError[256] = "";


// ---------- Display Toggles ----------
static bool showAllBooks = false;
static bool showAllStudents = false;

// =====================================================
// THEME COLORS
// =====================================================
namespace Theme {
    const ImVec4 Primary = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);      // Blue
    const ImVec4 PrimaryHover = ImVec4(0.36f, 0.69f, 1.0f, 1.0f);
    const ImVec4 Success = ImVec4(0.2f, 0.8f, 0.3f, 1.0f);         // Green
    const ImVec4 Danger = ImVec4(0.9f, 0.3f, 0.3f, 1.0f);          // Red
    const ImVec4 Warning = ImVec4(0.95f, 0.75f, 0.15f, 1.0f);      // Yellow
    const ImVec4 CardBg = ImVec4(0.12f, 0.13f, 0.15f, 0.95f);      // Dark card
    const ImVec4 HeaderBg = ImVec4(0.16f, 0.17f, 0.19f, 1.0f);     // Darker header
    const ImVec4 TextDim = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);         // Dim text
    const ImVec4 Border = ImVec4(0.25f, 0.27f, 0.3f, 1.0f);        // Subtle border
}

void ApplyCustomStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Spacing & Rounding
    style.WindowPadding = ImVec2(20, 20);
    style.FramePadding = ImVec2(12, 6);
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 12.0f;
    
    // Rounding
    style.WindowRounding = 8.0f;
    style.ChildRounding = 6.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 6.0f;
    
    // Colors
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.09f, 0.11f, 1.0f);
    colors[ImGuiCol_ChildBg] = Theme::CardBg;
    colors[ImGuiCol_Border] = Theme::Border;
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.17f, 0.19f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.22f, 0.24f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.26f, 0.28f, 1.0f);
    colors[ImGuiCol_TitleBg] = Theme::HeaderBg;
    colors[ImGuiCol_TitleBgActive] = Theme::HeaderBg;
    colors[ImGuiCol_Button] = Theme::Primary;
    colors[ImGuiCol_ButtonHovered] = Theme::PrimaryHover;
    colors[ImGuiCol_ButtonActive] = ImVec4(0.16f, 0.49f, 0.88f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.22f, 0.24f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.28f, 0.30f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.32f, 0.34f, 1.0f);
    colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.17f, 0.19f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.28f, 0.30f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.22f, 0.24f, 1.0f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.18f, 0.20f, 0.22f, 1.0f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.25f, 0.27f, 0.30f, 1.0f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.22f, 0.24f, 1.0f);
}

void ToggleFullscreen(GLFWwindow* window)
{
    isFullscreen = !isFullscreen;

    if (isFullscreen)
    {
        glfwGetWindowPos(window, &windowedX, &windowedY);
        glfwGetWindowSize(window, &windowedW, &windowedH);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        // Borderless fullscreen (NOT exclusive)
        glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
        glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
        glfwSetWindowMonitor(window, nullptr, 0, 0, mode->width, mode->height, 0);
    }
    else
    {
        glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
        glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_TRUE);
        glfwSetWindowMonitor(
            window,
            nullptr,
            windowedX, windowedY,
            windowedW, windowedH,
            0
        );
    }
}


// =====================================================
// HELPERS
// =====================================================
void ShowStatus(const char* msg, ImVec4 color)
{
    strncpy(statusMsg, msg, 255);
    statusColor = color;
}

// Card-style container helper
void BeginCard(const char* label, float height = 0) {
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, Theme::CardBg);
    ImGui::BeginChild(label, ImVec2(0, height), true, ImGuiWindowFlags_None);
    ImGui::PopStyleColor();
}

void EndCard() {
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

// Section header
void SectionHeader(const char* text)
{
    ImGui::TextColored(Theme::Primary, "%s", text);
    ImGui::Spacing();
}


// Centered button
bool CenteredButton(const char* label, const ImVec2& size) {
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - size.x) * 0.5f;
    if (off > 0.0f) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
    return ImGui::Button(label, size);
}

// =====================================================
// STATISTICS
// =====================================================
Book* GetMostPopularBook(int& count)
{
    count = 0;
    unordered_map<int, int> freq;
    Stack* history = library.getTransactionHistory();
    Stack temp;

    while (!history->isEmpty())
    {
        Transaction t = history->pop();
        temp.push(t);
        if (t.getType() == TransactionType::BORROW)
            freq[t.getBookID()]++;
    }
    while (!temp.isEmpty()) history->push(temp.pop());

    Book* best = nullptr;
    for (auto& p : freq)
        if (p.second > count)
        {
            count = p.second;
            best = library.searchBookByID(p.first);
        }
    return best;
}

void ShowTransactionHistory()
{
    Stack* history = library.getTransactionHistory();
    if (!history || history->isEmpty())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Theme::TextDim);
        ImGui::Text("No transactions recorded yet.");
        ImGui::PopStyleColor();
        return;
    }

    BeginCard("TxnHistoryCard", 280);
    SectionHeader("Recent Transaction History");

    if (ImGui::BeginTable(
            "TransactionsTable",
            5,
            ImGuiTableFlags_Borders |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollY))
    {
        ImGui::TableSetupColumn("Txn ID", ImGuiTableColumnFlags_WidthFixed, 70);
        ImGui::TableSetupColumn("Student ID", ImGuiTableColumnFlags_WidthFixed, 90);
        ImGui::TableSetupColumn("Book ID", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Date");
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        Stack temp;
        while (!history->isEmpty())
        {
            Transaction t = history->pop();
            temp.push(t);

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%d", t.getTransactionID());
            ImGui::TableNextColumn(); ImGui::Text("%d", t.getStudentID());
            ImGui::TableNextColumn(); ImGui::Text("%d", t.getBookID());

            ImGui::TableNextColumn();
            if (t.getType() == TransactionType::BORROW)
                ImGui::TextColored(Theme::Primary, "BORROW");
            else
                ImGui::TextColored(Theme::Success, "RETURN");

            ImGui::TableNextColumn();
            ImGui::Text("%s", t.getDate().c_str());
        }

        while (!temp.isEmpty())
            history->push(temp.pop());

        ImGui::EndTable();
    }

    EndCard();
}

void ShowCurrentlyBorrowedBooks(Student* s)
{
    bool any = false;

    if (ImGui::BeginTable(
        "CurrentBorrowTable",
        4,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY))
    {
        ImGui::TableSetupColumn("Book ID", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Title");
        ImGui::TableSetupColumn("Author");
        ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 100);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        library.getBookList()->inOrderForEach([&](Book* b)
        {
            if (s->hasBorrowedBook(b->getBookID()))
            {
                any = true;

                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text("%d", b->getBookID());
                ImGui::TableNextColumn(); ImGui::Text("%s", b->getTitle().c_str());
                ImGui::TableNextColumn(); ImGui::Text("%s", b->getAuthor().c_str());
                ImGui::TableNextColumn(); 
                ImGui::TextColored(Theme::Warning, "Borrowed");
            }
        });

        ImGui::EndTable();
    }

    if (!any) {
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, Theme::TextDim);
        ImGui::Text("No books currently borrowed");
        ImGui::PopStyleColor();
    }
}
void ShowBorrowError(int studentID, int bookID)
{
    Student* s = library.getStudentByID(studentID);
    Book* b = library.searchBookByID(bookID);

    if (!s)
    {
        ShowStatus("Invalid Student ID", Theme::Danger);
        return;
    }

    if (!b)
    {
        ShowStatus("Invalid Book ID", Theme::Danger);
        return;
    }

    if (s->getBorrowedCount() >= 3)
    {
        ShowStatus("Borrow limit reached (Max 3 books)", Theme::Warning);
        return;
    }

    if (s->hasBorrowedBook(bookID))
    {
        ShowStatus("Student has already borrowed this book", Theme::Warning);
        return;
    }

    if (b->getAvailableCopies() <= 0)
    {
        ShowStatus("Book unavailable. Added to request queue", Theme::Warning);
        return;
    }

    ShowStatus("Issue failed due to unknown error", Theme::Danger);
}


// =====================================================
// GUI
// =====================================================
void DrawGUI()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin(
        "LibraryManagementSystem",
        nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_MenuBar
    ); 
    ImGui::PopStyleVar();

    // ================= MENU BAR =================
    if (ImGui::BeginMenuBar())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, Theme::Primary);
        ImGui::Text(" Library Management System");
        ImGui::PopStyleColor();
        
        ImGui::Spacing();
        ImGui::Spacing();
        
        if (ImGui::MenuItem(" Toggle Fullscreen"))
            ToggleFullscreen(glfwGetCurrentContext());

        ImGui::EndMenuBar();
    }

    ImGui::Spacing();
    
    // Content area with padding
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 15));
ImGui::BeginChild("ContentArea", ImVec2(0, -50), false, ImGuiWindowFlags_None);


    if (ImGui::BeginTabBar("MainTabs", ImGuiTabBarFlags_None))
    {
        // =================================================
        // 1️⃣ BOOK MANAGEMENT
        // =================================================
        if (ImGui::BeginTabItem(" Book Management"))
        {
            ImGui::Spacing();
            
            // === ADD BOOK CARD ===
            BeginCard("AddBookCard", 290);
            SectionHeader("Add New Book");
            
            ImGui::Columns(2, nullptr, false);
            
            ImGui::SetColumnWidth(0, 200);
            ImGui::AlignTextToFramePadding();
			ImGui::Text("Book ID");

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Title");

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Author");

			ImGui::AlignTextToFramePadding();
			ImGui::Text("ISBN");
			
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Quantity");

            
            ImGui::NextColumn();
            
            ImGui::PushItemWidth(-1);
            ImGui::InputInt("##BookID", &bookId);
            ImGui::InputText("##Title", bookTitle, 128);
            ImGui::InputText("##Author", bookAuthor, 128);
            ImGui::InputText("##ISBN", bookISBN, 64);
            ImGui::InputInt("##Quantity", &bookCopies);
            ImGui::PopItemWidth();
            
            ImGui::Columns(1);
            
            ImGui::Spacing();
			ImGui::SetCursorPosX(0);

            if (CenteredButton("Add Book", ImVec2(200, 35)))
            {
                if (library.addBook(bookId, bookTitle, bookAuthor, bookCopies))
                {
                    library.saveBooksToCSV("data/books.csv");
                    ShowStatus(" Book added successfully", Theme::Success);
                }
                else
                    ShowStatus(" Book ID already exists", Theme::Danger);
            }
    

            EndCard();
            
            ImGui::Spacing();
            
            // === SEARCH CARD ===
            BeginCard("SearchBookCard", 180);
            SectionHeader("Search Books");
            
            ImGui::RadioButton("By ID", &searchMode, 0); 
            ImGui::SameLine(150);
            ImGui::RadioButton("By Title", &searchMode, 1); 
            ImGui::SameLine(300);
            ImGui::RadioButton("By Author", &searchMode, 2);
            
            ImGui::Spacing();
            
            ImGui::PushItemWidth(400);
            if (searchMode == 0)
                ImGui::InputInt("##SearchID", &searchBookID);
            else
                ImGui::InputText("##SearchText", searchText, 128);
            ImGui::PopItemWidth();
            
            ImGui::SameLine();
            if (ImGui::Button("Search", ImVec2(120, 0)))
            {
                searchResults.clear();
                showSearchResults = true;
            
                if (searchMode == 0)
                {
                    Book* b = library.searchBookByID(searchBookID);
                    if (b)
                    {
                        searchResults.push_back(b);
                        ShowStatus(" Book found", Theme::Success);
                    }
                    else
                    {
                        ShowStatus(" Book ID not found", Theme::Danger);
                    }
                }
                else if (searchMode == 1)
                {
                    searchResults = library.searchBookByTitle(searchText);
                    if (searchResults.empty())
                        ShowStatus(" No book found with this title", Theme::Danger);
                    else
                        ShowStatus(" Books found", Theme::Success);
                }
                else
                {
                    searchResults = library.searchBookByAuthor(searchText);
                    if (searchResults.empty())
                        ShowStatus(" No book found by this author", Theme::Danger);
                    else
                        ShowStatus(" Books found", Theme::Success);
                }
            }
            
            EndCard();
            
            ImGui::Spacing();
            
            // === DISPLAY BOOKS ===
            if (ImGui::Checkbox("Show All Books", &showAllBooks))
			{
   				 if (showAllBooks)
        		showSearchResults = false;   // ✅ RESET SEARCH MODE
}

            ImGui::Spacing();

            if (showAllBooks || showSearchResults) 
{
                BeginCard("BooksTableCard", 0);
                SectionHeader("Book Collection");

                if (ImGui::BeginTable("BooksTable", 5, 
                    ImGuiTableFlags_Borders | 
                    ImGuiTableFlags_RowBg | 
                    ImGuiTableFlags_ScrollY |
                    ImGuiTableFlags_Resizable))
                {
                    ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 70);
                    ImGui::TableSetupColumn("Title");
                    ImGui::TableSetupColumn("Author");
                    ImGui::TableSetupColumn("Available", ImGuiTableColumnFlags_WidthFixed, 90);
                    ImGui::TableSetupColumn("Total", ImGuiTableColumnFlags_WidthFixed, 70);
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableHeadersRow();

                    auto renderBook = [&](Book* b)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn(); ImGui::Text("%d", b->getBookID());
                        ImGui::TableNextColumn(); ImGui::Text("%s", b->getTitle().c_str());
                        ImGui::TableNextColumn(); ImGui::Text("%s", b->getAuthor().c_str());
                        ImGui::TableNextColumn(); 
                        if (b->getAvailableCopies() > 0)
                            ImGui::TextColored(Theme::Success, "%d", b->getAvailableCopies());
                        else
                            ImGui::TextColored(Theme::Danger, "%d", b->getAvailableCopies());
                        ImGui::TableNextColumn(); ImGui::Text("%d", b->getTotalCopies());
                    };

                    if (showSearchResults && !searchResults.empty())
					{	
    					for (auto* b : searchResults)
        				renderBook(b);
					}
else
{
    library.getBookList()->inOrderForEach(renderBook);
}


                    ImGui::EndTable();
                }
                
                EndCard();
            }

            ImGui::EndTabItem();
        }

        // =================================================
        // 2️⃣ STUDENT MANAGEMENT
        // =================================================
        if (ImGui::BeginTabItem(" Student Management"))
        {
            ImGui::Spacing();
            
            // === REGISTER STUDENT CARD ===
            BeginCard("RegisterCard", 200);
            SectionHeader("Register New Student");
            
            ImGui::Columns(2, nullptr, false);
            ImGui::SetColumnWidth(0, 200);
            
            ImGui::AlignTextToFramePadding();
			ImGui::Text("Student ID");
			ImGui::AlignTextToFramePadding();
            ImGui::Text("Name");
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Department");
            
            ImGui::NextColumn();
            
            ImGui::PushItemWidth(-1);
            ImGui::InputInt("##StudentID", &studentId);
            ImGui::InputText("##Name", studentName, 128);
            ImGui::InputText("##Department", studentDept, 128);
            ImGui::PopItemWidth();
            
            ImGui::Columns(1);
            
            ImGui::Spacing();
            if (CenteredButton("Register Student", ImVec2(200, 35)))
            {
                if (library.registerStudent(studentId, studentName, studentDept))
                {
                    library.saveStudentsToCSV("data/students.csv");
                           showAllStudents = true;     // force student table to show
        		foundStudent = nullptr; 
                    ShowStatus(" Student registered successfully", Theme::Success);
                } 
                else
                {
                    ShowStatus(library.getLastError().c_str(), Theme::Danger);

                }
            }
            
            EndCard();
            
            ImGui::Spacing();
            
            // === SEARCH STUDENT CARD ===
			BeginCard("SearchStudentCard", 140);
			SectionHeader("Search Student");
			
			ImGui::Columns(2, nullptr, false);
			ImGui::SetColumnWidth(0, 200);
			
			// Label
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Student ID");
			
			ImGui::NextColumn();
			
			// Input + Button
			ImGui::PushItemWidth(250);
			ImGui::InputInt("##SearchStud", &searchStudentID);
			ImGui::PopItemWidth();
			
			ImGui::SameLine();
			
			if (ImGui::Button("Search", ImVec2(120, 0)))
			{
			    foundStudent = library.getStudentByID(searchStudentID);
			
			    if (foundStudent)
			        ShowStatus("Student found", Theme::Success);
			    else
			        ShowStatus(library.getLastError().c_str(), Theme::Danger);
			}
			
			ImGui::Columns(1);
			
			EndCard();
            
            ImGui::Spacing();
            
            // === STUDENT INFO ===
            if (foundStudent)
            {
                BeginCard("StudentInfoCard", 0);
                SectionHeader("Student Information");
                
                ImGui::Text("ID: %d", foundStudent->getStudentID());
                ImGui::Text("Name: %s", foundStudent->getName().c_str());
                ImGui::Text("Department: %s", foundStudent->getDepartment().c_str());
                
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                
                SectionHeader("Currently Borrowed Books");
                ShowCurrentlyBorrowedBooks(foundStudent);
                
                EndCard();
                
                ImGui::Spacing();
            }

            // === ALL STUDENTS ===
            ImGui::Checkbox("Show All Students", &showAllStudents);
            ImGui::Spacing();

            if (showAllStudents)
            {
                BeginCard("AllStudentsCard", 0);
                SectionHeader("All Registered Students");

                if (ImGui::BeginTable(
                    "StudentsTable",
                    3,
                    ImGuiTableFlags_Borders |
                    ImGuiTableFlags_RowBg |
                    ImGuiTableFlags_Resizable |
                    ImGuiTableFlags_ScrollY))
                {
                    ImGui::TableSetupColumn("Student ID", ImGuiTableColumnFlags_WidthFixed, 100);
                    ImGui::TableSetupColumn("Name");
                    ImGui::TableSetupColumn("Department");
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableHeadersRow();

                    library.getStudentsList()->forEach([](void* data)
                    {
                        Student* s = static_cast<Student*>(data);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn(); ImGui::Text("%d", s->getStudentID());
                        ImGui::TableNextColumn(); ImGui::Text("%s", s->getName().c_str());
                        ImGui::TableNextColumn(); ImGui::Text("%s", s->getDepartment().c_str());
                    });

                    ImGui::EndTable();
                }
                
                EndCard();
            }

            ImGui::EndTabItem();
        }

        // =================================================
        // 3️⃣ ISSUE / RETURN
        // =================================================
        if (ImGui::BeginTabItem(" Issue / Return"))
        {
            ImGui::Spacing();
            
            ImGui::Columns(2, nullptr, false);
            
            // === ISSUE BOOK ===
            BeginCard("IssueCard", 220);
            SectionHeader("Issue Book");
            
            ImGui::Text("Student ID");
            ImGui::PushItemWidth(-1);
            ImGui::InputInt("##IssueStudentID", &issueStudentID);
            ImGui::PopItemWidth();
            
            ImGui::Spacing();
            
            ImGui::Text("Book ID");
            ImGui::PushItemWidth(-1);
            ImGui::InputInt("##IssueBookID", &issueBookID);
            ImGui::PopItemWidth();
            
            ImGui::Spacing();
            ImGui::Spacing();
            
           if (CenteredButton("Issue Book", ImVec2(180, 40)))
		{
		    if (library.borrowBook(issueStudentID, issueBookID))
		    {
		        ShowStatus("Book issued successfully", Theme::Success);
		    }
		    else
		    {
		        ShowBorrowError(issueStudentID, issueBookID);
		    }
		}

            
            EndCard();
            
            ImGui::NextColumn();
            
            // === RETURN BOOK ===
            BeginCard("ReturnCard", 220);
            SectionHeader("Return Book");
            
            ImGui::Text("Student ID");
            ImGui::PushItemWidth(-1);
            ImGui::InputInt("##ReturnStudentID", &returnStudentID);
            ImGui::PopItemWidth();
            
            ImGui::Spacing();
            
            ImGui::Text("Book ID");
            ImGui::PushItemWidth(-1);
            ImGui::InputInt("##ReturnBookID", &returnBookID);
            ImGui::PopItemWidth();
            
            ImGui::Spacing();
            ImGui::Spacing();
            
            if (CenteredButton("Return Book", ImVec2(180, 40)))
            {
                if (library.returnBook(returnStudentID, returnBookID))
                    ShowStatus("Book returned successfully", Theme::Success);
                    else
    				ShowStatus(library.getLastError().c_str(), Theme::Danger);

            }
            
            EndCard();
            
            ImGui::Columns(1);

            ImGui::EndTabItem();
        }

        // =================================================
        // 4️⃣ REQUEST QUEUE
        // =================================================
        if (ImGui::BeginTabItem("Request Queue"))
        {
            ImGui::Spacing();
            
            BeginCard("QueueCard", 0);
            SectionHeader("Pending Requests");
            
            Queue* q = library.getRequestQueue();
            int pos = 1;
            Queue temp;

            ImGui::BeginChild("QueueTableArea", ImVec2(0, 250), true);

				if (ImGui::BeginTable("Queue", 2,
				    ImGuiTableFlags_Borders |
				    ImGuiTableFlags_RowBg |
				    ImGuiTableFlags_ScrollY))
				{
				    ImGui::TableSetupColumn("Position", ImGuiTableColumnFlags_WidthFixed, 100);
				    ImGui::TableSetupColumn("Student ID");
				    ImGui::TableSetupScrollFreeze(0, 1);
				    ImGui::TableHeadersRow();
				
				    while (!q->isEmpty())
				    {
				        int id = q->dequeue();
				        temp.enqueue(id);
				
				        ImGui::TableNextRow();
				        ImGui::TableNextColumn(); ImGui::Text("%d", pos++);
				        ImGui::TableNextColumn(); ImGui::Text("%d", id);
				    }
				
				    while (!temp.isEmpty())
				        q->enqueue(temp.dequeue());
				
				    ImGui::EndTable();
				}
				
				ImGui::EndChild();

            
            ImGui::Spacing();
            
            if (CenteredButton("Process Next Request", ImVec2(220, 40)))
                library.processNextRequest();
            
            EndCard();

            ImGui::EndTabItem();
        }

        // =================================================
        // 5️⃣ HISTORY & STATISTICS
        // =================================================
        if (ImGui::BeginTabItem("History & Statistics"))
        {
            ImGui::Spacing();
            
            // === TRANSACTION HISTORY ===
            ShowTransactionHistory();
            
            ImGui::Spacing();
            
            // === UNDO BUTTON ===
           // === UNDO BUTTON ===
BeginCard("UndoCard", 100);
ImGui::Spacing();

if (CenteredButton(" Undo Last Transaction", ImVec2(240, 40)))
{
    if (library.undoLastTransaction())
    {
        library.saveBooksToCSV("data/books.csv");
        ShowStatus(" Last transaction undone successfully", Theme::Success);
    }
    else
    {
        ShowStatus(" No transaction to undo", Theme::Danger);
    }
}

EndCard();


            ImGui::Spacing();

            // === STATISTICS CARD ===
            BeginCard("StatsCard", 0);
            SectionHeader("Library Statistics");

            int total = 0, available = 0;
            library.getBookList()->inOrderForEach([&](Book* b)
            {
                total += b->getTotalCopies();
                available += b->getAvailableCopies();
            });

            ImGui::Text(" Total Books: %d", total);
            ImGui::Text(" Available Books: %d", available);
            ImGui::Text(" Borrowed Books: %d", total - available);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            SectionHeader("Most Popular Book");

            int count = 0;
            Book* popular = GetMostPopularBook(count);

            if (popular)
            {
                ImGui::Text("Title: %s", popular->getTitle().c_str());
                ImGui::Text("Author: %s", popular->getAuthor().c_str());
                ImGui::Text("Times Borrowed: %d", count);
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Text, Theme::TextDim);
                ImGui::Text("No borrow transactions yet.");
                ImGui::PopStyleColor();
            }

            EndCard();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();

    // ================= STATUS BAR =================
    ImGui::Separator();
    if (strlen(statusMsg) > 0)
    {
        if (strlen(statusMsg) > 0)
{
    ImGui::TextColored(statusColor, "%s", statusMsg);
}

    }

    ImGui::End();
}
// =====================================================
// MAIN
// =====================================================
int main()
{

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


    GLFWwindow* window = glfwCreateWindow(1400, 900, "Library", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault(); // normal font

statusFont = io.Fonts->AddFontFromFileTTF(
    "C:/Windows/Fonts/segoeuib.ttf", // bold font
    18.0f
);



    ApplyCustomStyle();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    library.loadBooksFromCSV("data/books.csv");
    library.loadStudentsFromCSV("data/students.csv");
    library.loadTransactionsFromCSV("data/transactions.csv");
    library.rebuildBorrowedLists();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DrawGUI();

        ImGui::Render();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);

        glClearColor(0.08f, 0.09f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
