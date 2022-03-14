#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *p_fd, *p_fd_new_file;
    char *dni, *nota_c, *line, *path, *mensaje, *media_c;
    path = malloc(50 * sizeof(char));
    mensaje = malloc(100 * sizeof(char));
    media_c = malloc(10 * sizeof(char));
    int nota, nota_necesaria, total = 0, num_alumnos = 0;
    float media;
    size_t len = 0;
    ssize_t read;

    if((p_fd = fopen("./estudiantes_p1.text", "r")) == NULL){
        printf("[PC] Error al abrir el archivo estudiantes_p1.text\n");
        exit(EXIT_FAILURE);
    }
    while((read = getline(&line,&len,p_fd))!= -1){
        num_alumnos++;
        dni = strtok(line, " ");
        strtok(NULL, " ");
        nota_c = strtok(NULL, " ");
        nota = strtol(nota_c, NULL, 10);
        total += nota; 
        nota_necesaria = 10-nota;
        sprintf(path, "./estudiantes/%s/nota_necesaria.txt", dni);
        sprintf(mensaje,"La nota que debes obtener en este nuevo examen para superar la prueba es %d.", nota_necesaria);
        p_fd_new_file = fopen(path,"w");
        if(fwrite(mensaje, 1, strlen(mensaje), p_fd_new_file) == 0){
            printf("[PC] Error al escribir en el archivo\n");
            exit(EXIT_FAILURE);
        }

    }

    media = total/num_alumnos;
    sprintf(media_c, "%.2f", media); //Casting de la nota a char para poder pasarla por la tubería
    if(write(atoi(argv[0]), media_c, strlen(media_c)) == -1){
        printf("[PC] Error al escribir en la tubería\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);

}