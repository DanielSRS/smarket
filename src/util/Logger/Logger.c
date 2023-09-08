#include "Logger.h"
#include <stdio.h> // printf
#include <stdlib.h> // malloc
#include "../Cstrings/Cstrings.h" // duplicateString

typedef struct _loggerOpt {
  alocatedCString namespace;
  void (*destroy)(struct _loggerOpt** self);
} loggerOptions;

void destroyLoggerOptions(loggerOptions** self) {
  if(!*self) return;
  alocatedCString namespace = (*self)->namespace;

  if (namespace) free(namespace);                 // libera os membros da estrutura
  free(*self);                                    // libera a própria estrutura
  *self = NULL;
}

loggerOptions* newLoggerOptions() {
  loggerOptions* _newLoggerOptions = calloc(1, sizeof(loggerOptions));
  _newLoggerOptions->destroy = destroyLoggerOptions;
  return _newLoggerOptions;
}

/** Envia para a saída padrão a mensagem de log */
void debug(Logger* self, const char* logMessage) {
  printf("DEBUG : %s", logMessage);
}

/** Envia para a saída padrão a mensagem de log */
void info(Logger* self, const char* logMessage) {
  printf("INFO : %s", logMessage);
}

/** Envia para a saída padrão a mensagem de log */
void warn(Logger* self, const char* logMessage) {
  printf("WARN : %s", logMessage);
}

/** Envia para a saída de erro a mensagem de log */
void error(Logger* self, const char* logMessage) {
  printf("ERROR : %s", logMessage);
}

/** Extende o logger para o namespace informado */
Logger* extend(Logger* self, const char* namespace) {
  self->_loggerOptions->namespace = duplicateString(namespace);
  return self;
}

/** Destroi o logger */
void destroy(Logger** self) {
  if (!*self) return;
  if (!(*self)->_loggerOptions) return;

  (*self)->_loggerOptions->destroy(&((*self)->_loggerOptions));

  free(*self);
  *self = NULL;
}

Logger* createLogger() {
  Logger *newLogger = malloc(sizeof(Logger));
  newLogger->debug = debug;
  newLogger->destroy = destroy;
  newLogger->error = error;
  newLogger->extend = extend;
  newLogger->info = info;
  newLogger->warn = warn;
  newLogger->_loggerOptions = newLoggerOptions();

  return newLogger;
}