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
    FILE *p_fd;
    char *mensaje = "Ejecución interrumpida";

    eliminar_directorio("./estudiantes");

    if((p_fd = fopen("./log.txt", "w")) == NULL){
        printf("[PD] Error al abrir el archivo log.txt\n");
        exit(EXIT_FAILURE);
    }
    if(fwrite(mensaje, 1, strlen(mensaje), p_fd) == 0){
        printf("[PD] Error al escribir en el archivo log.txt\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

//Método para eliminar todos los archivos de un directorio recursivamente
void eliminar_directorio(char *path){
    DIR *p_dir;
    struct dirent *p_file;
    struct stat stat_file;
    char *new_path;
    new_path = malloc(300UL);
    if((p_dir = opendir(path)) == NULL){
        printf("[PD] Error al abrir el directorio %s\n", path);
        exit(EXIT_FAILURE);
    }
    while ((p_file = readdir(p_dir)) != NULL){
        sprintf(new_path, "%s/%s", path, p_file->d_name);
        stat(new_path, &stat_file);
        if(strcmp(".",p_file->d_name) && strcmp("..", p_file->d_name)){
            if(S_ISDIR(stat_file.st_mode)){
                eliminar_directorio(new_path);
            }else{
                if(remove(new_path) == -1){
                    printf("[PD] Error al eliminar el archivo %s\n", new_path);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    if(rmdir(path) == -1){
        printf("[PD] Error al eliminar el directorio %s\n", path);
        exit(EXIT_FAILURE);
    }
}