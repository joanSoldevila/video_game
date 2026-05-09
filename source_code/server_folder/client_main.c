#include "client_main.h"
#include "client_send_data.h"
void setupConnection(int* client_file_descriptor,struct sockaddr_in* server_address, int port){

	*(client_file_descriptor) = socket(AF_INET, SOCK_STREAM,0);

	server_address->sin_family = AF_INET;

	server_address->sin_port = htons(port);


	int result_translation = 0;

	int status = 0;


	result_translation =  inet_pton(AF_INET, "127.0.0.1", &server_address->sin_addr);

	status = connect(*(client_file_descriptor), (struct sockaddr*)server_address, sizeof(*(server_address)));

}


void printMenuSC(){

	printf("Menu for Server communication\n1. Ask server for number of active players\n2. Ask server for all players names\n3. Message someone specificly\n4. Ask server for your incoming messages\n5. Create/change your name or send message to server\n6. Create a game\n7. Quit server communication\n");
}

int handleoption(int option, int file_descriptor){

	char buffer_send[BUFFER_SIZE] = {0};

	char buffer_receive[BUFFER_SIZE] = {0};

	memset(buffer_send, 0, sizeof(buffer_send));

	memset(buffer_receive,0,sizeof(buffer_send));

	char* temporary_buffer;

	int temporary_option = 0, counter = 0, return_state = 0;

	buffer_send[counter++] = option + '0';

	buffer_send[counter++] = '|';


	switch(option){

		case 1:
		case 2:
		case 3:
		case 4:

			//for now we are going to use no memory here
			return_state = 0;
			temporary_buffer = "protocol has not been defined yet";

			break;


		case 5:

			temporary_buffer= "This is the message that we want to send";

			return_state = 0;
			break;


		case 6:

			//we can ask the user first for a name of the game that they want. For now no authentification is needed to be able to use the video game or simply just enter a game, you just see it in the menu, and then....
			printf("Enter a name for the game, do not make the name very long (max 62 bytes)\n");
			scanf("%s", temporary_buffer);
			return_state = 1;
			break;

		case 7:

			//here we are just trying to quit


			return_state = 0;
			temporary_buffer ="this is the client, we are closing";

			//here state does not matter, the connection is being closed
			break;


		default:
			printf("Error, no message will be relayed to the sever, invalid option enterd\n");

			temporary_buffer ="message is not valid";

			//FOR INVALIED MESSAGEA, it can remain statless

			return_state = 0;

			break;
		}

	for(int i = 0;temporary_buffer[i]!='\0';i++){

		buffer_send[counter++]=temporary_buffer[i];

	}


	int bytes_receive = send_data_to_server(buffer_receive, buffer_send , file_descriptor);

	printf("Server respondse:  ");

	for(int i =0;buffer_receive[i]!='\0';i++){

		printf("%c", buffer_receive[i]);

	}

	printf("\n\n");

	return return_state;
}



void handleServerCommunication(int server_port){

	char buffer_send[BUFFER_SIZE]={0};

	char buffer_receive[BUFFER_SIZE]={0};

	struct sockaddr_in server_address={0};

	int bytes_receive = 0, bytes_send = 0, option = 0, client_file_descriptor = 0, temporary_option = 0;

        bool quit = false;


	setupConnection(&client_file_descriptor, &server_address, server_port);

	printMenuSC();

	while(!quit){


		if(temporary_option == 0){

		do{

			printf("please enter your option:\n");

			scanf("%d", &option);
			if(option<1 || option>7){
				printf("input option invalid\n");
			}

		}while(option<1 || option>7);

		temporary_option = handleoption(option, client_file_descriptor);

		if(option==7){

			quit = true;

		}

		}else if(temporary_option == 1){//we need memory

			//in this case we are going to be waiting until the server sends us somehting, so we are going to remain idle until

			//this is the part that requires memory: after users enters the value 6, the process of creating a game begins: we send the tcp package to the server, the serever then creates the game, and if  max capacity has not been reached,the game will be created.
			//after this, temporary_option is set to 1, we need to wait until the server responds to use with a message indicating that someone has enterd the our lobby and the game can begin.
			//we need to continue to think about how the protocol is going to work, the architecutre aswell as implement the code for this to happen

//we are commenting this line because we have not defined the protocol yet	temporary_option = handleoption(); //we are going to have to add a case,where w wait the server to give us a player for our game

		}else if(temporary_option = 2){





		}

	}

}

int main(){

	printf("This is the client interface\n");

	int port = 8080;

	handleServerCommunication(port);
}
