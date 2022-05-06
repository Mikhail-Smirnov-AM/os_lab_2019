#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {

  char addr[16];
  int serv_port = 20001;
  int bufsize = 1024;
  
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

  int sockfd, n;
  char sendline[bufsize], recvline[bufsize + 1];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;
  
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(serv_port);

  if (inet_pton(AF_INET, addr, &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    exit(1);
  }
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  write(1, "Enter string\n", 13);

  while ((n = read(0, sendline, bufsize)) > 0) {
    if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
      perror("sendto problem");
      exit(1);
    }

    if (recvfrom(sockfd, recvline, bufsize, 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      exit(1);
    }

    printf("REPLY FROM SERVER= %s\n", recvline);
  }
  close(sockfd);
}
