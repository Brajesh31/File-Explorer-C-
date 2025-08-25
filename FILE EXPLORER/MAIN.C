#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void list_files(const char *path) {
    struct dirent *entry;
    struct stat file_stat;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    printf("Contents of %s:\n", path);
    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == 0) {
            printf("%s\t%ld bytes\n", entry->d_name, file_stat.st_size);
        } else {
            perror("stat");
        }
    }

    closedir(dir);
}

int main() {
    char current_dir[1024];
    char input[256];

    // Get the current working directory
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd");
        return EXIT_FAILURE;
    }

    while (1) {
        printf("\nCurrent Directory: %s\n", current_dir);
        list_files(current_dir);

        printf("\nEnter a command (cd <dir>, exit): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove trailing newline

        if (strncmp(input, "cd ", 3) == 0) {
            char *new_dir = input + 3;
            if (chdir(new_dir) == 0) {
                if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
                    perror("getcwd");
                    return EXIT_FAILURE;
                }
            } else {
                perror("chdir");
            }
        } else if (strcmp(input, "exit") == 0) {
            break;
        } else {
            printf("Unknown command.\n");
        }
    }

    return 0;
}
