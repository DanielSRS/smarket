#include "../Map/Map.h"

#ifndef READTAGS_H
#define READTAGS_H

/**
 * Le as tags e salva os ids no buffer
 */
Map* readTags();

/**
 * Le as tags e salva os ids no buffer
 */
Map* readTagsWithArgs(char *tmr, char *antena, Map * result);

#endif //READTAGS_H