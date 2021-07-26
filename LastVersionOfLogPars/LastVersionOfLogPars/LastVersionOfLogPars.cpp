#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iterator>
#include <math.h>
using namespace std;


void rollTheBitrate(long long int &bitrate)
{
	int digitCount = 0;
	long long int temp = bitrate;
	string tempString = to_string(bitrate);
	while (temp > 0)
	{
		temp /= 10;
		digitCount++;
	}

	for(int i = digitCount - 1; digitCount > 5 && i > 1; i--)
	{
		tempString[i] = '0';
	}
	bitrate = stoi(tempString);
	
}



class ParsedLog
{
	string ip;
	string eventTime;
	string requestMethod;
	string contentAddress;
	string contentName;
	long long int bitrate;
	string contentType;
	int httpStatus;
	int totalSentBytes;
	vector <string> userAgent;
	string cacheStatus;
public:
	ParsedLog(string, string, string, string, string, int, string, int, int, vector <string>, string);
	int getTotalByte() { return totalSentBytes; }
	long long int getBitrate() { return bitrate; }
	vector <string> getUserAgent() { return userAgent; }
	int gethttpStatus() {return httpStatus;}
};

ParsedLog::ParsedLog(string ip_, string event_time, string requested_method, string content_address, string content_name, int bitrate_, string content_type, int http_status, int total_sent_bytes, vector <string> user_agent, string cache_status) : ip(ip_), eventTime(event_time), requestMethod(requested_method), contentAddress(content_address), contentName(content_name), bitrate(bitrate_), contentType(content_type), httpStatus(http_status), totalSentBytes(total_sent_bytes), userAgent(user_agent), cacheStatus(cache_status)
{
	//cout << ip << eventTime << requestMethod << contentAddress << contentName << bitrate << contentType << httpStatus << totalSentBytes << userAgent[0] << cacheStatus << endl;
}

class User
{
	string ip;
	int numOfEvent;
public:
	User(string IP, int numEvents) : ip(IP), numOfEvent(numEvents) { ; }
	string getIP() { return ip; }
	void incNumEvent() { numOfEvent++; }
	int getNumOfEvent() { return numOfEvent; }
};

class Content
{
	string ID;
	int numOfSeen;
public:
	Content(string id, int num_of_seen) : ID(id), numOfSeen(num_of_seen) {}
	string getID() { return ID; }
	int getNumofSeen() { return numOfSeen; }
	void incNum() { numOfSeen++; }
};

class GeneralLogs
{
	int numOfUniqueIP;
	int numOfUniqueID;
	int totalData;
	string mostViewerIP;
	string mostViewedID;
	map<long long int, int> bitrateRate;
	map<string, int> browserRate;
	map<int, int> httpStatusRate;
public:
	void parseTheLog(string);
	GeneralLogs()
	{
		numOfUniqueIP = 0;
		numOfUniqueID = 0;
		totalData = 0;
		mostViewerIP = "";
		mostViewedID = "";
	}
};



void GeneralLogs::parseTheLog(string filename)
{
	string line;
	fstream readFile;
	readFile.open(filename, ios::in);


	if (!readFile.is_open())
	{
		cout << "File could not opened!" << endl;
		return;
	}

	vector<User> users;
	vector<Content> contents;
	vector<ParsedLog> logs;

	Content* newcontent;
	User* newuser;
	ParsedLog* newlog;


	string ip;
	string eventTime;
	string requestMethod;
	string contentAddress;
	string contentName;
	long long int bitrate = 0;
	string contentType;
	int httpStatus = 0;
	int totalSentBytes = 0;
	vector <string> userAgent;
	string cacheStatus;


	string tempDate;
	string tempBitrate;
	int counter = 0;
	int x = 0;
	int checkAgentCount = 0;
	while (readFile >> line)
	{
		tempBitrate = "";
		// check for unused elements
		if (line == "-")
			continue;
		if (line == "\"-\"")
		{
			checkAgentCount++;
			if (checkAgentCount == 3)
				counter++;
			if(counter != 11)
				continue;
		}

		// unused rt=
		if (line[0] == 'r' && line[1] == 't' && line[2] == '=')
			continue;
		// unused rt=

		// unused ut=
		if (line[0] == 'u' && line[1] == 't' && line[2] == '=')
			continue;
		// unused ut=



		// event time
		if (line[0] == '[')
		{
			tempDate = line;
			continue;
		}

		if (line[line.length() - 1] == ']')
		{
			line = tempDate + " " + line;
			eventTime = line;
			counter++;
			if (counter != 11)
				continue;
		}
		// event time


		// request method
		if (line[0] == '"' && (line[1] < 91 && line[1] > 64) && (line[2] < 91 && line[2] > 64))
		{
			for (int i = 0; i != line.length(); i++)
			{
				line[i] = line[i + 1];
			}
			requestMethod = line;
			counter++;
			if (counter != 11)
				continue;
		}
		// request method

		// content address && content name && bitrate && content type
		if (line[0] == '/')
		{
			contentAddress = line;
			counter++;
			for (int i = 0; i != contentAddress.length(); i++)
			{
				if (contentAddress[i] == 'I' && contentAddress[i + 1] == 'D')
				{
					contentName = "";
					for (int j = i; contentAddress[j] != '.'; j++)
					{
						contentName.push_back(contentAddress[j]);
					}
					counter++;
					break;
				}
			}

			
			//go for bitrate with index a
			int a = 0;
			for (; contentAddress[a] != '(' && a != contentAddress.size()-1; a++)
			{
			}

			if(a == contentAddress.size() - 1)
			{
				counter++;
			}
			
			//finished pharantesis
			int b = 0;
			bool ifBitrate = false;
			
			for (int i = a + 1; a != contentAddress.size() - 1 && contentAddress[i] != ')'; i++)
			{
				ifBitrate = true;
				tempBitrate.push_back(contentAddress[i]);
				b = i;
			}

			if (ifBitrate)
			{
				// string to int for bitrate
				bitrate = stoll(tempBitrate);
				rollTheBitrate(bitrate);
				counter++;
			}


			// content type
			int c = b;
			contentType = "";
			for (; a != contentAddress.size() - 1 &&  contentAddress[c] != '('; c++) {}
			for (int i = c + 1; a != contentAddress.size() - 1 && contentAddress[i] != '='; i++)
			{
				contentType.push_back(contentAddress[i]);
			}
			counter++;
			if (counter != 11)
				continue;
			// content type

		}
		// content address && content name && bitrate && content type


		// http status
		if (line.length() == 3 && (line[0] < 58 && line[0] > 47) && (line[1] < 58 && line[1] > 47) && (line[2] < 58 && line[2] > 47))
		{
			httpStatus = stoi(line);
			counter++;
			if (counter != 11)
				continue;
		}
		// http status

		// byte
		bool isInteger = false;
		for (int i = 0; i < line.length() && line.length() != 3; i++)
		{
			if (!(line[i] < 58 && line[i] > 47))
			{
				isInteger = false;
				break;
			}
			else
				isInteger = true;
		}
		if (isInteger)
		{
			totalSentBytes = stoi(line);
			counter++;
			if (counter != 11)
				continue;
		}
		// byte


		//agent
		
		if ((line[0] < 91 && line[0] > 64) && (line[1] < 123 && line[1] > 97))
		{
			if (line[line.length() - 1] == '\"' || line[line.length() - 1] == '\;')
				line[line.length() - 1] = '\0';
			userAgent.push_back(line);
			if (userAgent.empty())
				counter++;
			if (counter != 11)
				continue;
		}

		if (line[0] == '"')
		{
			for (int i = 0; i != line.length(); i++)
			{
				line[i] = line[i + 1];
			}

			if (line[line.length() - 2] == '\"')
				line[line.length() - 2] = '\0';
			
			if (userAgent.empty())
				counter++;
			userAgent.push_back(line);
			if (counter != 11)
				continue;
		}

		//agent


		// status
		if (line[0] == 'c' && line[1] == 's' && line[2] == '=')
		{
			for (int i = 3; i != line.length(); i++)
			{
				cacheStatus.push_back(line[i]);
			}
			counter++;
			if (counter != 11)
				continue;
		}
		//status




		// ip
		int dotCount = 0;
		for (int i = 0; i != line.length() && line[0] && ((line[0] < 58 && line[0] > 47)); i++)
		{
			if (line[i] == '.')
				dotCount++;
		}

		if (dotCount == 3)
		{

			ip = line;
			counter++;
			if (counter != 11)
				continue;
		}
		// ip

		
		if (counter == 11)
		{
			//cout << x << " - " << ip << endl;
			checkAgentCount = 0;
			x++;
			newlog = new ParsedLog(ip, eventTime, requestMethod, contentAddress, contentName, bitrate, contentType, httpStatus, totalSentBytes, userAgent, cacheStatus);
			logs.push_back(*newlog);
			
			userAgent.clear();
			bool checkDublication = false;
			for (int i = 0; i < contents.size(); i++)
			{
				if (contents[i].getID() == contentName)
				{
					checkDublication = true;
					contents[i].incNum();
					continue;
				}
			}
			if (!checkDublication)
			{
				newcontent = new Content(contentName, 1);
				contents.push_back(*newcontent);
			}

			bool checkDublication2 = false;
			for (int i = 0; i < users.size(); i++)
			{
				if (users[i].getIP() == ip)
				{
					checkDublication2 = true;
					users[i].incNumEvent();
					continue;
				}
			}
			if (!checkDublication2)
			{
				newuser = new User(ip, 1);
				users.push_back(*newuser);
			}
			
			//cout << ip << " - " << x << endl << endl;
			counter = 0;
		}

	}

	numOfUniqueIP = users.size();
	numOfUniqueID = contents.size();
	//toplam veri
	int max = 0;
	string maxViewIP = "";
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].getNumOfEvent() > max)
		{
			max = users[i].getNumOfEvent();
			maxViewIP = users[i].getIP();
		}
	}
	mostViewerIP = maxViewIP;

	max = 0;
	string maxViewID = "";
	for (int i = 0; i < contents.size(); i++)
	{
		if (contents[i].getNumofSeen() > max)
		{
			max = contents[i].getNumofSeen();
			maxViewID = contents[i].getID();
		}
	}
	mostViewedID = maxViewID;
	totalData = 0;
	for (int i = 0; i < logs.size(); i++)
	{
		totalData += logs[i].getTotalByte();
	}

	//bitrate rate
	bool checkExist = false;
	map<long long int, int>::iterator itr;
	for (int i = 0; i < logs.size(); i++)
	{
		checkExist = false;
		for(itr = bitrateRate.begin(); itr != bitrateRate.end(); ++itr)
		{
			if(itr->first == logs[i].getBitrate())
			{
				checkExist = true;
				(itr->second)++;
			}
		}
		if (!checkExist)
		{
			bitrateRate.insert(pair<long long int, int>(logs[i].getBitrate(),1));
		}
	}
	//bitrate rate

	
	//browser rate
	bool checkExist2 = false;
	map<string, int>::iterator itr2;
	for (int i = 0; i < logs.size(); i++)
	{
		for (int j = 0; j < logs[i].getUserAgent().size(); j++)
		{
			checkExist2 = false;
			for (itr2 = browserRate.begin(); itr2 != browserRate.end(); ++itr2)
			{
				if (itr2->first == logs[i].getUserAgent()[j])
				{
					checkExist2 = true;
					(itr2->second)++;
				}
			}
			if (!checkExist2)
			{
				browserRate.insert(pair<string, int>(logs[i].getUserAgent()[j], 1));
			}
		}
		
	}
	//browser rate


	//HTTP Status rate
	bool checkExist3 = false;
	map<int, int>::iterator itr3;
	for (int i = 0; i < logs.size(); i++)
	{
		checkExist = false;
		for (itr3 = httpStatusRate.begin(); itr3 != httpStatusRate.end(); ++itr3)
		{
			if (itr3->first == logs[i].gethttpStatus())
			{
				checkExist = true;
				(itr3->second)++;
			}
		}
		if (!checkExist)
		{
			httpStatusRate.insert(pair<int, int>(logs[i].gethttpStatus(), 1));
		}
	}
	//HTTP Status rate



	cout << "Number of Unique IP: " << numOfUniqueIP << endl
		 << "Number of Unique ID: " << numOfUniqueID << endl
		 << "Total Data sended:   " << totalData << endl
		 << "Most viewer User:    " << mostViewerIP << endl
		 << "Most viewed Content: " << mostViewedID << endl
		 << "Bitrate Rates:       ";

	for (itr = bitrateRate.begin(); itr != bitrateRate.end(); ++itr)
	{
		cout << itr->first << " : " << itr->second << "  |  ";
	}
	cout << endl;
	
	cout << "Browser Rate       : ";
	for (itr2 = browserRate.begin(); itr2 != browserRate.end(); ++itr2)
	{
		cout << itr2->first << " : " << itr2->second << "  |  ";
	}
	cout << endl;
	
	cout << "HTTP Status Rate   : ";
	for (itr3 = httpStatusRate.begin(); itr3 != httpStatusRate.end(); ++itr3)
	{
		cout << itr3->first << " : " << itr3->second << "  |  ";
	}
	cout << endl;

	readFile.close();
}





int main()
{
	GeneralLogs Logs;
	Logs.parseTheLog("nginx.access.log");
	return 0;
}