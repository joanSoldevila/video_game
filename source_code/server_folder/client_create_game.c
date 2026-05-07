#include "client_create_game.h"
void setupConnection(int* client_file_descriptor,struct sockaddr_in* server_address, int port){

*(client_file_descriptor) = socket(AF_INET, SOCK_STREAM,0);


server_address->sin_family = AF_INET;
server_address->sin_port = htons(port);

int result_translation= inet_pton(AF_INET, "127.0.0.1", &server_address->sin_addr);

int status = connect(*(client_file_descriptor), (struct sockaddr*)server_address, sizeof(*(server_address)));

}
int send_data_to_server(char* buffer_r,char* buffer_s , int client_file_descriptor){

int bytes_sent = send(client_file_descriptor, buffer_s,strlen(buffer_s),0);
int bytes_read = read(client_file_descriptor,buffer_r, 1023);
if(bytes_read>=0 && (bytes_read-1)){
buffer_r[bytes_read] = '\0';
}
return bytes_read;
}


void printMenuSC(){

	printf("Menu for Server communication\n");
	printf("1. Ask server for number of active players");
	printf("2. Ask server for all players names");
	printf("3. Message someone specificly");
	printf("4. Ask server for your incoming messages");
	printf("5. Create/change your name\n");
	printf("6. Quit server communication\n");
}
void handleoption(int option, int file_descriptor){
	buffer_send[0] = option + '0';
	bytes_receive = send_data_to_server(buffer_receive, buffer_send , client_file_descriptor);
	printf("Number of bytes received from server: %d\n", bytes_receive);
	printf("Servers resposne: \n%s",buffer_receive);
	int temorary_option = 0 ;
	int counter = 0;
	switch(option){

		case 1:
			temporary_option = buffer_receive[counter++];
			++counter;
			int active_players = buffer_receive[counter++];
			printf("Number of active players: %d\n", active_players);
			break;
		case 2:
			//here we are going to ask for how many active players the server has. in the server side we might need to define a buffer that holds all file descriptors 

			//for every message that the server  responds, the first character of the incomiung message is the option that we have chosen. After that the protocol changes based on what option we ahve chosen.
			//in this case, after receving the optoin, there will be a "|"to separate the number of active players. After that another "|" will be used to separate protocol sepcific information to actual information that we have asked for or the lcient asked for.
			//example of incoming message of the server: 1|67|user_1;user_2;user_4; ....
			
			temporary_option = buffer_receive[counter++];
			++counter;
			int active_players = buffer_receive[counter++] - '0';
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
			break;
		case 6:
			//here is where we quit communication with the server 
			close(file_descriptor);
			 break;


}
}
void handleServerCommunication(int server_port){
	char buffer_send[1024]={0};
	char buffer_receive[1024]={0};
	struct sockaddr_in server_address={0};
	int bytes_receive = 0;
	int bytes_send = 0 ;
        bool quit = false;
        int option = 0;
	int client_file_descriptor = 0;

	setupConnection(&client_file_descriptor, &server_address, server_port);
	printMenuSC();
	while(!quit){
		printf("Enter option: \n");
		scanf("%d", &option);
		handleoption(option, client_file_descriptor);
		if(option==6){

			quit = true;

		}

	}

}
int main(){
printf("This is the client interface\n");
int port = 8080;
handleServerCommunication(port);
}
