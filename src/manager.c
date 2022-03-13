#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#define LECTURA 0
#define ESCRITURA 1

pid_t procesos [3];

void write_log(float);
void handler(int);
void terminar_procesos(void);

int main(){
    pid_t pa_child, pb_child, pc_child;
    int estado;
    float media;
    char arg_tub[2];
    char *args [] = {NULL};
    char *env [] = {NULL};
    char buffer[2048];
    int tub_media[2];
    signal(SIGINT, handler);
    pipe(tub_media);
    sprintf(arg_tub, "%d", tub_media[ESCRITURA]);
    char *args_pc [] = {arg_tub, NULL};
    pa_child = fork();
    switch (pa_child)
    {
    case -1:
        perror("Error al crear el hijo PA");
        break;
    case 0:
        execve("./exec/PA", args, env);
        break;
    default:
        procesos[0] = pa_child;
        waitpid(pa_child, &estado, 0);
        printf("HIJO PA terminado\n");
        sleep(5);
        pb_child = fork();
        
        break;
    }
    switch (pb_child)
    {
    case -1:
        perror("Error al crear el hijo PB");
        break;
    case 0:
        execve("./exec/PB", args, env);
        printf("error con pb");
        break;
    default:
        procesos[1] = pb_child;
        pc_child = fork();
        break;
    }
    switch (pc_child)
    {
    case -1:
        perror("Error al crear el hijo PC");
        break;
    case 0:
        close(tub_media[LECTURA]);
        execve("./exec/PC", args_pc, env);
        printf("error con pc");
        break;
    default:
        procesos[2] = pc_child;
        read(tub_media[LECTURA], buffer, sizeof(buffer));
        media = strtol(buffer, NULL, 10);
        printf("Hijo PC terminado\n");
        waitpid(pb_child, &estado, 0);
        printf("Hijo PB terminado\n");
        write_log(media);
        printf("FIN DEL PROGRAMA\n");
        break;
    }

    exit(EXIT_SUCCESS);
}

void write_log(float media){
    FILE *fd;
    char *p1 = "******** Log del sistema ********\nCopia de modelos de examen, finalizada.\nCreación de archivos con nota necesaria para alcanzar la nota de corte, finalizada.\n";
    char *l4 = malloc(300UL);
    sprintf(l4, "%sLa nota media de la clase es: %.2f\nFIN DE PROGRAMA", p1, media);
    fd = fopen("../log.txt", "w");
    fwrite(l4, 1, strlen(l4), fd);


}

void terminar_procesos(){
    printf("Terminando procesos...\n");
    for (int i = 0; i < 3; i++)
    {
        kill(procesos[i], SIGINT);
        
    }
    
}

void handler(int sig){
    pid_t pd_child;
    char *args [] = {NULL};
    char *env [] = {NULL};
    int estado;
    printf("Interrupción capturada - CTRL C\n");
    terminar_procesos();
    pd_child = fork();
    switch (pd_child)
    {
    case -1:
        perror("Error al crear el hijo PD");
        break;
    case 0:
        execve("./exec/PD", args, env);
        break;
    default:
        waitpid(pd_child, &estado, 0);
        printf("Hijo PD terminado\n");
        break;
    }
    exit(EXIT_SUCCESS);
}