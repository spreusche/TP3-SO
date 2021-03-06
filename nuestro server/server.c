#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h> 
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "server.h"

#define NUMBER_OF_CHALLENGES 12
#define BUFFER_SIZE 1024
#define PORT 8080
#define M_PI 3.14159265358979323846


int main(void){
    int socket_fd, fd, enable = 1;
    struct sockaddr_in server_address;

    run_and_check_error(socket_fd = socket(AF_INET, SOCK_STREAM, 0), "Error with socket", -1);

    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    server_address.sin_port = htons(PORT); 

    run_and_check_error(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(enable)), "Error in setsockopt", -1);
    run_and_check_error(bind(socket_fd, (struct sockaddr *)&server_address,sizeof(server_address)), "Error in bind", -1);
    run_and_check_error(listen(socket_fd, 1), "Error in listen", -1);
    run_and_check_error(fd = accept(socket_fd, (struct sockaddr *)0x0, (socklen_t *)0x0), "Error in accept", -1);
    
    run_all_challenges(fd);

    return 0;
}

void run_all_challenges(int fd){
    char buffer[BUFFER_SIZE];

    for(int i = 0; i < NUMBER_OF_CHALLENGES; i++){
        printf("------------- DESAFIO -------------\n");
        printf("%s\n", all_challenges[i].hint);
        run_challenge_function(all_challenges[i]);

        printf("--------- PREGUNTA PARA INVESTIGAR ---------\n");
        printf("%s\n", all_challenges[i].question);

        get_answer(fd, buffer, BUFFER_SIZE);
        if(strcmp(all_challenges[i].answer, buffer) == 0){
            printf("Respuesta Correcta: %s\n", buffer);
        } else{
            printf("Respuesta Incorrecta: %s\n", buffer);
            i--; //para que vaya al proximo y continue denuevo con este
        }
        printf("\n\n\n\n");
    }
    printf("Felicitaciones, finalizaron el juego. Ahora deberan implementar el servidor que se comporte como el servidor provisto\n");
}

void get_answer(int fd, char * buffer, int buffer_size){
    int resp;
    run_and_check_error(resp = recv(fd, buffer, buffer_size-1, 0), "Error in recv", -1);
    if(resp != 0){
        buffer[resp] = '\0';
        return;
    }
    exit(0);
}

void run_challenge_function(Challenge challenge){
    if(challenge.func != NULL){
        (challenge.func)();
    }
}

void challenge4(void){
    char * ans;
    ans = "La respuesta es: fk3wfLCm3QvS";

    int ret = write(13, ans, strlen(ans));
    if(ret == -1)
        perror("write");
}

void challenge7(void){
    srand(time(0));

    int i, j = 0;
    char answer [] = "K5n2UFfpFMUN\n";
    char aux [] = "0";
    int ret;

    for (i = 0; i < 121; i++) {
        if(i % 10 == 0) {
            aux[0] = answer[j++];
            run_and_check_error(ret = write(STDOUT_FILENO, aux, sizeof(aux)), "Error in write", -1);          
        } else {
            aux[0] = rand() % 95 + 32;
            run_and_check_error(ret = write(STDERR_FILENO, aux, sizeof(aux)), "Error in write", -1);
        }
    }
}

void challenge8(void){
   printf("\033[1;91;101mLa respuesta es: BUmyYq5XxXGt\033[0m\n");
}

void challenge10(void){
    int ret;
    ret = system("gcc quine.c -o quine");

    if (ret == 0) {
        printf("¡Genial!, ya lograron meter un programa en quine.c, veamos si hace lo que corresponde:\n");
        unsigned int diff = system("./quine | diff - quine.c");
        if (diff == 0) {
            printf("La respuesta es chin_chu_lan_cha\n");
        }
        else {
            printf("diff encontro diferencias\n ENTER PARA REINTENTAR\n");
        }
    }
}

void gdbme(void){
    int var;
    var = getpid();
    if (var != 0x12345678) {
        printf("ENTER PARA REINTENTAR\n");
    }else
        printf("La respuesta es gdb_rules");
}

void challenge11(void){
    gdbme();
}

void challenge12(void){
    srand(time(0));
    
    double U_1, U_2, R, O, z;
    int cont = 1000;

    do{
        U_1 = ((double) rand() / (RAND_MAX));
        U_2 = ((double) rand() / (RAND_MAX));

        R = sqrt(-2 * log(U_1));
        O = 2 * M_PI * U_2;

        z = R * cos(O);

        printf("%f ", z);
        cont--;

    }while(cont != 0);
    printf("\n");
}

void run_and_check_error(int error, char message[], int retval){
    if(error == retval) {
        dprintf(STDERR_FILENO, "%s\n", message);
        exit(EXIT_FAILURE);
    }
}
