/**
 *  Defines the index_t struct, to represent an index.
 *
 *  Copyright (C) 2022-2023  Saúl Valdelvira Iglesias
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  Email: saulvaldelvira@gmail.com
 *  Version: 11-01-2023
 */
#pragma once
#ifndef INDEX_T_H
#define INDEX_T_H

/**
 * Represents a position.
 * This structure is used when an index must be returned by a function. Since these strcutures use
 * size_t (long unsigned int) we must wrap this index along an integer that represent if the value
 * is actually useful or else the operation has failed.
 * If status inidcates success, the value is valid. Else, it is just garbage.
*/
typedef struct index{
	size_t value;
	int status;
} index_t;

#define index_t(i,s)    (index_t) {.value = i, .status = s}

#endif
