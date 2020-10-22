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

#define BUFF_SIZE 1024


int request_group_from_server(int tcp_sock) {

    int gp_size;
    char gp_size_str[3];

    printf("Please enter your group size: ");
    scanf("%d", &gp_size);
    int len = sprintf(gp_size_str, "%d", gp_size);
    gp_size_str[len] = '\0';
    send(tcp_sock, gp_size_str, strlen(gp_size_str), 0);
    printf("Waiting for server response... \n");

    return gp_size;

}


int get_id_from_server(int sockfd, char *recvBuff) {

    int msg_len;
    if ((msg_len = recv(sockfd, recvBuff, sizeof(recvBuff), 0)) <= 0) {
        
        printf("ERR: Receive Data Error\n");
        exit(EXIT_FAILURE);

    }
    recvBuff[msg_len] = '\0';

    int id = atoi(recvBuff);

    printf("Your id in group is %d\n", id);

    return id;

}


int get_port_from_server (int sockfd, char *recvBuff) {

    int msg_len;
    if ((msg_len = recv(sockfd, recvBuff, sizeof(recvBuff), 0)) <= 0) {

        printf("ERR: Receive Data Error\n");
        exit(EXIT_FAILURE);

    }
    recvBuff[msg_len] = '\0';

    int port = atoi(recvBuff);

    printf("The group port is %d\n", port);

    return port;

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
        printf("Usage: %s <port_number>\n",argv[0]);
        exit(EXIT_FAILURE);
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(&broadcast_addr, '0', sizeof(broadcast_addr));

    if((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        
        printf("Error: Could not create socket \n");
        exit(EXIT_FAILURE);

    } 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(tcp_port); 

    if( connect(tcp_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
       
       printf("Error: Connect Failed \n");
       exit(EXIT_FAILURE);

    }
    printf("Connected to server!\n");

    gp_size = request_group_from_server(tcp_sock);

    sleep(1);
    
    id = get_id_from_server(tcp_sock, recvBuff);

    sleep(1);

    group_port = get_port_from_server(tcp_sock, recvBuff);


    printf("++++++++++++++++++++++++++++++++++++++++\n");

    printf("%d\n", id);

    printf("%d\n", group_port);

    printf("++++++++++++++++++++++++++++++++++++++++\n");


    sleep(3);


    if((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        
        printf("Error: Could not create socket \n");
        exit(EXIT_FAILURE);

    }
    
    if (setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof(broadcast)) < 0) {
        
        printf("Error: Could not set option broadcast for socket\n");
        exit(EXIT_FAILURE);

    }

    if (setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        
        printf("Error: Could not set option reuse for socket\n");
        exit(EXIT_FAILURE);

    } 

    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons(group_port);
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(udp_sock, (struct sockaddr *) &bind_addr, sizeof(bind_addr)) < 0) {

        printf("Error: Can't bind socket to address\n");
        exit(EXIT_FAILURE);

    }

    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(group_port);
    broadcast_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

    if(id == 0) {

        int len = sprintf(sendBuff, "Hi! I'm user id %d !\n", id);
        sendBuff[len] = '\0';
        if(sendto(udp_sock, sendBuff, strlen(sendBuff), 0, 
                    (struct sockaddr *) &broadcast_addr, sizeof(broadcast_addr)) < 0) {
            
            printf("Error: Can't send message broadcast\n");
            exit(EXIT_FAILURE);

        }
        printf("Message sent succesfully!\n");
    
    }
    else {

        if((msg_len = recv(udp_sock, recvBuff, sizeof(recvBuff), 0)) < 0) {

            printf("Error: Can't receive message\n");
            exit(EXIT_FAILURE);

        }
        recvBuff[msg_len] = '\0';
        printf("Message received: %s\n", recvBuff);

    }

    return 0;
}