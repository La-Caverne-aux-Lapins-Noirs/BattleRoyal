/*
** Jason Brillante "Damdoshi"
** Hanged Bunny Studio 2014-2022
**
** Battle Royal
*/

#include		"server.h"

t_bunny_response	message(int		fd,
				const char	*buffer,
				size_t		size,
				t_server	*server)
{
  t_bunny_position	cpos = server->players[fd].position;
  bool			badcommand = false;

  if (server->players[fd].active == false)
    return (GO_ON);

  if (strncmp(buffer, "left", size) == 0)
    cpos.x -= 1;
  else if (strncmp(buffer, "right", size) == 0)
    cpos.x += 1;
  else if (strncmp(buffer, "up", size) == 0)
    cpos.y -= 1;
  else if (strncmp(buffer, "down", size) == 0)
    cpos.y += 1;
  else
    badcommand = true;
  if (get_tile(server, cpos) != WALKABLE)
    return (GO_ON);
  server->players[fd].position = cpos;
  if (strncmp(buffer, "look", size) == 0)
    {
      char		view[128];
      t_bunny_position	see;
      int		tile;
      int		n = 0;

      for (see.y = cpos.y - 3; see.y <= cpos.y + 3; ++see.y)
	{
	  for (see.x = cpos.x - 3; see.x <= cpos.x + 3; ++see.x)
	    {
	      if ((tile = get_tile(server, see)) != OUTSIDE)
		{
		  for (int p = 0; p < NBRCELL(server->players); ++p)
		    if (server->players[p].position.x == see.x
			&& server->players[p].position.y == see.y)
		      {
			if (p == fd)
			  tile = 'O';
			else
			  tile = '!';
		      }
		  server->shadow[see.y][see.x] = true;
		  view[n++] = tile;
		}
	      else
		view[n++] = 'X';
	    }
	  view[n++] = '\n';
	}
      view[n] = '\0';
      bunny_server_write(server->server, &view[0], n, fd);
    }
  else if (strncmp(buffer, "shoot", size < 5 ? size : 5) == 0)
    {
      t_bullet		*b = &server->bullets[server->nbr_bullet];

      if (server->nbr_bullet >= NBRCELL(server->bullets))
	return (GO_ON);
      b->position = cpos;
      if (strncmp(buffer, "shootleft", size) == 0)
	b->position.x = cpos.x - 1;
      else if (strncmp(buffer, "shootright", size) == 0)
	b->position.x = cpos.x + 1;
      else if (strncmp(buffer, "shootup", size) == 0)
	b->position.y = cpos.y - 1;
      else if (strncmp(buffer, "shootdown", size) == 0)
	b->position.y = cpos.y + 1;
      else
	return (GO_ON);
      bunny_server_write(server->server, "BANG", 4, fd);
      b->speed.x = b->position.x - cpos.x;
      b->speed.y = b->position.y - cpos.y;
      server->nbr_bullet += 1;
    }
  else if (strncmp(buffer, "position", size) == 0)
    {
      char		buffer[32];

      snprintf(&buffer[0], sizeof(buffer), "%d %d", server->players[fd].position.x, server->players[fd].position.y);
      bunny_server_write(server->server, &buffer[0], strlen(buffer), fd);
    }
  else if (badcommand)
    bunny_server_write(server->server, "Invalid command", 15, fd);
  return (GO_ON);
}

