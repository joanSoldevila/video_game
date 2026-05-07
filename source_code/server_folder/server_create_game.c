//this file is going to contain the server code
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

void handle_client(int temporary_fd){

	char buffer_receive[1024] = {0};

	char buffer_send[1024] = {0};

	int bytes_received = 0;

	int result = 0;

	printf("\nSERVER CONNECTED TO CLIENT WITH FD: %d\n", temporary_fd);

	if(temporary_fd<0){

		printf("something went wrong, we were not able to create a socket or assigne file descriptor");

		exit(EXIT_FAILURE);

	}


	bytes_received = read(temporary_fd, buffer_receive, 1023);

	printf("CLIENT NUMBER %d HAS SENT THE FOLLOWING MESSAGE: %s\n\n",temporary_fd, buffer_receive);

	result = buffer_receive[0] - '0';

	switch(result){
//when the server receives the result, depending on what the client has sent, we are going to call a function, that will load up the char* that we will send to the client
		case 1:
			//in this option the user wants to know how many people are playing the game.
			//first we need to define what is an active player: an active player is someone who is either in a game or someone who is waiting for another player to enter his or her game.

			break;

		case 2:
			//here we ask for how many active players but also the players names(all active players names)
			break;

		case 3:
			//here we try to message someone in specific
			break;

		case 4:
			//here we are looking into our "bustia" if someone gave us a message or not.
			break;

		case 5:
			//here is where you create or change your name
			break;

		case 6:
			//here is where we create a game
			break;

		case 7:
			//we are asking the server to close the tcp connection that we have established
			break;

//based on all of the explanations, we need a integer that conatins the amount of active players
//we also need a buffer that contains all of the active players names
//

		default:
			//in this case, when the server sends something wrong, what happens?
			break;

		}
	send(temporary_fd,buffer_send, strlen(buffer_send),0);

	close(temporary_fd);

}

int main(int argc, char const* argv[])
{

	int server_file_descriptor = 0;

	int new_socket = 0;

	ssize_t valread;

	struct sockaddr_in address;

	int opt = 1;

	int port_number =8080;


	socklen_t addrlen = sizeof(address);

	char buffer_receive[1024];

	char buffer_send[1024] = "HELLO FROM SERVER";


	server_file_descriptor = socket(AF_INET, SOCK_STREAM,0);

	if(server_file_descriptor<0){

		printf("Error, the operating system was not able to assign us a file descriptor for the tcp ip socket that we wanted to create\n");

		return 1;
	}

	if(setsockopt(server_file_descriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){

		printf("for some reason there was an error with configuring the socket\n");

		return 2;

	}


	address.sin_family = AF_INET;

	address.sin_addr.s_addr = INADDR_ANY;


	address.sin_port = htons(port_number);

	if (bind(server_file_descriptor, (struct sockaddr*)&address,sizeof(address))< 0) {

	        perror("bind failed");

	        exit(EXIT_FAILURE);
	}

	printf("THIS IS THE SERVER\n THE SERVER HAS THE FOLLOWING INFORMATION\n");

	printf("SERVER FAMILY: %hd\n", address.sin_family);

	printf("SERVER PORT NUMBER: %hu\n",ntohs(address.sin_port));

	printf("\nTHE SERVER IS READY\n\n\n");

	listen(server_file_descriptor,2);

	while(1){

		new_socket = accept(server_file_descriptor, (struct sockaddr*)&address,(socklen_t*)&addrlen);

		printf("\nSERVER CONNECTED TO CLIENT WITH FD: %d\n", new_socket);

		if(new_socket<0){

			printf("something went wrong, we were not able to create a socket or assigna file descriptor");

			exit(EXIT_FAILURE);

		}
		//this will be fixed in the future

		int bytes_received = 0;


		read(new_socket, buffer_receive, 1023);

		printf("CLIENT NUMBER %d HAS SENT THE FOLLOWING MESSAGE: %s\n\n",new_socket, buffer_receive);

		send(new_socket,buffer_send, strlen(buffer_send),0);

		close(new_socket);


		}

	close(server_file_descriptor);
}
