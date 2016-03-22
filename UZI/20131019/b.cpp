/* 2013
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <cassert>

const int MAX_RESULT = 1001;
using namespace std;

struct Declaration
{
    char name;
    int number;
    char type;

    int argc;
    char argv[16];
};

struct Call
{
    char name;
    int number;

    int argc;
    Call *argv[16];
};

bool read_declaration(Declaration &decl);
bool read_call(Call &call, int &next_free);
int eval_call(char type, Call &call, bool findOne = false);
void print_call(Call &call);

Call call_stack[1024];
unordered_map<char, vector<Declaration> > func_map;

int main(void)
{
    Declaration decl;
    char previous = 0;
    int count = 1;
    while(read_declaration(decl))
    {
        if(previous == decl.name)
            ++ count;

        else
            count = 1;

        decl.number = count;
        func_map[decl.name].push_back(decl);
        previous = decl.name;
    }

    char type[8] = {};
    int stack = 1;
    while(scanf("%s = ", type) != -1 && type[0] != '#' && read_call(call_stack[0], stack))
    {
        int result = eval_call(type[0], call_stack[0]);
        if(result == 0)
            puts("impossible");

        else if(result == 1)
        {
            printf("%c = ", type[0]);
            print_call(call_stack[0]);
            puts("");
        }

        else if(result >= MAX_RESULT)
            puts("ambiguous >1000");

        else
            printf("ambiguous %d\n", result);

        assert(0 <= result && result <= MAX_RESULT);
        assert(stack < 1024);
        for(int c = 0; c < stack + 8; ++ c)
            call_stack[c].argc = call_stack[c].number = call_stack[c].name = 0;

        stack = 1;
    }

    return 0;
}

bool read_declaration(Declaration &decl)
{
    char buff[8] = {};
    assert(scanf("%s", buff) != -1);
    if(buff[0] == '#')
        return false;


    decl.name = buff[0];
    assert('a' <= decl.name && decl.name <= 'z');
    decl.argc = 0;
    assert(scanf("%d", &decl.argc) != -1);
    assert(1 <= decl.argc && decl.argc <= 9);
    for(int a = 0; a < decl.argc; ++ a)
    {
        assert(scanf("%s", buff) != -1);
        decl.argv[a] = buff[0];
        assert('A' <= decl.argv[a] && decl.argv[a] <= 'Z');
    }

    assert(scanf("%s", buff) != -1);
    decl.type = buff[0];
    assert('A' <= decl.type && decl.type <= 'Z');
    return true;
}

bool read_call(Call &call, int &next_free/* = 1*/)
{
    char buff[8] = {};
    assert(scanf("%s", buff) != -1);
    assert(buff[0] != '#');

    call.name = buff[0];
    call.argc = 0;
    call.number = 0;
    if('A' <= call.name && call.name <= 'Z')
        return true;

    assert('a' <= call.name && call.name <= 'z');
    assert(scanf("%d", &call.argc) != -1);
    assert(1 <= call.argc && call.argc <= 9);
    for(int a = 0; a < call.argc; ++ a)
    {
        call.argv[a] = &call_stack[next_free ++];
        if(!read_call(*call.argv[a], next_free))
            throw "Invalid input!";
    }

    return true;
}

int eval_call(char type, Call &call, bool findOne/* = false*/)
{
    if(call.argc == 0)
        throw "Invalid eval!";

    assert('a' <= call.name && call.name <= 'z');
    assert('A' <= type && type <= 'Z');
    int result = 0;
    for(const Declaration &decl: func_map[call.name])
    {
        assert(decl.name == call.name);
        if(decl.argc != call.argc || type != decl.type)
            continue;

        int current_result = 1;
        for(int a = 0; a < decl.argc && current_result; ++ a)
        {
            if(call.argv[a]->argc > 0)
                continue;

            assert('A' <= call.argv[a]->name && call.argv[a]->name <= 'Z');
            assert('A' <= decl.argv[a] && decl.argv[a] <= 'Z');
            current_result *= decl.argv[a] == call.argv[a]->name;
        }

        for(int a = 0; a < decl.argc && current_result; ++ a)
        {
            if(call.argv[a]->argc == 0)
                continue;

            assert('a' <= call.argv[a]->name && call.argv[a]->name <= 'z');
            current_result *= eval_call(decl.argv[a], *call.argv[a], findOne || current_result >= MAX_RESULT);
            if(current_result >= MAX_RESULT)
                current_result = MAX_RESULT;
        }

        result += current_result;
        if(current_result)
            call.number = decl.number;

        if(result >= MAX_RESULT)
            return MAX_RESULT;

        if(result && findOne)
            return result;
    }

    return result;
}

void print_call(Call &call)
{
    putchar(call.name);
    if(!call.argc)
    {
        assert('A' <= call.name && call.name <= 'Z');
        putchar(' ');
        return;
    }

    assert('a' <= call.name && call.name <= 'z');
    assert(1 <= call.number && call.number <= 500);
    assert(1 <= call.argc && call.argc <= 9);
    printf("%d %d ", call.number, call.argc);
    for(int a = 0; a < call.argc; ++ a)
        print_call(*call.argv[a]);
}
