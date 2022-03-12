#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *fd, *fd_new_file;
    char *dni, *nota_c, *line, *path, *mensaje, *media_c;
    path = malloc(50 * sizeof(char));
    mensaje = malloc(100 * sizeof(char));
    media_c = malloc(10 * sizeof(char));
    int nota, nota_necesaria, total = 0, i = 0;
    float media;
    size_t len = 0;
    ssize_t read;

    fd = fopen("../estudiantes_p1.text", "r");
    while((read = getline(&line,&len,fd))!= -1){
        i++;
        dni = strtok(line, " ");
        strtok(NULL, " ");
        nota_c = strtok(NULL, " ");
        nota = strtol(nota_c, NULL, 10);
        total += nota; 
        nota_necesaria = 10-nota;
        sprintf(path, "../estudiantes/%s/nota_necesaria.txt", dni);
        sprintf(mensaje,"La nota que debes obtener en este nuevo examen para superar la prueba es %d.", nota_necesaria);
        fd_new_file = fopen(path,"w");
        fwrite(mensaje, 1, strlen(mensaje), fd_new_file);

    }

    media = total/i;
    sprintf(media_c, "%.2f", media);
    write(atoi(argv[0]), media_c, strlen(media_c));
    return(EXIT_SUCCESS);

}