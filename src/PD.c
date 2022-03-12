#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(){
    FILE *fd;
    char *mensaje = "Ejecución interrumpida";

    fd = fopen("../log.txt", "w");
    fwrite(mensaje, 1, strlen(mensaje), fd);
    
}