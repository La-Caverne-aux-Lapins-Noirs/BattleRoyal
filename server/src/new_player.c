/*
** Jason Brillante "Damdoshi"
** Hanged Bunny Studio 2014-2022
**
** Battle Royal
*/

#include		"server.h"

t_bunny_response	message(int			fd,
				const char		*buffer,
				size_t			size,
				t_server		*server);
t_bunny_response	new_player(int			fd,
				   t_bunny_event_state	status,
				   t_server		*server)
{
  if (status == DISCONNECTED)
    {
      fprintf(stderr, "Player %d left.\n", fd);
      server->players[fd].active = false;
      return (GO_ON);
    }

  if ((fd >= NBRCELL(server->players)))
    {
      fprintf(stderr, "Player %d rejected.\n", fd);
      bunny_server_write(server->server, "Server is full", 14, fd);
      bunny_server_doom_client(server->server, fd);
      return (GO_ON);
    }
  fprintf(stderr, "Player %d accepted.\n", fd);
  bunny_server_write(server->server, "Welcome", 7, fd);
  memset(&server->players[fd], 0, sizeof(server->players[fd]));
  server->players[fd].active = true;
  do
    {
      server->players[fd].position.x = rand() % NBRCELL(server->current_map->tile);
      server->players[fd].position.y = rand() % NBRCELL(server->current_map->tile[0]);
    }
  while (get_tile(server, server->players[fd].position) != WALKABLE);
  return (message(fd, "look", 4, server));
}

