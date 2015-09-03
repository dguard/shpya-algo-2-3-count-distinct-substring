#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <stdio.h>

#define LOG_LEVEL 0

void LOG(size_t level, std::string str) {
    if (level <= LOG_LEVEL) {
        std::cout << str << std::endl;
    }
}

struct state {
    int link;
    size_t len;
    std::map<char,int> next;
};

std::vector<state> sa_init(size_t &size, size_t &last) {
    std::vector<state> states(1);
    size = last = 0;
    states[0].len = 0;
    states[0].link = -1;
    ++size;
    return states;
}

void sa_allocate_state(std::vector<state> &states, size_t &size) {
    states.resize(size*2);
}

void sa_extend(std::vector<state> &states, size_t &size, size_t &last, char c) {
    int cur = size++;
    sa_allocate_state(states, size);
    states[cur].len = states[last].len + 1;
    int p;
    for (p=last; p!=-1 && !states[p].next.count(c); p=states[p].link)
        states[p].next[c] = cur;
    if (p == -1)
        states[cur].link = 0;
    else {
        int q = states[p].next[c];
        if (states[p].len + 1 == states[q].len)
            states[cur].link = q;
        else {
            int clone = size++;
            sa_allocate_state(states, size);
            states[clone].len = states[p].len + 1;
            states[clone].next = states[q].next;
            states[clone].link = states[q].link;
            for (; p!=-1 && states[p].next[c]==q; p=states[p].link)
                states[p].next[c] = clone;
            states[q].link = states[cur].link = clone;
        }
    }
    last = cur;
}

long long count_for_st(std::vector<long long> &visited_st, std::vector<state> &states, int v) {
    long long m = 1;
    if(visited_st[v] == -1) {
        for(auto imap: states[v].next)
            m += count_for_st(visited_st, states, imap.second);
        visited_st[v] = v == 0 ? m-1 : m; // for state 0 it is (value - 1), we shouldn't include root state for task
    }
    return visited_st[v];
}

int main() {
    long start_time = clock();

    size_t size, last;
    std::vector<state> states = sa_init(size, last);
    char c;
    while (std::cin.get(c)) {
        if(c == '\n') break;
        sa_extend(states, size, last, c);
    }

    std::vector<long long> visited_st(2*last, -1); // cache for calculated states
    long long m = count_for_st(visited_st, states, 0);

    std::cout << m << std::endl;

    long total_time = clock() - start_time;
    char buffer[50];
    sprintf(buffer, "Total time: %ld", total_time);
    LOG(1, buffer);

    return 0;
}