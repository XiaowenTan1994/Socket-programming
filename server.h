#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<errno.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<fstream> 
#include<vector>
#include<sstream>
#include<string>
#include<iostream>
#include<math.h>
using namespace std;

bool m=0;
float bandwidth,length, velocity, Noise_power;
long int link_id_database;


int max(int a, int b)
{
	if (a >= b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
long int convert_stringtolongint(string s)
{
	long int num=0;
	int position=0;
	for (int i = s.length()-1; i >= 0; i--)
	{
		num = num + (s[i] - '0')*pow(10,position);
		position++;
	}
	return num;

}

float convert_stringtofloat(string s)
{
	float num=0;
	int dot_position;
	int e_position;
	int e = max(s.find('e'),s.find('E'));
	if (e > -1)  //for the data form like X.xxxE+02 or X.xxxe+02
	{
		int a = s.find('.'); //find the position of '.'
		int b = s.find('-');
		int integar_position = 0;
		int decimal_position = 1;
		int exponent=0;
		int integar_position_exponent = 0;
		for (int i = s.find('.') - 1; i >= 0; i--)
		{
			num = num + (s[i] - '0')*pow(10, integar_position);
			integar_position++;
		}
		for (int i = s.find('.') + 1; i < e; i++)
		{
			num = num + (s[i] - '0')*pow(0.1, decimal_position);
			decimal_position++;
		}
		for (int i = s.length() - 1; i > e + 1; i--)
		{
			exponent = exponent + (s[i] - '0')*pow(10, integar_position_exponent);
			integar_position_exponent++;
		}
		if (b>-1)
		{
			num = num * pow(0.1, exponent);
		}
		else
		{
			num = num * pow(10, exponent);
		}

	}
	else
	{
		int a = s.find('.'); //find the position of '.'
		if (a > -1)   //for the data form xxxx.xxx
		{
			int integar_position = 0;
			int decimal_position = 1;
			int minus=s.find('-');
			if(minus==-1)
			{
				for (int i = a - 1; i >= 0; i--)
				{
					num = num + (s[i] - '0')*pow(10, integar_position);
					integar_position++;
				}
				for (int i = a + 1; i < s.length(); i++)
				{
					num = num + (s[i] - '0')*pow(0.1, decimal_position);
					decimal_position++;
				}
			}
			else
			{
				for (int i = a - 1; i >= 1; i--)
				{
					num = num + (s[i] - '0')*pow(10, integar_position);
					integar_position++;
				}
				for (int i = a + 1; i < s.length(); i++)
				{
					num = num + (s[i] - '0')*pow(0.1, decimal_position);
					decimal_position++;
				}
				num=-num;
			}


		}
		else  //for the data form xxxxx
		{
			int position = 0;
			int minus=s.find('-');
			if(minus==-1)
			{
				for (int i = s.length()-1; i >= 0; i--)
				{
					num = num + (s[i] - '0')*pow(10,position);
					position++;
				}
			}
			else
			{
				for (int i = s.length()-1; i >= 1; i--)
				{
					num = num + (s[i] - '0')*pow(10,position);
					position++;
				}
				num=-num;
			}

		}
	}
	return num;
}

void search(char filename[],long int link_id)
{
	ifstream infile;
	infile.open(filename, ios::in);
	string lineStr;
	string::size_type sz;
	m=0;
	//float link, band, length, velocity, noise;
	while (getline(infile, lineStr))  // get data from databasw
									  // line 138-153 code is the reuse code from https://blog.csdn.net/u012234115/article/details/64465398
	{
		stringstream ss(lineStr);
		string str;
		vector<string> lineArray;
		while (getline(ss, str, ','))
		{
			lineArray.push_back(str);
		}
		link_id_database=convert_stringtolongint(lineArray[0]);
		if(link_id_database==link_id)  // if find same link id in database, m=1.
		{
			//cout<<lineArray[1]<<"  "<<lineArray[2]<<"   "<<lineArray[3]<<"   "<<lineArray[4]<<endl;
			bandwidth = convert_stringtofloat(lineArray[1]);
			length = convert_stringtofloat(lineArray[2]);
			velocity = convert_stringtofloat(lineArray[3]);
			Noise_power= convert_stringtofloat(lineArray[4]);
			//cout<<bandwidth<<"  "<<length<<"   "<<velocity<<"   "<<Noise_power<<endl;
			m=1;
			break;
		}
	}
}