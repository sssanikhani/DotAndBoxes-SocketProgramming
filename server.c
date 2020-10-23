#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define TRUE 1
#define FALSE 0
#define MAX_USRS 30
#define FIRST_PORT 1024
#define BUFF_SIZE 1024
#define BACKLOG 5
#define MAX_GROUP_CAPACITY 4
#define MIN_GROUP_CAPACITY 2
#define NUM_GROUP_VARIETY (MAX_GROUP_CAPACITY - MIN_GROUP_CAPACITY + 1)


void please_print(char *format, ...) {
    va_list ap;
    
    va_start(ap, format);

    char str[BUFF_SIZE];
    vsprintf(str, format, ap);
    write(STDOUT_FILENO, str, strlen(str));

    va_end(ap);

}


void init_fds (int listenfd, fd_set *readfds, int *client_sock, int *max_sd) {

    FD_ZERO(readfds);   
     
        
    FD_SET(listenfd, readfds);   
    *max_sd = listenfd;   
            
    int sd;
    for ( int i = 0 ; i < MAX_USRS ; i++ )   
    {   
        
        sd = client_sock[i];   
                
        
        if(sd > 0)   
            FD_SET( sd , readfds );   
                
        
        if(sd > *max_sd)   
            *max_sd = sd;   
    }

}


void add_new_player (int sd, int req_capacity, int **groups, int *last) {
    
    int gp_index = req_capacity - MIN_GROUP_CAPACITY;
    groups[ gp_index ][ last [gp_index]++ ] = sd;

}


int send_group_port_and_id (int *group, int capacity, int port, char *sendBuff) {

    for (int i = 0; i < capacity; i++) {
        

        sprintf(sendBuff, "%d/%d", i, port);
        send( group[i], sendBuff, strlen(sendBuff), 0 );
        please_print("ID %d and port %d sent to user in group with size %d, Socket FD: %d\n", 
                                                            i, port, capacity, group[i]);

    }

}


int handle_new_connection (int listenfd, struct sockaddr *serv_addr, 
                                socklen_t *addrlen, int *client_sock ) {

    int new_sock;
    if ((new_sock = accept(listenfd, serv_addr, addrlen)) < 0) {   
        please_print("Error: Couldn't accept connection\n");   
        exit(EXIT_FAILURE);   
    }   
        
    
    please_print("New connection! Socket FD: %d\n" , new_sock);   
            
    
    for (int i = 0; i < MAX_USRS; i++) {   
        
        if( client_sock[i] == 0 )   
        {   
            client_sock[i] = new_sock;   
            please_print("Adding to list of sockets as %d\n" , i);   
                    
            break;   
        }

    }

    return new_sock;

}


void check_client_message (int *client_sock, int index, char *buffer,
                                    int **groups, int *last, int *last_port ) {

    int valread, sd = client_sock[index];
    if ((valread = read( sd , buffer, BUFF_SIZE )) == 0) {   
        
        please_print("Host disconnected. Socket FD: %d\n", sd);   

        close( sd );   
        client_sock[index] = 0;
    }   
            
    
    else {   
        
        buffer[valread] = '\0';
        
        int requested_capacity = atoi(buffer);
        int group_index = requested_capacity - MIN_GROUP_CAPACITY;

        please_print("User with socket FD %d requested group size %d\n", 
                            client_sock[index], requested_capacity);

        add_new_player(sd, requested_capacity, groups, last);
        
        if ( last [ group_index ] == requested_capacity) {
            
            last [group_index] = 0;
            send_group_port_and_id ( groups[ group_index ], requested_capacity, (*last_port)++, buffer);
        
        }

    }

}


int main(int argc, char *argv[]) {

    // Main Variables
    int listenfd = 0, new_sock = 0, i;
    struct sockaddr_in serv_addr;

    int tcp_port = atoi(argv[1]);

    int client_sock[MAX_USRS];

    int groups[NUM_GROUP_VARIETY][MAX_GROUP_CAPACITY]; // INCOMPLETE GROUPS
    int last [NUM_GROUP_VARIETY] = { };

    char buffer[BUFF_SIZE];

    int last_port = FIRST_PORT;

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(tcp_port);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    listen(listenfd, BACKLOG);

    int addrlen = sizeof(serv_addr);

    for (i = 0; i < MAX_USRS; i++) {

        client_sock[i] = 0;   
    
    }

    // Select Variables
    fd_set readfds;
    int max_sd, activity, sd;

    please_print("Server started running!\n");
    while(TRUE)   
    {

        init_fds (listenfd, &readfds, client_sock, &max_sd);
        
        please_print("Waiting for user requests...\n");
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR)) {   
            
            please_print("Error: Couldn't select sockets\n");
            exit(EXIT_FAILURE);
        
        }   
             
        
        if (FD_ISSET(listenfd, &readfds)) 
               new_sock = handle_new_connection 
                        ( listenfd, (struct sockaddr *) &serv_addr, 
                            (socklen_t *) &addrlen, client_sock );  
             
        
        for (i = 0; i < MAX_USRS; i++) {   
            
            sd = client_sock[i];   
                 
            if (FD_ISSET( sd , &readfds ))     
                check_client_message ( client_sock, i, buffer, (int **)groups, last, &last_port );

        }   
    }


}