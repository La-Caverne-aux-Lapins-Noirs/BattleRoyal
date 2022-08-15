/*
** Jason Brillante "Damdoshi"
** Hanged Bunny Studio 2014-2022
**
** Battle Royal
*/

#include		"server.h"

char			get_tile(t_server		*server,
				 t_bunny_position	p)
{
  if (p.x < 0 || p .y < 0
      || p.y >= NBRCELL(server->current_map->tile)
      || p.x >= NBRCELL(server->current_map->tile))
    return (OUTSIDE);
  return (server->current_map->tile[p.y][p.x]);
}

