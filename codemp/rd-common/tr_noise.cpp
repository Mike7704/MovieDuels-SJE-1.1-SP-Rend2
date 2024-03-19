/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2005 - 2015, ioquake3 contributors
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

// tr_noise.c
#include "tr_common.h"

#define NOISE_SIZE 256
#define NOISE_MASK ( NOISE_SIZE - 1 )

#define VAL( a ) s_noise_perm[ ( a ) & ( NOISE_MASK )]
#define INDEX( x, y, z, t ) VAL( x + VAL( y + VAL( z + VAL( t ) ) ) )

static float s_noise_table[NOISE_SIZE];
static int s_noise_perm[NOISE_SIZE];

#define LERP( a, b, w ) ( a * ( 1.0f - w ) + b * w )

static float GetNoiseValue(int x, int y, int z, int t)
{
	const int index = INDEX((int)x, (int)y, (int)z, (int)t);

	return s_noise_table[index];
}

float GetNoiseTime(int t)
{
	const int index = VAL(t);

	return (1 + s_noise_table[index]);
}

void R_NoiseInit(void)
{
	srand(1001);

	for (int i = 0; i < NOISE_SIZE; i++)
	{
		s_noise_table[i] = static_cast<float>(((rand() / (float)RAND_MAX) * 2.0 - 1.0));
		s_noise_perm[i] = static_cast<unsigned char>(rand() / (float)RAND_MAX * 255);
	}
}

float R_NoiseGet4f(float x, float y, float z, float t)
{
	float front[4];
	float back[4];
	float value[2];

	const int ix = static_cast<int>(floor(x));
	const float fx = x - ix;
	const int iy = static_cast<int>(floor(y));
	const float fy = y - iy;
	const int iz = static_cast<int>(floor(z));
	const float fz = z - iz;
	const int it = static_cast<int>(floor(t));
	const float ft = t - it;

	for (int i = 0; i < 2; i++)
	{
		front[0] = GetNoiseValue(ix, iy, iz, it + i);
		front[1] = GetNoiseValue(ix + 1, iy, iz, it + i);
		front[2] = GetNoiseValue(ix, iy + 1, iz, it + i);
		front[3] = GetNoiseValue(ix + 1, iy + 1, iz, it + i);

		back[0] = GetNoiseValue(ix, iy, iz + 1, it + i);
		back[1] = GetNoiseValue(ix + 1, iy, iz + 1, it + i);
		back[2] = GetNoiseValue(ix, iy + 1, iz + 1, it + i);
		back[3] = GetNoiseValue(ix + 1, iy + 1, iz + 1, it + i);

		const float fvalue = LERP(LERP(front[0], front[1], fx), LERP(front[2], front[3], fx), fy);
		const float bvalue = LERP(LERP(back[0], back[1], fx), LERP(back[2], back[3], fx), fy);

		value[i] = LERP(fvalue, bvalue, fz);
	}

	const float finalvalue = LERP(value[0], value[1], ft);

	return finalvalue;
}