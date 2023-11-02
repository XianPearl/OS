//to computational difference in child and parent process

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int sum_natural_numbers(int n) {
    int sum = 0;
    for(int i = 1; i <= n; i++) {
        sum += i;
    }
    return sum;
}
int main() {
    int n;
    printf("Enter a number: ");
    scanf("%d", &n);
    pid_t pid = fork();
    if(pid == 0) {
        printf("Child process:\n");
        printf("Sum of first %d natural numbers is: %d\n", n, sum_natural_numbers(n));
    } else if(pid > 0) {
        printf("Parent process:\n");
        printf("Sum of first %d natural numbers is: %d\n", n, sum_natural_numbers(n));
    } else {
        printf("Fork failed!\n");
        return 1;
    }
}

//to create 8 processes with minimum number of forking

#include <stdio.h>
#include <unistd.h>
int main() {
    printf("Parent process ID: %d\n", getpid());
    for(int i = 0; i < 3; i++) {
        if(fork() == 0) {
            printf("Child process PID: %d, Parent process PID: %d\n", getpid(), getppid());
            break;
        }
    }
    for(int i = 0; i < 3; i++) {
        wait(NULL);
    }
}

//to implement and explain the orphan process

#include <stdio.h>
#include <unistd.h>
int main() {
    int pid = fork();
    if(pid > 0) {
        printf("In parent process (PID: %d)\n", getpid());
    } else if(pid == 0) {
        sleep(1);
        printf("In child process (PID: %d, PPID: %d)\n", getpid(), getppid());
    } else {
        // Fork failed
        printf("Fork failed!\n");
        return 1;
    }
}

//to implement and explain the zombie process

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
    pid_t pid = fork();
    if(pid > 0) {
        sleep(1);
        printf("In parent process\n");
    } else if(pid == 0) {
        printf("In child process\n");
        exit(0);
    } else {
        printf("Fork failed!\n");
        return 1;
    }
    // Run a system command to display process status
    system("ps -eo pid,ppid,state,cmd");
}

//to kill a process using SIGKILL signal

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int main() {
    pid_t pid = fork();
    if(pid > 0) {
        sleep(1);
        printf("In parent process\n");
        printf("Sending SIGKILL signal to child process (PID: %d)\n", pid);
        kill(pid, SIGKILL);
        printf("Signal sent, child process should be killed\n");
    } else if(pid == 0) {
        // If we are in the child process, go into an infinite loop.
        printf("In child process\n");
        while(1) {
            printf("Child process is running...\n");
            sleep(1);
        }
    } else {
        printf("Fork failed!\n");
        return 1;
    }
}

//to implement semaphore(below program shows synchronization between 2 semaphores)

#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#define SEM_NAME "/mysemaphore"
int main() {
    // Create a semaphore
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    if(sem == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }
    // Create child process
    pid_t pid = fork();
    if(pid > 0) {
        // If we are in the parent process, wait for a while and then release the semaphore.
        sleep(1);
        printf("Parent process releasing semaphore\n");
        sem_post(sem);
    } else if(pid == 0) {
        // If we are in the child process, wait for the semaphore to be released.
        printf("Child process waiting for semaphore\n");
        sem_wait(sem);
        printf("Child process got semaphore\n");
    } else {
        // Fork failed
        printf("Fork failed!\n");
        return 1;
    }
    // Clean up
    sem_close(sem);
    if(pid > 0) {
        sem_unlink(SEM_NAME);
    }
}

//to implement 1 way communication through pipe between parent and child process

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1
int main(void) {
    char write_msg[BUFFER_SIZE] = "Hello, child!";
    char read_msg[BUFFER_SIZE];
    int fd[2];
    // Create the pipe.
    if (pipe(fd) == -1) {
        fprintf(stderr,"Pipe failed");
        return 1;
    }
    // Fork a child process.
    pid_t pid = fork();
    if (pid < 0) { 
        fprintf(stderr, "Fork failed");
        return 1;
    }
    if (pid > 0) {  // Parent process.
        // Close the unused end of the pipe.
        close(fd[READ_END]);
        // Write to the pipe.
        write(fd[WRITE_END], write_msg, strlen(write_msg)+1); 
        // Close the write end of the pipe.
        close(fd[WRITE_END]);
    } else {  // Child process.
        // Close the unused end of the pipe.
        close(fd[WRITE_END]);
        // Read from the pipe.
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        printf("Child reads: %s\n",read_msg);
        // Close the read end of the pipe.
        close(fd[READ_END]);
    }
}

//to replace a running program with another program

#include <stdio.h>
#include <unistd.h>
int main() {
    // Print the pid of this process
    printf("Current process ID: %d\n", getpid());
    // Replace this program with the "/bin/ls" program.
    // execlp() will search for the "ls" executable in the directories specified in the PATH environment variable.
    execlp("ls", "ls", "-l", NULL);
    // If execlp() is successful, we should never reach this point.
    // If we do, then there was an error.
    perror("execlp");
    return 1;
}
