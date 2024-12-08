#include <bits/stdc++.h>
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


#include <unistd.h>

#define GetCurrentDir getcwd



struct InvalidSyntaxException : public exception {
public:
    string place;

    InvalidSyntaxException(string text) : place(text) {}

    const char *what() const throw() override {
        return (string("invalid syntax in ") + place).c_str();
    }
};

enum Types {
    TIMESTAMP, BOOLEAN, INTEGER, CHAR
};

class Field {
public:
    string name;
    bool is_unique = false;
    Types type;
    int max_length = 0;

    Field(const char *line) {
        cout << line << endl;
        string unique_string, type_string;
        istringstream input(line);
        cin.rdbuf(input.rdbuf());
        cin >> name >> unique_string;
        if (unique_string == "UNIQUE") {
            is_unique = true;
            cin >> type_string;
        } else
            type_string = unique_string;


        if (type_string == "TIMESTAMP")
            type = TIMESTAMP;
        else if (type_string == "BOOLEAN")
            type = BOOLEAN;
        else if (type_string == "INTEGER")
            type = INTEGER;
        else if (type_string == "CHAR") {
            type = CHAR;
            cin >> max_length;
            if (max_length <= 0)
                throw InvalidSyntaxException(line);
        } else
            throw InvalidSyntaxException(line);

        cout << name << ' ' << is_unique << ' ' << type << ' ' << max_length << endl;
    }
};


class Value {
public:
    Field field;
    string s_value;
    int i_value;
    bool b_value;

    Value(Field field, string value) : field(field) {
        if (value.empty()) {
            throw "value can't be empty [field: " + field.name + "]";
        }
        s_value = value;
        if (field.type == Types::INTEGER) {
            i_value = atoi(value.c_str());
        }
        if (field.type == Types::BOOLEAN) {
            if (value == "TRUE") {
                b_value = true;
            } else if (value == "FALSE") {
                b_value = false;
            } else {
                throw "value " + value + " is not boolean";
            }
        }
        if (field.type == Types::CHAR) {
            if (value.size() > field.max_length) {
                throw ("value " + value + " longer than maximum length " + to_string(field.max_length));
            }
        }
    }
};

class Row {
public:
    vector<Value> values;

    void add_value(Value value) {
        values.push_back(value);
    }

    Value get_field(string field_name) {
        for (auto value: values) {
            if (field_name == value.field.name) {
                return value;
            }
        }
        throw "field not found: " + field_name;
    }

    string to_string() {
        string res;
        for (auto item: values) {
            res += item.s_value + " ";
        }
        return res;
    }
};

class Table {
public:
    string name;
    int max_id = 0;
    vector<Field> fields;
    vector<int> index;
    vector<Row> rows;

    Table(string &name, ifstream &fin) {
        this->name = name;
        fields.push_back(Field("id UNIQUE INTEGER"));
        while (fin) {
            string input;
            getline(fin, input);
            if (input.empty()) break;
            fields.push_back(Field(input.c_str()));
        }
        cout << get_table_file() << endl;
        if (access(get_table_file().c_str(), F_OK) == -1) {
            ofstream file{get_table_file().c_str()};
            string header;
            for (auto field : fields)
                header += field.name + ' ';
            file << header << '\n';
            file.close();
        }
        ifstream table_file(get_table_file().c_str());
        string header;
        getline(table_file, header);

        istringstream input(header);
        cin.rdbuf(input.rdbuf());

        for (int i = 0; i < fields.size(); i++) {
            string field_name;
            cin >> field_name;
            if (get_field_index(field_name) != -1)
                throw InvalidSyntaxException(header + "(Duplicate column)");
            for (int j = 0; j < fields.size(); j++) {
                if (fields[j].name == field_name) {
                    index.push_back(j);
                }
            }
            if (index.size() != i + 1) {
                throw InvalidSyntaxException(header + "(wrong column set)");
            }
        }

        string line;
        while (table_file) {
            getline(table_file, line);
            if (!line.empty()) {
                add_row(line);
            }
        }
    }

    string get_table_file() {
        return ( name + ".txt");
    }

    int get_field_index(string field_name) {
        for (int i: index) {
            if (fields[i].name == field_name) {
                return i;
            }
        }
        return -1;
    }

    Field &get_field(string field_name) {
        int i = get_field_index(field_name);
        if (i != -1) {
            return fields[i];
        }
    }

    void insert_row(string values) {
        values = to_string(++max_id) + " " + values;
        append_row_to_file(add_row(values));
    }

    Row add_row(string values) {
        Row row;
        istringstream input(values);
        cin.rdbuf(input.rdbuf());
        for (auto i: index) {
            string value;
            cin >> value;
            row.add_value(Value(fields[i], value));
        }
        if (have_row(row)) {
            throw "Duplicate values";
        } else {
            rows.push_back(row);
        }
        max_id = max(max_id, row.get_field("id").i_value);
        return row;
    }

    void append_row_to_file(Row row) {
        ofstream fout;
        fout.open(get_table_file(), ios_base::app);
        fout << row.to_string() << endl;
    }

    bool have_row(Row &row) {
        for (auto field:fields) {
            if (!field.is_unique)
                continue;
            for (auto item: rows) {
                if (row.get_field(field.name).s_value == item.get_field(field.name).s_value)
                    return true;
            }
        }
        return false;
    }
};

vector<Table> tables;

void load_tables() {
    ifstream fin("Schema.txt");
    string table_name;
    while (fin) {
        getline(fin, table_name);
        cout << table_name << endl;
        tables.push_back(Table(table_name, fin));
        cout << "end" << endl;
    }
    fin.close();
}

string get_current_dir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir(buff, FILENAME_MAX);
    string current_working_dir(buff);
    return current_working_dir;
}

int get_table_index(string &table_name) {
    for (int i = 0; i < tables.size(); i ++) {
        if (tables[i].name == table_name) {
            return i;
        }
    }
    throw "table not found";
}

string process_line(string &input) {
    if (regex_match(input, regex(R"(INSERT INTO \S+ VALUES \(\S+(,\S+)*\);)"))) {
        smatch m;
        regex_search(input, m, regex(R"(INTO \S+)"));
        string table_name = m[0].str();
        table_name = table_name.substr(5, table_name.size() - 5);
        int table_index = get_table_index(table_name);
        cout << table_index << endl;

        regex_search(input, m, regex(R"(VALUES \S+)"));
        string values = m[0].str();
        values = values.substr(7, values.size() - 7);
        values = regex_replace(values, regex(","), " ");
        tables[table_index].insert_row(values);
        return "Done";
    }

    if (regex_match(input, regex(R"(SELECT FROM \S+ WHERE .+;)"))) {
        return "mahan salam! () () TRUE";
    }
    return "Done";
}

char msg[1500];
int newSd, bytesRead, bytesWritten = 0;

void send_to(string data)
{


    cout << "Client: " << msg << endl;
    //cout << ">";
    //getline(cin, data);
    memset(&msg, 0, sizeof(msg)); //clear the buffer
    strcpy(msg, data.c_str());
    if(data == "exit")
    {
        //send to the client that server has closed the connection
        send(newSd, (char*)&msg, strlen(msg), 0);
    }
    //send the message to client
    bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);
}

int main() {
    int port = 1501, x;
    streambuf *orig = std::cin.rdbuf();
    load_tables();
    cin.rdbuf(orig);

    string line;
    cout << "1. Server   2. Shell" << endl;
    cin >> x;
    if (x == 2)
    {
        while (x++ < 50)
        {
            getline(cin, line);
            cout << process_line(line) << endl;
        }
    }

    //setup a socket and connection tools
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr,
                          sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
    cout << "Waiting for a client to connect..." << endl;
    //listen for up to 5 requests at a time
    listen(serverSd, 5);
    //receive a request from client using accept
    //we need a new address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to
    //handle the new connection with client
    newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    cout << "Connected with client!" << endl;
    //lets keep track of the session time
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);


    cout << get_current_dir() << endl;

    int cnt = 0;
    while (cnt++ < 50) {
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);
        line = msg;
        string res = process_line(line);
        send_to(res);
        cout << "Awaiting client response..." << endl;

    }
    close(newSd);
    close(serverSd);
}
