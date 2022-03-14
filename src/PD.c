#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

void eliminar_directorio(char*);
int main(){
    FILE *fd;
    char *mensaje = "Ejecución interrumpida";

    eliminar_directorio("./estudiantes");

    fd = fopen("./log.txt", "w");
    fwrite(mensaje, 1, strlen(mensaje), fd);
    exit(EXIT_SUCCESS);
}

void eliminar_directorio(char *path){
    DIR *dir;
    struct dirent *file;
    struct stat stat_file;
    char *new_path;
    new_path = malloc(300UL);
    dir = opendir(path);
    while ((file = readdir(dir)) != NULL){
        sprintf(new_path, "%s/%s", path, file->d_name);
        stat(new_path, &stat_file);
        if(strcmp(".",file->d_name) && strcmp("..", file->d_name)){
            if(S_ISDIR(stat_file.st_mode)){
                eliminar_directorio(new_path);
            }else{
                remove(new_path);
            }
        }
    }
    rmdir(path);
}