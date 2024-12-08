#include <iostream>
#include "vector"
#include "fstream"
#include <sstream>
#include <string>

using namespace std;



void Er(string msg)
{
    cerr << msg << endl;
    exit(0);
}

string send(string s)
{
    /*ofstream f("Tasks.txt", ios::app);
    f << s << endl;
    f.close();*/
    cout << s << endl << "Respond: (Done / (empty string or  res): ";
    string re;
    cin.ignore();
    getline(cin, re);
    return re;


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
    return "Done" == send("SELECT FROM " + Table + " WHERE " + condition + ";");
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
    UPDATE("Tweets", "username==" + user, vv);

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
    User now_user("");
    while (x != -1)
    {

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
                cout << (stat ? "Done\n" : "Wrong inputs\n");
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

}
