/*
** Jason Brillante "Damdoshi"
** Hanged Bunny Studio 2014-2022
**
** BattleRoyal
*/

#ifndef			__SERVER_H__
# define		__SERVER_H__
# include		<lapin.h>

typedef enum		e_tile
  {
   OUTSIDE		= -1,
   WALKABLE		= '.',
   WALL			= 'X'
  }			t_tile;

typedef struct		s_map
{
  // Les cartes sont toujours carres
  char			*tile[32];
}			t_map;

extern t_map		gl_maps[2];

typedef struct		s_player
{
  bool			active;
  t_bunny_position	position;
}			t_player;

typedef struct		s_bullet
{
  t_bunny_position	position;
  t_bunny_position	speed;
}			t_bullet;

typedef struct		s_server
{
  t_bunny_configuration	*configuration;
  bool			shadow[32][32];
  t_map			*current_map;
  t_bunny_server	*server;
  t_player		players[64];
  t_bullet		bullets[256];
  size_t		nbr_bullet;
}			t_server;

char			get_tile(t_server		*server,
				 t_bunny_position	pos);


#endif	/*		__SERVER_H__		*/
