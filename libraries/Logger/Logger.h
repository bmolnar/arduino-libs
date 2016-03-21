#ifndef LOGGER_H_
#define LOGGER_H_

#include "PrintfWrap.h"

struct logtag
{
  const char *  file;
  unsigned long line;
  const char *  func;
};
typedef struct logtag logtag_t;
#define LOGTAG_INIT3(fi,li,fn)  { .file = (fi), .line = (li), .func = (fn) }
#define LOGTAG_INIT()           LOGTAG_INIT3(__FILE__, __LINE__, __func__)
#define LOGTAG_DECLARE(name)    static logtag_t name = LOGTAG_INIT()



typedef unsigned long LogEntryId;
typedef unsigned long LogTimestamp;

class LogEntryInfo
{
 private:
  const logtag_t *tag_;
  const LogTimestamp ts_;
  const LogEntryId id_;

  LogEntryInfo(const logtag_t *tag, LogTimestamp ts, LogEntryId id);
  LogEntryInfo(const logtag_t *tag, LogTimestamp ts);

 public:
  LogEntryInfo(const logtag_t *tag);

  LogEntryId       id()  const { return this->id_; }
  LogTimestamp     ts()  const { return this->ts_; }
  const logtag_t * tag() const { return this->tag_; }

  static LogEntryId s_lastid;
  static LogEntryId nextid() { return ++LogEntryInfo::s_lastid; }
  static LogTimestamp now() { return millis(); }
};

class LogEntryData
{
 private:
  const void *buf_;
  unsigned int buflen_;
 public:
  LogEntryData(const void *buf, unsigned int buflen) : buf_(buf), buflen_(buflen) {}
  const void *buf() const { return this->buf_; }
  const unsigned int buflen() { return this->buflen_; }
};

class LogEntry
{
 private:
  const LogEntryInfo info_;
  const LogEntryData data_;
 public:
  LogEntry(const LogEntryInfo& info, const LogEntryData& data) : info_(info), data_(data) {}
  const LogEntryInfo& info() const { return this->info_; }
  const LogEntryData& data() const { return this->data_; }
};









class LogFormatter
{
 public:
  bool print_entry(const LogEntry& entry);
};



class Logger
{
 public:
  virtual const char * type() const { return "Logger"; }
  virtual bool append_entry(const LogEntryInfo& info, const LogEntryData& data) = 0;
};
class NullLogger : public Logger
{
public:
  const char * type() const { return "NullLogger"; }
  bool append_entry(const LogEntryInfo& info, const LogEntryData& data);
};
class PrintLogger : public Logger
{
 private:
  PrintfWrap printf_;
 public:
  PrintLogger(Print& print);
  const char * type() const { return "PrintLogger"; }
  bool append_entry(const LogEntryInfo& info, const LogEntryData& data);
};





class LogHandle : public Printf
{
 private:
  Logger& logger_;
  const LogEntryInfo& einfo_;
  uint8_t buf_[64];
  unsigned int bufpos_;

 public:
  LogHandle(Logger& logger, const LogEntryInfo& einfo);
  ~LogHandle();

  const void *dataptr()  const { return &this->buf_[0]; }
  unsigned int datalen() const { return this->bufpos_; }
  const char *datastr()  const { return (const char *) this->dataptr(); }
  size_t write(uint8_t);
};





class ManagedLogger
{
 private:
  Logger* logger_;
 public:
  ManagedLogger(Logger* logger);
  static Logger* s_logger;
  static Logger* get(void);
  static void set(Logger* logger);
};

#define LOG(fmt,...) { LOGTAG_DECLARE(__tag); LogHandle __hnd(*ManagedLogger::get(), LogEntryInfo(&__tag)); __hnd.printf(fmt, __VA_ARGS__); }

#endif // LOGGER_H_

