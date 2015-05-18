#include "../server/logger.h"

int main()
{
    log_open("log.txt");
    FM_LOG_NOTICE("just a test");
    return 0;   
}
