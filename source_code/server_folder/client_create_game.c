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


	bytes_read =  read_all(client_file_descriptor,buffer_r, BUFFER_SIZE-1);


	return bytes_read;

}



void printMenuSC(){

	printf("Menu for Server communication\n");

	printf("1. Ask server for number of active players\n");

	printf("2. Ask server for all players names\n");

	printf("3. Message someone specificly\n");

	printf("4. Ask server for your incoming messages\n");

	printf("5. Create/change your name or send message to server\n");

	printf("6. Create a game\n");

	printf("7. Quit server communication\n");

}


void handleoption(int option, int file_descriptor){

	//no m'agrada el que estem, fent, el loop no esta dins d'aquesta funcio, la cpu ha d'alocar sempre que es crida aquseta funcio,a apart dels paramtres totes les va3riables que utiltizem,que pot ser costos.
	//per ara ho deixerem, pero aixo es valor computacional gastat, no cal repetir-ho, mentres que buffer_send , buffer_receive sempre es fa un reset abans de ser utitlizats un altre cop, no tindremproblemes.
	//pero un altre cop no ho canviarem per ara.
	char buffer_send[BUFFER_SIZE] = {0};

	char buffer_receive[BUFFER_SIZE] = {0};

	memset(buffer_send, 0, sizeof(buffer_send));

	memset(buffer_receive,0,sizeof(buffer_send));

	char* temporary_buffer;

	int temporary_option = 0 ;

	int counter = 0;

	buffer_send[counter++] = option + '0';

	buffer_send[counter++] = '|';

	switch(option){

		case 1:
		case 2:
		case 3:
		case 4:
		
			temporary_buffer = "protocol has not been defined yet";

			break;


		case 5:

			temporary_buffer= "This is the message that we want to send";

			break;


		case 6:

			//we can ask the user first for a name of the game that they want. For now no authentification is needed to be able to use the video game or simply just enter a game, you just see it in the menu, and then....
			printf("Enter a name for the game, do not make the name very long (max 62 bytes)\n");
			scanf("%s", temporary_buffer);

			break;

		case 7:

			//here we are just trying to quit

			temporary_buffer ="this is the client, we are closing";

			break;


		default:
			printf("Error, no message will be relayed to the sever, invalid option enterd\n");

			temporary_buffer ="message is not valid";

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

	printMenuSC();

	while(!quit){

	//	printf("Enter option: \n");

		do{

	//		scanf("%d", &option);

			printf("please enter your option:\n");

			scanf("%d", &option);
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
