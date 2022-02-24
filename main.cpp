#include <stdio.h>
#include <assert.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "FolderReadAndTravel.h"
#include "FolderResume.h"
#define DEFAULT_BACKUP_FILE "/dev/shm/backup.bin"
#define BACKUP_DIR "/home/user/Documents/ebook-1-master/04_network"
#define BUFF_SIZE (1024 * 1024)
static char buffer[BUFF_SIZE];
int backup_fd = 0;
char name_buff[1024];



void backup(std::string &path, std::string &relative_path, struct dirent *dir) {
    std::string relative = relative_path + dir->d_name;
    std::string file_path = path + "/" + relative;
    //header write
    struct stat st{};
    stat(file_path.c_str(),&st);
    sprintf(name_buff,"\n%s %ld %d\n",relative.c_str(),st.st_size,st.st_mode);
    write(backup_fd,name_buff,strlen(name_buff));
    if(dir->d_type == 4)
        return;
    //read content
    int fd = open(file_path.c_str(), O_RDONLY);
    assert(fd > 0);
    long length = read(fd, buffer, BUFF_SIZE);
    while (length > 0) {
        write(backup_fd, buffer, length);
        length = read(fd, buffer, BUFF_SIZE);
    }
    close(fd);
}


int main() {
    backup_fd = backup_file_open(DEFAULT_BACKUP_FILE);
    std::string header = "the backup dir:" + std::string(BACKUP_DIR);
    write(backup_fd,header.c_str(),header.length());
    operate_dir(BACKUP_DIR,"",backup);
    FolderResume(DEFAULT_BACKUP_FILE,"/dev/shm/test");
    return 0;
}

