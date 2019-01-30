#include"client.h"
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <iomanip>
using namespace std;

int main(int argc, char *argv[])
{
	string link_id_string,size_string,power_string;

	// get link_id size power from command line argument
	link_id_string=argv[1];
	size_string=argv[2];
	power_string=argv[3];



	int client_socket;
	client_socket=socket(AF_INET, SOCK_STREAM, 0);//create TCP socket
	struct sockaddr_in server_address;//specify an address for the socket
	
	//code line 19-27 is the reused code form https://www.youtube.com/watch?v=LtXEMwSG5-8
	//about how to use bind function, send function, recv function is from https://www.youtube.com/watch?v=LtXEMwSG5-8
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(25495);//port number
	server_address.sin_addr.s_addr=inet_addr("127.0.0.1");//localhost IP address

	if(connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address))==-1)//connect to aws server
	{
		cout<<"connection is not established"<<endl;
	}
	else
	{
		cout<<"The client is up and running"<<endl;
	}

	long int link_id,size;
	float power;
	float delay;
	bool flag_AB;


	//cout<<"The client is up and running"<<endl;
	while(1)
	{
		// Convert link_id,size,power to float
		link_id=convert_stringtolongint(link_id_string);
		size=convert_stringtolongint(size_string);
		power=convert_stringtofloat(power_string);
		cout<<fixed<<setprecision(2);
		// Send link_id size power to AWS server
		if((send(client_socket, &link_id, sizeof(link_id),0)>-1)&&(send(client_socket, &size, sizeof(size),0)>-1)&&(send(client_socket, &power, sizeof(power),0)>-1))
		{
			cout<<"The client sent ID = <"<<link_id<<">, size = <"<<size<<"> bit, and power = <"<<power<<">dB to AWS"<<endl;
		}
		else
		{
			cout<<"The client failed to send data to AWS"<<endl;
		}
		// get the infromation whether there is a match in server A and server B, if it has, flag_AB=1; if there is no match, flag_AB=0;
		if(recv(client_socket,&flag_AB, sizeof(flag_AB),0)<0)
		{
			cout<<"The clietn failed to received flag_AB from AWS"<<endl;
		}
		if(flag_AB)
		{
			//get the value of delay from the aws server
			if(recv(client_socket,&delay, sizeof(delay),0)<0)
			{
				cout<<"The client failed to received data from AWS"<<endl;
			}
			cout<<"The delay for link <"<<link_id<<"> is <"<<delay<<"> ms"<<endl;
		}
		else
		{
			cout<<"Found no matches for link <"<<link_id<<">"<<endl;
		}
		break;

	}
	close(client_socket);
	exit(0);
	return 0;
}