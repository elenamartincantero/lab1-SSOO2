#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

void crear_dir_si_no_existe(char*);
int main(){
    FILE *p_fd;
    char *dni;
    char *line;
    size_t len = 0;
    char *path = "./estudiantes";
    char *new_path;
    ssize_t read;
    crear_dir_si_no_existe(path);
    
    if((p_fd = fopen("./estudiantes_p1.text", "r")) == NULL){
        printf("[PA] Error al abrir el archivo estudiantes_p1.text\n");
        exit(EXIT_FAILURE);
    }

    while((read = getline(&line,&len,p_fd))!= -1){
        dni = strtok(line, " ");
        sprintf(new_path, "%s/%s", path, dni); //Ruta del directorio
        crear_dir_si_no_existe(new_path);
    }
    exit(EXIT_SUCCESS);
}

//Método para crear un directorio si no existe
void crear_dir_si_no_existe(char *path){
    DIR *p_dir = opendir(path);
    if(ENOENT == errno){ //Comprobación de que no existe
        if(mkdir(path, 0700) == -1){
            printf("[PA] Error al crear el directorio %s\n", path);
            exit(EXIT_FAILURE);
        }
    }
}