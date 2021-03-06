// SPDX-License-Identifier: GPL-2.0+
#include "sparx5_main.h"

void sparx5_pgid_init(struct sparx5 *spx5)
{
	int i;

	for (i = 0; i < PGID_TABLE_SIZE; i++)
		spx5->pgid_map[i] = SPX5_PGID_FREE;

	/* Reserved for unicast, flood control, broadcast, and CPU.
	 * These cannot be freed.
	 */
	for (i = 0; i <= PGID_CPU; i++)
		spx5->pgid_map[i] = SPX5_PGID_RESERVED;
}

int sparx5_pgid_alloc_glag(struct sparx5 *spx5, u16 *idx)
{
	int i;

	for (i = PGID_GLAG_START; i <= PGID_GLAG_END; i++)
		if (spx5->pgid_map[i] == SPX5_PGID_FREE) {
			spx5->pgid_map[i] = SPX5_PGID_GLAG;
			*idx = i;
			return 0;
		}

	return -EBUSY;
}

int sparx5_pgid_alloc_mcast(struct sparx5 *spx5, u16 *idx)
{
	int i;

	for (i = PGID_MCAST_START; i < PGID_TABLE_SIZE; i++) {
		if (i == PGID_GLAG_START)
			i = PGID_GLAG_END + 1;

		if (spx5->pgid_map[i] == SPX5_PGID_FREE) {
			spx5->pgid_map[i] = SPX5_PGID_MULTICAST;
			*idx = i;
			return 0;
		}
	}

	return -EBUSY;
}

int sparx5_pgid_free(struct sparx5 *spx5, u16 idx)
{
	if (idx <= PGID_CPU || idx >= PGID_TABLE_SIZE)
		return -EINVAL;

	if (spx5->pgid_map[idx] == SPX5_PGID_FREE)
		return -EINVAL;

	spx5->pgid_map[idx] = SPX5_PGID_FREE;
	return 0;
}
