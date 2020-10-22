#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
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


void is_last_player (int sd, int req_capacity, int **groups, int *last) {
    
    int gp_index = req_capacity - 2;
    groups[ gp_index ][ last [gp_index]++ ] = sd;

}


int send_port (int *group, int capacity, int port) {

    

}




int main(int argc, char *argv[]) {

    int listenfd = 0, new_sock = 0;
    struct sockaddr_in serv_addr;

    int tcp_port = atoi(argv[1]);

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(tcp_port);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    listen(listenfd, 5);

    int addrlen = sizeof(serv_addr);

    fd_set readfds;
    int max_sd, addrlen, activity, valread, sd, full, i;
    int client_sock[MAX_USRS];

    int last_port = FIRST_PORT;

    int groups[3][4];
    int last [3];

    char buffer[BUFF_SIZE];

    while(TRUE)   
    {   
        
        // initial_fds (&readfds)

        FD_ZERO(&readfds);   
     
        
        FD_SET(listenfd, &readfds);   
        max_sd = listenfd;   
             
        
        for ( i = 0 ; i < MAX_USRS ; i++)   
        {   
            
            sd = client_sock[i];   
                 
            
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            
            if(sd > max_sd)   
                max_sd = sd;   
        }   
     
        
        
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }   
             
        
        
        if (FD_ISSET(listenfd, &readfds))   
        {   
            if ((new_sock = accept(listenfd,  
                    (struct sockaddr *)&serv_addr, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            
            printf("New connection! Socket FD: %d\n" , new_sock);   
                 
            
            for (i = 0; i < MAX_USRS; i++)   
            {   
                
                if( client_sock[i] == 0 )   
                {   
                    client_sock[i] = new_sock;   
                    printf("Adding to list of sockets as %d\n" , i);   
                         
                    break;   
                }   
            }   
        }   
             
        
        for (i = 0; i < MAX_USRS; i++)   
        {   
            sd = client_sock[i];   
                 
            if (FD_ISSET( sd , &readfds))   
            {   
                
                
                if ((valread = read( sd , buffer, BUFF_SIZE)) == 0)   
                {   
                    
                    printf("Host disconnected. Socket FD: %d\n", sd);   

                    close( sd );   
                    client_sock[i] = 0;
                }   
                     
                
                else 
                {   
                    
                    buffer[valread] = '\0';
                    
                    int requested_capacity = atoi(buffer);
                    int group_index = requested_capacity - 2;

                    add_new_player(sd, requested_capacity, groups, last);
                    
                    if ( last [ group_index ] == requested_capacity) {
                        
                        last [group_index] = 0;
                        send_port ( groups[ group_index ], requested_capacity, last_port++);
                    
                    }   

                }   
            }   
        }   
    }


}