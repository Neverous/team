#include <cstdio>
#include <map>
#include <set>
#include <string>
#include <vector>

int students;
char name[32],
     surname[32],
     course[32];

typedef std::pair<std::string, std::string> Value;
typedef std::map<std::string, std::set<Value> > Dict;

Dict sched;

int main(void)
{
    scanf("%d", &students);
    for(int s = 0; s < students; ++ s)
    {
        scanf("%s %s %s", name, surname, course);
        sched[course].insert(make_pair(std::string(name), std::string(surname)));
    }

    for(Dict::iterator it = sched.begin(); it != sched.end(); ++ it)
        printf("%s %d\n", it->first.c_str(), it->second.size());

    return 0;
}
