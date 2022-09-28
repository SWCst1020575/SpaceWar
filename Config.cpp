#include "Config.h"

ALLEGRO_CONFIG* cfg;

int config_read(const char* var) {
    cfg = al_load_config_file("Config.cfg");
    int val = std::atoi(al_get_config_value(cfg, "setting", var));
    al_destroy_config(cfg);
    return val;
}
int config_read(const char* sec, const char* var) {
    cfg = al_load_config_file("GameData.cfg");
    int val = std::atoi(al_get_config_value(cfg, sec, var));
    al_destroy_config(cfg);
    return val;
}
double config_read_double(const char* sec, const char* var) {
    cfg = al_load_config_file("GameData.cfg");
    double val = std::stod(std::string(al_get_config_value(cfg, sec, var)));
    al_destroy_config(cfg);
    return val;
}
void config_write(const char* var, int val) {
    cfg = al_load_config_file("Config.cfg");
    std::string val_str(std::to_string(val));
    al_set_config_value(cfg, "setting", var, val_str.c_str());
    al_save_config_file("Config.cfg", cfg);
    al_destroy_config(cfg);
}
void config_write(const char* sec, const char* var, int val) {
    cfg = al_load_config_file("GameData.cfg");
    if (cfg == NULL)
        cfg = al_create_config();
    al_add_config_section(cfg, sec);
    std::string val_str(std::to_string(val));
    al_set_config_value(cfg, sec, var, val_str.c_str());
    al_save_config_file("GameData.cfg", cfg);
    al_destroy_config(cfg);
}
void config_write(const char* sec, const char* var, double val) {
    cfg = al_load_config_file("GameData.cfg");
    if (cfg == NULL)
        cfg = al_create_config();
    al_add_config_section(cfg, sec);
    std::string val_str(std::to_string(val));
    al_set_config_value(cfg, sec, var, val_str.c_str());
    al_save_config_file("GameData.cfg", cfg);
    al_destroy_config(cfg);
}