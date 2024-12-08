#include <iostream>
#include "vector"
#include "fstream"
#include <sstream>
#include <string>
#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
using namespace std;


char msg[1500];
int clientSd, bytesRead, bytesWritten = 0;
string send(string data)
{
    memset(&msg, 0, sizeof(msg));//clear the buffer
    strcpy(msg, data.c_str());
    if(data == "exit")
    {
        send(clientSd, (char*)&msg, strlen(msg), 0);
    }
    bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
    cout << "Awaiting server response..." << endl;
    memset(&msg, 0, sizeof(msg));//clear the buffer
    bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
    if(!strcmp(msg, "exit"))
    {
        cout << "Server has quit the session" << endl;
    }
    cout << "Server: "  << msg << endl;
    return msg;
}


void Er(string msg)
{
    cerr << msg << endl;
    exit(0);
}


bool INSERT(string Table, vector<string> &val)
{
    string s = "INSERT INTO " + Table + " VALUES (" , respond;
    for (string i : val)
        s += (i + ",");
    respond = send (s + "\b);");
    return respond == "Done";
}

string SELECT(string Table, string condition){
    return send("SELECT FROM " + Table + " WHERE " + condition + ";");
}

bool DELETE(string Table, string condition){
    return "Done" == send( "DELETE FROM " + Table + " WHERE " + condition + ";");
}

bool UPDATE(string Table, string condition, vector<string> &val){
    string s = "UPDATE FROM " + Table + " WHERE " + condition + " VALUES (" , respond;
    for (string i : val)
        s += (i + ",");
    respond = send (s + "\b);");
    return respond == "Done";
}
// val = {user, pass}, check if UNIQUE values in val are ok

class User
{
public:
    User(string u);
    void set_name(string s);
    void tweet(string s);
    void view(string s);
    void like(int s);
    void retweet(int n);
private:
    string name;
};

User::User(string u) {
    name = u;
}
void User::set_name(string s){
    name = s;
}
void User::tweet(string s){
    vector<string> v;
    v.push_back(name);
    v.push_back(s);
    v.push_back("()");
    v.push_back("()");v.push_back("TRUE");
    INSERT("Tweets", v);
};
void User::view(string Table){
    string S = SELECT(Table, "all==TRUE");
    cout << S << endl;
};

void User::retweet(int n){
    vector<string> v, vv;
    string s = SELECT("Tweets", "ID=="+to_string(n));
    istringstream input(s);
    string user, tweet, like, ret;
    input >> user >> tweet >> like >> ret;
    vv.push_back(user); vv.push_back(tweet); vv.push_back(like);
    vv.push_back(ret+"\b"+","+name+")");vv.push_back("TRUE");
    //Updating user that has been retweeted from
    cerr << "HERE" << endl;
    UPDATE("Tweets", "username==" + user, vv);
    cerr << "Here2" << endl;
    v.push_back("Retweeted from " + user+": " +tweet);
    v.push_back("()");
    v.push_back("()");v.push_back("TRUE");
    INSERT("Tweets", v);
};
void User::like(int n){
    vector<string> vv;
    string s = SELECT("Tweets", "ID=="+to_string(n));
    istringstream input(s);
    string user, tweet, like, ret;
    input >> user >> tweet >> like >> ret;
    vv.push_back(user); vv.push_back(tweet);
    vv.push_back(like+"\b"+","+name+")"); vv.push_back(ret);
    vv.push_back("TRUE");
    //Updating user that has been retweeted from
    UPDATE("Tweets", "username==" + user, vv);
};



bool Login;
int x;
int main() {
    char *serverIp = "127.0.0.1"; int port = 1501;

    //setup a socket and connection tools
    struct hostent* host = gethostbyname(serverIp);
    sockaddr_in sendSockAddr;
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect...
    int status = connect(clientSd,
                         (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        cout<<"Error connecting to socket!"<<endl; exit(0);
    }
    cout << "Connected to the server!" << endl;

    struct timeval start1, end1;
    gettimeofday(&start1, NULL);


    User now_user("");
    while (x != -1)
    {

        Login = 1;

        if (!Login)
        {
            cout << "1. Login  2. Register" << endl;
            cin >> x;
            if (x == 2)
            {
                string user, pass;
                vector<string> v;
                cout << "Username : " ;
                cin >> user;
                cout << "Pass : ";
                cin >> pass;
                v.push_back(user); v.push_back(pass);
                bool stat = INSERT("Users", v);
                //cout << (stat ? "" : "Wrong inputs\n");
            }
            else
            {
                string user, pass;
                vector<string> v;
                cout << "Username : " ;
                cin >> user;
                cout << "Pass : ";
                cin >> pass;
                v.push_back(user); v.push_back(pass);
                string row = SELECT("Users", "username=="+user);
                if (row.size()) {
                    istringstream input(row);
                    string a, b;
                    input >> a >> b;
                    if (b == pass) {
                        now_user.set_name(user);
                        Login = 1;
                        cout << "Welcome " + user << endl;
                    }
                    else
                        cout << "Wrong user or pass" << endl;
                }
                else
                    cout << "Wrong user or pass" << endl;
            }
        }
        else // User X Logged in
        {
            enum menu {Tweet = 1, View , Like, Retweet, Logout};
            cout << "1. Tweet   2. View All   3.Like   4.Retweet   5.Logout\n: ";
            cin >> x;
            switch (x) {
                case Tweet: {
                    string s;
                    cout << "(< 250 char) :";
                    cin >> s;
                    now_user.tweet(s);
                    break;
                }
                case View: {
                    now_user.view("Tweets");
                    break;
                }
                case Like: {
                    int n;
                    cout << "Enter the tweet's number: ";
                    cin >> n;
                    now_user.like(n);
                    break;
                }
                case Retweet: {
                    int n;
                    cout << "Enter the tweet's number: ";
                    cin >> n;
                    now_user.retweet(n);
                    break;
                }
                case Logout:{
                    Login = 0;
                    break;
                }
                default: {
                    cout << "Wrong Choice!" << endl;
                }
            }
        }

    }
    close(clientSd);

    cout << "Connection closed" << endl;

}
