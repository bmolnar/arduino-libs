#include <Arduino.h>

#include "Logger.h"

#include "xmembuf.h"
#include "xstream.h"


LogEntryInfo::LogEntryInfo(const logtag_t *tag, LogTimestamp ts, LogEntryId id)
  : tag_(tag), ts_(ts), id_(id)
{
}
LogEntryInfo::LogEntryInfo(const logtag_t *tag, LogTimestamp ts)
  : LogEntryInfo(tag, ts, LogEntryInfo::nextid())
{
}
LogEntryInfo::LogEntryInfo(const logtag_t *tag)
  : LogEntryInfo(tag, LogEntryInfo::now(), LogEntryInfo::nextid())
{
}
LogEntryId LogEntryInfo::s_lastid = 0;



LogHandle::LogHandle(Logger& logger, const LogEntryInfo& einfo)
  : logger_(logger), einfo_(einfo), bufpos_(0)
{
  this->buf_[0]='\0';
}
LogHandle::~LogHandle()
{
  this->logger_.append_entry(this->einfo_, LogEntryData(&this->buf_[0], this->bufpos_));
}
size_t LogHandle::write(uint8_t wbyte)
{
  if (this->bufpos_ + 1 >= sizeof(this->buf_))
  {
    return 0;
  }
  this->buf_[this->bufpos_++] = wbyte;
  this->buf_[this->bufpos_] = '\0';
  return 1;
}



bool NullLogger::append_entry(const LogEntryInfo& info, const LogEntryData& data)
{
  return true;
}

PrintLogger::PrintLogger(Print& print)
  : printf_(print)
{
}
bool PrintLogger::append_entry(const LogEntryInfo& info, const LogEntryData& data)
{
  this->printf_.printf("id=%lu: %s", info.id(), data.buf());
  return true;
}


static NullLogger _definst;
Logger* ManagedLogger::s_logger = &_definst;
ManagedLogger::ManagedLogger(Logger* logger)
  : logger_(logger)
{
  ManagedLogger::s_logger = logger;
}
Logger* ManagedLogger::get()
{
  return ManagedLogger::s_logger;
}
void ManagedLogger::set(Logger* logger)
{
  ManagedLogger::s_logger = logger;
}
