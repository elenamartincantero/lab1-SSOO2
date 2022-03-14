#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void crear_enlace(char*, char*);
int main(){
    FILE *p_fd;
    char *model_path, *modelo, *line, *dni, *path;
    model_path = malloc(10 * sizeof(char));
    path = malloc(40 * sizeof(char));
    size_t len = 0;
    ssize_t read;
    if((p_fd = fopen("./estudiantes_p1.text", "r")) == NULL){
        printf("[PB] Error al abrir el archivo estudiantes_p1.text\n");
        exit(EXIT_FAILURE);
    }
    while((read = getline(&line,&len,p_fd))!= -1){
        
        dni = strtok(line, " ");
        modelo = strtok(NULL, " ");
        sprintf(model_path,"./MODELOSEXAMEN/MODELO%s.pdf", modelo); //Ruta del archivo del modelo
        sprintf(path, "./estudiantes/%s/MODELO%s.pdf", dni, modelo); //Ruta de donde se va a copiar
        
        crear_enlace(model_path, path);
        
    }
    exit(EXIT_SUCCESS);
}

//Método para crear un enlace
void crear_enlace(char *model_path, char*path){
    if(link(model_path, path) == -1){
        if(EEXIST != errno){ //No pasa nada si ya existe
            printf("[PB] Error al crear el enlace\n");
            exit(EXIT_FAILURE);
        }
        
    }
}