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

class Table
{
public:
    Table(string s);
    void set_uni();
    void set_now(int a);
    void set_max(int a);
    void set_type(string t);
    void set_name(string s);
    void insert(string s);
    void print();
private:
    string table_name;
    int now = 1, max_length[100];
    string types[100], val_names[100];
    bool is_unique[100];
};

Table::Table(string s) {
    table_name = s;
    types[0] = "INTEGER";
    val_names[0] = "ID";
}

void Table::print() {
    cout << "Table name : " << table_name << endl;
    cout << "Col num : " << now << endl;
    for (int i = 0; i < now; i++)
    {
        cout << val_names[i] << ' ' << types[i] << ' ';
        if (types[i] == "CHAR")cout << max_length[i] << ' ';
        if (is_unique[i]) cout << "UNIQUE";
        cout << endl;
    }
}
void Table::insert(string s) {
    ofstream db((table_name + ".txt").c_str(), ios::app);
    db << s << endl;
    cout << table_name << endl;
    db.close();
}
void Table::set_uni() {
    is_unique[now] = 1;
}

void Table::set_now(int a) {
    now += a;
}

void Table::set_max(int a) {
    max_length[now] = a;
}
void Table::set_type(string t) {
    types[now] = t;
}
void Table::set_name(string s) {
    val_names[now] = s;
}
vector<Table> Tables;

void schema_init() {
    ifstream f("Schema.txt");
    string line;
    int table_num = -1, flag = 0;
    while (getline(f, line)) {
        if (flag && line.empty()) {
            flag = 0;
            continue;
        } else if (!flag) {  //Table name
            if (table_num >= 0)
                Tables[table_num].print();
            Tables.push_back(Table(line));
            table_num++;
            flag = 1;
        } else {  //Table initialize
            //cout << line << endl;
            string name, a, b;
            istringstream iss(line);
            if (!(iss >> name >> a)) {Er("Wrong schema");}
            if (!(iss >> b)) b=  a;
            else{
                if (a != "UNIQUE") Er("Wrong Schema");
                Tables[table_num].set_uni();
                a = b;
            }
            if(!iss.eof()) Er("Wrong Schema");
            Tables[table_num].set_name(name);
            //cout << a << endl;

            if (a[0] == 'C')
            {
                string num="";
                for (int i = 0; i < a.size(); i++)
                    if (a[i] == '(')
                        for (i++;a[i]!=')'; i++)
                            num += a[i];
                if (num == "" || a[a.size()-1] != ')')Er("Wrong Schema");
                Tables[table_num].set_type("CHAR");
                Tables[table_num].set_max(stoi(num));
            }
            else
                Tables[table_num].set_type(a);
            Tables[table_num].set_now(1);
            //cout << name << a << b << endl;



        }
    }
    f.close();
}
int main() {

    cout << "Enter 1 for Server and 2 for Shell" << endl;
    int x;

    //cin >> x;

    schema_init();
    /*if (x == 2)
    {

    }
    else
    {}*/

    return 0;
}
