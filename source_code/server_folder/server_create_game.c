//this file is going to contain the server code
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>


#define BUFFER_SIZE 64


int read_all(int temporary_fd, char buffer[], int length){

	int total_length  = 0;

	int bytes_left = length;

	int n = 0;


	while(total_length<length){

		n = read(temporary_fd,buffer+total_length,bytes_left);

		if(n == -1){


			printf("Error,the tcp connection was lost\n");

			break;

		}

		bytes_left-=n;

		total_length+=n;

	}


	if(n == -1){

		return bytes_left;

	}

		//even though we have memset that sets eveyrhting to '\0', we will just do it for practice:

	buffer[total_length] = '\0';


	//so total length conts '\0'null operator

	return total_length;

}

/*
Errors: tcp is reliable, but reliable does not mean that the entire message will fit into a single packet.
The operating system has a limited buffer which actually contains what the socket will actualy send, meaning we send the message to the OS kernel, the kernel only saves part of the message if there is not enought space in the temporary buffer, meaning the rest of the message is lost, it does not get sent.
TCP is reliable but only for this OS kernel temporary buffer.
*/

int send_all(int temporary_fd, const char*  buffer, int length){

	int total_length  = 0;

	int bytes_left = length;

	int n = 0;


	//we finish until the total amount of bytes
	while(total_length<length){

		n = send(temporary_fd,buffer+total_length,bytes_left,0);

		if(n == -1){

		//if this happens,this probably means that the connection was lost, the tcp conneection was cutoff.
			break;

		}

		bytes_left-=n;

		total_length+=n;

	}


	if(n == -1){

		return bytes_left;

	}

	return total_length;

}


//protocol: number_of_bytes_of_message|option|res of message -> exapmle 67|5|Hello my name is joan soldevila villalba

void handle_client(int temporary_fd){

	char buffer_receive[BUFFER_SIZE];

	char buffer_send [BUFFER_SIZE];

	int bytes_received = 0;

	int bytes_sent = 0;

	int result = 0;

	int counter = 0;

	memset(buffer_receive,0,sizeof(buffer_receive));

	memset(buffer_send,0,sizeof(buffer_send));
	bytes_received = read_all(temporary_fd, buffer_receive, BUFFER_SIZE-1);

	if(bytes_received>0){

		buffer_receive[bytes_received] = '\0';

	}else{

		printf("Error, we recevied zero bytes");

	}

	printf("CLIENT NUMBER %d HAS SENT THE FOLLOWING MESSAGE: %s\n\n",temporary_fd, buffer_receive);

	result = buffer_receive[0] - '0'; //here we are accessing the first element of the char string that we have received via the tcp socket
	//we can already prepare what we are going to add to the send message to the client:

	buffer_send[counter++] = result + '0';

	buffer_send[counter++] = '|';

	char* temporary_char_pointer;

	switch(result){

		case 5:
			temporary_char_pointer = "This is the server, we have receid your message";
			break;

		default:

			temporary_char_pointer ="Error, invalid option was sent over";
			break;

		}

	for(int i =0;temporary_char_pointer[i]!='\0';i++){

		buffer_send[counter++] = temporary_char_pointer[i];

	}

	printf("the server is sending the following message\n");

	for(int i =0;buffer_send[i]!='\0';i++){


		printf("%c", buffer_send[i]);

	}

	printf("\n");
	bytes_sent = send_all(temporary_fd , buffer_send, BUFFER_SIZE);

	printf("bytes thave been sent: %d\n", bytes_sent);

	close(temporary_fd);

}

int main()
{

	int server_file_descriptor = 0;

	int new_socket = 0;

	struct sockaddr_in address;

	int opt = 1;

	int port_number = 8080;


	socklen_t addrlen = sizeof(address);

//	char buffer_receive[1024];

//	char buffer_send[1024] = "HELLO FROM SERVER";


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

		handle_client(new_socket);

//		int bytes_received = 0;


//		read(new_socket, buffer_receive, 1023);

//		printf("CLIENT NUMBER %d HAS SENT THE FOLLOWING MESSAGE: %s\n\n",new_socket, buffer_receive);

//		send(new_socket,buffer_send, strlen(buffer_send),0);

//		close(new_socket);


		}

	close(server_file_descriptor);
}
