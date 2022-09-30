// Gal Giladi
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <wait.h>

// closes the server, if there is no request from the client.
// before exit the server, kill all the zombies
void alarmHandler (int sig) {
    printf("The server was closed because no service request was received for the last 60 second\n");
    // terminates all the zombie proccesses
    while(wait(NULL) != -1);
    exit(-1);
}

// the main handler of the server, which calculates the result we need from the given
// "to_srv.txt" file, then closes this file, and creating a new "to_client_xxxx.txt" file
// which contains the result we send to client by a signal. 
void workingHandler(int sig) {
    signal(SIGUSR1, workingHandler);
    // Forking a child
    pid_t pid;
	if ((pid = fork()) == -1) {
        printf("ERROR_FROM_EX4\n");
        exit(-1);
	// if fork = 0 I'm in son proccess
	} else if (pid == 0) {
        // open the given file from the client
        int fd = open("to_srv.txt", O_RDWR);
        if (fd == -1) {
            printf("ERROR_FROM_EX4\n");
            exit(-1);
        }
        // read the given string from the file, and save them, so we can delete "to_srv.txt" asap
        char buff[64] = {'\0'};
        int rslt = read(fd, buff, 64);
        char* clientPid = strtok(buff, "\n");
        char* firstNum = strtok(NULL, "\n");
        char* operator = strtok(NULL, "\n");
        char* secondNum = strtok(NULL, "\n");
        //printf("\n");

        // after we read all the lines, we delete the file
        if (close(fd) == -1) {
            printf("ERROR_FROM_EX4\n");
            exit(-1);
        }
        unlink("to_srv.txt");

        // creating a new file "to_client_xxxx.txt"
        char nameOfFile[30] = {'\0'};
        strcat(nameOfFile, "to_client_");
        strcat(nameOfFile, clientPid);
        strcat(nameOfFile, ".txt");
        int fdNew = open(nameOfFile, O_RDWR | O_CREAT, 0777);
        if (fdNew == -1) {
            printf("ERROR_FROM_EX4\n");
            exit(-1);
        }

        // calculates the result we need to send to the client
        int result;
        int firstNumVal = atoi(firstNum);
        int secondNumVal = atoi(secondNum);
        pid_t clientPidInt = atoi(clientPid);

        if (!strcmp(operator, "1")) {
            result = firstNumVal + secondNumVal;
        } else if (!strcmp(operator, "2")) {
            result = firstNumVal - secondNumVal;
        } else if (!strcmp(operator, "3")) {
            result = firstNumVal * secondNumVal;
        } else if (!strcmp(operator, "4")) {
            if (secondNumVal == 0) {
                write(fdNew, "CANNOT_DIVIDE_BY_ZERO\n", 23);
                // we send here the signal to client and exit, because there is no need
                // to keep the son proccess.
                kill(clientPidInt, SIGUSR2);
                exit(-1);
            }
            result = firstNumVal / secondNumVal;
        }

        // write the calculated result to "to_client_xxxxx.txt" file, as a string
        char resultString[16];
        sprintf(resultString, "%d", result);
        write(fdNew, resultString, strlen(resultString));

        // now we should send a signal to the client we have finished
        kill(clientPidInt, SIGUSR2);
        exit(-1);
	} else {
		// NOT waiting for child to terminate, father let more clients to join
        alarm(60);
		return;
	}
 } // end of workingHandler

int main(int argc, char* argv[]) {
    // declare signals
    signal(SIGUSR1, workingHandler);
    signal(SIGALRM, alarmHandler);

    // checks if "to_srv.txt" file exists. if it does exists, the server will close it
    if (access("to_srv.txt", F_OK) != -1) {
        int fd = open("to_srv.txt", O_RDWR);
        if (fd == -1) {
            printf("ERROR_FROM_EX4\n");
            exit(-1);
        }
        if (close(fd) == -1) {
            printf("ERROR_FROM_EX4\n");
            exit(-1);
        }
        unlink("to_srv.txt");
    }

    // waiting to a signal from a client
    while(1) {
        pause();
    }
}
