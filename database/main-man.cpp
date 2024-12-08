#include <bits/stdc++.h>

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
                cerr << "invalid syntax in " << line; //InvalidSyntaxException(line);
        } else
            cerr << "invalid syntax in " << line;

        cout << name << ' ' << is_unique << ' ' << type << ' ' << max_length << endl;
    }

    string get_name() { return name; }

    bool get_is_unique() { return is_unique; }

    Types get_type() { return type; }

    int get_max_length() { return max_length; }

private:
    string name;
    bool is_unique = false;
    Types type;
    int max_length = 0;
};


class Value {
public:

    Value(Field field, string value) : field(field) {
        if (value.empty()) {
            cerr << "value can't be empty [field: " + field.get_name() + "]";
        }
        s_value = value;
        if (field.get_type() == Types::INTEGER) {
            i_value = atoi(value.c_str());
        }
        if (field.get_type() == Types::BOOLEAN) {
            if (value == "TRUE") {
                b_value = true;
            } else if (value == "FALSE") {
                b_value = false;
            } else {
                cerr << "value " + value + " is not boolean";
            }
        }
        if (field.get_type() == Types::CHAR) {
            if (value.size() > field.get_max_length()) {
                cerr << ("value " + value + " longer than maximum length " + to_string(field.get_max_length()));
            }
        }
    }

    Field get_field() { return field; }

    string get_s_value() { return s_value; }

    int get_i_value() { return i_value; }

    bool get_b_value() { return b_value; }

private:
    Field field;
    string s_value;
    int i_value;
    bool b_value;
};

class Row {
    vector<Value> values;
public:

    void add_value(Value value) {
        values.push_back(value);
    }

    Value get_field(string field_name) {
        for (auto value: values) {
            if (field_name == value.get_field().get_name()) {
                return value;
            }
        }
        cerr << "field not found: " + field_name;
    }

    string to_string() {
        string res;
        for (auto item: values) {
            res += item.get_s_value() + " ";
        }
        return res;
    }
};

class Table {
public:


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
                header += field.get_name() + ' ';
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
                cerr << "invalid syntax in "
                     << header + "(Duplicate column)"; //InvalidSyntaxException(header + "(Duplicate column)");
            for (int j = 0; j < fields.size(); j++) {
                if (fields[j].get_name() == field_name) {
                    index.push_back(j);
                }
            }
            if (index.size() != i + 1) {
                cerr << "invalid syntax in "
                     << header + "(Duplicate column)"; //cerr << InvalidSyntaxException(header + "(wrong column set)");
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

    string get_header() {
        string header;
        for (auto field : fields)
            header += field.get_name() + ' ';
        return header;
    }

    void dump() {
        std::ofstream ofs;
        ofs.open(get_table_file(), std::ofstream::out | std::ofstream::trunc);
        ofs << get_header() << endl;
        for (auto row: rows) {
            ofs << row.to_string() << endl;
        }
        ofs.close();
    }

    string get_table_file() {
        return ( name + ".txt");
    }

    int get_field_index(string field_name) {
        for (int i: index) {
            if (fields[i].get_name() == field_name) {
                return i;
            }
        }
        return -1;
    }

    vector<Row> &get_rows() {
        return rows;
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
            cerr << "Duplicate values";
        } else {
            rows.push_back(row);
        }
        max_id = max(max_id, row.get_field("id").get_i_value());
        return row;
    }

    void append_row_to_file(Row row) {
        ofstream fout;
        fout.open(get_table_file(), ios_base::app);
        fout << row.to_string() << endl;
    }

    bool have_row(Row &row) {
        for (auto field:fields) {
            if (!field.get_is_unique())
                continue;
            for (auto item: rows) {
                if (row.get_field(field.get_name()).get_s_value() == item.get_field(field.get_name()).get_s_value())
                    return true;
            }
        }
        return false;
    }

    string get_name() { return name; }

private:
    string name;
    int max_id = 0;
    vector<Field> fields;
    vector<int> index;
    vector<Row> rows;
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
    for (int i = 0; i < tables.size(); i++) {
        if (tables[i].get_name() == table_name) {
            return i;
        }
    }
    cerr << "table not found";
}

int get_closed(string &conditions, int pos) {
    int level = 0;
    for (int i = pos; i < conditions.size(); i++) {
        if (conditions[i] == '(') {
            level++;
        }
        if (conditions[i] == ')') {
            level--;
        }
        if (level < 0) {
            return i;
        }
    }
    return -1;
}

int get_condition_string(string &conditions, int p) {
    int cnt = 0;
    for (p; p < conditions.size(); p++) {
        if (conditions[p] == '"') cnt++;
        if (cnt == 2) return p + 1;
        if (cnt == 0 && conditions[p] == ' ') return p;
    }
    return conditions.size();
}

bool evaluate_condition(Row &row, string &conditions, int st, int ed) {
    int eqpos = conditions.find("==", st);
    int ieqpos = conditions.find("!=", st);
    if (eqpos == -1) eqpos = 1000;
    if (ieqpos == -1) ieqpos = 1000;
    int pos = min(eqpos, ieqpos);
    string field_name = conditions.substr(st, pos - st);
    string value;
    if (conditions[pos + 2] == '"') {
        value = conditions.substr(pos + 3, ed - 1 - pos - 3);
    } else {
        value = conditions.substr(pos + 2, ed - pos - 2);
    }
    cout << "condition: " << field_name << ':' << value << endl;
    if (eqpos < ieqpos) return row.get_field(field_name).get_s_value() == value;
    else return row.get_field(field_name).get_s_value() != value;
}

bool is_row_satisfied(Row &row, string conditions) {
    cout << conditions << endl;
    int p = 0;
    bool is_or = true;
    bool res = false;
    while (p < conditions.size()) {
        if (conditions[p] == ' ') p++;
        else if (conditions[p] == 'O') p += 3, is_or = true;
        else if (conditions[p] == 'A') p += 4, is_or = false;
        cout << p << " in conditions" << endl;
        if (conditions[p] == '(') {
            int pos = get_closed(conditions, p + 1);
            bool tmp = is_row_satisfied(row, conditions.substr(p + 1, pos - p - 1));
            if (is_or) res = res || tmp;
            else res = res && tmp;
            p = pos + 1;
        } else {
            int pos = get_condition_string(conditions, p);
            if (pos == -1) {
                p++;
                continue;
            }
            bool tmp = evaluate_condition(row, conditions, p, pos);
            if (is_or) res = res || tmp;
            else res = res && tmp;
            p = pos + 1;
        }
    }
    return res;
}

string process_line(string input) {
    cout << input << endl;
    if (regex_match(input, regex(R"(INSERT INTO \S+ VALUES \(\S+(,\S+)*\);)"))) {
        smatch m;
        regex_search(input, m, regex(R"(INTO \S+)"));
        string table_name = m[0].str();
        table_name = table_name.substr(5, table_name.size() - 5);
        int table_index = get_table_index(table_name);
        cout << table_index << endl;

        regex_search(input, m, regex(R"(VALUES \S+)"));
        string values = m[0].str();
        values = values.substr(1, values.size() - 3);
        values = values.substr(7, values.size() - 7);
        values = regex_replace(values, regex(" "), "`");
        values = regex_replace(values, regex(","), " ");
        tables[table_index].insert_row(values);
        return "Done";
    }

    if (regex_match(input, regex(R"(SELECT FROM \S+ WHERE .+;)"))) {
        smatch m;
        regex_search(input, m, regex(R"(FROM \S+)"));
        string table_name = m[0].str();
        table_name = table_name.substr(5, table_name.size() - 5);
        int table_index = get_table_index(table_name);
        cout << "table_index: " << table_index << endl;

        regex_search(input, m, regex(R"(WHERE .+)"));
        string values = m[0].str();
        values = values.substr(6, values.size() - 7);
        values = regex_replace(values, regex(","), " ");
        string text;
        for (auto row: tables[table_index].get_rows()) {
            if (is_row_satisfied(row, values)) {
                cerr << row.to_string() << endl;
                text += row.to_string() + "\n";
            }
        }
        return text;
    }

    if (regex_match(input, regex(R"(DELETE FROM \S+ WHERE .+;)"))) {
        smatch m;
        regex_search(input, m, regex(R"(FROM \S+)"));
        string table_name = m[0].str();
        table_name = table_name.substr(5, table_name.size() - 5);
        int table_index = get_table_index(table_name);
        cout << "table_index: " << table_index << endl;

        regex_search(input, m, regex(R"(WHERE .+)"));
        string values = m[0].str();
        values = values.substr(6, values.size() - 7);
        values = regex_replace(values, regex(","), " ");
        auto i = std::begin(tables[table_index].get_rows());

        while (i != std::end(tables[table_index].get_rows())) {
            if (is_row_satisfied(*i, values))
                i = tables[table_index].get_rows().erase(i);
            else
                ++i;
        }
        tables[table_index].dump();
        return "Done";
    }

    if (regex_match(input, regex(R"(UPDATE \S+ WHERE .+;)"))) {
        smatch m;
        regex_search(input, m, regex(R"(UPDATE \S+)"));
        string table_name = m[0].str();
        table_name = table_name.substr(7, table_name.size() - 7);
        int table_index = get_table_index(table_name);
        cout << "table_index: " << table_index << endl;

        regex_search(input, m, regex(R"(WHERE .+ VALUES)"));
        string values = m[0].str();
        values = values.substr(6, values.size() - 13);
        values = regex_replace(values, regex(","), " ");
        string text;

        auto i = std::begin(tables[table_index].get_rows());

        while (i != std::end(tables[table_index].get_rows())) {
            if (is_row_satisfied(*i, values)) {
                regex_search(input, m, regex(R"(VALUES \S+)"));
                string values = m[0].str();
                values = values.substr(1, values.size() - 3);
                values = values.substr(7, values.size() - 7);
                values = regex_replace(values, regex(" "), "`");
                values = regex_replace(values, regex(","), " ");
                string id = (*i).get_field("id").get_s_value();
                i = tables[table_index].get_rows().erase(i);
                tables[table_index].add_row(id + " " + values);
                tables[table_index].dump();
            } else
                ++i;

            return text;
        }
    }

        cerr << "Invalid Input" << endl;
        return "Done";
}

/*char msg[1500];
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
}*/


char * getline(char* line) {
    scanf("%[^\n]%*c", line);
    return line;
}

string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        if (a[i] == '\0') return s;
        s = s + a[i];
    }
    return s;
}


void get_input() {
    string s;
    char line[1024];
    s = convertToString(getline(line), 1024);
    cout << "line: " << s << endl;
    if (s.empty()) {
        return;
    }
    cout << process_line(s) << endl;
}

int main() {
    cout << get_current_dir() << endl;
    streambuf *orig = std::cin.rdbuf();
    load_tables();
    cin.rdbuf(orig);

    while (true) {
        get_input();
        cout << "hey" << endl;
    }

}


/*
int main1() {
    int port = 1501;


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
    streambuf *orig = std::cin.rdbuf();
    load_tables();
    cin.rdbuf(orig);

    string line;
    int cnt = 0;
    while (cnt++ < 5) {
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);
        line = msg;
        string res = process_line(line);
        send_to(res);
        cout << "Awaiting client response..." << endl;

    }
    close(newSd);
    close(serverSd);
}*/
