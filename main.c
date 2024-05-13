#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

void listDirectories(const char *path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    int count = 1;
    printf("Directories in %s:\n", path);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
            printf("%d. %s\n", count++, entry->d_name);
        }
    }
    closedir(dir);
}

int main() {
    char path[256];
    int choice;

    // 输入要扫描的路径
    printf("Enter the path to scan: ");
    scanf("%s", path);

    // 列出目录
    listDirectories(path);

    // 用户选择目录
    printf("Enter the directory number to enter: ");
    scanf("%d", &choice);

    // 执行cd操作
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    int selected = 0;
    while ((selected < choice) && (dir != NULL)) {
        struct dirent *entry = readdir(dir);
        if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
            selected++;
        }
    }
    if (selected != choice) {
        printf("Invalid choice!\n");
        exit(EXIT_FAILURE);
    }
    rewinddir(dir);
    selected = 0;
    while (selected < choice) {
        struct dirent *entry = readdir(dir);
        if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
            selected++;
            if (selected == choice) {
                chdir(entry->d_name);
                printf("Changed directory to: %s\n", entry->d_name);
                break;
            }
        }
    }
    closedir(dir);

    // 执行code ./
    system("code ./");

    return 0;
}