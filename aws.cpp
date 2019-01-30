#include"aws.h"
#include <iomanip>


int main()
{
	//code line 10-50 is the reused code form https://www.youtube.com/watch?v=LtXEMwSG5-8
	//about how to use bind function, send function, recv function is from https://www.youtube.com/watch?v=LtXEMwSG5-8
	//abput how to use sendto function, recvfrom is from https://www.youtube.com/watch?v=Emuw71lozdA
	// socket in the interface connected to client 
	int aws_socket=socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in aws_address; //set the IP address and port number of the socket connected to client  
	aws_address.sin_family=AF_INET;
	aws_address.sin_port=htons(25495);
	aws_address.sin_addr.s_addr=inet_addr("127.0.0.1");

	//socket in the interface connected to monitor
	int monitor_socket=socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in monitor_address;
	monitor_address.sin_family=AF_INET;
	monitor_address.sin_port=htons(26495);
	monitor_address.sin_addr.s_addr=inet_addr("127.0.0.1");


	//int serverA_socket=socket(AF_INET, SOCK_DGRAM,0);
	//Set serverA IP address and port number
	struct sockaddr_in serverA_address;
	serverA_address.sin_family=AF_INET;
	serverA_address.sin_port=htons(21495);
	serverA_address.sin_addr.s_addr=inet_addr("127.0.0.1");

	//int serverB_socket=socket(AF_INET, SOCK_DGRAM,0);
	//Set serverB IP address and port number
	struct sockaddr_in serverB_address;
	serverB_address.sin_family=AF_INET;
	serverB_address.sin_port=htons(22495);
	serverB_address.sin_addr.s_addr=inet_addr("127.0.0.1");

	//int serverC_socket=socket(AF_INET, SOCK_DGRAM,0);
	//Set serverC IP address and port number
	struct sockaddr_in serverC_address;
	serverC_address.sin_family=AF_INET;
	serverC_address.sin_port=htons(23495);
	serverC_address.sin_addr.s_addr=inet_addr("127.0.0.1");

	//socket in the interface connect to serverA, serverB and serverC
	int udp_socket=socket(AF_INET, SOCK_DGRAM,0);
	struct sockaddr_in udp_address;
	udp_address.sin_family=AF_INET;
	udp_address.sin_port=htons(24495);
	udp_address.sin_addr.s_addr=inet_addr("127.0.0.1");

    //bind sockets
    if((bind(aws_socket,(struct sockaddr*) &aws_address, sizeof(aws_address))>-1)&&(bind(monitor_socket,(struct sockaddr*) &monitor_address, sizeof(monitor_address))>-1)&&(bind(udp_socket,(struct sockaddr*) &udp_address, sizeof(udp_address))>-1))
    {
    	//printf("The AWS is up and running\n");
    }
    else
    {
    	printf("binding server failed\n");
    }

    //wait for the connection from monitor and client
	listen(aws_socket, 30);
	int client_socket;
	client_socket=accept(aws_socket, NULL,NULL);// establish connection between AWS and client

	listen(monitor_socket,30);
	int monitor_client;
   	monitor_client=accept(monitor_socket,NULL,NULL);//establish connection between AWS and monitor



    socklen_t addr_lenA=sizeof(serverA_address);
    socklen_t addr_lenB=sizeof(serverB_address);
    socklen_t addr_lenC=sizeof(serverC_address);


    long int link_id,size;
	float power;
	bool m_A, m_B,flag_AB;
	float Tt, Tp,delay;
	float bandwidth,length, velocity, Noise_power;
	printf("The AWS is up and running\n");
	bool inital=false;


	while(1)
	{
		//listen(aws_socket, 30);
		//int client_socket;
		if(inital)
		{
			listen(aws_socket, 30);
			client_socket=accept(aws_socket, NULL,NULL);
		}
		inital=true;
		
		// receive link id, size, power from the client
		if((recv(client_socket,&link_id, sizeof(link_id),0)>-1) &&(recv(client_socket,&size, sizeof(size),0)>-1) &&(recv(client_socket,&power, sizeof(power),0)>-1))
		{
			printf("The AWS received link ID = <%ld>, size = <%ld> bit, and power = <%.2f>dB from the client using TCP over port <25495>\n",link_id,size,power);		
		
			// send link id, size, power to monitor	
			if((send(monitor_client,&link_id, sizeof(link_id),0)>-1) &&(send(monitor_client,&size, sizeof(size),0)>-1) &&(send(monitor_client,&power, sizeof(power),0)>-1))
			{
				printf("The AWS sent link ID = <%ld>, size = <%ldf> bit, and  power = <%.2f>dB to the monitor using TCP over port <26495>\n",link_id,size,power);
			}
			else
			{
				printf("The data AWS sent to monitor is wrong\n"); break;
			}
			// send link id to serverA
			if(sendto(udp_socket,&link_id,sizeof(link_id),0, (struct sockaddr*) &serverA_address, addr_lenA)>-1)
			{
				printf("The AWS sent link ID = <%ld> to Backend-Server <A> using UDP over port <24495> \n",link_id);
			}
			else
			{
				printf("The data AWS sent to the Server A is wrong\n");break;
			}
			//get to know whether server A find a match. When m_A=1, server A finds a match. When m_A=0, server A finds 0 match
			if(recvfrom(udp_socket,&m_A, sizeof(m_A),0, (struct sockaddr*) &serverA_address, &addr_lenA)>-1)
			{
				if(m_A)
				{
					recvfrom(udp_socket,&bandwidth, sizeof(bandwidth),0, (struct sockaddr*) &serverA_address, &addr_lenA);
					recvfrom(udp_socket,&length, sizeof(length),0, (struct sockaddr*) &serverA_address, &addr_lenA);
					recvfrom(udp_socket,&velocity, sizeof(velocity),0, (struct sockaddr*) &serverA_address, &addr_lenA);
					recvfrom(udp_socket,&Noise_power, sizeof(Noise_power),0, (struct sockaddr*) &serverA_address, &addr_lenA)	;		
					printf("The AWS received 1 match from Backend-Server <A> using UDP over port <24495>\n");
					//printf("bandwidth %g; length %g; velocity %g; Noise_power %g", bandwidth, length, velocity, Noise_power);
				}
				else
				{
					printf("A received 0 match from Backend-Server <A> using UDP over port <24495>\n");
				}
			}
			else
			{
				printf("The AWS failled received data from server B\n");break;
			}
			//get to know whether server B find a match. When m_B=1, server B finds a match. When m_B=0, server B finds 0 match
			if(sendto(udp_socket,&link_id,sizeof(link_id),0, (struct sockaddr*) &serverB_address, addr_lenB)==-1)
			{
				printf("The AWS failled to send data to server B\n");
			}
			else
			{
				if(recvfrom(udp_socket,&m_B, sizeof(m_B),0, (struct sockaddr*) &serverB_address, &addr_lenB)>-1)
				{
					if(m_B)
					{
						recvfrom(udp_socket,&bandwidth, sizeof(bandwidth),0, (struct sockaddr*) &serverB_address, &addr_lenB);
						recvfrom(udp_socket,&length, sizeof(length),0, (struct sockaddr*) &serverB_address, &addr_lenB);
						recvfrom(udp_socket,&velocity, sizeof(velocity),0, (struct sockaddr*) &serverB_address, &addr_lenB);
						recvfrom(udp_socket,&Noise_power, sizeof(Noise_power),0, (struct sockaddr*) &serverB_address, &addr_lenB)	;		
						printf("AWS received 1 match from Backend-Server <B> using UDP over port <24495>\n");
						//printf("bandwidth %g; length %g; velocity %g; Noise_power %g", bandwidth, length, velocity, Noise_power);
					}
					else
					{
						printf("The AWS received 0 match from Backend-Server <B> using UDP over port <24495>\n");
					}
				}
				else
				{
					printf("The AWS failed to receive data from server B\n");break;
				}
			
			}








			flag_AB=m_B||m_A;
			//tell client and monitor whether serverA and serverB found a match. when flag_AB=1, find a match; when flag_AB=0, find 0 match.
			send(client_socket,&flag_AB, sizeof(flag_AB),0);
			send(monitor_client,&flag_AB, sizeof(flag_AB),0);
			if(flag_AB)
			{	// send detailed information to serverC
				if((sendto(udp_socket,&flag_AB,sizeof(flag_AB),0, (struct sockaddr*) &serverC_address, addr_lenC)>-1)
					&&(sendto(udp_socket,&link_id,sizeof(link_id),0, (struct sockaddr*) &serverC_address, addr_lenC)>-1)
					&&(sendto(udp_socket,&bandwidth,sizeof(bandwidth),0, (struct sockaddr*) &serverC_address, addr_lenC)>-1)
					&&(sendto(udp_socket,&size,sizeof(size),0, (struct sockaddr*) &serverC_address, addr_lenC)>-1)
					&&(sendto(udp_socket,&power,sizeof(power),0, (struct sockaddr*) &serverC_address, addr_lenC)>-1)
					&&(sendto(udp_socket,&velocity,sizeof(velocity),0, (struct sockaddr*) &serverC_address, addr_lenC)>-1)
					&&(sendto(udp_socket,&length,sizeof(length),0, (struct sockaddr*) &serverC_address, addr_lenC)>-1)
					&&(sendto(udp_socket,&Noise_power,sizeof(Noise_power),0, (struct sockaddr*) &serverC_address, addr_lenC)>-1))
				{
					printf("The AWS sent link ID = <%ld>, size = <%ld>, power = <%.2f>dB, and link information to Backend-Server <C> using UDP over port <24495>\n", link_id,size,power);
					/// receive computed result from serverC
					if((recvfrom(udp_socket,&Tt, sizeof(Tt),0, (struct sockaddr*) &serverC_address, &addr_lenC)>-1)
						&&(recvfrom(udp_socket,&Tp, sizeof(Tp),0, (struct sockaddr*) &serverC_address, &addr_lenC)>-1)
						&&(recvfrom(udp_socket,&delay, sizeof(delay),0, (struct sockaddr*) &serverC_address, &addr_lenC)>-1))
					{
						printf("The AWS received outputs from Backend-Server C using UDP over port <24495>\n");
					}
					else
					{
						printf("The AWS failled to receive the data from server C\n");break;
					}
					// send result to client
					if(send(client_socket,&delay, sizeof(delay),0)>-1)
					{
						printf("The AWS sent delay = <%.2f>ms to the client using TCP over port <25495>\n", delay);
					}
					else
					{
						printf("The AWS failled to send the data to client\n");
					}
					//send detailed result to monitor
					if((send(monitor_client,&Tt, sizeof(Tt),0)>-1)&&(send(monitor_client,&Tp, sizeof(Tp),0)>-1)&&(send(monitor_client,&delay, sizeof(delay),0)>-1)) //send Tt, Tp, delay to monitor
					{
						printf("The AWS sent detailed result to the monitor using TCP over port <26495>\n");
					}
				}
				else
				{
					printf("The AWS failed to send data from the server C\n");break;
				}

			}
			else
			{
				printf("The AWS sent No Match to the monitor and the client using TCP over ports <26495> and <25495>, respectively \n");
			}
		}
		else
		{
			printf("The data from client is not received\n"); break;
		}
		close(client_socket);

	}


    close(monitor_socket);
	close(aws_socket);
	close(udp_socket);
	return 0;
}
