// Something Awesome
/**
 * save state to text file
 * implement add
 * change bg color task to something more visible - blue
 * Display intruction at top of window
 */
#include <splashkit.h>
#include "utilities.h"
#include "dynamic-array.hpp"
#include "file-operations.hpp"

enum Category {
    Work = 1,
    School,
    Personal
};

class Task {
private:
    int id;
    string detail;
    bool complete;
    system_clock::time_point creation_date;
    system_clock::time_point due_date;
    Category category;

    rectangle rect = rectangle_from(0,0,800, 80);
    color bg_color = COLOR_BLUE;

public:

    Task() : id(0), detail(""), complete(false), creation_date(system_clock::now()), due_date(system_clock::now()), category(Category::Work), bg_color(COLOR_BLUE), rect(rectangle_from(0,0,800, 80)) {}

    Task(int id, string detail, string str_due_date, Category category)
        : id(id), detail(detail), complete(false), category(category), creation_date(system_clock::now()) {
        due_date = parseDate(str_due_date);
    }


    // Getters
    int get_id() const {
        return id;
    }

    string get_detail() const {
        return detail;
    }

    bool is_complete() const {
        return complete;
    }

    system_clock::time_point get_creation_date() const {
        return creation_date;
    }

    system_clock::time_point get_due_date() const {
        return due_date;
    }

    Category get_category() const {
        return category;
    }

    void draw()
    {
        fill_rectangle(bg_color, rect);
        draw_text(detail, COLOR_WHITE, "arial", 36, rect.x + 20, rect.y + 20);

    }

    // make each task a clickable button
     void check_click(int &task_selected_id, int &next_screen_index)
    {
        if (point_in_rectangle(mouse_position(), rect) && mouse_clicked(LEFT_BUTTON))
        {
            // When clicked, display task detail in terminal
            write_line("Task selected: " + detail);
            task_selected_id = id;
            next_screen_index = 2;
        }
    }


    void set_position(int x, int y) {
        rect.x = x;
        rect.y = y;
    }

    void set_detail(const string &new_detail) {
        detail = new_detail;
    }

    void set_id(const string &new_id) {
        id = convert_to_integer(new_id);
    }

    void set_creation_date(const string &new_creation_date) {
        creation_date = parseDate(new_creation_date);
    }
    void set_due_date(const string &new_due_date) {
        due_date = parseDate(new_due_date);
    }

    void set_complete(const bool &new_complete) {
        complete = new_complete;
    }

    void set_category(const Category &new_category) {
        category = new_category;
    }

    void mark_complete() {
        complete = true;
        bg_color = COLOR_DARK_GREEN;
    }

     

};


class TaskList
{
private:
    dynamic_array<Task> *tasks;

public:
    // Constructor to initialize tasks list
    TaskList() {
        tasks = new_dynamic_array<Task>(10);
    }

    // get size 
    int get_size(){
        return tasks->size;
    }

    // get task 
    Task *get_task_at(int id){
        return get(tasks, id, {});
    }

    // Create new task
    void add_task(string detail, string str_due_date, Category category, int x, int y, int width, int height, color bg_color) {
        int id = tasks->size;
        Task new_task(id, detail, str_due_date, category);
        add(tasks, new_task);
    }

    // Delete task
    void delete_task(int id) {
        remove_at(tasks, id);
    }

    // Edit task detail
    void edit_task_detail(int id, const string &new_detail) {
        Task *task = get(tasks, id, {});
        task->set_detail(new_detail);
    }

    // Edit task due date
    void edit_task_due_date(int id, const string &new_due_date) {
        Task *task = get(tasks, id, {});
        task->set_due_date(new_due_date);
    }

    // Mark task as complete
    void mark_complete(int id) {
        Task *task = get(tasks, id, {});
        task->mark_complete();
    }

    // Print only tasks that are not complete
    void print_tasks() const {
        for (int i = 0; i < tasks->size; i++) {
            Task *task = get(tasks, i, {});
            if (!task->is_complete()) {
                write_line(to_string(task->get_id()) + ". " + task->get_detail());
            }
        }
    }

    // Print all tasks with details
    void print_tasks_array() const {
        for (int i = 0; i < tasks->size; i++) {
            Task *task = get(tasks, i, {});
            write_line("ID: " + to_string(task->get_id()));
            write_line("Detail: " + task->get_detail());
            write_line("Complete: " + string(task->is_complete() ? "Yes" : "No"));
            write_line("Creation Date: " + formatDate(task->get_creation_date()));
            write_line("Due Date: " + formatDate(task->get_due_date()));

            write("Category: ");
            switch (task->get_category()) {
                case Category::Work:
                    write_line("Work");
                    break;
                case Category::School:
                    write_line("School");
                    break;
                case Category::Personal:
                    write_line("Personal");
                    break;
            }
            write_line("-------------------");
        }
    }

    void print_pending_tasks() const{
    for (int i = 0; i <tasks->size; i++){
        Task *task = get(tasks,i,{});
        if (task->is_complete() == false) {
            write_line(to_string(task->get_id()) + ". " + task->get_detail());
        }
    }
    }

    void load_tasks_from_file(){
        string filename = "example.txt";
        
        ifstream myfile;
        myfile.open(filename);

        if (!myfile) {
            std::cerr << "Error opening file!" << std::endl;
            return;
        }

        string line;
        string lines[6];
        int counter = 0;
        // read 6 lines 
        // add_task(tasks, "Complete project report", "2024-09-30", Category::Work);
        while (getline(myfile, line)) {
            lines[counter] = (line);
            counter++;
            // when 6 lines read, add task to array and reset counter
            if (counter == 6){
                Task new_task;
                new_task.set_id(lines[0]);
                new_task.set_detail(lines[1]);
                if (lines[2] == "No"){
                    new_task.set_complete(false);
                }
                else{
                    new_task.set_complete(true);
                }
                new_task.set_creation_date(lines[3]);
                new_task.set_due_date((lines[4]));

                // select category
                Category this_category;
                if (lines[5] == "Work") {
                    this_category = Category::Work;
                } else if (lines[5] == "School") {
                    this_category = Category::School;
                } else if (lines[5] == "Personal") {
                    this_category = Category::Personal;
                }
                new_task.set_category(this_category);
                add(tasks,new_task);

                counter = 0;
            }
    
        }

        myfile.close();
}



void write_tasks_to_file() {
    string filename = "example2.txt";
    
    ofstream myfile;
    myfile.open(filename);

    if (!myfile) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    for (int i = 0; i <tasks->size; i++){
        Task *task = get(tasks,i,{});
        
        myfile << task->get_id() << endl;                    // Task ID
        myfile << task->get_detail() << endl;                // Task Detail
        myfile << (task->is_complete() ? "Yes" : "No") << endl; // Completion Status
        myfile << formatDate(task->get_creation_date()) << endl;         // Creation Date
        myfile << formatDate(task->get_due_date()) << endl;              // Due Date
        
        // Write category
        switch (task->get_category()) {
            case Category::Work:
                myfile << "Work" << endl;
                break;
            case Category::School:
                myfile << "School" << endl;
                break;
            case Category::Personal:
                myfile << "Personal" << endl;
                break;
        }
    }


    myfile.close();
}


};

Category select_category() {
    write_line("Select a category:");
    write_line("1. Work");
    write_line("2. School");
    write_line("3. Personal");

    int choice = 0;
    while (choice < 1 || choice > 3) {
        choice = read_integer("Enter the number corresponding to your choice:");
        if (choice < 1 || choice > 3) {
            write_line("Invalid choice. Please select a valid category.");
        }
    }

    return static_cast<Category>(choice); // static_cast is a type of casting in c++ that converts between enums and their underlying integer types
}


// void add_task_handler(TaskList *taskList, int &next_screen_index){
//         write_line("");
//         // Prompt the user to enter task details
//         write("Enter task detail: ");
//         string newtask_detail = read_line();

//         write("Enter due date (format: YYYY-MM-DD): ");
//         string newtask_duedate = read_line();

//         Category newtask_category = select_category();

//         taskList->add_task(newtask_detail, newtask_duedate, newtask_category,0,0,0,0,COLOR_BLUE);  // Add task using TaskList
//         write_line("New task added");
        
//         next_screen_index = 3;
//     }





// MENU METHODS 



// void display_menu() {
//     write_line("Main Menu:");
//     write_line("1. View Tasks");
//     write_line("2. Add Task");
//     write_line("3. Add Task");
//     // write_line("2. Delete Task");
//     // write_line("3. Edit Task");
//     // write_line("4. Mark Task Complete");
//     // write_line("7. Print Tasks Array");
//     write_line("8. Quit Program");
//     write("Enter your choice: ");
// }

// void task_menu(){
//     write_line("Task Menu:");
//     write_line("Here are the task details");
//     write_line("*DELETE BUTTON*");
// }



class MenuItem
{
private:
    string label;
    rectangle rect;
    color bg_color;
    bool is_hovered;

public:
    MenuItem(string label, int x, int y, int width, int height, color bg_color)
        : label(label), bg_color(bg_color), is_hovered(false)
    {
        rect = rectangle_from(x, y, width, height);
    }

    bool point_in_rect(point_2d point, const rectangle &rect)
    {
        return point.x >= rect.x && point.x <= rect.x + rect.width &&
               point.y >= rect.y && point.y <= rect.y + rect.height;
    }

    void draw()
    {
        // Draw with hover effect
        color draw_color = is_hovered ? COLOR_GRAY : bg_color;
        fill_rectangle(draw_color, rect);
        draw_text(label, COLOR_WHITE, rect.x + 20, rect.y + 20);
    }

    void check_hover()
    {
        point_2d mouse_pos = mouse_position();
        is_hovered = point_in_rect(mouse_pos, rect);
    }

    bool is_clicked()
    {
        return is_hovered && mouse_clicked(LEFT_BUTTON);
    }

    string get_label()
    {
        return label;
    }
};


class Menu
{
private:
    vector<MenuItem> items; //Using vectors here, could have used dynamic arrays as well. but using vectors for simplicity

public:
    // Constructor to initialize the menu with predefined items
    Menu()
    {
        items.push_back(MenuItem("1. View tasks", 200, 150, 400, 80, COLOR_DARK_RED));
        //items.push_back(MenuItem("2. Add task", 200, 250, 400, 80, COLOR_DARK_GREEN));
        items.push_back(MenuItem("3. Exit", 200, 250, 400, 80, COLOR_DARK_BLUE));
    }

    void draw()
    {
        for (MenuItem &item : items)
        {
            item.check_hover();
            item.draw();
        }
    }

    void handle_input(int &next_screen_index, window w, TaskList *tasklist)
    {
        for (MenuItem &item : items)
        {
            if (item.is_clicked())
            {
                if (item.get_label() == "1. View tasks")
                {
                    // switch to task screen
                    write_line("Viewing tasks...");
                    next_screen_index = 1;
                }
                else if (item.get_label() == "2. Add task")
                {
                    write_line("Use terminal to add new task");
                    // add_task_handler(tasklist, next_screen_index);
                    //next_screen_index = 3;
                }
                else if (item.get_label() == "3. Exit")
                {
                    write_line("Exiting without saving...");
                    close_window(w);
                    exit(0); // Exit the program
                }
            }
        }
    }
};

// Screen base class
class Screen
{
public:
    virtual void handle_screen(window w, int &next_screen_index, int &task_selected_id, TaskList *tasklist) = 0;
};


// MenuScreen class integrating the Menu class
class MenuScreen : public Screen
{
private:
    Menu menu;

public:

    void handle_screen(window w, int &next_screen_index,int &task_selected_id, TaskList *tasklist) override{
        // Clear the window
        clear_window(w, COLOR_BLACK);
        
        // Draw the menu
        menu.draw();

        // Handle input (hovering and clicks)
        menu.handle_input(next_screen_index, w, tasklist);
    }
};
// updated List Screen class
class ListScreen: public Screen
{
private:
    TaskList *tasklist;     // Pointer to the task list
    int task_count;         // Number of tasks
    int start_index;        // Index of the first task to display
    int selected_task_index; // Index of the clicked task


public:
    // Constructor that takes a TaskList object
    ListScreen(TaskList *tasklist) 
        : tasklist(tasklist), start_index(0), selected_task_index(-1) // Initialize to -1 (no task selected)
    {
        task_count = tasklist->get_size(); // Get the task count from TaskList

        // Calculate the horizontal center of the screen for the task list
        int window_width = screen_width();
        int task_width = 800; // Task width
        int start_x = (window_width - task_width) / 2; // Center the tasks horizontally
    }

    // Draw function to display tasks
    void draw()
    {
        int window_width = screen_width();
        int task_width = 800;
        int start_x = (window_width - task_width) / 2;  // Horizontal center for each task
        int task_height = 80;
        

        // Draw 5 tasks at a time in a fixed position
        for (int i = 0; i < 5; i++)
        {
            int index = start_index + i;
            //only draw for not completed task
            int not_completed = i;
            if (index < task_count)
            {   
                Task *task = tasklist->get_task_at(index);
                if (!task->is_complete())
                {
                    not_completed++;
                    int task_y_position = 100 + not_completed * (task_height + 20);  // Fixed vertical positions
                    task->set_position(start_x, task_y_position);
                    task->draw();  // Draw the task
                }
                
                
            }
        }

        

    }

    

    // Handle input for navigation and mouse clicks
    void handle_input(window w, int &next_screen_index, int &task_selected_id) 
    {
        // Handle keyboard navigation (up/down)
        if (key_typed(DOWN_KEY))
        {
            if (start_index + 5 < task_count)
            {
                start_index += 5; // Move down
            }
        }
        else if (key_typed(UP_KEY))
        {
            if (start_index - 5 >= 0)
            {
                start_index -= 5; // Move up
            }
        }

        for (int i = 0; i < 5; i++)
        {
            int index = start_index + i;
            if (index < task_count)
            {
                Task *task = tasklist->get_task_at(index);
                task->check_click(task_selected_id, next_screen_index); // Check for button click

            }
        }
        
        // Go to previous screen if space key is pressed
        if (key_typed(SPACE_KEY))
        {
            next_screen_index = 0;
        }

    }

    // Function to handle drawing and input
    void handle_screen(window w, int &next_screen_index, int &task_selected_id, TaskList *tasklist)
    {
        draw();  // Draw tasks
        handle_input(w, next_screen_index, task_selected_id);  // Handle keyboard and mouse input
    }
};




class TaskScreen : public Screen
{
private:
    Task *task; // The task being displayed
    rectangle big_rect; // Big rectangle for task details
    rectangle info_rect; // Rectangle for task metadata (category, due date, etc.)
    rectangle button_rect; // Rectangle for "Mark as Complete" button
    color button_color; // Color of the button

public:
    // Constructor to initialize the task and layout
    TaskScreen(Task *t)
        : task(t), big_rect(rectangle_from(100, 100, 600, 150)), info_rect(rectangle_from(100, 270, 600, 100)),
          button_rect(rectangle_from(100, 400, 200, 50)), button_color(COLOR_GREEN)
    {
    }

    // Draw method to render the TaskScreen
    void draw()
    {
        // Get the window dimensions
        int window_width = screen_width();
        int window_height = screen_height();

        // Calculate X and Y positions to center the rectangles
        int center_x = window_width / 2;
        int center_y = window_height / 2;

        // Set positions based on the center of the window
        big_rect.x = center_x - big_rect.width / 2;
        big_rect.y = center_y - (big_rect.height + info_rect.height + button_rect.height + 40); // Vertical offset for centering

        info_rect.x = center_x - info_rect.width / 2;
        info_rect.y = big_rect.y + big_rect.height + 10; // Position info_rect below big_rect with a 10-pixel gap

        button_rect.x = center_x - button_rect.width / 2;
        button_rect.y = info_rect.y + info_rect.height + 10; // Position button below info_rect with a 10-pixel gap

        // Draw the big rectangle for task details
        fill_rectangle(COLOR_LIGHT_GRAY, big_rect);
        draw_text("Task: " + task->get_detail(), COLOR_BLACK, "arial", 36, big_rect.x + 20, big_rect.y + 40);

        // Draw the smaller rectangle for task metadata (category, due date, creation date)
        fill_rectangle(COLOR_WHITE, info_rect);
        draw_text("Category: " + category_to_string(task->get_category()), COLOR_BLACK, "arial", 24, info_rect.x + 20, info_rect.y + 10);
        draw_text("Due Date: " + formatDate(task->get_due_date()), COLOR_BLACK, "arial", 24, info_rect.x + 20, info_rect.y + 40);
        draw_text("Created: " + formatDate(task->get_creation_date()), COLOR_BLACK, "arial", 24, info_rect.x + 20, info_rect.y + 70);
        
        // Draw the green "Mark as Complete" button
        fill_rectangle(button_color, button_rect);
        draw_text("Mark as Complete", COLOR_WHITE, "arial", 24, button_rect.x + 20, button_rect.y + 10);
    }

    // Method to handle input (e.g., button click)
    void handle_screen(window w, int &next_screen_index,int &task_selected_id, TaskList *tasklist) override
    {
        if (mouse_clicked(LEFT_BUTTON))
        {
            // Get the mouse position
            point_2d mouse_pos = mouse_position();

            // Check if the mouse click was inside the button rectangle
            if (point_in_rectangle(mouse_pos, button_rect))
            {
                // Mark the task as complete and change the button color to gray
                task->mark_complete();
                button_color = COLOR_DARK_GRAY;
            }
        }

        // Redraw the screen to reflect changes
        draw();

        // Go to previous screen if space key is pressed
        if (key_typed(SPACE_KEY))
        {
            next_screen_index = 1;
        }
    }

    // Utility method to convert Category enum to string
    string category_to_string(Category category)
    {
        switch (category)
        {
        case Category::Work:
            return "Work";
        case Category::School:
            return "School";
        case Category::Personal:
            return "Personal";
        default:
            return "Unknown";
        }
    }
};

// class AddScreen : public Screen {
// public:
//     bool task_added;
//     AddScreen(): task_added(false) {} // Constructor

//     void handle_screen(window w, int &next_screen_index, int &task_selected_id, TaskList *tasklist) override {
//         // If task has not been added yet, display the message
//         if (!task_added) {
//             // Display message on the screen
//             draw_text("Please use the terminal to add a task.", COLOR_WHITE, "Arial", 24, 100, 100);

//             // After drawing, we set the flag and proceed to task addition in the next iteration
//             task_added = true;
//         }
//         else {
//             // Call the add_task_handler function to handle task addition via terminal
//             add_task_handler(tasklist, next_screen_index);

//             // After adding the task, set the next screen index to 0 to return to the main menu
//             task_added = false;  // Reset flag for the next time this screen is displayed
            
//         }

        

        

//     }

//     void add_task_handler(TaskList *taskList, int &next_screen_index){
//         write_line("");
//         // Prompt the user to enter task details
//         write("Enter task detail: ");
//         string newtask_detail = read_line();

//         write("Enter due date (format: YYYY-MM-DD): ");
//         string newtask_duedate = read_line();

//         Category newtask_category = select_category();

//         taskList->add_task(newtask_detail, newtask_duedate, newtask_category,0,0,0,0,COLOR_BLUE);  // Add task using TaskList
//         write_line("New task added");
        
        
//     }

// };




int main() {
    // Create a TaskList object
    TaskList *taskList = new TaskList();

    // Load existing tasks from file
    taskList->load_tasks_from_file();
    taskList->print_pending_tasks();  // Print tasks after loading

    // Create default task for testing
    Task task(1, "Complete project report", "2024-10-10", Category::Work);

    window w = open_window("To-Do", 1024, 768); // Create a window

    // Create a List Screen and set the current tasklist to its tasklist attribute
    ListScreen list_screen(taskList);
    MenuScreen menu_screen;
    
    // Create initial task screen for the task (placeholder)
    TaskScreen *task_screen = new TaskScreen(&task);

    // // Create an add screen object
    // AddScreen *add_screen = new AddScreen();
    // Create an array of screen pointers
    Screen* screens[4];
    screens[0] = &menu_screen; // MenuScreen
    screens[1] = &list_screen; // ListScreen
    screens[2] = task_screen;   // Initial TaskScreen
    // screens[3] = add_screen;

    int screen_index = 0;
    int next_screen_index = 0;
    Screen* current_screen = screens[screen_index]; // Start with MenuScreen
    int task_selected_id = -1;

    while (true && !(quit_requested())) {
        clear_window(w, COLOR_BLACK);

        // Handle screen updates
        int next_screen_index = screen_index;
        current_screen->handle_screen(w, next_screen_index, task_selected_id, taskList);

        // Check if a task was selected
        if (task_selected_id != -1) {
            // Create a new task screen for the selected task
            Task *selected_task = taskList->get_task_at(task_selected_id);
            if (selected_task != nullptr) {
                TaskScreen *new_task_screen = new TaskScreen(selected_task);
                screens[2] = new_task_screen; // Update the screen array
                next_screen_index = 2; // Move to TaskScreen
                task_selected_id = -1; // Reset task selected id to prevent repeated updates
            }
        }

        // Switch screens if index changes
        if (next_screen_index != screen_index) {
            screen_index = next_screen_index;
            current_screen = screens[screen_index];
        }

        process_events(); // Handle events
        refresh_screen(60); // Refresh the screen
    }

    // save array to file
    taskList->write_tasks_to_file();

    // Clean up (you can implement proper cleanup for screens if needed)
    delete taskList; // Delete task list
    delete task_screen; // Clean up the initial task screen if it is no longer needed

    return 0;
}
