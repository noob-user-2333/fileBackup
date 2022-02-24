//
// Created by user on 2022/2/23.
//

#ifndef BACKUP_FOLDERRESUME_H
#define BACKUP_FOLDERRESUME_H

#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cassert>
#include <iostream>

#define  BUFFSIZE 1024 * 1024
static char name[1024];
static char content_buffer[BUFFSIZE];
void FolderResume(std::string backup_file,std::string dest_dir){
    struct stat st{};
    assert(stat(backup_file.c_str(),&st) >= 0);
    long max_offset = st.st_size;
    if(access(dest_dir.c_str(),F_OK))
        assert(mkdir(dest_dir.c_str(),0777) >= 0);
    char *buffer = nullptr;
    FILE *f = nullptr;
    size_t length,mode;
    assert(f = fopen(backup_file.c_str(),"r"));
    size_t size = 5;

    getdelim(&buffer, &size,'\n', f);
    std::cout << buffer <<std::endl;
    max_offset -= strlen(buffer);
    while(getdelim(&buffer, &size,'\n', f)) {
        max_offset -= strlen(buffer);
        sscanf(buffer, "%s %ld %ld", name, &length, &mode);
        int fd = open((dest_dir + "/" + name).c_str(),O_CREAT | O_WRONLY | O_TRUNC | O_APPEND,0666);
        assert(fd > 0);
        size = 0;
        for(;length > 0 && max_offset > 0;length -= size, max_offset -= size){
            size = length;
            if(length > BUFFSIZE)
                size = BUFFSIZE;
            fread(content_buffer,size,1,f);
            write(fd,content_buffer,size);
        }
        close(fd);
        if(max_offset <= 0)
            break;
        fseek(f,1,SEEK_CUR);
        max_offset--;
    }




    fclose(f);
}
#endif //BACKUP_FOLDERRESUME_H
