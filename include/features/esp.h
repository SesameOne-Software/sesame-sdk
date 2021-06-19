#ifndef FEATURES_ESP_H
#define FEATURES_ESP_H

#include "ses.h"

bool features_esp_init( void );
bool features_esp_free( void );
bool features_esp_level_init( void );
bool features_esp_level_shutdown( void );
void features_esp_run( void );

#endif // !FEATURES_ESP_H
