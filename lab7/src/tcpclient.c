#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
  
  char addr[16];
  int serv_port = 10050;
  int bufsize = 100;
  
   while (1) 
  {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"addr", required_argument, 0, 0},
                                      {"serv_port", required_argument, 0, 0},
                                      {"bufsize", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1) break;

    switch (c) 
    {
      case 0:
        switch (option_index) 
        {
          case 0:
            strcpy(addr, optarg);
          break;
          case 1:
            serv_port = atoi(optarg);
            if (serv_port <= 0) serv_port = -1;
          break;
          case 2:
            bufsize = atoi(optarg);
            if (bufsize <= 0) bufsize = -1;
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
          }
      break;
    
      case '?':
        printf("Some error has been occurred. getopt_long returned ?\n");
        return 1;
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) 
  {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (serv_port == -1 || bufsize == -1) 
  {
    printf("Usage: %s --addr \"str\" --serv_port \"num\" --bufsize \"num\"\n",
           argv[0]);
    return 1;
  }
  
  int fd;
  int nread;
  char buf[bufsize];
  struct sockaddr_in servaddr;
  
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }
  
  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;

  if (inet_pton(AF_INET, addr, &servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  servaddr.sin_port = htons(serv_port);

  if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) {
    perror("connect");
    exit(1);
  }

  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, bufsize)) > 0) {
    if (write(fd, buf, nread) < 0) {
      perror("write");
      exit(1);
    }
  }

  close(fd);
  exit(0);
}
