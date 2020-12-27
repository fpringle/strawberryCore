/* Copyright 2020 Freddy Pringle */
#ifndef SRC_CORE_UTIL_H_
#define SRC_CORE_UTIL_H_

#include <time.h>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define LOG_DIR "log/"
#define LOG_EXT ".log"

/**
 *  \namespace chessUtil
 *  \brief The namespace for general utility functions
 */
namespace chessUtil {

/**
 *  Return a string with the current time formatting to a specific pattern.
 *
 *  \param fmt      The pattern to format the time string
 *  \return         Formatted string with the current time.
 */
std::string timestring(const char* fmt = "%Y/%m/%d %H:%M:%S") {
    time_t t = time(0);
    struct tm* now = new tm;
    localtime_r(&t, now);
    std::string timestr(80, '\0');
    int len = strftime(&timestr[0], timestr.size(), fmt, now);
    return timestr.substr(0, len);
}

/**
 *  \enum LogLevel
 *  \brief An Enum representing the severity of a log message.
 */
enum LogLevel {
    logDEBUG,
    logINFO,
    logWARN,
    logERROR,
    logFATAL
};


/**
 *  Convert a LogLevel object to a string.
 *
 *  \param level        The LogLevel object to convert
 *  \return             A string representing the log message severity.
 */
inline std::string levelString(LogLevel level) {
    return (level == logDEBUG ? "DEBUG" :
            level == logINFO ? "INFO" :
            level == logWARN ? "WARNING" :
            level == logERROR ? "ERROR" :
            level == logFATAL ? "FATAL" :
            "UNKNOWN");
}

/**
 *  \class Logger
 *  \brief A simple class for logging to file
 */
class Logger {
 private:
    /** The file stream to write to. */
    std::ofstream logfile;

 public:
    /**
     *  Default constructor for Logger objects.
     *
     *  \param filename     The name of the logfile, without log/ or .log
     */
    explicit Logger(std::string filename);
    /** Destructor for Logger class. */
    virtual ~Logger();
    /**
     *  \brief Overloaded insertion operator.
     *
     *  Write a message to a log file with a certain message severity.
     *  For example, to write "2020/12/27 19:45:37 | WARNING | Example warning message"
     *  to the file "log/logfile.log":
     *
     *  \code
     *  Logger log("logfile");
     *  log << logWARN << "Example warning message";
     *  \endcode
     *
     *  \param log      The Logger object
     *  \param level    The log message severity.
     *  \return         The file object to write to.
     */
    friend std::ofstream& operator<<(Logger& log, LogLevel level);
};

Logger::Logger(std::string filename) {
    logfile.open(LOG_DIR + filename + LOG_EXT, std::ofstream::app);
}

std::ofstream& operator<<(Logger& log, LogLevel level) {
    log.logfile << std::endl;
    log.logfile << timestring() << "  | ";
    log.logfile << std::setw(7);
    log.logfile << levelString(level) << " |  ";
    return log.logfile;
}

Logger::~Logger() {
    logfile << std::endl;
    logfile.close();
}

}   // namespace chessUtil


#endif  // SRC_CORE_UTIL_H_
