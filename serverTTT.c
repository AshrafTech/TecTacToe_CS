#include<stdio.h>
#include<winsock2.h>


#pragma comment(lib,"ws2_32.lib") //Winsock Library


void displayBoard(char*);
void serverMove(char*, int);
void getIndex(char*);
int go = 1;
int x = 0;
int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int recv_size, y, row = 0, column = 0, winner = 0, c, j = 0;
	char board[3][3] = {                         /* The board */
		{ '1', '2', '3' },          /* Initial values are reference numbers */
		{ '4', '5', '6' },          /* used to select a vacant square for   */
		{ '7', '8', '9' }           /* a turn.                              */
	};
	printf("\nInitialising Winsock...");

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");


	if ((s = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	server.sin_family = PF_INET;
	server.sin_addr.s_addr = INADDR_ANY; //lets any ip connect
	server.sin_port = htons(20000); //port number 


	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) //might be just sockaddr with no _in
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	puts("Bind done");

	listen(s, 3);


	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr);

	new_socket = accept(s, (struct sockaddr *)&client, &c); //might be just sockaddr with no _in
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
	}

	puts("Connection accepted");
	//////////////////////////////////////CONNECTED////////////////////////////////////////////////////
	//////////////////////////////////////PLAY GAME///////////////////////////////////////////////////
	for (j = 0; j < 9 && winner == 0; j++) {
		 if ((recv_size = recv(new_socket, &x, sizeof(x), 0)) == SOCKET_ERROR)
		{
			puts("recv failed");
		}
		puts("Reply received from client\n");

	
		 getIndex(board);

		displayBoard(board);

		serverMove(board, x);
		
		displayBoard(board);

		//go = go++;
		if (send(new_socket, &go, sizeof(go), 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		puts("Data Send from the Client int\n");

		}
	}
	return 0;
}

void getIndex(char board[3][3]){
	int row, column;
	row = --x / 3;         /* Get row index of square      */
	column = x % 3;
	board[row][column] = 'X';
}
void displayBoard(char board[3][3])
{
	printf("\n\n");
	printf(" %c | %c | %c\n", board[0][0], board[0][1], board[0][2]);
	printf("---+---+---\n");
	printf(" %c | %c | %c\n", board[1][0], board[1][1], board[1][2]);
	printf("---+---+---\n");
	printf(" %c | %c | %c\n", board[2][0], board[2][1], board[2][2]);
}

void serverMove(char board[3][3], int x)
{
	int row, column;
	do
	{
		go = go + 1;
		row = (go - 1) / 3;                                 /* Get row index of square      */
		column = (go - 1) % 3;									/* Get column index of square   */			
	} while (go < 0 || go>9 || board[row][column]>'9' || go == x);
	board[row][column] = 'O';
	
}