#ifndef LOGGER_H
#define LOGGER_H

typedef struct _loggerOpt loggerOptions;

/** Objeto Logger */
typedef struct Logger
{
  loggerOptions* _loggerOptions;
  /** Log com nível de severidade: Debug */
  void (*debug)(struct Logger* self, const char* logMessage);
   /** Log com nível de severidade: Info */
  void (*info)(struct Logger* self, const char* logMessage);
   /** Log com nível de severidade: Warn */
  void (*warn)(struct Logger* self, const char* logMessage);
   /** Log com nível de severidade: Error */
  void (*error)(struct Logger* self, const char* logMessage);
  /** Estender o logger para namespaces diferentes */
  struct Logger* (*extend)(struct Logger* self, const char* namespace);
  /** Destroi o objeto */
  void (*destroy)(struct Logger** self);
} Logger;

Logger* createLogger();

#endif // LOGGER_H