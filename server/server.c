#include "server.h"

int main()
{
    log_open("log.txt");
    if (db_init() < 0) {
        FM_LOG_MONITOR("Failed to init database.");
        return -1;
    }
    if (server_start() < 0) {
        FM_LOG_MONITOR("Failed to start server."); 
        return -1;
    }
    return 0;
}
