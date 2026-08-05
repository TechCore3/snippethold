#ifndef CLIPSTORE_H
#define CLIPSTORE_H

int write_file(const char *content, const char *file);

int get_single_entry(const char *file);

int remove_entry(const char *file);

int get_all_entries(const char *dir_path);

#endif
