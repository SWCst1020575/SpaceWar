#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <allegro5/allegro.h>

#include <iostream>
#include <string>

int config_read(const char*);
int config_read(const char*, const char*);
double config_read_double(const char*, const char*);
void config_write(const char*, int);
void config_write(const char*, const char*, int);
void config_write(const char*, const char*, double);

#endif  // CONFIG_H_INCLUDED
