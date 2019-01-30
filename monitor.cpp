#include"client.h"
#include <iomanip>
int monitor_socket=socket(AF_INET, SOCK_STREAM,0); //create the TCP socket 
struct sockaddr_in monitor_address;

int main()
{
	//code line 17-22 is the reused code form https://www.youtube.com/watch?v=LtXEMwSG5-8
	//about how to use bind function, send function, recv function is from https://www.youtube.com/watch?v=LtXEMwSG5-8
	int monitor_socket;
	char aws_message[1000];
	monitor_socket=socket(AF_INET, SOCK_STREAM, 0);//create TCP socket

	struct sockaddr_in aws_address;//specify an address for the socket
	

	aws_address.sin_family=AF_INET;
	aws_address.sin_port=htons(26495);// port number
	aws_address.sin_addr.s_addr=inet_addr("127.0.0.1");//IP address


	cout<<"monitor is up and running"<<endl;


	connect(monitor_socket, (struct sockaddr*) &aws_address, sizeof(aws_address));//connect to AWS server

	while(1)
	{
		long int link_id,size;
		float power,delay,Tt,Tp;
		bool flag_AB;
		cout<<fixed<<setprecision(2);
		//receive link_id, size, power from AWS server
		if((recv(monitor_socket,&link_id, sizeof(link_id),0)>0) &&(recv(monitor_socket,&size, sizeof(size),0)>0) &&(recv(monitor_socket,&power, sizeof(power),0)>0))
		{
			cout<<"The monitor received link ID = <"<<link_id<<">, size = <"<<size<<">bit, and power = <"<<power<<">dB from the AWS"<<endl;
		}
		else
		{
			cout<<"     "<<endl;break;
		}
		//get to know whether there is a match found by server A and server B
		if((recv(monitor_socket,&flag_AB, sizeof(flag_AB),0)>0))
		{
			if(flag_AB)
			{
				// receive the detailed result from AWS server
				if((recv(monitor_socket,&Tt, sizeof(Tt),0)>0)&&(recv(monitor_socket,&Tp, sizeof(Tp),0)>0)&&(recv(monitor_socket,&delay, sizeof(delay),0)>0))
				{
					cout<<"The result for link <"<<link_id<<">:"<<endl<<"Tt = <"<<Tt<<">ms"<<endl<<"Tp = <"<<Tp<<">ms"<<endl<<"Delay=<"<<delay<<">ms"<<endl;
				}
				else
				{
					cout<<"The monitor failed to received data from the AWS"<<endl;
				}
			}
			else// no match found
			{
				cout<<"Found no matches for link <"<<link_id<<">"<<endl;
			}
		}
		else
		{
			cout<<"The monitor failed to receive data from the AWS"<<endl;
		}
		//cout<<"1"<<endl;

	}
	close(monitor_socket);

	return 0;
}