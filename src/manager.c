#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define LECTURA 0
#define ESCRITURA 1

pid_t g_procesos [3];

void write_log(float);
void handler(int);
void terminar_procesos(void);

int main(){
    pid_t pa_child, pb_child, pc_child;
    int estado;
    float media;
    char arg_tub[2];
    char *p_args [] = {NULL};
    char *p_env [] = {NULL};
    char buffer[2048];
    int tub_media[2];

    if(signal(SIGINT, handler) == SIG_ERR){
        printf("[MANAGER] Error al crear el manejador\n");
        exit(EXIT_FAILURE);
    }

    if(pipe(tub_media) == -1){
        printf("[MANAGER] Error al crear la tubería\n");
        exit(EXIT_FAILURE);
    }
    sprintf(arg_tub, "%d", tub_media[ESCRITURA]); //Descriptor de escritura tubería para nota media 
    char *p_args_pc [] = {arg_tub, NULL}; //Argumentos para el proceso c

    pa_child = fork(); //Se crea al hijo A
    switch (pa_child)
    {
    case -1:
        printf("[MANAGER] Error al crear el hijo PA\n");
        break;
    case 0:
        execve("./exec/PA", p_args, p_env);
        printf("[MANAGER] Error al ejecutar PA\n");
        exit(EXIT_FAILURE);
        break;
    default:
        g_procesos[0] = pa_child;
        waitpid(pa_child, &estado, 0); //Se espera a A
        g_procesos[0] = 0;
        printf("[MANAGER] HIJO PA terminado\n");
        sleep(5);
        pb_child = fork(); //Se crea al hijo B
        break;
    }
    switch (pb_child)
    {
    case -1:
        printf("[MANAGER] Error al crear el hijo PB\n");
        break;
    case 0:
        execve("./exec/PB", p_args, p_env);
        printf("[MANAGER] Error al ejecutar PB\n");
        exit(EXIT_FAILURE);
        break;
    default:
        g_procesos[1] = pb_child;
        pc_child = fork(); //Se crea a C sin esperar a que acabe B (concurrencia)
        break;
    }
    switch (pc_child)
    {
    case -1:
        printf("[MANAGER] Error al crear el hijo PC\n");
        break;
    case 0:
        close(tub_media[LECTURA]); //Se cierra el descriptor de lectura para el hijo
        execve("./exec/PC", p_args_pc, p_env);
        printf("[MANAGER] Error al ejecutar PC\n");
        exit(EXIT_FAILURE);
        break;
    default:
        g_procesos[2] = pc_child;
        if(read(tub_media[LECTURA], buffer, sizeof(buffer)) == -1){ //Como es bloqueante, con esta función se espera a que acabe C
            printf("[MANAGER] Error al leer de la tubería\n");
            exit(EXIT_FAILURE);
        } 
        media = strtol(buffer, NULL, 10);
        g_procesos[2] = 0;
        printf("[MANAGER] Hijo PC terminado\n");
        waitpid(pb_child, &estado, 0); //Se espera a B
        g_procesos[1] = 0;
        printf("[MANAGER] Hijo PB terminado\n");
        write_log(media);
        printf("[MANAGER] FIN DEL PROGRAMA\n");
        break;
    }

    exit(EXIT_SUCCESS);
}

//Método para escribir el archivo log.txt
void write_log(float media){
    FILE *p_fd;
    char *p1 = "******** Log del sistema ********\nCopia de modelos de examen, finalizada.\nCreación de archivos con nota necesaria para alcanzar la nota de corte, finalizada.\n";
    char *l4 = malloc(300UL);
    sprintf(l4, "%sLa nota media de la clase es: %.2f\nFIN DE PROGRAMA", p1, media);
    if((p_fd = fopen("./log.txt", "w")) == NULL){
        printf("[MANAGER] Error al abrir el archivo log.txt\n");
        exit(EXIT_FAILURE);
    }
    if(fwrite(l4, 1, strlen(l4), p_fd) == 0){
        printf("[MANAGER] Error al escribir en el archivo log.txt\n");
        exit(EXIT_FAILURE);
    }


}

//Método para terminar los procesos activos
void terminar_procesos(){
    printf("[MANAGER] Terminando procesos...\n");
    for (int i = 0; i < 3; i++)
    {
        if(kill(g_procesos[i], SIGINT) == -1){
            printf("[MANAGER] Error al matar el proceso\n");
            exit(EXIT_FAILURE);
        }
    }
    
}

//Manejador de la señal SIGINT
void handler(int sig){
    pid_t pd_child;
    char *p_args [] = {NULL};
    char *p_env [] = {NULL};
    int estado;
    printf("[MANAGER] Interrupción capturada - CTRL C\n");
    terminar_procesos();
    pd_child = fork();
    switch (pd_child)
    {
    case -1:
        printf("[MANAGER] Error al crear el hijo PD\n");
        break;
    case 0:
        execve("./exec/PD", p_args, p_env);
        printf("[MANAGER] Error al ejecutar PD\n");
        exit(EXIT_FAILURE);
        break;
    default:
        waitpid(pd_child, &estado, 0);
        printf("[MANAGER] Hijo PD terminado\n");
        break;
    }
    exit(EXIT_SUCCESS);
}