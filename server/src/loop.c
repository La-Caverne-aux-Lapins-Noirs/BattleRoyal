/*
** Jason Brillante "Damdoshi"
** Hanged Bunny Studio 2014-2022
**
** Battle Royal
*/

#include		<sys/stat.h>
#include		"server.h"

t_bunny_response	loop(t_server		*server)
{
  
  bunny_configuration_setf(server->configuration, 0, "nbr_players"); 
  for (size_t i = 0; i < NBRCELL(server->players); ++i)
    {
      bunny_configuration_setf
	(server->configuration, (int)server->players[i].active, "players[%zu].active", i);
      bunny_configuration_setf
	(server->configuration, server->players[i].position.x, "players[%zu].x", i);
      bunny_configuration_setf
	(server->configuration, server->players[i].position.y, "players[%zu].y", i);
    }
  for (size_t y = 0; y < NBRCELL(server->current_map->tile); ++y)
    for (size_t x = 0; x < NBRCELL(server->current_map->tile); ++x)
      {
	if (server->shadow[y][x] == false)
	  bunny_configuration_setf
	    (server->configuration, -1, "map[%zu][%zu]", y, x);
	else
	  bunny_configuration_setf
	    (server->configuration, ((int)server->current_map->tile[y][x]) & 255, "map[%zu][%zu]", y, x);
      }
  bunny_configuration_setf(server->configuration, NULL, "bullets");
  for (size_t i = 0, j = 0; i < server->nbr_bullet; ++i)
    {
      for (size_t k = 0; k < NBRCELL(server->players); ++k)
	if (server->players[k].active
	    && server->players[k].position.x == server->bullets[i].position.x
	    && server->players[k].position.y == server->bullets[i].position.y)
	  {
	    bunny_server_write(server->server, "You are dead", 12, k);
	    bunny_server_doom_client(server->server, k);
	    server->players[k].active = false;
	  }
      if (get_tile(server, server->bullets[i].position) == WALKABLE)
	{
	  bunny_configuration_setf
	    (server->configuration, server->bullets[i].position.x, "bullets[%zu].x", j);
	  bunny_configuration_setf
	    (server->configuration, server->bullets[i].position.y, "bullets[%zu].y", j);
	  server->bullets[i].position.x += server->bullets[i].speed.x;
	  server->bullets[i].position.y += server->bullets[i].speed.y;
	  j += 1;
	}
      else
	{
	  bunny_memswap(&server->bullets[server->nbr_bullet], &server->bullets[i], sizeof(server->bullets[j]));
	  server->nbr_bullet -= 1;
	}
    }
  const char		*conf_file = "/var/www/atlantis.json";
  
  if (bunny_save_configuration(BC_JSON, conf_file, server->configuration) == false)
    fprintf(stderr, "Cannot write JSON configuration for web interface.\n");
  chmod(conf_file, 0644);
  return (GO_ON);
}

