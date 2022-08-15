/*
** Jason Brillante "Damdoshi"
** Hanged Bunny Studio 2014-2022
**
** Battle Royal
*/

#include		"server.h"

t_bunny_response	loop(void			*server);
t_bunny_response	new_player(int			fd,
				   t_bunny_event_state	status,
				   void			*server);
t_bunny_response	message(int			fd,
				const void		*buffer,
				size_t			size,
				void			*data);

int			main(int			argc,
			     char			**argv)
{
  int			port;
  
  if (argc == 1)
    {
      port = 8086;
      fprintf(stderr, "Server will use port %d.\n", port);
    }
  else if ((port = atoi(argv[1])) < 1024 || port > 65535)
    {
      fprintf(stderr, "Invalid port. Must be an integer inside [1024; 65535].\n");
      return (EXIT_FAILURE);
    }
  static t_server	server;

  if ((server.server = bunny_new_server(port)) == NULL)
    {
      bunny_perror("bunny_new_server");
      return (EXIT_FAILURE);
    }
  int			map;
  
  if (argc == 2)
    map = 0;
  else if ((map = atoi(argv[2])) < 0 || map >= NBRCELL(gl_maps))
    {
      fprintf(stderr, "Invalid map number. Must be an integer inside [0; %d].\n", NBRCELL(gl_maps));
      return (EXIT_FAILURE);
    }
  server.current_map = &gl_maps[map];

  server.configuration = bunny_new_configuration();
  bunny_set_server_to_scheduler(server.server);
  bunny_set_connect_response(new_player);
  bunny_set_message_response(message);
  bunny_set_loop_main_function(loop);
  bunny_loop(NULL, 2, &server);
  bunny_delete_server(server.server);
  return (EXIT_SUCCESS);
}
