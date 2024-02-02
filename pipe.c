#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	if (argc == 1){
		perror("No arguments");
		exit(EXIT_FAILURE);
	}
	if (argc == 2){
		int exec = execlp(argv[1], argv[1], NULL);
		if (exec == -1){
			perror("Error");
			exit(EXIT_FAILURE);
		}
	}
	int pipe_fd[2];		// initialize an integer array of size 2


	for (int i = 1; i < argc; i++){
		//printf("Running argument %d\n", i);
		if (pipe(pipe_fd) == -1){ // creates a pipe 
			perror("Pipe");
			exit(EXIT_FAILURE);
		};	
		//printf("FORKING\n");
		int pid = fork();

		if (i == 1){ // Special case for first sys call
			//printf("First sys call\n");
			if (pid > 0){ // Parent process
				int status;
				// printf("In parent process\n");
				// printf("pipe_fd: [%d, %d] \n", pipe_fd[0], pipe_fd[1]);
				close(pipe_fd[1]); // Close write, all we need to do is read
				dup2(pipe_fd[0], STDIN_FILENO); // save location of read of current pipe for next iteration
				close(pipe_fd[0]);

				int ret_status = waitpid(pid, &status, 0);
				if (ret_status == -1){
					exit(EXIT_FAILURE);
				}
				if (status != 0){
					exit(EXIT_FAILURE);
				}
			}
			if (pid == 0){
				//printf("In child process\n");
				//printf("pipe_fd: [%d, %d] \n", pipe_fd[0], pipe_fd[1]);
				dup2(pipe_fd[1], STDOUT_FILENO);
				//printf("Check1");
				close(pipe_fd[0]); // Close read end, we only need to write
				//printf("Check2");
				close(pipe_fd[1]); // Close write end, already pointed STDOUT to write end
				// printf("Should print before execlp\n");
				// printf("pipe_fd: [%d, %d] \n", pipe_fd[0], pipe_fd[1]);

				if (execlp(argv[i], argv[i], NULL) != 0){
					perror("Error");
					exit(EXIT_FAILURE);
				}
			}
		}
		else if (i == argc - 1){ // Special case for last sys call
			// printf("end-case\n");
			// printf("argument: %d\n", i);
			if (pid > 0){ // Parent process
				int status;
				dup2(pipe_fd[0], STDIN_FILENO); // Point parent stdin to 2nd pipe's read end
				close(pipe_fd[0]);
				close(pipe_fd[1]);

				
				int ret_status = waitpid(pid, &status, 0);
				if (ret_status == -1){
					exit(EXIT_FAILURE);
				}
				if (status != 0){
					exit(EXIT_FAILURE);
				}

			}
			else if (pid == 0){

				close(pipe_fd[0]);
				close(pipe_fd[1]);

				if (execlp(argv[i], argv[i], NULL) != 0){
					perror("Error");
					exit(EXIT_FAILURE);
				}
			}
		}
		else{ // Case: Reading from pipe and writing to pipe

			// printf("In-between case\n");
			if (pid > 0){ // Parent process
				int status;
				dup2(pipe_fd[0], STDIN_FILENO); // Point parent stdin to 2nd pipe's read end
				close(pipe_fd[0]);
				close(pipe_fd[1]);

				
				int ret_status = waitpid(pid, &status, 0);
				if (ret_status == -1){
					exit(EXIT_FAILURE);
				}
				if (status != 0){
					exit(EXIT_FAILURE);
				}

			}
			else if (pid == 0){ // Child process
				//dup2(pipe_fd[0], STDIN_FILENO); // point stdin to the read end of pipe (which pipe?)
				close(pipe_fd[0]); // stdin is already pointing to read end of previous pipe right?

				dup2(pipe_fd[1], STDOUT_FILENO); // should write to the pipe instead of stdout
				close(pipe_fd[1]);

				if (execlp(argv[i], argv[i], NULL) != 0){
					perror("Error");
					exit(EXIT_FAILURE);
				}
				printf("SHOULD NOT PRINT 2");

			}
			else{
				// TO DO: Throw an error
				//printf("Error in forking\n");
				perror("fork");
				exit(EXIT_FAILURE);
			}
		}
		
		
	}


	


	return 0;
}

// else{ // Case: Reading from pipe and writing to pipe

// 			pipe(pipe_fd);
// 			int pid = fork();

// 			if (pid == 0){
			
// 			close(pipe_fd[1]); // close write end of pipe

// 			dup2(pipe_fd[1], STDOUT_FILENO); // point standard output to write end of the pipe

// 			close(pipe_fd[0]); // close read end of the pipe
			
// 			printf("This is the child process\n");
// 			execlp(argv[i], argv[i], NULL);
// 			}
// 			else if (pid > 0){
// 				//int status = 0;
				
// 				close(pipe_fd[0]); // close read end of the pipe

// 				dup2(pipe_fd[0], STDIN_FILENO); // redirect stdin to the read end of the pipe

// 				close(pipe_fd[1]); // close write end of the pipe

// 				printf("This is the parent process\n");
// 				execlp(argv[2], argv[2], NULL);
// 				//waitpid(pid, &status, 0);
// 				wait(NULL);

// 			}
// 			else{
// 				// TO DO: Throw an error
// 				printf("Error in forking\n");
// 				perror("fork");
// 				exit(EXIT_FAILURE);
// 			}
// 		}