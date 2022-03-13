#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(){
    FILE *fd;
    char *dni;
    char c;
    char *line;
    size_t len = 0;
    char *path = "./estudiantes";
    char *new_path;
    ssize_t read;
    mkdir(path, 0700);
    fd = fopen("./estudiantes_p1.text", "r");
    while((read = getline(&line,&len,fd))!= -1){
        dni = strtok(line, " ");
        sprintf(new_path, "%s/%s", path, dni);
        mkdir(new_path, 0700);
    }
    exit(EXIT_SUCCESS);
}