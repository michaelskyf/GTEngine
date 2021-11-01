/*
    This file is part of GTEngine.

    GTEngine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GTEngine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GTEngine. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GTE_MATH_H
#define GTE_MATH_H

/*
 * This file defines:
 * vec2, vec3, vec4
 *
 */

typedef struct {
	union {
		struct {float x,y;};
		float v[2];
	};
} vec2;

typedef struct {
	union {
		struct {float x,y,z;};
		float v[3];
	};
} vec3;

typedef struct {
	union {
		struct {float x,y,z,w;};
		float v[4];
	};
} vec4;

#endif
