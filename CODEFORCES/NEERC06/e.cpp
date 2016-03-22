/* 2014
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <algorithm>
#include <queue>

int messages,
    first, second;

int buyer_size[16384],
    buyer_price[16384],
    seller_size[16384],
    seller_price[16384],
    sum_bid_size[131072],
    sum_ask_size[131072],
    canceled[16384],
    is_buyer[16384],
    is_seller[16384];

char action[8];

class Buyers_compare
{
    public:
        bool operator()(int a, int b) const;
}; // class Buyers_compare

class Sellers_compare
{
    public:
        bool operator()(int a, int b) const;
}; // class Sellers_compare

std::priority_queue<int, std::vector<int>, Buyers_compare> buyers;
std::priority_queue<int, std::vector<int>, Sellers_compare> sellers;

void buy(int id, int size, int price);
void sell(int id, int size, int price);
void cancel(int id);
void quote(void);

int main(void)
{
#ifndef SELENE
    freopen("exchange.in", "r", stdin);
    freopen("exchange.out", "w", stdout);
#endif // SELENE

    scanf("%d", &messages);
    for(int m = 0; m < messages; ++ m)
    {
        scanf("%s", action);
        switch(action[0])
        {
            case 'B':
                scanf("%d %d", &first, &second);
                buy(m, first, second);
                break;

            case 'S':
                scanf("%d %d", &first, &second);
                sell(m, first, second);
                break;

            case 'C':
                scanf("%d", &first);
                cancel(first - 1);
                break;
        }

        quote();
    }

    return 0;
}

inline
bool Buyers_compare::operator()(int a, int b) const
{
    if(buyer_price[a] < buyer_price[b])
        return true;

    if(buyer_price[a] > buyer_price[b])
        return false;

    return a > b;
}

inline
bool Sellers_compare::operator()(int a, int b) const
{
    if(seller_price[a] > seller_price[b])
        return true;

    if(seller_price[a] < seller_price[b])
        return false;

    return a > b;
}

inline
void buy(int id, int size, int price)
{
    while(!sellers.empty() && canceled[sellers.top()])
        sellers.pop();

    while(size > 0 && !sellers.empty() && seller_price[sellers.top()] <= price)
    {
        int seller = sellers.top(); sellers.pop();
        int bought = std::min(seller_size[seller], size);
        size                                -= bought;
        seller_size[seller]                 -= bought;
        sum_ask_size[seller_price[seller]]  -= bought;

        if(seller_size[seller] > 0)
            sellers.push(seller);

        printf("TRADE %d %d\n", bought, seller_price[seller]);
        while(!sellers.empty() && canceled[sellers.top()])
            sellers.pop();
    }

    if(size > 0)
    {
        is_buyer[id]        = true;
        buyer_size[id]      = size;
        buyer_price[id]     = price;
        sum_bid_size[price] += size;
        buyers.push(id);
    }
}

inline
void sell(int id, int size, int price)
{
    while(!buyers.empty() && canceled[buyers.top()])
        buyers.pop();

    while(size > 0 && !buyers.empty() && buyer_price[buyers.top()] >= price)
    {
        int buyer = buyers.top(); buyers.pop();
        int sold = std::min(buyer_size[buyer], size);
        size                                -= sold;
        buyer_size[buyer]                   -= sold;
        sum_bid_size[buyer_price[buyer]]    -= sold;

        if(buyer_size[buyer] > 0)
            buyers.push(buyer);

        printf("TRADE %d %d\n", sold, buyer_price[buyer]);
        while(!buyers.empty() && canceled[buyers.top()])
            buyers.pop();
    }

    if(size > 0)
    {
        is_seller[id]       = true;
        seller_size[id]     = size;
        seller_price[id]    = price;
        sum_ask_size[price] += size;
        sellers.push(id);
    }
}

inline
void cancel(int id)
{
    if(canceled[id])
        return;

    canceled[id] = true;
    if(is_buyer[id])
        sum_bid_size[buyer_price[id]] -= buyer_size[id];

    if(is_seller[id])
        sum_ask_size[seller_price[id]] -= seller_size[id];
}

inline
void quote(void)
{
    int bid_price   = 0,
        ask_price   = 99999;

    while(!buyers.empty() && canceled[buyers.top()])
        buyers.pop();

    while(!sellers.empty() && canceled[sellers.top()])
        sellers.pop();

    if(!buyers.empty())
        bid_price = buyer_price[buyers.top()];

    if(!sellers.empty())
        ask_price = seller_price[sellers.top()];

    printf("QUOTE %d %d - %d %d\n", sum_bid_size[bid_price], bid_price, sum_ask_size[ask_price], ask_price);
}
