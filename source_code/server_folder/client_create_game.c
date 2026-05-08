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

int calculate_amount_data(char* buffer){

	int i;


	for(i =0;(i<buffer[i]!='\0');i++);

	return i;

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

			printf("Error, the connection was lost, not bytes where sent\n");

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

	//we are going to have to check if the amount of data in buffer_s is equal to bytes_sent

	int bytes_sent_client = 0;

	bytes_sent_client = calculate_amount_data(buffer_s);

	bytes_sent = send_all(client_file_descriptor, buffer_s,BUFFER_SIZE);

	if(bytes_sent != bytes_sent_client){

		printf("Error, no estem enviant els mateixos bytes");

		printf("bytes que hem comptat abans d'enviar: %d\n", bytes_sent_client);

		printf("bytes que hem comptat despres d'enviar: %d\n", bytes_sent);
	}

	bytes_read =  read_all(client_file_descriptor,buffer_r, BUFFER_SIZE-1);

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



//this function is called when the user enterd the server-menu option
//depending on the option, the server will respond with different messages using different protocols
/*

in handle option we need to be carefull with the following things:

bytes size for buffers.

receving everthing that is being sent



remember that in this case, latency is not a very big issue, this will become a problem when sending udp packates when a game is in session.

we need a few buffers:

buffer_send > this will contain what we want to send to the server

buffer_receive > this will contain what the server has sent to use

*/

void handleoption(int option, int file_descriptor){

	char buffer_send[BUFFER_SIZE]="5hello this is the client";


	char buffer_receive[BUFFER_SIZE] = {0};

	int temporary_option = 0 ;

	int counter = 0;

	buffer_send[0] = option + '0';

//	buffer_send[1] = '\0'; //we need to end it with a null terminator


	int bytes_receive = send_data_to_server(buffer_receive, buffer_send , file_descriptor);

	printf("Number of bytes received from server: %d\n", bytes_receive);

	printf("Servers resposne: \n%s",buffer_receive);

	switch(option){

		case 1:

			temporary_option = buffer_receive[counter++];

			++counter;

			int active_players = 0;

			active_players = buffer_receive[counter++];

			printf("Number of active players: %d\n", active_players);

			break;

		case 2:
			//here we are going to ask for how many active players the server has. in the server side we might need to define a buffer that holds all file descriptors 

			//for every message that the server  responds, the first character of the incomiung message is the option that we have chosen. After that the protocol changes based on what option we ahve chosen.
			//in this case, after receving the optoin, there will be a "|"to separate the number of active players. After that another "|" will be used to separate protocol sepcific information to actual information that we have asked for or the lcient asked for.
			//example of incoming message of the server: 1|67|user_1;user_2;user_4; ....

			temporary_option = buffer_receive[counter++];

			++counter;

			active_players = buffer_receive[counter++] - '0';

			++counter;

			//remember that everything is indexed to zero, and the last character is '\0'(null termiantor) 
			printf("Number of active players: %d\n", active_players);

			printf("Active player names:\n");

			for(int i = 0 ;i<active_players;i++){

				for (int j =counter;buffer_receive[j]!=';';j++){

					printf("%c",buffer_receive[j]);

				}

				printf("\n");
			}

			break;

		case 3:

			temporary_option = buffer_receive[counter++];

			//this is something very specific and will later be implemented

			break;

		case 4:

			//this is something very specific and will later be implemented

			break;

		case 5:

			//this is something very specific and will later be implemented

			//for now we are going to be using number 5 for sending simple messages to the server just to make sure taht everything is working in intercommunication
			//we are also going to be stress testing it.
			temporary_option = buffer_receive[counter++];

//			++counter;

			//now we are going to print the number that we have just received

			printf("What the server sent us:\n");

			for(int i = 0 ;buffer_receive[i]!='\0';i++){

				printf("%c",buffer_receive[i]);

			}
			printf("\n");


			break;
		case 6:
			//this is one of the most important things. This is where we create a game.

			//when creating a game a few things most happen:_

			/*
			after explaning to the sergver that we have created a game, the  creates the spaces, indicates that the only player in the game is you and adds the game to a buffer for games that need a player.

			*/

			 break;
		case 7:

			close(file_descriptor);

			break;
		default:

			printf("Error, invalid number\n");

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

	//preparing the addresses and type of connection we are going to be using

	setupConnection(&client_file_descriptor, &server_address, server_port);

	printMenuSC();

	while(!quit){

		printf("Enter option: \n");

		do{

			scanf("%d", &option);

			if(option<1 || option>7){

				printf("Error, invalid option, please enter a number between one and 7 (both included)\n");

			}

		}while(option<1 || option>7);

		handleoption(option, client_file_descriptor);

		if(option==7){

			quit = true;

		}

	}

}
/*
lets go threw everything again:
the user in the main loop in main.c decides that he or she wants to interact with the server.
The user then  enters the value number 1 and handleServerCommunication is called with a port number which has been hardcoded into the client side code.
The following explanation is in the scope of handleServerCommunication:

we first setupConnection-> this function creates a socket using IPv4 IP address version aswell as the server port and connects.
now we have the file descriptor of the scoket which is now conected to the server.
After connecting to the server,we can now ask the user what he or shee wants to do.
Keep in mind that we might need to change this, maybe the tcp connection that we have establihes closes before the user tries to enter something, meaning, maybe we should create the connection only when we really need it, not at the beginning of handleServerCommunication().
but for now we are going to leave it like this.
After setting up the tcp connection, we now we wait for user input. The options or now hardcoded into the server because this game is not going to be very complicated. If we wanted to make it more scalable we would probably not hardcode the options into the client side, but rather have the server show us the menu, and the client side sends back what the user wants to do.
Our videogame is not going to overcomplicate things, so the options will just be harcoded, making a sharded protocol between server and client.
After entering the option, we call handleoption
*/

int main(){

	printf("This is the client interface\n");

	int port = 8080;

	handleServerCommunication(port);
}
