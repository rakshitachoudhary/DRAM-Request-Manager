#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
using namespace std;

bool is_integer(const string& strings1) //boolean function to check if the given string strings1 is an integer or not
{
    string::const_iterator iter1 = strings1.begin(); //iterator
    if (strings1.length() > 1) {
        if (strings1.substr(0, 1) == "-") {
            iter1++;
        }
    }
    while (iter1 != strings1.end() && std::isdigit(*iter1)) ++iter1; //checking for integer using the isdigit() function
    return !strings1.empty() && iter1 == strings1.end(); //boolean value indicating if the string is an integer or not is returned
}

//////////////////////*New code*//////////////////////
bool vector_find(vector<string> vec, string element){
    for (int t = 0; t < vec.size(); t++){
        if (vec[t] == element){
            return true;
        }
    }
    return false;
}
bool vector_find2(vector<int> vec, int element){
    for (int t = 0; t < vec.size(); t++){
        if (vec[t] == element){
            return true;
        }
    }
    return false;
}
int find_queue(vector<pair<int, int>> queue, int one, int two){
    for (int t=0; t<queue.size(); t++){
        if (queue[t].first == one and queue[t].second == two){
            return t;
        }
    }
    return -1;
}
bool check_dep_lw(vector<string> lw_dep, vector<string> sw_dep, vector<int> lw_inst, vector<int> sw_inst, vector<int> addr, vector<int> addr_inst, int ins, string reg1, string reg2, int a){
    for (int k=0; k<lw_dep.size(); k++){
        if (lw_dep[k] == reg1 or lw_dep[k] == reg2){
            if (lw_inst[k] < ins){
                cout<<"no1\n";
                return true;
            }
        }
    }
    for (int k=0; k<sw_dep.size(); k++){
        if (sw_dep[k] == reg1 and sw_inst[k] < ins){
            cout <<"no2\n";
            return true;
        }
    }
    for (int k=0; k<addr.size(); k++){
        if (addr[k] == a and addr_inst[k] < ins){
            cout<<"no3\n";
            return true;
        }
    }
    return false;
}
bool check_dep_sw(vector<string> lw_dep, vector<int> lw_inst, vector<int> addr, vector<int> addr_inst, int ins, string reg1, string reg2, int a){
    for (int k=0; k<lw_dep.size(); k++){
        if (lw_dep[k] == reg1 or lw_dep[k] == reg2){
            if (lw_inst[k] < ins){
                cout << "no11\n";
                return true;
            }
        }
    }
    for (int k=0; k<addr.size(); k++){
        if (addr[k] == a and addr_inst[k] < ins){
            cout << "no12\n";
            return true;
        }
    }
    return false;
}
//////////////////////*New code*//////////////////////

void split(string str1, vector<string>& vectors) { //function to split a string at white spaces and make a vector "vectors" of the words present in the string
    string temporary = ""; //temp string
    for (int t = 0; t < str1.length(); ++t) { //iterate through the length of the string
        if (str1[t] == ' ') { //check for whitespaces
            vectors.push_back(temporary);
            temporary = "";
        }
        else {
            temporary.push_back(str1[t]);
        }
    }
    vectors.push_back(temporary);
}

string decimal2hexadecimal(int decimalval) { //function to carry out decimal to hexadecimal conversion
    stringstream ss1;
    ss1 << hex << decimalval; // convert decimal to hexadecimal value
    string res(ss1.str());
    return res; //hexadecimal value returned
}

void sim(vector<string> files, int row_access_delay, int col_access_delay, int N, int M){
    string* memoryBlock = new string[(int)pow(2, 20)];  //memory block of 2^20 bytes
    string* instBlock = new string[(int)pow(2,16)];
    string* PC = memoryBlock;   //define program counter, initialize to start address of memory block
    string s;   //string to be used to read lines from input file
    int num[N];    //counter for number of instructions
    int invalid_instruction[N]; //count to indicate if an invalid instruction is found or not. The program would terminate after this becomes 1, i.e. stops after the occurrence of an invalid instruction
    int ROW_BUFFER_UPDATE = 0;  //counter for number of row buffer updates
    int dram = (int)pow(2, 20);  //instruction set memory
    int dram_mem = dram/N;
    register int $r[N][32];
    string* counter[N];
    counter[0] = instBlock;
    vector<string> labels[N];              //to store label strings
    vector<int> labelnum[N];               //instruction numbers corresponding to labels
    bool error[N];
    int inst_num[N];
    bool stopped[N];
    int dependent_on[N];
    vector<bool> executed[N];
    for (int j = 0; j < N; j++) {
        stopped[j] = false;
        error[j] = false;
        for (int i = 0; i < 32; i++){
            $r[j][i] = 0;  //initialize registers to 0
        }
        invalid_instruction[j] = 0;
        inst_num[j] = 0;
        num[j] = 0;
        ifstream file;
        file.open(files[j]); //open file
        if (!file.is_open()){
            cout << "Cannot open file " << j+1 << endl;
        }
        while (getline(file, s)) //read line by line from input file
        {
            string s1;            //string to store modified instruction format
            bool prev = false;    //a bool to check commas/whitespaces
            int len = s.length(); //length of input line
            if (s == ""){ 
                continue;
            }
            for (int i = 0; i < len; i++) { //iterate over string s
                if (s.substr(i, 1) == "\t"){
                    continue;
                }
                else if (s.substr(i, 1) != "," and s.substr(i, 1) != " " and s.substr(i, 1) != "(" and s.substr(i, 1) != ")")
                { //remove commas, parenthesis, whitespaces from instructions
                    s1 = s1 + s.substr(i, 1);
                    prev = false;
                }
                else if (prev == false and i != len - 1)
                { //insert whitespace after each token
                    s1 = s1 + " ";
                    prev = true;
                }
                else if (s.substr(i, 1).find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789, ()$-:") != std::string::npos)
                { //check if the string contains any special characters other than alphabets, integers (including negative), $, (, )," ", ","
                    cout << "Core " << j+1 << ": Invalid Instruction.\n";
                    invalid_instruction[j]++;
                }
            }

            vector<string> tokens; //tokens is the vector containing the words of the space separated line as its elements
            split(s1, tokens);     // split s1 into tokens

            if (tokens.size() == 0)
            { //for an empty instruction line
                continue;
            }
            else if (tokens.size() == 1)
            {
                if (tokens[0].substr(tokens[0].size() - 1, 1) != ":")
                {
                    cout << "Core " << j+1 << ": Missing colon \n";
                    error[j] = true;
                }
                else
                {
                    labels[j].push_back(tokens[0].substr(0, tokens[0].size() - 1));
                    labelnum[j].push_back(num[j]);
                }
            }
            else
            {
                //the first element of the vector indicates the instruction type out of add, sub, mul, beq, bne, slt, j, lw, sw, addi
                if (tokens[0] == "add" or tokens[0] == "sub" or tokens[0] == "mul" or tokens[0] == "slt")
                { //the add,sub,mul,slt instructions' format should be add dest_register_name, register1_name, register2_name, thus 4 elements in the vector
                    if (tokens.size() != 4)
                    {
                        invalid_instruction[j]++;
                        cout << "Core " << j+1  << ": Invalid input instruction entered" << endl;
                    }
                    for (int k = 1; k < 4; k++)
                    {
                        if (tokens[k].substr(0, 1) != "$" or (tokens[k].substr(1, 1) != "s" and tokens[k].substr(1, 1) != "t" and tokens[k].substr(1, 1) != "z"))
                        {
                            invalid_instruction[j]++;
                            cout << "Core " << j+1  << ": Invalid register name" << endl;
                        }
                    }
                }
                else if (tokens[0] == "beq" or tokens[0] == "bne")
                { //the beq instruction format should be beq register1_name, register2_name, integer_val_of_line, thus 4 elements in the vector
                    if (tokens.size() != 4)
                    {
                        invalid_instruction[j]++;
                        cout << "Core " << j+1  << ": Invalid input instruction entered." << endl;
                    }
                    for (int k = 1; k < 3; k++)
                    {
                        if (tokens[k].substr(0, 1) != "$" or (tokens[k].substr(1, 1) != "s" and tokens[k].substr(1, 1) != "t" and tokens[k].substr(1, 1) != "z"))
                        {
                            invalid_instruction[j]++;
                            cout << "Core " << j+1  << ": Invalid register name" << endl;
                        }
                    }
                }
                else if (tokens[0] == "j")
                { //the j instruction format should be j integer_val_of_line, thus 2 elements in the vector
                    if (tokens.size() != 2)
                    {
                        invalid_instruction[j]++;
                        cout << "Core " << j+1  << ": Invalid input instruction entered." << endl;
                    }
                }
                else if (tokens[0] == "lw" or tokens[0] == "sw")
                { //the lw instruction format should be lw register1_name, register2_name, thus 3 elements in the vector
                    if (tokens.size() != 4)
                    {
                        invalid_instruction[j]++;
                        cout << "Core " << j+1  << ": Invalid input instruction entered." << endl;
                    }
                    if (is_integer(tokens[2]) == false)
                    { //if offset is not given as an integer, consider this instruction as an invalid instruction
                        invalid_instruction[j]++;
                        cout << "Core " << j+1  << ": Invalid input instruction entered." << endl;
                    }
                    for (int k = 1; k < 4; k++)
                    {
                        if (k != 2 and (tokens[k].substr(0, 1) != "$" or (tokens[k].substr(1, 1) != "s" and tokens[k].substr(1, 1) != "t" and tokens[k].substr(1, 1) != "z")))
                        {
                            invalid_instruction[j]++;
                            cout << "Core " << j+1  << ": Invalid register name" << endl;
                        }
                    }
                }
                else if (tokens[0] == "addi")
                { //the addi instruction format should be addi register1_name, register2_name, immediate_val, thus 4 elements in the vector
                    if (tokens.size() != 4)
                    {
                        invalid_instruction[j]++;
                        cout << "Core " << j+1  << ": Invalid input instruction entered." << endl;
                    }
                    if (is_integer(tokens[3]) == false)
                    { //if immediate value is not given as an integer, consider this instruction as an invalid instruction
                        invalid_instruction[j]++;
                        cout << "Core " << j+1  << ": Invalid input instruction entered." << endl;
                    }
                    for (int k = 1; k < 3; k++)
                    {
                        if (tokens[k].substr(0, 1) != "$" or (tokens[k].substr(1, 1) != "s" and tokens[k].substr(1, 1) != "t" and tokens[k].substr(1, 1) != "z"))
                        {
                            invalid_instruction[j]++;
                            cout << "Core " << j+1  << ": Invalid register name" << endl;
                        }
                    }
                }
                else
                { //any other invalid instruction
                    cout << "Core " << j+1  << ": Invalid input instruction entered." << endl;
                    invalid_instruction[j]++;
                }

                *(counter[j] + num[j]) = s1; //store instruction in memory
                num[j]++;            //add to number of instructions
                executed[j].push_back(false);
            }
        }
        if (j != N-1){
            counter[j+1] = counter[j] + num[j];
        }
    }
    vector<string> lw_dependencies[N];     //the names of registers used for loading data from memory
    vector<int> lw_instr_num[N];           //instruction number corresponding to register names in lw_dependencies
    vector<string> sw_dependencies[N];     //names of registers used in address values and for storing in sw instructions
    vector<int> sw_instr_num[N];           //instruction number corresponding to register names in sw_dependencies
    vector<int> addresses[N];
    vector<int> addr_inst[N];
    string* ROW_BUFFER = new string[1024]; //Row buffer array of size 1024
    int row_buffer_num = -1;    //to store the row number in row buffer
    vector<string> reg_names[N];
    int cycle_count = 1;
    int count_add[N];  //number of times add gets executed
    int count_sub[N];  //number of times sub gets executed
    int count_mul[N];  //number of times mul gets executed
    int count_beq[N];  //number of times beq gets executed
    int count_bne[N];  //number of times bne gets executed
    int count_slt[N];  //number of times slt gets executed
    int count_j[N];    //number of times j gets executed
    int count_lw[N];   //number of times lw gets executed
    int count_sw[N];   //number of times sw gets executed
    int count_addi[N];     //number of times addi gets executed
    for (int k=0; k<N; k++){
        count_add[k] = 0;
        count_sub[k] = 0;
        count_addi[k] = 0;
        count_mul[k] = 0;
        count_beq[k] = 0;
        count_bne[k] = 0;
        count_slt[k] = 0;
        count_j[k] = 0;
        count_lw[k] = 0;
        count_sw[k] = 0;
    }
    int total_exe_inst = 0;
    string inst;
    vector<pair<int, int>> queue;
    bool DRAM_busy = false;
    int time_left = 0;
    int actual_time = 0;
    pair<int,int> current = make_pair(-1,-1);
    pair<int, int> execute_now;
    string curr = "";
    int rbu = 0;
    while(cycle_count <= M){
        cout << "\nClock cycle " << cycle_count << ": " << endl;
        if (time_left != 0){
            if (time_left != 1){
                inst = *(counter[current.first] + current.second);
                cout << "DRAM: Execution going on for " << inst << " (Core " << current.first + 1 << ")" << endl;
            }
            time_left--;
            if (time_left == 0){
                inst = *(counter[current.first] + current.second);
                istringstream ss(inst);
                string data, data1, data2, data3;
                ss >> data;
                int i = current.first;
                if (data == "lw"){
                    auto buffer = std::find(lw_instr_num[i].begin(), lw_instr_num[i].end(), current.second);
                    int bufindex = buffer - lw_instr_num[i].begin();
                    lw_instr_num[i].erase(buffer);
                    lw_dependencies[i].erase(bufindex + lw_dependencies[i].begin());
                    buffer = std::find(addr_inst[i].begin(), addr_inst[i].end(), current.second);
                    bufindex = buffer - addr_inst[i].begin();
                    addr_inst[i].erase(buffer);
                    addresses[i].erase(bufindex + addresses[i].begin());
                    bufindex = find_queue(queue, current.first, current.second);
                    queue.erase(bufindex + queue.begin());
                    executed[i][current.second] = true;
                    count_lw[i]++;
                    total_exe_inst++;
                    ss >> data1;
                    ss >> data2;
                    ss >> data3;
                    auto x = std::find(reg_names[i].begin(), reg_names[i].end(), data3);
                    int adr;
                    if (x == reg_names[i].end()){
                        adr = std::stoi(data2) + i*dram_mem;
                    }
                    else{
                        int index = x - reg_names[i].begin();
                        adr = std::stoi(data2) + $r[i][index] + i*dram_mem;
                    }
                    string value = *(PC + adr);
                    if (value == ""){
                        value = "0";
                    }
                    cout << "DRAM: Modified value in register " << data1 << " to  " << value << endl;
                }
                else {
                    auto buffer = std::find(addr_inst[i].begin(), addr_inst[i].end(), current.second);
                    int bufindex = buffer - addr_inst[i].begin();
                    addr_inst[i].erase(buffer);
                    addresses[i].erase(bufindex + addresses[i].begin());
                    bufindex = find_queue(queue, current.first, current.second);
                    queue.erase(bufindex + queue.begin());
                    executed[i][current.second] = true;
                    count_sw[i]++;
                    total_exe_inst++;
                    ss >> data1;
                    ss >> data2;
                    ss >> data3;
                    auto x = std::find(reg_names[i].begin(), reg_names[i].end(), data3);
                    int adr;
                    int value;
                    if (x == reg_names[i].end()){
                        adr = std::stoi(data2) + i*dram_mem;
                    }
                    else{
                        int index = x - reg_names[i].begin();
                        adr = std::stoi(data2) + $r[i][index] + i*dram_mem;
                    }
                    x = std::find(reg_names[i].begin(), reg_names[i].end(), data1);
                    if (x == reg_names[i].end()){
                        value = 0;
                    }
                    else{
                        int index = x - reg_names[i].begin();
                        value = $r[i][index];
                    }
                    cout << "DRAM: Modified memory at DRAM location " << adr << " to value " << to_string(value) << endl;
                }
                ROW_BUFFER_UPDATE += rbu;
                cout << "DRAM: Execution completed for " << inst << " (Core " << i + 1 << ")" << endl;
                DRAM_busy = false;
                bool found = false;
                for (int j = 0; j < queue.size(); j++){
                    int i1 = queue[j].first;
                    int sec = queue[j].second;
                    inst = *(counter[i1] + sec);
                    istringstream ss(inst);
                    string data, data1, data2, data3;
                    ss >> data;
                    ss >> data1;
                    ss >> data2;
                    ss >> data3;
                    auto x = std::find(reg_names[i1].begin(), reg_names[i1].end(), data3);
                    int adr;
                    if (x == reg_names[i1].end()){
                        adr = std::stoi(data2) + i1*dram_mem;
                    }
                    else{
                        int index = x - reg_names[i1].begin();
                        adr = std::stoi(data2) + $r[i1][index] + i1*dram_mem;
                    }
                    int row = adr/1024;
                    if (row == row_buffer_num){
                        if (data == "lw" and !check_dep_lw(lw_dependencies[i1], sw_dependencies[i1], lw_instr_num[i1], sw_instr_num[i1], addresses[i1], addr_inst[i1], sec, data1, data3, adr)){
                            execute_now = queue[j];
                            inst_num[i1] = sec;
                            found = true;
                            stopped[i1] = false;
                            DRAM_busy = true;
                            break;
                        }
                        else if (data == "sw" and !check_dep_sw(lw_dependencies[i1], lw_instr_num[i1], addresses[i1], addr_inst[i1], sec, data1, data2, adr)){
                            execute_now = queue[j];
                            DRAM_busy = true;
                            inst_num[i1] = sec;
                            found = true;
                            stopped[i1] = false;
                            DRAM_busy = true;
                            break;
                        }
                    }
                }
                for (int j=current.second + 1; j<num[i]; j++){
                    if (!executed[i][j]){
                        inst = *(counter[i] + j);
                        istringstream ss(inst);
                        string data;
                        ss >> data;
                        if (data != "lw" and data != "sw"){
                            inst_num[i] = j;
                            stopped[i] = false;
                        }
                        else if (!found) {
                            string data1, data2, data3;
                            ss >> data1;
                            ss >> data2;
                            ss >> data3;
                            auto x = std::find(reg_names[i].begin(), reg_names[i].end(), data3);
                            int adr;
                            if (x == reg_names[i].end()){
                                adr = std::stoi(data2) + i*dram_mem;
                            }
                            else{
                                int index = x - reg_names[i].begin();
                                adr = std::stoi(data2) + $r[i][index] + i*dram_mem;
                            }
                            if (data == "lw" and !check_dep_lw(lw_dependencies[i], sw_dependencies[i], lw_instr_num[i], sw_instr_num[i], addresses[i], addr_inst[i], j, data1, data3, adr)){
                                execute_now = make_pair(i, j);
                                DRAM_busy = true;
                                inst_num[i] = j;
                            }
                            else if (data == "sw" and !check_dep_sw(lw_dependencies[i], lw_instr_num[i], addresses[i], addr_inst[i], j, data1, data3, adr)){
                                execute_now = make_pair(i, j);
                                DRAM_busy = true;
                                inst_num[i] = j;
                            }
                            else {
                                int f = 2147483647;
                                int k;
                                for (k=0; k<queue.size(); k++){
                                    if (queue[k].first == i and queue[k].second < f){
                                        f = queue[k].second;
                                    }
                                }
                                execute_now = make_pair(i, f);
                                DRAM_busy = true;
                                inst_num[i] = f;
                            }
                            found = true;
                            stopped[i] = false;
                        }
                        stopped[i] = false;
                        break;
                    }
                }
                if (!found){
                    for (int i=0; i<N; i++){
                        if (stopped[i]){
                            inst = *(counter[i] + dependent_on[i]);
                            istringstream ss(inst);
                            string data, data1, data2, data3;
                            ss >> data;
                            ss >> data1;
                            ss >> data2;
                            ss >> data3;
                            auto x = std::find(reg_names[i].begin(), reg_names[i].end(), data3);
                            int adr;
                            if (x == reg_names[i].end()){
                                adr = std::stoi(data2) + i*dram_mem;
                            }
                            else{
                                int index = x - reg_names[i].begin();
                                adr = std::stoi(data2) + $r[i][index] + i*dram_mem;
                            }
                            if (data == "lw" and !check_dep_lw(lw_dependencies[i], sw_dependencies[i], lw_instr_num[i], sw_instr_num[i], addresses[i], addr_inst[i], dependent_on[i], data1, data3, adr)){
                                execute_now = make_pair(i, dependent_on[i]);
                                DRAM_busy = true;
                                inst_num[i] = dependent_on[i];
                                stopped[i] = false;
                                break;
                            }
                            else if (data == "sw" and !check_dep_sw(lw_dependencies[i], lw_instr_num[i], addresses[i], addr_inst[i], dependent_on[i], data1, data3, adr)){
                                execute_now = make_pair(i, dependent_on[i]);
                                DRAM_busy = true;
                                inst_num[i] = dependent_on[i];
                                stopped[i] = false;
                                break;
                            }
                            else {
                                for (int m=i+1; m<N; m++){
                                    if (stopped[m]){
                                        inst = *(counter[m] + dependent_on[m]);
                                        istringstream ss(inst);
                                        string data, data1, data2, data3;
                                        ss >> data;
                                        ss >> data1;
                                        ss >> data2;
                                        ss >> data3;
                                        auto x = std::find(reg_names[m].begin(), reg_names[m].end(), data3);
                                        int adr;
                                        if (x == reg_names[m].end()){
                                            adr = std::stoi(data2) + m*dram_mem;
                                        }
                                        else{
                                            int index = x - reg_names[m].begin();
                                            adr = std::stoi(data2) + $r[m][index] + m*dram_mem;
                                        }
                                        if (data == "lw" and !check_dep_lw(lw_dependencies[m], sw_dependencies[m], lw_instr_num[m], sw_instr_num[m], addresses[m], addr_inst[m], dependent_on[m], data1, data3, adr)){
                                            execute_now = make_pair(m, dependent_on[m]);
                                            DRAM_busy = true;
                                            inst_num[m] = dependent_on[m];
                                            found = true;
                                            stopped[m] = false;
                                            break;
                                        }
                                        else if (data == "sw" and !check_dep_sw(lw_dependencies[m], lw_instr_num[m], addresses[m], addr_inst[m], dependent_on[m], data1, data3, adr)){
                                            execute_now = make_pair(i, dependent_on[m]);
                                            DRAM_busy = true;
                                            inst_num[i] = dependent_on[i];
                                            found = true;
                                            stopped[m] = false;
                                            break;
                                        }
                                    }
                                }
                                if (!found){
                                    int f = 2147483647;
                                    int k;
                                    for (k=0; k<queue.size(); k++){
                                        if (queue[k].first == i and queue[k].second < f){
                                            f = queue[k].second;
                                        }
                                    }
                                    execute_now = make_pair(i, f);
                                    DRAM_busy = true;
                                    inst_num[i] = f;
                                    found = true;
                                    stopped[i] = false;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (!found){
                    if (queue.size()==1){
                        execute_now = queue[0];
                        inst_num[queue[0].first] = queue[0].second;
                        DRAM_busy = true;
                        found = true;
                        stopped[i] = false;
                    }
                    else {
                        int one, two;
                        for (int k=0; k<queue.size(); k++){
                            if (k==0){
                                one = queue[0].first;
                                two = queue[0].second;
                            }
                            else if (queue[k].first == one){
                                if (queue[k].second < two){
                                    two = queue[k].second;
                                }
                            }
                        }
                        if (!queue.empty()){
                            execute_now = make_pair(one, two);
                            DRAM_busy = true;
                            found = true;
                            stopped[one] = false;
                            inst_num[one] = two;
                        }
                    }
                }
            }
        }
        for (int i=0; i<N; i++){
            if (inst_num[i] < num[i] and !stopped[i] and invalid_instruction[i] == 0 and !error[i]){
                while (executed[i][inst_num[i]] and inst_num[i] < num[i]-1){
                    inst_num[i]++;
                }
                inst = *(counter[i] + inst_num[i]);
                istringstream ss(inst);
                string data;
                ss >> data;
                if (!executed[i][inst_num[i]] and data != "lw" and data != "sw" and !(i == current.first and time_left < col_access_delay and curr == "lw")){
                    if (data == "add" or data == "sub" or data == "mul" or data == "slt"){ //if first token in instruction is add
                        int index1, index2, index3; //register indexes in register file (array)
                        string name = data;
                        ss >> data;                 //register string 1
                        if (data == "$zero"){cout << "Core " << i+1  << " Error: Cannot update the value in $zero register\n"; invalid_instruction[i]++;}
                        else {
                            string temporary = data;
                            auto x = std::find(reg_names[i].begin(), reg_names[i].end(), data); //look if this register already occurred in instructions and has a registered mapped to it in register array
                            if (x == reg_names[i].end()){         //if register is not already present
                                reg_names[i].push_back(data);     //add register name string to v
                                index1 = reg_names[i].size() - 1; //assign it an index and hence register in register array
                            }
                            else{
                                index1 = x - reg_names[i].begin(); //else, get its index
                            }
                            ss >> data; //repeat for second register
                            x = std::find(reg_names[i].begin(), reg_names[i].end(), data);
                            if (x == reg_names[i].end()){
                                reg_names[i].push_back(data);
                                index2 = reg_names[i].size() - 1;
                            }
                            else{
                                index2 = x - reg_names[i].begin();
                            }
                            ss >> data;
                            x = std::find(reg_names[i].begin(), reg_names[i].end(), data);
                            if (x == reg_names[i].end()){
                                reg_names[i].push_back(data);
                                index3 = reg_names[i].size() - 1;
                            }
                            else{
                                index3 = x - reg_names[i].begin();
                            }
                            if (index1 > 31 or index2 > 31 or index3 > 31){
                                cout << "Core " << i+1  << " Error: More than 32 registers are not allowed." << endl; //if the register index being added is beyond the limit of 32 registers that can be used, terminate the program
                                error[i] = true;
                            }
                            else {
                                if ((lw_dependencies[i].size() == 0 and sw_dependencies[i].size() == 0) or ((vector_find(lw_dependencies[i], reg_names[i][index1]) == false) and (vector_find(lw_dependencies[i], reg_names[i][index2]) == false) and (vector_find(lw_dependencies[i], reg_names[i][index3]) == false) and (vector_find(sw_dependencies[i], reg_names[i][index1]) == false))){
                                    if (name == "add"){
                                        $r[i][index1] = $r[i][index2] + $r[i][index3]; //perform addition
                                        count_add[i]++;
                                    }
                                    else if (name == "sub"){
                                        $r[i][index1] = $r[i][index2] - $r[i][index3]; //perform subtraction
                                        count_sub[i]++;
                                    }
                                    else if (name == "mul"){
                                        $r[i][index1] = $r[i][index2] * $r[i][index3]; //perform multiplication
                                        count_mul[i]++;
                                    }
                                    else {
                                        if ($r[i][index2] < $r[i][index3]){
                                            $r[i][index1] = 1;
                                        }
                                        else {
                                            $r[i][index1] = 0;
                                        }
                                        count_slt[i]++;
                                    }
                                    cout << "Core " << i+1 << ": " << temporary << " = " << $r[i][index1] << ", Executed Instruction: " << inst << endl;
                                    executed[i][inst_num[i]] = true;
                                    total_exe_inst++;
                                    inst_num[i]++;
                                }
                                else{
                                    if (vector_find(lw_dependencies[i], reg_names[i][index1]) == true)
                                    { //if register to be updated is dependent on any lw instruction in waiting queue
                                        string removeval = reg_names[i][index1];
                                        auto buffer = std::find(lw_dependencies[i].begin(), lw_dependencies[i].end(), removeval);
                                        int bufindex = buffer - lw_dependencies[i].begin(); //index for instruction on which current instruction is dependent
                                        dependent_on[i] = lw_instr_num[i].at(bufindex);               //go back to that instruction
                                        stopped[i] = true;                              //give permission to execute that instruction
                                    }
                                    else if (vector_find(lw_dependencies[i], reg_names[i][index2]) == true)
                                    { //if register to be accessed is dependent on any lw instruction in waiting queue
                                        string removeval = reg_names[i][index2];
                                        auto buffer = std::find(lw_dependencies[i].begin(), lw_dependencies[i].end(), removeval);
                                        int bufindex = buffer - lw_dependencies[i].begin();
                                        dependent_on[i] = lw_instr_num[i].at(bufindex);
                                        stopped[i] = true;
                                    }
                                    else if (vector_find(lw_dependencies[i], reg_names[i][index3]) == true)
                                    { //if register to be accessed is dependent on any lw instruction in waiting queue
                                        string removeval = reg_names[i][index3];
                                        auto buffer = std::find(lw_dependencies[i].begin(), lw_dependencies[i].end(), removeval);
                                        int bufindex = buffer - lw_dependencies[i].begin();
                                        dependent_on[i] = lw_instr_num[i].at(bufindex);
                                        stopped[i] = true;
                                    }
                                    else if (vector_find(sw_dependencies[i], reg_names[i][index1]) == true)
                                    { //if register to be updated is dependent on any sw instruction in waiting queue
                                        string removeval = reg_names[i][index1];
                                        auto buffer = std::find(sw_dependencies[i].begin(), sw_dependencies[i].end(), removeval);
                                        int bufindex = buffer - sw_dependencies[i].begin();
                                        dependent_on[i] = sw_instr_num[i].at(bufindex);
                                        stopped[i] = true;
                                    }
                                }
                            }
                        }
                    }
                    else if (data == "addi"){
                        int index1, index2; //register indicies in register file (array)
                        ss >> data;                 //register string 1
                        if (data == "$zero"){cout << "Core " << i+1  << "Error: Cannot update the value in $zero register\n"; invalid_instruction[i]++;}
                        else {
                            string temporary = data;
                            auto x = std::find(reg_names[i].begin(), reg_names[i].end(), data); //look if this register already occurred in instructions and has a registered mapped to it in register array
                            if (x == reg_names[i].end()){         //if register is not already present
                                reg_names[i].push_back(data);     //add register name string to v
                                index1 = reg_names[i].size() - 1; //assign it an index and hence register in register array
                            }
                            else{
                                index1 = x - reg_names[i].begin(); //else, get its index
                            }
                            ss >> data; //repeat for second register
                            x = std::find(reg_names[i].begin(), reg_names[i].end(), data);
                            if (x == reg_names[i].end()){
                                reg_names[i].push_back(data);
                                index2 = reg_names[i].size() - 1;
                            }
                            else{
                                index2 = x - reg_names[i].begin();
                            }
                            ss >> data;
                            int offset = std::stoi(data);
                            if (index1 > 31 or index2 > 31){
                                cout << "Core " << i+1  << " Error: More than 32 registers are not allowed." << endl; //if the register index being added is beyond the limit of 32 registers that can be used, terminate the program
                                error[i] = true;
                            }
                            else {
                                if ((lw_dependencies[i].size() == 0 and sw_dependencies[i].size() == 0) or ((vector_find(lw_dependencies[i], reg_names[i][index1]) == false) and (vector_find(lw_dependencies[i], reg_names[i][index2]) == false) and (vector_find(sw_dependencies[i], reg_names[i][index1]) == false))){
                                    $r[i][index1] = $r[i][index2] + offset;
                                    count_addi[i]++;
                                    total_exe_inst++;
                                    cout << "Core " << i+1 << ": " << temporary << " = " << $r[i][index1] << ", Executed Instruction: " << inst << endl;
                                    executed[i][inst_num[i]] = true;
                                    inst_num[i]++;
                                }
                                else{
                                    if (vector_find(lw_dependencies[i], reg_names[i][index1]) == true)
                                    { //if register to be updated is dependent on any lw instruction in waiting queue
                                        string removeval = reg_names[i][index1];
                                        auto buffer = std::find(lw_dependencies[i].begin(), lw_dependencies[i].end(), removeval);
                                        int bufindex = buffer - lw_dependencies[i].begin(); //index for instruction on which current instruction is dependent
                                        dependent_on[i] = lw_instr_num[i].at(bufindex);               //go back to that instruction
                                        stopped[i] = true;                              //give permission to execute that instruction
                                    }
                                    else if (vector_find(lw_dependencies[i], reg_names[i][index2]) == true)
                                    { //if register to be accessed is dependent on any lw instruction in waiting queue
                                        string removeval = reg_names[i][index2];
                                        auto buffer = std::find(lw_dependencies[i].begin(), lw_dependencies[i].end(), removeval);
                                        int bufindex = buffer - lw_dependencies[i].begin();
                                        dependent_on[i] = lw_instr_num[i].at(bufindex);
                                        stopped[i] = true;
                                    }
                                    else if (vector_find(sw_dependencies[i], reg_names[i][index1]) == true)
                                    { //if register to be updated is dependent on any sw instruction in waiting queue
                                        string removeval = reg_names[i][index1];
                                        auto buffer = std::find(sw_dependencies[i].begin(), sw_dependencies[i].end(), removeval);
                                        int bufindex = buffer - sw_dependencies[i].begin();
                                        dependent_on[i] = sw_instr_num[i].at(bufindex);
                                        stopped[i] = true;
                                    }
                                }
                            }
                        }
                    }
                    else if (data == "beq" or data == "bne"){
                        bool check = false;
                        int fir = -1;
                        string name = data;
                        for (int k=0; k<inst_num[i]; k++){
                            if (!executed[i][k]){
                                fir = k;
                                check = true;
                                break;
                            }
                        }
                        if (!check){
                            int index1, index2; //register indicies in register file (array)
                            ss >> data;                 //register string 1
                            auto x = std::find(reg_names[i].begin(), reg_names[i].end(), data); //look if this register already occurred in instructions and has a registered mapped to it in register array
                            if (x == reg_names[i].end()){         //if register is not already present
                                reg_names[i].push_back(data);     //add register name string to v
                                index1 = reg_names[i].size() - 1; //assign it an index and hence register in register array
                            }
                            else{
                                index1 = x - reg_names[i].begin(); //else, get its index
                            }
                            ss >> data; //repeat for second register
                            x = std::find(reg_names[i].begin(), reg_names[i].end(), data);
                            if (x == reg_names[i].end()){
                                reg_names[i].push_back(data);
                                index2 = reg_names[i].size() - 1;
                            }
                            else{
                                index2 = x - reg_names[i].begin();
                            }
                            ss >> data;
                            if (index1 > 31 or index2 > 31){
                                cout << "Core " << i+1  << " Error: More than 32 registers are not allowed." << endl; //if the register index being added is beyond the limit of 32 registers that can be used, terminate the program
                                error[i] = true;
                            }
                            else {
                                int index, line;
                                auto l = std::find(labels[i].begin(), labels[i].end(), data); //find label in "labels"
                                if (l == labels[i].end()){ //if label is not present, raise error
                                    cout << "Core " << i+1  << ": Invalid label entered.\n";
                                    invalid_instruction[i]++;
                                }
                                else {
                                    index = l - labels[i].begin(); //find index at which label is stored
                                    line = labelnum[i][index] + 1; //find corresponding instruction number (+1 for simplicity)
                                    if (line <= 0) { //invalid line number
                                        cout << "Core " << i+1  << ": Invalid instruction" << endl;
                                        invalid_instruction[i]++;
                                    }
                                    else {
                                        if (name == "beq"){
                                            if ($r[i][index1] == $r[i][index2]){  //if condition is true
                                                if (line-1 > inst_num[i]){    //if destination is after current instruction, set all middle instructions to "executed" (true)
                                                    for (int p = i; p < min(line - 1, num[i]); p++){
                                                        executed[i][p] = true;
                                                    }
                                                }
                                                else {      //if destination is before current instruction, set all middle instructions to "unexecuted" (false)
                                                    for (int p = line-1; p <= inst_num[i]; p++){
                                                        executed[i][p] = false;
                                                    }
                                                }
                                                inst_num[i] = line - 1;
                                            }
                                            else {
                                                executed[i][inst_num[i]] = true;
                                            }
                                            count_beq[i]++;
                                            total_exe_inst++;
                                            cout << "Core " << i+1 << ": beq, " << "Executed Instruction: " << inst << endl;
                                        }
                                        else {
                                            if ($r[i][index1] != $r[i][index2]){  //if condition is true
                                                if (line-1 > inst_num[i]){    //if destination is after current instruction, set all middle instructions to "executed" (true)
                                                    for (int p = i; p < min(line - 1, num[i]); p++){
                                                        executed[i][p] = true;
                                                    }
                                                }
                                                else {      //if destination is before current instruction, set all middle instructions to "unexecuted" (false)
                                                    for (int p = line-1; p <= inst_num[i]; p++){
                                                        executed[i][p] = false;
                                                    }
                                                }
                                                inst_num[i] = line - 1;
                                            }
                                            else {
                                                executed[i][inst_num[i]] = true;
                                            }
                                            count_bne[i]++;
                                            total_exe_inst++;
                                            cout << "Core " << i+1 << ": bne, " << "Executed Instruction: " << inst << endl;
                                        }
                                        for (int k=0; k<queue.size(); k++){
                                            if (executed[queue[k].first][queue[k].second]){
                                                queue.erase(k + queue.begin());
                                                k=0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            stopped[i] = true;
                            dependent_on[i] = fir;
                        }
                    }
                    else if (data == "j"){
                        bool check = false;
                        int fir = -1;
                        for (int k=0; k<inst_num[i]; k++){
                            if (!executed[i][k]){
                                fir = k;
                                check = true;
                                break;
                            }
                        }
                        if (!check){
                            int index, line;
                            ss >> data;
                            auto l = std::find(labels[i].begin(), labels[i].end(), data); //find label in "labels"
                            if (l == labels[i].end()){ //if label is not present, raise error
                                cout << "Core " << i+1  << ": Invalid label entered.\n";
                                invalid_instruction[i]++;
                            }
                            else {
                                index = l - labels[i].begin(); //find index at which label is stored
                                line = labelnum[i][index]; //find corresponding instruction number (+1 for simplicity)
                                if (line < 0) { //invalid line number
                                    cout << "Core " << i+1  << ": Invalid instruction" << endl;
                                    invalid_instruction[i]++;
                                }
                                else {
                                    if (line > inst_num[i]){    //if destination is after current instruction, set all middle instructions to "executed" (true)
                                        for (int p = i; p < min(line, num[i]); p++){
                                            executed[i][p] = true;
                                        }
                                    }
                                    else {      //if destination is before current instruction, set all middle instructions to "unexecuted" (false)
                                        for (int p = line; p <= inst_num[i]; p++){
                                            executed[i][p] = false;
                                        }
                                    }
                                    for (int k=0; k<queue.size(); k++){
                                        if (executed[queue[k].first][queue[k].second]){
                                            queue.erase(k + queue.begin());
                                            k=0;
                                        }
                                    }
                                    executed[i][inst_num[i]] = true;
                                    inst_num[i] = line;
                                    count_j[i]++;
                                    total_exe_inst++;
                                    cout << "Core " << i+1 << ": jump, " << "Executed Instruction: " << inst << endl;
                                }
                            }
                        }
                        else {
                            stopped[i] = true;
                            dependent_on[i] = fir;
                        }
                    }
                }
                else if (!executed[i][inst_num[i]]) {
                    if (data == "lw"){
                        int y = find_queue(queue, i, inst_num[i]); //look if this register already occurred in instructions and has a register mapped to it in register array
                        if (y == -1 or (execute_now.first == i and execute_now.second == inst_num[i])){
                            int index1, index2;
                            ss >> data; //register 1
                            if (data == "$zero"){
                                cout << "Core " << i+1  << " Error: Cannot update the value in $zero register\n";
                                error[i] = true;
                            }
                            else 
                            {
                                string reg1 = data;
                                auto x = std::find(reg_names[i].begin(), reg_names[i].end(), data);
                                if (x == reg_names[i].end()){
                                    reg_names[i].push_back(data);
                                    index1 = reg_names[i].size() - 1;
                                }
                                else{
                                    index1 = x - reg_names[i].begin();
                                }
                                ss >> data; //offset
                                int offset = std::stoi(data);
                                ss >> data; //register 2
                                string reg2 = data;
                                x = std::find(reg_names[i].begin(), reg_names[i].end(), data);
                                if (x == reg_names[i].end()){
                                    reg_names[i].push_back(data);
                                    index2 = reg_names[i].size() - 1;
                                }
                                else{
                                    index2 = x - reg_names[i].begin();
                                }
                                if (index1 > 31 or index2 > 31){
                                    cout << "Core " << i+1  << ": More than 32 registers are not allowed." << endl; //if the register index being added is beyond the limit of 32 registers that can be used, terminate the program
                                    error[i] = true;
                                }
                                else {
                                    int addr = offset + $r[i][index2] + i*dram_mem; //address to be accessed in DRAM
                                    if (addr >= dram_mem*(i+1) or addr < dram_mem*(i) or addr % 4 != 0){ //if address is negative or greater than the max value or is not a multiple of 4 then raise error
                                        cout << "Core " << i+1  << " Error: Invalid memory address" << endl;
                                        invalid_instruction[i]++;
                                    }
                                    else {
                                        int row_num = addr / 1024;         //get row number from addr
                                        int start_addr = row_num * 1024;   //get start address of this row
                                        int col_num = addr - (start_addr); //get column number
                                        string temp;
                                        if (queue.empty() or (execute_now.first == i and execute_now.second == inst_num[i])){
                                            DRAM_busy = true;
                                            current = make_pair(i, inst_num[i]);
                                            curr = "lw";
                                            if (queue.empty()){
                                                queue.push_back(make_pair(i,inst_num[i]));
                                                lw_dependencies[i].push_back(reg1);
                                                lw_instr_num[i].push_back(inst_num[i]);
                                                addresses[i].push_back($r[i][index2] + offset);
                                                addr_inst[i].push_back(inst_num[i]);
                                            }
                                            else{
                                                auto buffer = std::find(sw_instr_num[i].begin(), sw_instr_num[i].end(), current.second);
                                                int bufindex = buffer - sw_instr_num[i].begin();
                                                sw_instr_num[i].erase(buffer);
                                                sw_dependencies[i].erase(bufindex + sw_dependencies[i].begin());
                                            }
                                            if (row_buffer_num == -1){
                                                for (int m = 0; m < 1024; m++){
                                                    *(ROW_BUFFER + m) = *(PC + start_addr + m);
                                                }
                                                cout <<"Core " << i+1 << ": DRAM request initiated for " << inst;
                                                row_buffer_num = row_num;
                                                temp = *(ROW_BUFFER + col_num);
                                                if (temp == ""){
                                                    cout << " (warning: accessing garbage value)";
                                                    $r[i][index1] = 0;
                                                }
                                                else{
                                                    $r[i][index1] = std::stoi(temp);
                                                }
                                                cout << endl;
                                                rbu = 1;
                                                time_left = row_access_delay + col_access_delay;
                                                actual_time = time_left;
                                            }
                                            else if (row_buffer_num == row_num){
                                                temp = *(ROW_BUFFER + col_num);
                                                cout << "Core " << i+1 << ": DRAM request initiated for " << inst;
                                                if (temp == ""){
                                                    cout << " (warning: accessing garbage value)";
                                                    $r[i][index1] = 0;
                                                }
                                                else{
                                                    $r[i][index1] = std::stoi(temp);
                                                }
                                                cout << endl;
                                                rbu = 0;
                                                time_left = col_access_delay;
                                                actual_time = time_left;
                                            }
                                            else {
                                                for (int j = 0; j < 1024; j++){
                                                    *(PC + 1024 * row_buffer_num + j) = *(ROW_BUFFER + j);
                                                    *(ROW_BUFFER + j) = *(PC + start_addr + j);
                                                }
                                                row_buffer_num = row_num;
                                                temp = *(ROW_BUFFER + col_num);
                                                cout << "Core " << i+1 << ": DRAM request initiated for " << inst;
                                                if (temp == ""){
                                                    cout << " (warning: accessing garbage value)";
                                                    $r[i][index1] = 0;
                                                }
                                                else{
                                                    $r[i][index1] = std::stoi(temp);
                                                }
                                                cout << endl;
                                                rbu = 1;
                                                time_left = 2*row_access_delay + col_access_delay;
                                                actual_time = time_left;
                                            }
                                            inst_num[i]++;
                                        }
                                        else if (queue.size() < 32){
                                            cout << "Core " << i+1 << ": DRAM request added to queue for " << inst << endl;
                                            queue.push_back(make_pair(i,inst_num[i]));
                                            lw_dependencies[i].push_back(reg1);
                                            sw_dependencies[i].push_back(reg2);
                                            lw_instr_num[i].push_back(inst_num[i]);
                                            sw_instr_num[i].push_back(inst_num[i]);
                                            addresses[i].push_back($r[i][index2] + offset);
                                            addr_inst[i].push_back(inst_num[i]);
                                            inst_num[i]++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else if (data == "sw"){
                        int y = find_queue(queue, i, inst_num[i]); //look if this register already occurred in instructions and has a register mapped to it in register array
                        if (y == -1 or (execute_now.first == i and execute_now.second == inst_num[i])){
                            int index1, index2;
                            ss >> data; //register 1
                            string reg1 = data;
                            auto x = std::find(reg_names[i].begin(), reg_names[i].end(), data);
                            if (x == reg_names[i].end()){
                                reg_names[i].push_back(data);
                                index1 = reg_names[i].size() - 1;
                            }
                            else{
                                index1 = x - reg_names[i].begin();
                            }
                            ss >> data; //offset
                            int offset = std::stoi(data);
                            ss >> data; //register 2
                            string reg2 = data;
                            x = std::find(reg_names[i].begin(), reg_names[i].end(), data);
                            if (x == reg_names[i].end()){
                                reg_names[i].push_back(data);
                                index2 = reg_names[i].size() - 1;
                            }
                            else{
                                index2 = x - reg_names[i].begin();
                            }
                            if (index1 > 31 or index2 > 31){
                                cout << "Core " << i+1  << ": More than 32 registers are not allowed." << endl; //if the register index being added is beyond the limit of 32 registers that can be used, terminate the program
                                error[i] = true;
                            }
                            else {
                                int addr = offset + $r[i][index2] + i*dram_mem; //address to be accessed in DRAM
                                if (addr >= dram_mem*(i+1) or addr < dram_mem*(i) or addr % 4 != 0){ //if address is negative or greater than the max value or is not a multiple of 4 then raise error
                                    cout << "Core " << i+1 << " Error: Invalid memory address" << endl;
                                    invalid_instruction[i]++;
                                }
                                else {
                                    int row_num = addr / 1024;         //get row number from addr
                                    int start_addr = row_num * 1024;   //get start address of this row
                                    int col_num = addr - (start_addr); //get column number
                                    if (queue.empty() or (execute_now.first == i and execute_now.second == inst_num[i])){
                                        DRAM_busy = true;
                                        current = make_pair(i, inst_num[i]);
                                        curr = "sw";
                                        if (queue.empty()){
                                            queue.push_back(make_pair(i,inst_num[i]));
                                            addresses[i].push_back($r[i][index2] + offset);
                                            addr_inst[i].push_back(inst_num[i]);
                                        }
                                        else{
                                            auto buffer = std::find(sw_instr_num[i].begin(), sw_instr_num[i].end(), current.second);
                                            int bufindex = buffer - sw_instr_num[i].begin();
                                            sw_instr_num[i].erase(buffer);
                                            sw_dependencies[i].erase(bufindex + sw_dependencies[i].begin());
                                            buffer = std::find(sw_instr_num[i].begin(), sw_instr_num[i].end(), current.second);
                                            bufindex = buffer - sw_instr_num[i].begin();
                                            sw_instr_num[i].erase(buffer);
                                            sw_dependencies[i].erase(bufindex + sw_dependencies[i].begin());
                                        }
                                        if (row_buffer_num == -1){
                                            for (int m = 0; m < 1024; m++){
                                                *(ROW_BUFFER + m) = *(PC + start_addr + m);
                                            }
                                            cout <<"Core " << i+1 << ": DRAM request initiated for " << inst << endl;
                                            row_buffer_num = row_num;
                                            *(ROW_BUFFER + col_num) = to_string($r[i][index1]);
                                            rbu = 2;
                                            time_left = row_access_delay + col_access_delay;
                                            actual_time = time_left;
                                        }
                                        else if (row_buffer_num == row_num){
                                            *(ROW_BUFFER + col_num) = to_string($r[i][index1]);
                                            cout << "Core " << i+1 << ": DRAM request initiated for " << inst << endl;
                                            rbu = 1;
                                            time_left = col_access_delay;
                                            actual_time = time_left;
                                        }
                                        else {
                                            for (int j = 0; j < 1024; j++){
                                                *(PC + 1024 * row_buffer_num + j) = *(ROW_BUFFER + j);
                                                *(ROW_BUFFER + j) = *(PC + start_addr + j);
                                            }
                                            row_buffer_num = row_num;
                                            *(ROW_BUFFER + col_num) = to_string($r[i][index1]);
                                            cout << "Core " << i+1 << ": DRAM request initiated for " << inst << endl;
                                            rbu = 2;
                                            time_left = 2*row_access_delay + col_access_delay;
                                            actual_time = time_left;
                                        }
                                        inst_num[i]++;
                                    }
                                    else if (queue.size() < 32){
                                        cout << "Core " << i+1 << ": DRAM request added to queue for " << inst << endl;
                                        queue.push_back(make_pair(i,inst_num[i]));
                                        sw_dependencies[i].push_back(reg1);
                                        sw_dependencies[i].push_back(reg2);
                                        sw_instr_num[i].push_back(inst_num[i]);
                                        sw_instr_num[i].push_back(inst_num[i]);
                                        addresses[i].push_back($r[i][index2] + offset);
                                        addr_inst[i].push_back(inst_num[i]);
                                        inst_num[i]++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (!DRAM_busy){
            current = make_pair(-1,-1);
            curr = "";
        }
        
        cycle_count++;
    }
    cout << endl;
    for (int k=0; k<N; k++)
    {
        cout << "Analysis for Core " << k+1 << ":\n";
        cout << "add: " << count_add[k] << endl;
        cout << "addi: " << count_addi[k] << endl;
        cout << "sub: " << count_sub[k] << endl;
        cout << "mul: " << count_mul[k] << endl;
        cout << "slt: " << count_slt[k] << endl;
        cout << "beq: " << count_beq[k] << endl;
        cout << "bne: " << count_bne[k] << endl;
        cout << "j: " << count_j[k] << endl;
        cout << "lw: " << count_lw[k] << endl;
        cout << "sw: " << count_sw[k] << endl;
        if (error[k] or invalid_instruction[k] > 0)
        {
            cout << "error occured in execution.\n";
        }
        cout << endl;
    }
    cout << "Total instructions executed: " << total_exe_inst << endl;
    cout << "Row Buffer Updates: " << ROW_BUFFER_UPDATE << endl;
}

int main() {
    int N, M, row_access_delay, col_access_delay;
    vector<string> files;
    cout << "Enter the number of CPU cores: ";
    cin >> N;
    cout << "Enter simulation time (number of clock cycles): ";
    cin >> M;
    for (int i=1; i<=N; i++){
        string file;
        cout << "Enter the path of file " << i << ": "; //Take input of the files to be used
        cin >> file;
        files.push_back(file);
    }
    cout << "Enter the row_access_delay: "; //Take input of the row_access_delay value to be used
    cin >> row_access_delay;
    cout << "Enter the col_access_delay: "; //Take input of the col_access_delay value to be used
    cin >> col_access_delay;
    cout << endl;
    if(N==0){
        cout<< "Number of cores should be at least one for execution to occur."<<endl;
        return 0;
    }
    sim(files, row_access_delay, col_access_delay, N, M);
    return 0;
}