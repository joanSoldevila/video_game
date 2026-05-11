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

int handleWaitGame(int file_descriptor){

//when we call handleWaitGame, it is because we have created a game, but now we wait for some other person to be able to connect to the game
//in socket programming the function read sits in idle until it recives bytes, which is way we are calling read_all.
//sense this function is very simple we are probably just going to put it direcly in the main loop of the client, we will change this in the future.
int number_of_bytes = read_all(file_descriptor, buffer, length);

//after this depending on how many bytes we have received, we are going to have some type of error, or everyting was done correcty

//we are going to try to return what is the client state.
//the values that we are returning will have to be revweid
if(number_of_bytes<=0){

printf("Error, we have received 0 bytes: indicates that connection was lost\n");


return 1;

}else{

return 0;

}

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


		}else if(temporary_option = 2){





		}

	}

}

int main(){

	printf("This is the client interface\n");

	int port = 8080;

	handleServerCommunication(port);
}
