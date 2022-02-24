//
// Created by user on 2022/2/23.
//

#ifndef BACKUP_FOLDERREADANDTRAVEL_H
#define BACKUP_FOLDERREADANDTRAVEL_H

#include <string>
#include <fcntl.h>
#include <dirent.h>
#include <cassert>
#include <iostream>

void _operate(std::string &path, std::string &relative_path, struct dirent *dir) {
    std::cout << "/" << relative_path << dir->d_name << std::endl;
}

void operate_dir(std::string begin_path, std::string relative_path = "",
                 void (*operate)(std::string &path, std::string &relative_path, struct dirent *dir) = _operate) {
    DIR *open_dir = opendir((begin_path + "/" + relative_path).c_str());
    assert(open_dir);
    struct dirent * dir = readdir(open_dir);
    dir = readdir(open_dir);
    dir = readdir(open_dir);
    while (dir) {
        operate(begin_path, relative_path, dir);
        if (dir->d_type == 4)
            operate_dir(begin_path, relative_path + dir->d_name + "/");
        dir = readdir(open_dir);
    }
}

int backup_file_open(const char *path) {
    int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, 0666);
    assert(fd > 0);
    return fd;
}
#endif //BACKUP_FOLDERREADANDTRAVEL_H
