#include <sys/socket.h> 
#include <netinet/in.h>
#include <stdarg.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 

#define TRUE 1
#define FALSE 0
#define MAX_USRS 30
#define FIRST_PORT 1024
#define BUFF_SIZE 1024
#define BACKLOG 5
#define MAX_GROUP_SIZE 4
#define MIN_GROUP_SIZE 2
#define NUM_GROUP_VARIETY 3

#define RESET 0
#define BLUE 1
#define RED 2
#define GREEN 3
#define YELLOW 4


void please_print(char *format, ...) {
    va_list ap;
    
    va_start(ap, format);

    char str[BUFF_SIZE];
    vsprintf(str, format, ap);
    write(STDOUT_FILENO, str, strlen(str));

    va_end(ap);

}

void set_color(int color) {

    switch (color)
    {
    case RESET:
        please_print("\033[0m");
        break;

    case BLUE:
        please_print("\033[1;34m");
        break;

    case RED:
        please_print("\033[1;31m");
        break;

    case GREEN:
        please_print("\033[1;32m");
        break;

    case YELLOW:
        please_print("\033[01;33m");
        break;
    
    default:
        please_print("\033[0m");
        break;
    }

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


void add_new_player (int sd, int req_size,
                    int groups[NUM_GROUP_VARIETY][MAX_GROUP_SIZE], int *last) {
    
    int gp_index = req_size - MIN_GROUP_SIZE;
    groups[ gp_index ][ last[gp_index]++ ] = sd;

}


int send_group_port_and_id (int *group, int size, int port, char *sendBuff) {

    for (int i = 0; i < size; i++) {
        

        sprintf(sendBuff, "%d/%d", i, port);
        send( group[i], sendBuff, strlen(sendBuff), 0 );
        set_color(GREEN);
        please_print("ID %d and port %d sent to user in group with size %d, Socket FD: %d\n", 
                                                            i, port, size, group[i]);

    }

}


int handle_new_connection (int listenfd, struct sockaddr *serv_addr, 
                                socklen_t *addrlen, int *client_sock ) {

    int new_sock;
    if ((new_sock = accept(listenfd, serv_addr, addrlen)) < 0) {
        
        set_color(RED);   
        please_print("Error: Couldn't accept connection\n");   
        return -1;  
    
    }   
        
    set_color(BLUE);
    please_print("New connection! Socket FD: %d\n" , new_sock);   
            
    
    for (int i = 0; i < MAX_USRS; i++) {   
        
        if( client_sock[i] == 0 )   
        {   
            client_sock[i] = new_sock;
            set_color(RESET);   
            please_print("Adding to list of sockets as %d\n" , i);   
                    
            break;   
        }

    }

    return new_sock;

}


void check_client_message (int *client_sock, int index, char *buffer,
                                    int groups[NUM_GROUP_VARIETY][MAX_GROUP_SIZE], 
                                                int *last, int *last_port ) {

    int valread, sd = client_sock[index];
    if ((valread = read( sd , buffer, BUFF_SIZE )) == 0) {   
        
        set_color(BLUE);
        please_print("Host disconnected. Socket FD: %d\n", sd);   

        close( sd );   
        client_sock[index] = 0;
    }   
            
    
    else {   
        
        buffer[valread] = '\0';
        
        int requested_size = atoi(buffer);
        int group_index = requested_size - MIN_GROUP_SIZE;

        set_color(BLUE);
        please_print("User with socket FD %d requested group size %d\n", 
                            client_sock[index], requested_size);

        add_new_player(sd, requested_size, groups, last);
        
        if ( last [ group_index ] == requested_size) {
            
            set_color(YELLOW);
            please_print("Group with size %d completed\n", requested_size);
            set_color(RESET);
            please_print("Sending id's and group port to users...\n");
            last [group_index] = 0;
            send_group_port_and_id ( groups[ group_index ], 
                                    requested_size, (*last_port)++, buffer);
        
        }

    }

}


int main(int argc, char *argv[]) {

    // Main Variables
    int listenfd = 0, new_sock = 0, i;
    struct sockaddr_in serv_addr;

    int tcp_port = atoi(argv[1]);
    int reuse = 1;

    int client_sock[MAX_USRS];

    int groups[NUM_GROUP_VARIETY][MAX_GROUP_SIZE]; // INCOMPLETE GROUPS
    int last [NUM_GROUP_VARIETY] = { };

    char buffer[BUFF_SIZE];

    int last_port = FIRST_PORT;

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(tcp_port);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {

        set_color(RED);
        please_print("Error: Couldn't set socket option\n");
        return -1;
        
    }
    bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    listen(listenfd, BACKLOG);

    int addrlen = sizeof(serv_addr);

    for (i = 0; i < MAX_USRS; i++) {

        client_sock[i] = 0;   
    
    }

    // Select Variables
    fd_set readfds;
    int max_sd, activity, sd;

    set_color(GREEN);
    please_print("Server started running!\n");
    while(TRUE)   
    {

        init_fds (listenfd, &readfds, client_sock, &max_sd);
        
        set_color(RESET);
        please_print("Waiting for user requests...\n");
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if (activity < 0) {   
            
            set_color(RED);
            please_print("Error: Couldn't select sockets\n");
            return -1;
        
        }   
             
        
        if (FD_ISSET(listenfd, &readfds)) {
            
            new_sock = handle_new_connection ( listenfd, 
                                                (struct sockaddr *) &serv_addr, 
                                                    (socklen_t *) &addrlen, client_sock );


            if (new_sock < 0) {

                set_color(RED);
                please_print("Err: Couldn't connect to client\n");
                return -1;

            }

        }
                 
             
        
        for (i = 0; i < MAX_USRS; i++) {   
            
            sd = client_sock[i];   
                 
            if (FD_ISSET( sd , &readfds ))     
                check_client_message ( client_sock, i, buffer, groups, last, &last_port );

        }   
    }


}