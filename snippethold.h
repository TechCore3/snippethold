/*
 * clipstore - A lightweight CLI snippet manager
 * Copyright (C) 2026 TechCore3
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef SNIPPETHOLD_H
#define SNIPPETHOLD_H

int write_file(const char *content, const char *file);

int get_single_entry(const char *file);

int remove_entry(const char *file);

int get_all_entries(const char *dir_path);

#endif
