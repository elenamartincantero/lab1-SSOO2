#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

void copy_dir(char *, char *);
void copy_file(char *, char *);
int main(int argc, char* argv[])
{
    FILE *fp= NULL;
    pid_t process_id = 0;
    pid_t sid = 0;
    
    
    process_id = fork();
    if (process_id < 0)
    {
        printf("Error al crear el proceso hijo\n");
        exit(1);
    }
    // PARENT PROCESS. 
    if (process_id > 0)
    {
        printf("Process_id del proceso demonio %d \n", process_id);
        exit(0);
    }
    umask(0);
    sid = setsid();
    if(sid < 0)
    {
        exit(1);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //BACKUP
    
    while (1)
    {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char *time = asctime(tm);
        char *dir_path;
        dir_path = malloc(strlen(time));
        sprintf(dir_path, "../%s", time);
        mkdir(dir_path, 0700);
        copy_dir(".", dir_path);
        sleep(60);
    }
    fclose(fp);
    return (0);
}

void copy_dir(char *path, char *path_backup){
    DIR *dir;
    struct dirent *file;
    struct stat stat_file;
    char *new_path;
    char *backup;
    new_path = malloc(300UL);
    backup = malloc(300UL);
    dir = opendir(path);
    while ((file = readdir(dir)) != NULL){
        sprintf(new_path, "%s/%s", path, file->d_name);
        stat(new_path, &stat_file);
        if(strcmp(".",file->d_name) && strcmp("..", file->d_name)){

            sprintf(backup, "%s/%s", path_backup, file->d_name);
            if(S_ISDIR(stat_file.st_mode)){
                mkdir(backup, 0700);
                copy_dir(new_path, backup);
            }else{
                copy_file(new_path, backup);
            }
        }
    }
}

void copy_file(char *source, char *dest){
    FILE *origin, *copy;
    struct stat stat_file;
    char buffer [2048];
    size_t size;
    int cantidad = 0;
    int total = 0;
    copy = fopen(dest, "w");
    origin = fopen(source, "r");
    size = stat_file.st_size;
    while(cantidad < size){
        cantidad = fread(buffer, 1, sizeof(buffer), origin);
        fwrite(buffer, 1, cantidad, copy);
        total += cantidad;
        if(cantidad == 0){
            break;
        }
    }
    fclose(origin);
    fclose(copy);
}