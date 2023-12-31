#include <stdio.h>  
#include <stdlib.h> 
#include <fcntl.h>  
#include <signal.h> 
#include <unistd.h> 
#include <string.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include <sys/wait.h>


void handler(int signum);

int main(int argc, char **argv)
{
  int connfd, sockfd, yes =1;
  pid_t child_pid;
  struct sockaddr_in cln_addr, info;
  socklen_t sLen = sizeof(cln_addr);

  memset(&info, 0, sizeof(info)); 
  info.sin_family = AF_INET;
  info.sin_addr.s_addr = INADDR_ANY;

  sockfd = socket(PF_INET, SOCK_STREAM, 0);

  info.sin_port = htons((unsigned short)atoi(argv[1]));
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  bind(sockfd, (struct sockaddr *)&info, sizeof(info));
  listen(sockfd, 10);

  signal(SIGCHLD, handler);
  while (1)
  {
    connfd = accept(sockfd, (struct sockaddr *)&cln_addr, &sLen);

    child_pid = fork();

    if (child_pid >= 0) // >0 means program in parent process
    {
      if (child_pid == 0) // in the new child process
      {
        dup2(connfd, STDOUT_FILENO);
        close(connfd);
        execlp("sl", "sl", "-l", NULL);
        exit(-1);
      }
      else
      {
        printf("Train ID: %d\n", (int)child_pid);
      }
    }
  }
  close(sockfd);
  return 0;
}


void handler(int signum)
{
  while(waitpid(-1, NULL, WNOHANG) > 0);
}