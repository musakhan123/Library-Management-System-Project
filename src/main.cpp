#include "LibraryManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>

// ============================================================================
// PROFESSIONAL STYLING
// ============================================================================

void SetupProfessionalStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.WindowRounding = 0.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 6.0f;
    
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(12, 8);
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.ScrollbarSize = 14.0f;
    
    ImVec4* colors = style.Colors;
    
    colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
    colors[ImGuiCol_Border] = ImVec4(0.85f, 0.87f, 0.89f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.98f, 0.98f, 0.99f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.94f, 0.96f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.26f, 0.59f, 0.98f, 0.75f);
    colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.31f, 0.64f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.21f, 0.54f, 0.93f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.55f);
    colors[ImGuiCol_Text] = ImVec4(0.15f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.90f, 0.92f, 0.94f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_TabActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.90f, 0.92f, 0.94f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.85f, 0.87f, 0.89f, 1.00f);
}

void DrawStatCard(const char* title, const char* value, const char* subtitle, ImVec4 bgColor, ImVec4 accentColor, float width = 240.0f) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
    
    ImGui::BeginChild(title, ImVec2(width, 140), true, ImGuiWindowFlags_NoScrollbar);
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    draw_list->AddRectFilled(ImVec2(p.x - 15, p.y - 15), ImVec2(p.x - 9, p.y + 155), ImGui::ColorConvertFloat4ToU32(accentColor));
    
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.9f));
    ImGui::Text("%s", title);
    ImGui::PopStyleColor();
    
    ImGui::Spacing();
    ImGui::Spacing();
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::SetWindowFontScale(2.8f);
    ImGui::Text("%s", value);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();
    
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.85f));
    ImGui::TextWrapped("%s", subtitle);
    ImGui::PopStyleColor();
    
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void DrawStatusBadge(const char* text, ImVec4 bgColor, ImVec4 textColor) {
    ImGui::PushStyleColor(ImGuiCol_Button, bgColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, bgColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, bgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::Button(text, ImVec2(95, 0));
    ImGui::PopStyleColor(4);
}

// ============================================================================
// APPLICATION STATE
// ============================================================================

enum class Page {
    Dashboard,
    BookManagement,
    StudentManagement,
    IssueReturn,
    RequestQueue,
    History
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "Library Management System - Professional Edition", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    SetupProfessionalStyle();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    LibraryManager library;
    library.loadBooksFromCSV("data/books.csv");
    library.loadStudentsFromCSV("data/students.csv");
    library.loadTransactionsFromCSV("data/transactions.csv");
    library.rebuildBorrowedLists();

    Page currentPage = Page::Dashboard;
    
    char bookID[64] = "";
    char bookTitle[128] = "";
    char bookAuthor[128] = "";
    char bookCopies[32] = "";
    
    char studentID[64] = "";
    char studentName[128] = "";
    char studentDept[128] = "";
    
    char searchBookID[64] = "";
    char searchText[128] = "";
    int searchMode = 0;
    std::vector<Book*> searchResults;
    bool showSearchResults = false;
    bool showAllBooks = false;
    
    char searchStudentID[64] = "";
    Student* foundStudent = nullptr;
    bool showAllStudents = false;
    bool showStudentSearch = false;
    
    char issueStudentID[64] = "";
    char issueBookID[64] = "";
    char returnStudentID[64] = "";
    char returnBookID[64] = "";
    
    char statusMsg[256] = "";
    ImVec4 statusColor = ImVec4(0.2f, 0.8f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        // HEADER BAR
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)display_w, 85));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("Header", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
        
        ImGui::SetCursorPosY(15);
        ImGui::Indent(25);
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
        ImGui::Button("BOOK", ImVec2(55, 55));
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        ImGui::SetCursorPosY(20);
        ImGui::BeginGroup();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::SetWindowFontScale(1.4f);
        ImGui::Text("City Central Library");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::SetWindowFontScale(0.9f);
        ImGui::Text("Digital Management System");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();
        ImGui::EndGroup();
        
        ImGui::SameLine(ImGui::GetWindowWidth() - 280);
        ImGui::SetCursorPosY(15);
        ImGui::BeginGroup();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.9f));
        ImGui::Text("Logged in as");
        ImGui::PopStyleColor();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::SetWindowFontScale(1.1f);
        ImGui::Text("Librarian");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();
        ImGui::EndGroup();
        
        ImGui::SameLine();
        ImGui::SetCursorPosY(15);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
        ImGui::Button("LB", ImVec2(55, 55));
        ImGui::PopStyleColor(2);
        
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        // SIDEBAR
        ImGui::SetNextWindowPos(ImVec2(0, 85));
        ImGui::SetNextWindowSize(ImVec2(280, (float)(display_h - 85)));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.97f, 0.98f, 0.99f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("Sidebar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
        
        ImGui::Dummy(ImVec2(0, 15));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
        ImGui::Text("   NAVIGATION");
        ImGui::PopStyleColor();
        ImGui::Dummy(ImVec2(0, 10));
        
        auto NavButton = [&](const char* label, Page page) {
            bool isActive = (currentPage == page);
            
            if (isActive) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.15f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.45f, 0.50f, 1.0f));
            }
            
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.10f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.26f, 0.59f, 0.98f, 0.20f));
            
            bool clicked = ImGui::Button(label, ImVec2(260, 45));
            
            ImGui::PopStyleColor(4);
            
            if (clicked) currentPage = page;
        };
        
        NavButton("   Dashboard", Page::Dashboard);
        NavButton("   Book Management", Page::BookManagement);
        NavButton("   Student Management", Page::StudentManagement);
        NavButton("   Issue / Return", Page::IssueReturn);
        NavButton("   Request Queue", Page::RequestQueue);
        NavButton("   Transaction History", Page::History);
        
        ImGui::Dummy(ImVec2(0, 20));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 10));
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
        ImGui::Text("   QUICK STATS");
        ImGui::PopStyleColor();
        ImGui::Dummy(ImVec2(0, 10));
        
        int totalBooks = 0, availableBooks = 0;
        library.getBookList()->inOrderForEach([&](Book* b) {
            totalBooks += b->getTotalCopies();
            availableBooks += b->getAvailableCopies();
        });
        
        ImGui::Indent(20);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.35f, 0.40f, 0.45f, 1.0f));
        ImGui::BulletText("Total Books");
        ImGui::SameLine(200);
        ImGui::Text("%d", totalBooks);
        ImGui::BulletText("Available");
        ImGui::SameLine(200);
        ImGui::TextColored(ImVec4(0.30f, 0.70f, 0.40f, 1.0f), "%d", availableBooks);
        ImGui::BulletText("Borrowed");
        ImGui::SameLine(200);
        ImGui::TextColored(ImVec4(0.95f, 0.50f, 0.30f, 1.0f), "%d", totalBooks - availableBooks);
        ImGui::PopStyleColor();
        ImGui::Unindent(20);
        
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        // MAIN CONTENT AREA
        ImGui::SetNextWindowPos(ImVec2(280, 85));
        ImGui::SetNextWindowSize(ImVec2((float)(display_w - 280), (float)(display_h - 85)));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("Content", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // DASHBOARD PAGE
        if (currentPage == Page::Dashboard) {
            ImGui::SetWindowFontScale(1.8f);
            ImGui::Text("Welcome to the Library!");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("Library Management Dashboard - Real-time Overview");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            char buf[32];
            
            sprintf(buf, "%d", totalBooks);
            DrawStatCard("Total Books", buf, "Books in catalog", 
                        ImVec4(0.18f, 0.47f, 0.85f, 1.0f), ImVec4(0.12f, 0.35f, 0.65f, 1.0f), 260);
            
            ImGui::SameLine();
            
            sprintf(buf, "%d", availableBooks);
            DrawStatCard("Available", buf, "Ready to borrow", 
                        ImVec4(0.20f, 0.75f, 0.50f, 1.0f), ImVec4(0.15f, 0.60f, 0.38f, 1.0f), 260);
            
            ImGui::SameLine();
            
            sprintf(buf, "%d", totalBooks - availableBooks);
            DrawStatCard("Borrowed", buf, "Currently issued", 
                        ImVec4(1.0f, 0.65f, 0.20f, 1.0f), ImVec4(0.85f, 0.50f, 0.10f, 1.0f), 260);
            
            ImGui::Dummy(ImVec2(0, 15));
            
            int studentCount = library.getStudentsList()->getSize();
            sprintf(buf, "%d", studentCount);
            DrawStatCard("Registered Students", buf, "Active members", 
                        ImVec4(0.65f, 0.35f, 0.85f, 1.0f), ImVec4(0.50f, 0.25f, 0.68f, 1.0f), 260);
            
            ImGui::SameLine();
            
            int queueSize = library.getRequestQueue()->getSize();
            sprintf(buf, "%d", queueSize);
            DrawStatCard("Pending Requests", buf, "In waiting queue", 
                        ImVec4(0.95f, 0.35f, 0.40f, 1.0f), ImVec4(0.78f, 0.25f, 0.30f, 1.0f), 260);
            
            ImGui::SameLine();
            
            int transactionCount = library.getTransactionHistory()->getSize();
            sprintf(buf, "%d", transactionCount);
            DrawStatCard("Transactions", buf, "Total operations", 
                        ImVec4(0.25f, 0.70f, 0.65f, 1.0f), ImVec4(0.18f, 0.55f, 0.50f, 1.0f), 260);
        }

        // BOOK MANAGEMENT PAGE
        else if (currentPage == Page::BookManagement) {
            ImGui::SetWindowFontScale(1.6f);
            ImGui::Text("Book Management");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("Add, search, and manage library books");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.95f, 0.97f, 1.0f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
            ImGui::BeginChild("AddBookForm", ImVec2(850, 290), true);
            ImGui::PopStyleVar();
            
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::TextColored(ImVec4(0.26f, 0.59f, 0.98f, 1.0f), "Add New Book");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::Columns(2, nullptr, false);
            ImGui::SetColumnWidth(0, 400);
            
            ImGui::Text("Book ID");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##bookid", bookID, IM_ARRAYSIZE(bookID));
            
            ImGui::Dummy(ImVec2(0, 10));
            
            ImGui::Text("Title");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##booktitle", bookTitle, IM_ARRAYSIZE(bookTitle));
            
            ImGui::NextColumn();
            
            ImGui::Text("Author");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##bookauthor", bookAuthor, IM_ARRAYSIZE(bookAuthor));
            
            ImGui::Dummy(ImVec2(0, 10));
            
            ImGui::Text("Number of Copies");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##bookcopies", bookCopies, IM_ARRAYSIZE(bookCopies));
            
            ImGui::Columns(1);
            
            ImGui::Dummy(ImVec2(0, 20));
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.75f, 0.50f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.80f, 0.55f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.70f, 0.45f, 1.0f));
            
            if (ImGui::Button("Add Book", ImVec2(200, 45))) {
                if (strlen(bookID) > 0 && strlen(bookTitle) > 0 && strlen(bookAuthor) > 0 && strlen(bookCopies) > 0) {
                    int id = atoi(bookID);
                    int copies = atoi(bookCopies);
                    
                    if (id <= 0 || copies <= 0) {
                        strncpy(statusMsg, "✗ Invalid Book ID or Copies! Must be greater than 0", 255);
                        statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                    } else {
                        if (library.addBook(id, bookTitle, bookAuthor, copies)) {
                            library.saveBooksToCSV("data/books.csv");
                            strncpy(statusMsg, "✓ Book added successfully!", 255);
                            statusColor = ImVec4(0.20f, 0.75f, 0.50f, 1.0f);
                            
                            bookID[0] = '\0';
                            bookTitle[0] = '\0';
                            bookAuthor[0] = '\0';
                            bookCopies[0] = '\0';
                        } else {
                            strncpy(statusMsg, "✗ Book already exists!", 255);
                            statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                        }
                    }
                } else {
                    strncpy(statusMsg, "✗ Please fill all fields!", 255);
                    statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                }
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.98f, 0.95f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
            ImGui::BeginChild("SearchBook", ImVec2(0, 0), true);
            ImGui::PopStyleVar();
            
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::TextColored(ImVec4(1.0f, 0.65f, 0.20f, 1.0f), "Search Books");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::RadioButton("By ID", &searchMode, 0);
            ImGui::SameLine(150);
            ImGui::RadioButton("By Title", &searchMode, 1);
            ImGui::SameLine(300);
            ImGui::RadioButton("By Author", &searchMode, 2);
            
            ImGui::Dummy(ImVec2(0, 10));
            
            ImGui::PushItemWidth(400);
            if (searchMode == 0)
                ImGui::InputText("##searchid", searchBookID, IM_ARRAYSIZE(searchBookID));
            else
                ImGui::InputText("##searchtext", searchText, IM_ARRAYSIZE(searchText));
            ImGui::PopItemWidth();
            
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.31f, 0.64f, 1.00f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.21f, 0.54f, 0.93f, 1.0f));
            
            if (ImGui::Button("Search", ImVec2(120, 0))) {
                searchResults.clear();
                showSearchResults = true;
                showAllBooks = false;
                
                if (searchMode == 0) {
                    if (strlen(searchBookID) > 0) {
                        Book* b = library.searchBookByID(atoi(searchBookID));
                        if (b) {
                            searchResults.push_back(b);
                            strncpy(statusMsg, "✓ Book found!", 255);
                            statusColor = ImVec4(0.20f, 0.75f, 0.50f, 1.0f);
                        } else {
                            strncpy(statusMsg, "✗ Book not found", 255);
                            statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                        }
                    } else {
                        strncpy(statusMsg, "✗ Please enter Book ID", 255);
                        statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                    }
                } else if (searchMode == 1) {
                    if (strlen(searchText) > 0) {
                        searchResults = library.searchBookByTitle(searchText);
                        if (!searchResults.empty()) {
                            char msg[256];
                            sprintf(msg, "✓ Found %d book(s) with title containing '%s'", (int)searchResults.size(), searchText);
                            strncpy(statusMsg, msg, 255);
                            statusColor = ImVec4(0.20f, 0.75f, 0.50f, 1.0f);
                        } else {
                            strncpy(statusMsg, "✗ No books found with that title", 255);
                            statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                        }
                    } else {
                        strncpy(statusMsg, "✗ Please enter book title", 255);
                        statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                    }
                } else {
                    if (strlen(searchText) > 0) {
                        searchResults = library.searchBookByAuthor(searchText);
                        if (!searchResults.empty()) {
                            char msg[256];
                            sprintf(msg, "✓ Found %d book(s) by author '%s'", (int)searchResults.size(), searchText);
                            strncpy(statusMsg, msg, 255);
                            statusColor = ImVec4(0.20f, 0.75f, 0.50f, 1.0f);
                        } else {
                            strncpy(statusMsg, "✗ No books found by that author", 255);
                            statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                        }
                    } else {
                        strncpy(statusMsg, "✗ Please enter author name", 255);
                        statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                    }
                }
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.65f, 0.20f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.70f, 0.25f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.95f, 0.60f, 0.15f, 1.0f));
            
            if (ImGui::Button("Show All Books", ImVec2(150, 0))) {
                showAllBooks = true;
                showSearchResults = false;
                strncpy(statusMsg, "✓ Displaying all books", 255);
                statusColor = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::Dummy(ImVec2(0, 15));
            
            if (showAllBooks || showSearchResults) {
                if (ImGui::BeginTable("BooksTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp,ImVec2(0, 420))) {
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableSetupColumn("Book ID", ImGuiTableColumnFlags_WidthFixed, 100);
                    ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Author", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Available", ImGuiTableColumnFlags_WidthFixed, 100);
                    ImGui::TableSetupColumn("Total", ImGuiTableColumnFlags_WidthFixed, 80);
                    ImGui::TableHeadersRow();
                    
                    auto renderBook = [&](Book* b) {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn(); ImGui::Text("%d", b->getBookID());
                        ImGui::TableNextColumn(); ImGui::Text("%s", b->getTitle().c_str());
                        ImGui::TableNextColumn(); ImGui::Text("%s", b->getAuthor().c_str());
                        ImGui::TableNextColumn();
                        if (b->getAvailableCopies() > 0)
                            ImGui::TextColored(ImVec4(0.20f, 0.75f, 0.50f, 1.0f), "%d", b->getAvailableCopies());
                        else
                            ImGui::TextColored(ImVec4(0.95f, 0.35f, 0.40f, 1.0f), "%d", b->getAvailableCopies());
                        ImGui::TableNextColumn(); ImGui::Text("%d", b->getTotalCopies());
                    };
                    
                    if (showSearchResults && !searchResults.empty()) {
                        for (auto* b : searchResults)
                            renderBook(b);
                    } else if (showAllBooks) {
                        library.getBookList()->inOrderForEach(renderBook);
                    }
                    
                    ImGui::EndTable();
                }
            }
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        // STUDENT MANAGEMENT PAGE
        else if (currentPage == Page::StudentManagement) {
            ImGui::SetWindowFontScale(1.6f);
            ImGui::Text("Student Management");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("Register and manage student members");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.97f, 0.95f, 1.0f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
            ImGui::BeginChild("RegisterStudent", ImVec2(850, 240), true);
            ImGui::PopStyleVar();
            
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::TextColored(ImVec4(0.65f, 0.35f, 0.85f, 1.0f), "Register New Student");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::Columns(2, nullptr, false);
            ImGui::SetColumnWidth(0, 400);
            
            ImGui::Text("Student ID (5 digits)");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##studentid", studentID, IM_ARRAYSIZE(studentID));
            
            ImGui::Dummy(ImVec2(0, 10));
            
            ImGui::Text("Full Name");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##studentname", studentName, IM_ARRAYSIZE(studentName));
            
            ImGui::NextColumn();
            
            ImGui::Text("Department");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##studentdept", studentDept, IM_ARRAYSIZE(studentDept));
            
            ImGui::Columns(1);
            
            ImGui::Dummy(ImVec2(0, 20));
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.35f, 0.85f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.70f, 0.40f, 0.90f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.60f, 0.30f, 0.80f, 1.0f));
            
            if (ImGui::Button("Register Student", ImVec2(200, 45))) {
                if (strlen(studentID) > 0 && strlen(studentName) > 0 && strlen(studentDept) > 0) {
                    std::string idStr = studentID;
                    
                    // Check if ID is exactly 5 digits
                    if (idStr.length() != 5) {
                        strncpy(statusMsg, "✗ Student ID must be exactly 5 digits!", 255);
                        statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                    } else if (!std::all_of(idStr.begin(), idStr.end(), ::isdigit)) {
                        strncpy(statusMsg, "✗ Student ID must contain only digits!", 255);
                        statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                    } else {
                        int id = atoi(studentID);
                        
                        if (library.registerStudent(id, studentName, studentDept)) {
                            library.saveStudentsToCSV("data/students.csv");
                            strncpy(statusMsg, "✓ Student registered successfully!", 255);
                            statusColor = ImVec4(0.20f, 0.75f, 0.50f, 1.0f);
                            
                            studentID[0] = '\0';
                            studentName[0] = '\0';
                            studentDept[0] = '\0';
                        } else {
                            strncpy(statusMsg, "✗ Student already exists with this ID!", 255);
                            statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                        }
                    }
                } else {
                    strncpy(statusMsg, "✗ Please fill all fields!", 255);
                    statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                }
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.95f, 1.0f, 0.98f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
            ImGui::BeginChild("SearchStudent", ImVec2(0, 0), true);
            ImGui::PopStyleVar();
            
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::TextColored(ImVec4(0.25f, 0.70f, 0.65f, 1.0f), "Search Student");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::PushItemWidth(300);
            ImGui::InputText("Student ID##search", searchStudentID, IM_ARRAYSIZE(searchStudentID));
            ImGui::PopItemWidth();
            
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.31f, 0.64f, 1.00f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.21f, 0.54f, 0.93f, 1.0f));
            
            if (ImGui::Button("Search", ImVec2(120, 0))) {
                if (strlen(searchStudentID) > 0) {
                    int id = atoi(searchStudentID);
                    foundStudent = library.getStudentByID(id);
                    showStudentSearch = true;
                    showAllStudents = false;
                    
                    if (foundStudent) {
                        strncpy(statusMsg, "✓ Student found!", 255);
                        statusColor = ImVec4(0.20f, 0.75f, 0.50f, 1.0f);
                    } else {
                        strncpy(statusMsg, "✗ Student not found with this ID", 255);
                        statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                    }
                } else {
                    strncpy(statusMsg, "✗ Please enter Student ID", 255);
                    statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                }
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.70f, 0.65f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.75f, 0.70f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.65f, 0.60f, 1.0f));
            
            if (ImGui::Button("Show All Students", ImVec2(170, 0))) {
                showAllStudents = true;
                showStudentSearch = false;
                foundStudent = nullptr;
                strncpy(statusMsg, "✓ Displaying all students", 255);
                statusColor = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::Dummy(ImVec2(0, 15));
            
            if (showStudentSearch && foundStudent) {
                ImGui::Text("ID: %d", foundStudent->getStudentID());
                ImGui::Text("Name: %s", foundStudent->getName().c_str());
                ImGui::Text("Department: %s", foundStudent->getDepartment().c_str());
                ImGui::Text("Books Borrowed: %d / 3", foundStudent->getBorrowedCount());
                
                ImGui::Dummy(ImVec2(0, 15));
                ImGui::Separator();
                ImGui::Dummy(ImVec2(0, 15));
                ImGui::Text("Currently Borrowed Books:");
                
                if (ImGui::BeginTable("BorrowedBooks", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, 250))) {
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableSetupColumn("Book ID");
                    ImGui::TableSetupColumn("Title");
                    ImGui::TableSetupColumn("Author");
                    ImGui::TableHeadersRow();
                    
                    bool hasBooks = false;
                    library.getBookList()->inOrderForEach([&](Book* b) {
                        if (foundStudent->hasBorrowedBook(b->getBookID())) {
                            hasBooks = true;
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn(); ImGui::Text("%d", b->getBookID());
                            ImGui::TableNextColumn(); ImGui::Text("%s", b->getTitle().c_str());
                            ImGui::TableNextColumn(); ImGui::Text("%s", b->getAuthor().c_str());
                        }
                    });
                    
                    if (!hasBooks) {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::TextColored(ImVec4(0.60f, 0.65f, 0.70f, 1.0f), "No books borrowed");
                    }
                    
                    ImGui::EndTable();
                }
            } else if (showAllStudents) {
                if (ImGui::BeginTable("StudentsTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, 400))) {
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableSetupColumn("Student ID", ImGuiTableColumnFlags_WidthFixed, 120);
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Department", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableHeadersRow();
                    
                    library.getStudentsList()->forEach([](void* data) {
                        Student* s = static_cast<Student*>(data);
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn(); ImGui::Text("%d", s->getStudentID());
                        ImGui::TableNextColumn(); ImGui::Text("%s", s->getName().c_str());
                        ImGui::TableNextColumn(); ImGui::Text("%s", s->getDepartment().c_str());
                    });
                    
                    ImGui::EndTable();
                }
            }
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        // ISSUE / RETURN PAGE
        else if (currentPage == Page::IssueReturn) {
            ImGui::SetWindowFontScale(1.6f);
            ImGui::Text("Issue / Return Books");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("Manage book borrowing and returns");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            ImGui::Columns(2, nullptr, false);
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.95f, 0.97f, 1.0f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
            ImGui::BeginChild("IssueBook", ImVec2(0, 280), true);
            ImGui::PopStyleVar();
            
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::TextColored(ImVec4(0.26f, 0.59f, 0.98f, 1.0f), "Issue Book");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::Text("Student ID");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##issuestudentid", issueStudentID, IM_ARRAYSIZE(issueStudentID));
            
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::Text("Book ID");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##issuebookid", issueBookID, IM_ARRAYSIZE(issueBookID));
            
            ImGui::Dummy(ImVec2(0, 25));
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.31f, 0.64f, 1.00f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.21f, 0.54f, 0.93f, 1.0f));
            
            if (ImGui::Button("Issue Book", ImVec2(-1, 45))) {
                if (strlen(issueStudentID) > 0 && strlen(issueBookID) > 0) {
                    int sID = atoi(issueStudentID);
                    int bID = atoi(issueBookID);
                    
                    if (library.borrowBook(sID, bID)) {
                        library.saveBooksToCSV("data/books.csv");
                        library.saveStudentsToCSV("data/students.csv");
                        library.saveTransactionsToCSV("data/transactions.csv");
                        strncpy(statusMsg, "✓ Book issued successfully!", 255);
                        statusColor = ImVec4(0.20f, 0.75f, 0.50f, 1.0f);
                        
                        issueStudentID[0] = '\0';
                        issueBookID[0] = '\0';
                    } else {
                        std::string error = library.getLastError();
                        if (!error.empty()) {
                            std::string msg = "✗ " + error;
                            strncpy(statusMsg, msg.c_str(), 255);
                        } else {
                            strncpy(statusMsg, "✗ Issue failed", 255);
                        }
                        statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                    }
                } else {
                    strncpy(statusMsg, "✗ Please enter both Student ID and Book ID!", 255);
                    statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                }
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
            
            ImGui::NextColumn();
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.95f, 1.0f, 0.97f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
            ImGui::BeginChild("ReturnBook", ImVec2(0, 280), true);
            ImGui::PopStyleVar();
            
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::TextColored(ImVec4(0.20f, 0.75f, 0.50f, 1.0f), "Return Book");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::Text("Student ID");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##returnstudentid", returnStudentID, IM_ARRAYSIZE(returnStudentID));
            
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::Text("Book ID");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##returnbookid", returnBookID, IM_ARRAYSIZE(returnBookID));
            
            ImGui::Dummy(ImVec2(0, 25));
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.75f, 0.50f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.80f, 0.55f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.70f, 0.45f, 1.0f));
            
            if (ImGui::Button("Return Book", ImVec2(-1, 45))) {
                if (strlen(returnStudentID) > 0 && strlen(returnBookID) > 0) {
                    int sID = atoi(returnStudentID);
                    int bID = atoi(returnBookID);
                    
                    if (library.returnBook(sID, bID)) {
                        library.saveBooksToCSV("data/books.csv");
                        library.saveStudentsToCSV("data/students.csv");
                        library.saveTransactionsToCSV("data/transactions.csv");
                        strncpy(statusMsg, "✓ Book returned successfully!", 255);
                        statusColor = ImVec4(0.20f, 0.75f, 0.50f, 1.0f);
                        
                        returnStudentID[0] = '\0';
                        returnBookID[0] = '\0';
                    } else {
                        std::string error = library.getLastError();
                        if (!error.empty()) {
                            std::string msg = "✗ " + error;
                            strncpy(statusMsg, msg.c_str(), 255);
                        } else {
                            strncpy(statusMsg, "✗ Return failed", 255);
                        }
                        statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                    }
                } else {
                    strncpy(statusMsg, "✗ Please enter both Student ID and Book ID!", 255);
                    statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                }
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
            
            ImGui::Columns(1);
        }

        // REQUEST QUEUE PAGE
        else if (currentPage == Page::RequestQueue) {
            ImGui::SetWindowFontScale(1.6f);
            ImGui::Text("Request Queue");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("Pending book requests from students");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.97f, 0.97f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
            ImGui::BeginChild("QueueContent", ImVec2(0, 0), true);
            ImGui::PopStyleVar();
            
            ImGui::Dummy(ImVec2(0, 10));
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.95f, 0.35f, 0.40f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.40f, 0.45f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.90f, 0.30f, 0.35f, 1.0f));
            
            if (ImGui::Button("Process Next Request", ImVec2(220, 40))) {
                int result = library.processNextRequest();
                if (result != -1) {
                    char msg[256];
                    sprintf(msg, "✓ Processed request for Student ID: %d", result);
                    strncpy(statusMsg, msg, 255);
                    statusColor = ImVec4(0.20f, 0.75f, 0.50f, 1.0f);
                } else {
                    strncpy(statusMsg, "✗ Queue is empty", 255);
                    statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                }
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::Dummy(ImVec2(0, 15));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 15));
            
            if (ImGui::BeginTable("QueueTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)) {
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Position", ImGuiTableColumnFlags_WidthFixed, 120);
                ImGui::TableSetupColumn("Student ID");
                ImGui::TableHeadersRow();
                
                Queue* q = library.getRequestQueue();
                int pos = 1;
                Queue temp;
                
                while (!q->isEmpty()) {
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
            ImGui::PopStyleColor();
        }

        // HISTORY PAGE
        else if (currentPage == Page::History) {
            ImGui::SetWindowFontScale(1.6f);
            ImGui::Text("Transaction History");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("All borrowing and return transactions");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.65f, 0.20f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.70f, 0.25f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.95f, 0.60f, 0.15f, 1.0f));
            
            if (ImGui::Button("Undo Last Transaction", ImVec2(240, 40))) {
                if (library.undoLastTransaction()) {
                    library.saveBooksToCSV("data/books.csv");
                    library.saveStudentsToCSV("data/students.csv");
                    library.saveTransactionsToCSV("data/transactions.csv");
                    strncpy(statusMsg, "✓ Transaction undone successfully!", 255);
                    statusColor = ImVec4(0.20f, 0.75f, 0.50f, 1.0f);
                } else {
                    strncpy(statusMsg, "✗ No transaction to undo", 255);
                    statusColor = ImVec4(0.95f, 0.35f, 0.40f, 1.0f);
                }
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.97f, 0.99f, 1.0f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
            ImGui::BeginChild("HistoryContent", ImVec2(0, 0), true);
            ImGui::PopStyleVar();
            
            if (ImGui::BeginTable("HistoryTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)) {
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Trans. ID", ImGuiTableColumnFlags_WidthFixed, 100);
                ImGui::TableSetupColumn("Student ID", ImGuiTableColumnFlags_WidthFixed, 120);
                ImGui::TableSetupColumn("Book ID", ImGuiTableColumnFlags_WidthFixed, 100);
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100);
                ImGui::TableSetupColumn("Date");
                ImGui::TableHeadersRow();
                
                Stack* history = library.getTransactionHistory();
                Stack temp;
                
                while (!history->isEmpty()) {
                    Transaction t = history->pop();
                    temp.push(t);
                    
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn(); ImGui::Text("%d", t.getTransactionID());
                    ImGui::TableNextColumn(); ImGui::Text("%d", t.getStudentID());
                    ImGui::TableNextColumn(); ImGui::Text("%d", t.getBookID());
                    ImGui::TableNextColumn();
                    
                    if (t.getType() == TransactionType::BORROW)
                        DrawStatusBadge("BORROW", ImVec4(0.89f, 0.95f, 0.99f, 1.0f), ImVec4(0.13f, 0.59f, 0.95f, 1.0f));
                    else
                        DrawStatusBadge("RETURN", ImVec4(0.91f, 0.96f, 0.91f, 1.0f), ImVec4(0.25f, 0.65f, 0.30f, 1.0f));
                    
                    ImGui::TableNextColumn(); ImGui::Text("%s", t.getDate().c_str());
                }
                
                while (!temp.isEmpty())
                    history->push(temp.pop());
                
                ImGui::EndTable();
            }
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        ImGui::End();
        ImGui::PopStyleVar();

        // Status bar
        if (strlen(statusMsg) > 0) {
            ImGui::SetNextWindowPos(ImVec2(280, (float)(display_h - 40)));
            ImGui::SetNextWindowSize(ImVec2((float)(display_w - 280), 40));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.97f, 0.98f, 0.99f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 10));
            ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::SetWindowFontScale(1.1f);
            ImGui::TextColored(statusColor, "%s", statusMsg);
            ImGui::SetWindowFontScale(1.0f);
            ImGui::End();
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.95f, 0.96f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}