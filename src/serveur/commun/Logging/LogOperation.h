
#ifndef LOGOPERATION_H
#define LOGOPERATION_H

class Logger;
struct LogMessage;

class LogOperation
{
    public:
        LogOperation(Logger const* _logger, LogMessage* _msg)
            : logger(_logger), msg(_msg)
        { }

        ~LogOperation();

        int call();

    protected:
        Logger const* logger;
        LogMessage* msg;
};

#endif
