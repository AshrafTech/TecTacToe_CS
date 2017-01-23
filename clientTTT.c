#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library


char getIndex(int, int);
void displayBoard();
void serverMove(int, int);
void checkWinner();
void checkWinner1();

int x = 0, go = 0, winner = 0;
char board[3][3] = {                         /* The board */
	{ '1', '2', '3' },          /* Initial values are reference numbers */
	{ '4', '5', '6' },          /* used to select a vacant square for   */
	{ '7', '8', '9' }           /* a turn.                              */
};
int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	int recv_size, y, row = 0, column = 0;


	printf("\nInitializing Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialized.\n");

	if ((s = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr("169.254.38.128"); //hardcode the ip address
	server.sin_family = PF_INET;
	server.sin_port = htons(20000); //hardcode the port

	if (connect(s, (struct sockaddr_in *)&server, sizeof(server)) < 0) //might just be sockaddr with no _in
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");

	//////////////////////////////////////CONNECTED////////////////////////////////////////////////////
	//////////////////////////////////////PLAY GAME///////////////////////////////////////////////////

		for (int i = 0; i < 8 && winner == 0; i++) {
			displayBoard();
			getIndex(row, column);
			if (send(s, &go, sizeof(go), 0) < 0)
			{
				puts("Send failed");
				return 1;
			}
			puts("Data Send from the Client int\n");
			displayBoard();
			checkWinner();
			if (winner == 1) break;
			if ((recv_size = recv(s, &x, sizeof(x), 0)) == SOCKET_ERROR)
			{
				puts("recv failed");
			}
			puts("Reply received from server\n");


			/* Check for a winning line - diagonals first */
			serverMove(column, row);
			displayBoard();
			checkWinner1();
			if (winner == 2) break;
			if (i == 8){
				break;
			}
		}

		if (winner == 1){
			printf("%d Client Won the game", winner);

		}
		else if (winner == 2){
			printf("%d Server Won the game", winner);

		}
		else if (winner == 0){
			printf("It is a tie");
			
		
	}

	return 0;
}


char getIndex(int row, int column)
{
	row = 0;
	column = 0;
	do
	{
		printf("\nWhere do you want to place your X: ");
		scanf("%d", &go);
		printf("%d", go);

		row = (go - 1) / 3;                                 /* Get row index of square      */
		column = (go - 1) % 3;								/* Get column index of square   */

	} while (go < 0 || go>9 || board[row][column] > '9');
	board[row][column] = 'X';
	

}


void displayBoard()
{
	printf("\n\n");
	printf(" %c | %c | %c\n", board[0][0], board[0][1], board[0][2]);
	printf("---+---+---\n");
	printf(" %c | %c | %c\n", board[1][0], board[1][1], board[1][2]);
	printf("---+---+---\n");
	printf(" %c | %c | %c\n", board[2][0], board[2][1], board[2][2]);
}


void serverMove(int column, int row)
{
	column = 0;
	row = 0;

	row = (x - 1) / 3;             /* Get row index of square      */
	column = (x - 1) % 3;

	board[row][column] = 'O';


}


void checkWinner(){

	if ((board[0][0] == board[1][1] && board[0][0] == board[2][2]) || (board[0][2] == board[1][1] && board[0][2] == board[2][0]))
		winner = 1;

	else
	for (int line = 0; line <= 2; line++){/* Check rows and columns for a winning line*/
		if ((board[line][0] == board[line][1] && board[line][0] == board[line][2]) || (board[0][line] == board[1][line] && board[0][line] == board[2][line]))
			winner = 1;

	}

}
void checkWinner1(){

	if ((board[0][0] == board[1][1] && board[0][0] == board[2][2]) || (board[0][2] == board[1][1] && board[0][2] == board[2][0]))
		winner = 2;

	else
	for (int line = 0; line <= 2; line++){/* Check rows and columns for a winning line*/
		if ((board[line][0] == board[line][1] && board[line][0] == board[line][2]) || (board[0][line] == board[1][line] && board[0][line] == board[2][line]))
			winner = 2;


	}

}