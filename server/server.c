#include "server.h"
#include "client.c"
#include "session.c"

int main()
{
    ClientList clientlist;
    SessionList sessionlist;

    clientlist_init(&clientlist);
    sessionlist_init(&sessionlist);

    log_open("log.txt");

    if (db_init(&clientlist) < 0) {
        FM_LOG_MONITOR("Failed to init database.");
        return -1;
    }

    if (server_start() < 0) {
        FM_LOG_MONITOR("Failed to start server."); 
        return -1;
    }
    return 0;
}
