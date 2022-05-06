
#include "pthread.h"
#include "utils.h"

unsigned int servers_num = 0;
uint64_t k = -1;
uint64_t mod = -1;

struct Server {
  char ip[255];
  int port;
  int num;
};

bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}

uint64_t send_receive(struct Server* to)
{
  struct hostent *hostname = gethostbyname(to->ip);
    if (hostname == NULL) 
    {
      fprintf(stderr, "gethostbyname failed with %s\n", to->ip);
      exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(to->port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
      fprintf(stderr, "Socket creation failed!\n");
      exit(1);
    }

    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
      fprintf(stderr, "Connection failed\n");
      exit(1);
    }

    // TODO: for one server
    // parallel between servers
    uint64_t begin = to->num*k/servers_num;
    uint64_t end = (to->num+1)*k/servers_num;

    if (to->num == 0) begin = 1;
    
    char task[sizeof(uint64_t) * 3];
    memcpy(task, &begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

    if (send(sck, task, sizeof(task), 0) < 0) {
      fprintf(stderr, "Send failed\n");
      exit(1);
    }

    char response[sizeof(uint64_t)];
    if (recv(sck, response, sizeof(response), 0) < 0) {
      fprintf(stderr, "Recieve failed\n");
      exit(1);
    }
    uint64_t answer = 0;
    memcpy(&answer, response, sizeof(uint64_t));

    close(sck);
  
    return answer; 
}

void * ThreadSendReceive(void *args) 
{
  struct Server *Sargs = (struct Server *)args;
  return (void *)(uint64_t *)send_receive(Sargs);
}

int main(int argc, char **argv) {
  char servers[255] = {'\0'}; // TODO: explain why 255

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        if(!ConvertStringToUI64(optarg, &k)) k = -1;
        break;
      case 1:
        if(!ConvertStringToUI64(optarg, &mod)) mod = -1;
        break;
      case 2:
        // TODO: your code her
        memcpy(servers, optarg, strlen(optarg));
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  
  FILE* file = fopen(servers,"r");
  if (file == NULL)
  {
    printf("fopen error!\n");
    return 1;
  }
  while (! feof(file))
  {
      if (fgetc(file) == '\n')
          servers_num++;
  }
  servers_num++;
  fclose(file);

  file = fopen(servers,"r");
  // TODO: for one server here, rewrite with servers from file
  struct Server *to = malloc(sizeof(struct Server) * servers_num);
  for(uint64_t i = 0; i < servers_num; i++)
    {
      fscanf(file,"%d:%s",&to[i].port, &to[i].ip);
      to[i].num = i;
    }
  fclose(file);
  
  pthread_t threads[servers_num];
  
  for (uint32_t i = 0; i < servers_num; i++) 
  { 
    if (pthread_create(&threads[i], NULL, ThreadSendReceive,(void *)&to[i])) 
    {
          printf("Error: pthread_create failed!\n");
          return 1;
    }
  }

  uint64_t total = 1;
  for (uint32_t i = 0; i < servers_num; i++) 
  {
    uint64_t result = 0;
    pthread_join(threads[i], (void **)&result);
    total = MultModulo(total, result, mod);
  }
  printf("answer: %lu\n", total);

  free(to);
  
  return 0;
}

