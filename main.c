#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define SCAN_PATH "/home/gloridust/Documents/Github"

void listDirectories(const char *path, int rows) {
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
            printf("%d. %s\t", count++, entry->d_name);
            if (count % rows == 1)
                printf("\n");
        }
    }
    closedir(dir);
}

int main() {
    char path[256];
    char selectedDir[256];
    int choice;

    // 使用预定义的路径
    strcpy(path, SCAN_PATH);

    // 获取命令行窗口大小
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int rows = w.ws_col / 20; // 每个文件夹名称占用约20个字符的宽度

    // 列出目录
    listDirectories(path, rows);

    // 用户选择目录
    printf("\n>> Enter the directory number to enter: ");
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
                strcpy(selectedDir, entry->d_name);
                chdir(entry->d_name);
                printf("Changed directory to: %s\n", entry->d_name);
                break;
            }
        }
    }
    closedir(dir);

    // 拼接路径
    char cmd[1024];  // 增加缓冲区大小
    snprintf(cmd, sizeof(cmd), "code %s/%s", path, selectedDir);

    // 执行code命令
    system(cmd);

    return 0;
}
