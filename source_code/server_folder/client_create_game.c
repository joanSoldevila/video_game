#include "client_create_game.h"

void setupConnection(int* client_file_descriptor,struct sockaddr_in* server_address, int port){

	*(client_file_descriptor) = socket(AF_INET, SOCK_STREAM,0);

	server_address->sin_family = AF_INET;

	server_address->sin_port = htons(port);


	int result_translation = 0;

	int status = 0;


	result_translation =  inet_pton(AF_INET, "127.0.0.1", &server_address->sin_addr);

	status = connect(*(client_file_descriptor), (struct sockaddr*)server_address, sizeof(*(server_address)));

}

void printString(char* buffer){

	for(int i =0;(i<buffer[i]!='\0');i++){

		printf("%c", buffer[i]);

	}

}

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

int read_all(int temporary_fd, char buffer[], int length){
//rememebr that for reading it is always going to be one less, beacuse of the null terminator,

	int total_length = 0;

	int bytes_left = length;

	int n = 0;


	while(total_length<length){


		n = read(temporary_fd, buffer+total_length,bytes_left);

		if(n == -1){

		//	printf("Error, the connection was lost, not bytes where sent\n");

			break;
		}


		total_length+=n;

		bytes_left-=n;

	}


	//after reading, we need to set the last thing that we have read to '\0', just in case:

	buffer[total_length] = '\0';

	return total_length;

}
int send_data_to_server(char* buffer_r,char* buffer_s , int client_file_descriptor){

	int bytes_sent = 0;

	int bytes_read = 0;


	int bytes_sent_client = 0;


	bytes_sent = send_all(client_file_descriptor, buffer_s,BUFFER_SIZE);

//	printf("number of bytes sent to the server %d\n", bytes_sent);


	bytes_read =  read_all(client_file_descriptor,buffer_r, BUFFER_SIZE-1);


//	printf("number of bytes recevied from the server: %d\n", bytes_read);

	return bytes_read;

}



void printMenuSC(){

	printf("Menu for Server communication\n");

	printf("1. Ask server for number of active players");

	printf("2. Ask server for all players names");

	printf("3. Message someone specificly");

	printf("4. Ask server for your incoming messages");

	printf("5. Create/change your name or send message to server\n");

	printf("6. Create a game\n");

	printf("7. Quit server communication\n");

}


void handleoption(int option, int file_descriptor){

	char buffer_send[BUFFER_SIZE]="5|hello this is the client";


	char buffer_receive[BUFFER_SIZE] = {0};

	int temporary_option = 0 ;

	int counter = 0;

	buffer_send[0] = option + '0';

	int bytes_receive = send_data_to_server(buffer_receive, buffer_send , file_descriptor);

//	printf("Number of bytes received from server: %d\n", bytes_receive);

	switch(option){

		case 1:

			break;

		case 2:
			break;

		case 3:

			break;

		case 4:

			break;

		case 5:

			temporary_option = buffer_receive[counter++];

		//	printf("What the server sent us:\n");
/*
			for(int i = 0 ;buffer_receive[i]!='\0';i++){

				printf("%c",buffer_receive[i]);

			}
			printf("\n");
*/

			break;
		case 6:
			 break;
		case 7:

			break;
		default:

			break;

		}
}



void handleServerCommunication(int server_port){

	char buffer_send[BUFFER_SIZE]={0};

	char buffer_receive[BUFFER_SIZE]={0};

	struct sockaddr_in server_address={0};

	int bytes_receive = 0;

	int bytes_send = 0 ;

        bool quit = false;

        int option = 0;

	int client_file_descriptor = 0;


	setupConnection(&client_file_descriptor, &server_address, server_port);

	//printMenuSC();

	while(!quit){

	//	printf("Enter option: \n");

		do{

	//		scanf("%d", &option);

			option = 5;
			if(option<1 || option>7){

			//	printf("Error, invalid option, please enter a number between one and 7 (both included)\n");

			}

		}while(option<1 || option>7);

		handleoption(option, client_file_descriptor);

		if(option==7){

			quit = true;

		}

	}

}

int main(){

	printf("This is the client interface\n");

	int port = 8080;

	handleServerCommunication(port);
}
