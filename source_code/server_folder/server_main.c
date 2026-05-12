//this file is going to contain the server code
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "server_send_data.h"

#define BUFFER_SIZE 64

#define MAX_GAMES_SIZE 2


typedef struct{

int first_player;

int second_player;

char name[62];

int game_id;
}game_struct_players;

char* create_game(int temporary_fd, char* buffer_receive, int counter, game_struct_players* game_list){

	int i =0;

	while(i<MAX_GAMES_SIZE){

		if((game_list+i)->game_id==-1){

			(game_list+i)->game_id = i;
			(game_list+i)->first_player = temporary_fd;
			return "1|game is being created";

		}else{

			i++;
		}

	}

	return "0|max capacity of games arrived";

}


int handle_client(int temporary_fd, game_struct_players* game_list){

	int client_game_id = -1;

	char buffer_receive[BUFFER_SIZE];

	char buffer_send [BUFFER_SIZE];

	int bytes_received = 0;

	int bytes_sent = 0;

	int result = 0;

	int counter = 0;

	int client_status = 2;


	counter = 0;

	memset(buffer_receive,0,sizeof(buffer_receive));

	memset(buffer_send,0,sizeof(buffer_send));


	bytes_received = read_all(temporary_fd, buffer_receive, BUFFER_SIZE-1);

	if(bytes_received>0){

		buffer_receive[bytes_received] = '\0';

	}else{

		printf("Error, we recevied zero bytes\n Client probably disconnected, breaking\n");

		break;

	}

	result = buffer_receive[0] - '0';

	buffer_send[counter++] = result + '0';

	buffer_send[counter++] = '|';

	char* temporary_char_pointer;

	switch(result){


		case 1:
		case 2:
		case 3:
		case 4:
			client_status = 2;
			temporary_char_pointer = "The server is responding without protocol";
			break;
		case 5:
			client_status = 2;
			temporary_char_pointer = "This is the server, we have received your message";

			break;

		case 6:
			client_status = 0;
			temporary_char_pointer = create_game(temporary_fd,  buffer_receive, counter,  game_list);

			break;

		case 7:
			client_status = 2;
			temporary_char_pointer = "This is the server speaking, goodbye";

			quit = true;

			break;
		default:
			client_status = 1;
			temporary_char_pointer ="Error, invalid option was sent over";

			break;

		}

		for(int i =0;temporary_char_pointer[i]!='\0';i++){

			buffer_send[counter++] = temporary_char_pointer[i];

		}


		bytes_sent = send_all(temporary_fd , buffer_send, BUFFER_SIZE);

		return client_status;
}

void initilizeGames(game_struct_players* game_list){

	for(int i =0;i<MAX_GAMES_SIZE;i++){

		(game_list+i)->first_player = -1;

		(game_list+i)->second_player = -1;

		memset((game_list+i)->name,0,sizeof((game_list+i)->name));

		(game_list+i)->game_id = -1;

	}

}
void lookForPlayers(){


}
int main()
{

	int server_file_descriptor = 0, new_socket = 0, opt = 1, port_number = 8080, client_status = 0;


	struct sockaddr_in address;



	game_struct_players game_list[MAX_GAMES_SIZE];

	socklen_t addrlen = sizeof(address);

	server_file_descriptor = socket(AF_INET, SOCK_STREAM,0);


	initilizeGames(game_list);

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



		do{

			client_status = handle_client(new_socket, game_list);

			//after receving the client status, we need to know what todo, 
			//if the client requested something that does not require memory, we will simple close, so no if statment for this
			//if it returns 0, that means that the client created a game, and now the server needs to find the game, and then tell the client that the game was found.
			//we might have to change this in the future due to branch predictions
			if(client_status == 0){

				//server executes code for game waiting

			}

			if(client_status == 1){

				//we can probably use this when the game starts or something.

			}

			//in any other case, no memory is needed, therefore we are able to leave the loop and close the connnection with the client.

		}while(client_status!=3);//this is a bad magic number,we are going to fix this in the future using typedef enums, this number indicates that we have lost connection or the client as decided to close the connnection with the server.




//when client_status == 0, the server now needs to look for a game, after creating the game, the server needs to send a message to the client.
//we are going to do the following:
/*
we are going to create a loop, in this loop, we repeadly call the handle_client function
*/

		close(new_socket);

	}
	close(server_file_descriptor);
}
