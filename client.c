#include <sys/socket.h> 
#include <netinet/in.h>
#include <stdarg.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <arpa/inet.h>

#include "consts.h"
#include "io_lib.h"
#include "game.h"


int connect_to_server(struct sockaddr_in *serv_addr, int tcp_port) {

    int tcp_sock;
    if((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        
        set_color(RED);
        please_print("Error: Couldn't create socket \n");
        return -1;

    } 

    serv_addr -> sin_family = AF_INET;
    serv_addr -> sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr -> sin_port = htons(tcp_port); 

    if( connect(tcp_sock, (struct sockaddr *) serv_addr, sizeof(*serv_addr)) < 0) {
       
       set_color(RED);
       please_print("Error: Couldn't connect to server \n");
       return -1;

    }
    set_color(GREEN);
    please_print("Connected to server!\n");

    return tcp_sock;

}


void print_id_and_group_port(int id, int group_port) {

    set_color(GREEN);
    please_print("++++++++++++++++++++++++++++++++++++++++\n");

    set_color(RESET);
    please_print("Your id in group is %d\n", id);

    please_print("Your group port is %d\n", group_port);

    set_color(GREEN);
    please_print("++++++++++++++++++++++++++++++++++++++++\n");

}


int get_id_and_port_from_server (int sockfd, int *id, int *port) {

    int msg_len;
    char recvBuff[BUFF_SIZE];


    if ((msg_len = recv(sockfd, recvBuff, sizeof(recvBuff), 0)) <= 0) {

        set_color(RED);
        please_print("Error: Receive Data Error\n");
        return -1;

    }
    recvBuff[msg_len] = '\0';

    sscanf(recvBuff, "%d/%d", id, port);

    print_id_and_group_port(*id, *port);

    return 0;

}


int request_group_from_server(int tcp_sock, int *id, int *group_port) {

    int gp_size;
    char gp_size_str[3];

    set_color(RESET);
    please_print("Please enter your group size: ");
    please_scan("%d", &gp_size);

    int len = sprintf(gp_size_str, "%d", gp_size);
    gp_size_str[len] = '\0';

    if (send(tcp_sock, gp_size_str, strlen(gp_size_str), 0) < 0) {

        set_color(RED);
        please_print("Error: Couldn't send data to server\n");
        return -1;

    }
    
    set_color(RESET);
    please_print("Waiting for server response... \n");

    if (get_id_and_port_from_server( tcp_sock, id, group_port ) < 0) {

        set_color(RED);
        please_print("Error: Couldn't get id and port from server\n");
        return -1;

    }

    return gp_size;

}


int create_broadcast_socket(struct sockaddr_in *bind_addr, 
                            struct sockaddr_in *broadcast_addr,
                             int group_port, int *broadcast, int *reuse) {

    int udp_sock;

    if((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        
        set_color(RED);
        please_print("Error: Couldn't create socket \n");
        return -1;

    }
    
    if (setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, broadcast,
        sizeof(*broadcast)) < 0) {
        
        set_color(RED);
        please_print("Error: Couldn't set option broadcast for socket\n");
        return -1;

    }

    if (setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR, reuse, sizeof(*reuse)) < 0) {
        
        set_color(RED);
        please_print("Error: Couldn't set option reuse for socket\n");
        return -1;

    } 

    bind_addr -> sin_family = AF_INET;
    bind_addr -> sin_port = htons(group_port);
    bind_addr -> sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(udp_sock, (struct sockaddr *) bind_addr, sizeof(*bind_addr)) < 0) {

        set_color(RED);
        please_print("Error: Couldn't bind socket to address\n");
        return -1;

    }

    broadcast_addr -> sin_family = AF_INET;
    broadcast_addr -> sin_port = htons(group_port);
    broadcast_addr -> sin_addr.s_addr = inet_addr("255.255.255.255");

    return udp_sock;

}



int main(int argc, char *argv[])
{

    int tcp_sock = 0, udp_sock = 0, n = 0, gp_size, msg_len, id, group_port;
    char recvBuff[BUFF_SIZE], printBuff[BUFF_SIZE], sendBuff[BUFF_SIZE];
    struct sockaddr_in serv_addr, broadcast_addr, bind_addr;
    
    int tcp_port = atoi(argv[1]);

    int broadcast = 1, reuse = 1;

    if(argc != 2)
    {
        set_color(YELLOW);
        please_print("Usage: %s <port_number>\n",argv[0]);
        return -1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(&broadcast_addr, '0', sizeof(broadcast_addr));

    tcp_sock = connect_to_server(&serv_addr, tcp_port);
    if (gp_size < 0) {

        set_color(RED);
        please_print("Error: Couldn't connect to server\n");
        return -1;

    }

    gp_size = request_group_from_server(tcp_sock, &id, &group_port);
    if (gp_size < 0) {

        set_color(RED);
        please_print("Error: Couldn't get group information from server\n");
        return -1;

    }

    sleep(4);

    udp_sock = create_broadcast_socket(&bind_addr, &broadcast_addr, 
                                        group_port, &broadcast, &reuse);
    if (udp_sock < 0) {

        set_color(RED);
        please_print("Error: Couldn't create UDP socket\n");
        return -1;

    }
    

    if(start_game(gp_size + MAPSIZE_DIFF_GPSIZE, id, udp_sock, &broadcast_addr) < 0) {

        set_color(RED);
        please_print("Error: Couldn't start game\n");
        return -1;

    }

    // if(id == 0) {

    //     int len = sprintf(sendBuff, "Hi! I'm user id %d!", id);
    //     sendBuff[len] = '\0';
    //     if(sendto(udp_sock, sendBuff, strlen(sendBuff), 0, 
    //                 (struct sockaddr *) &broadcast_addr, sizeof(broadcast_addr)) < 0) {
            
    //         set_color(RED);
    //         please_print("Error: Couldn't send message broadcast\n");
    //         return -1;

    //     }
    //     set_color(GREEN);
    //     please_print("Message sent succesfully!\n");
    
    // }
    // else {

    //     if((msg_len = recv(udp_sock, recvBuff, sizeof(recvBuff), 0)) < 0) {

    //         set_color(RED);
    //         please_print("Error: Can't receive message\n");
    //         return -1;

    //     }
    //     recvBuff[msg_len] = '\0';
    //     set_color(BLUE);
    //     please_print("Message received: %s\n", recvBuff);

    // }

    return 0;
}