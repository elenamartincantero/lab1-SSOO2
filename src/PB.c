#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(){
    FILE *fd;
    char *model_path, *modelo, *line, *dni, *path;
    model_path = malloc(10 * sizeof(char));
    path = malloc(40 * sizeof(char));
    size_t len = 0;
    ssize_t read;
    fd = fopen("./estudiantes_p1.text", "r");
    while((read = getline(&line,&len,fd))!= -1){
        
        dni = strtok(line, " ");
        modelo = strtok(NULL, " ");
        sprintf(model_path,"./MODELOSEXAMEN/MODELO%s.pdf", modelo);
        sprintf(path, "./estudiantes/%s/MODELO%s.pdf", dni, modelo);
        
        link(model_path, path);
        
    }
}