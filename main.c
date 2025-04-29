#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a Task
typedef struct {
    int id;             // Task ID
    char title[100];    // Task title
    int is_completed;   // 0 = Not Completed, 1 = Completed
} Task;

// Function declarations
void add_task(Task tasks[], int *task_count);
void view_tasks(Task tasks[], int task_count);
void delete_task(Task tasks[], int *task_count, int task_id);
void mark_completed(Task tasks[], int task_count, int task_id);
void save_tasks_to_file(Task tasks[], int task_count);
void load_tasks_from_file(Task tasks[], int *task_count);

// Add a new task
void add_task(Task tasks[], int *task_count) {
    Task new_task;
    new_task.id = *task_count + 1;
    new_task.is_completed = 0;

    printf("Enter the task title: ");
    getchar(); // Clear leftover newline
    fgets(new_task.title, sizeof(new_task.title), stdin);
    new_task.title[strcspn(new_task.title, "\n")] = 0; // Remove newline

    tasks[*task_count] = new_task;
    (*task_count)++;
    printf("Task added successfully!\n");
}

// View all tasks
void view_tasks(Task tasks[], int task_count) {
    if (task_count == 0) {
        printf("No tasks available.\n");
        return;
    }

    for (int i = 0; i < task_count; i++) {
        printf("Task ID: %d | Title: %s | Status: %s\n",
               tasks[i].id,
               tasks[i].title,
               tasks[i].is_completed ? "Completed" : "Not Completed");
    }
}

// Delete a task by ID
void delete_task(Task tasks[], int *task_count, int task_id) {
    int found = 0;
    for (int i = 0; i < *task_count; i++) {
        if (tasks[i].id == task_id) {
            found = 1;
            for (int j = i; j < *task_count - 1; j++) {
                tasks[j] = tasks[j + 1];
            }
            (*task_count)--;
            printf("Task deleted successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Task not found.\n");
    }
}

// Mark task as completed
void mark_completed(Task tasks[], int task_count, int task_id) {
    int found = 0;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].id == task_id) {
            tasks[i].is_completed = 1;
            printf("Task marked as completed.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Task not found.\n");
    }
}

// Save tasks to file
void save_tasks_to_file(Task tasks[], int task_count) {
    FILE *file = fopen("tasks.dat", "wb");
    if (file != NULL) {
        fwrite(&task_count, sizeof(int), 1, file);
        fwrite(tasks, sizeof(Task), task_count, file);
        fclose(file);
    } else {
        printf("Failed to save tasks.\n");
    }
}

// Load tasks from file
void load_tasks_from_file(Task tasks[], int *task_count) {
    FILE *file = fopen("tasks.dat", "rb");
    if (file != NULL) {
        fread(task_count, sizeof(int), 1, file);
        fread(tasks, sizeof(Task), *task_count, file);
        fclose(file);
    }
}

// Main function with menu
int main() {
    Task tasks[100];
    int task_count = 0;
    load_tasks_from_file(tasks, &task_count);

    int choice;
    do {
        printf("\n===== To-Do List Menu =====\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Delete Task\n");
        printf("4. Mark Task as Completed\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_task(tasks, &task_count);
                break;
            case 2:
                view_tasks(tasks, task_count);
                break;
            case 3: {
                int id;
                printf("Enter task ID to delete: ");
                scanf("%d", &id);
                delete_task(tasks, &task_count, id);
                break;
            }
            case 4: {
                int id;
                printf("Enter task ID to mark as completed: ");
                scanf("%d", &id);
                mark_completed(tasks, task_count, id);
                break;
            }
            case 5:
                save_tasks_to_file(tasks, task_count);
                printf("Tasks saved. Exiting...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);

    return 0;
}
