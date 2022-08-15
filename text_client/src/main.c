/*
** Jason Brillante "Damdoshi"
** Hanged Bunny Studio 2014-2022
**
** Battle Royal - Text client
*/

#include		<stdio.h>
#include		<fcntl.h>
#include		<lapin.h>

void			clear_screen(void)
{
  system("clear");
}

bool			custom_command(t_bunny_client	*client,
				       char		*command)
{
  // Do custom commands here
  (void)client;
  (void)command;
  return (false);
}

bool			custom_reading(t_bunny_client	*client,
				       char		*last_command,
				       char		*message)
{
  // React better here
  (void)client;
  (void)last_command;
  (void)message;
  return (false);
}

int			main(int			argc,
			     char			**argv)
{
  t_bunny_client	*client;
  int			port;

  if (argc != 3 || (port = atoi(argv[2])) < 1024 || port >= 65356)
    {
      fprintf(stderr, "%s: %s ip port[1024-65535]\n", *argv, *argv);
      return (EXIT_FAILURE);
    }
  if ((client = bunny_new_client(argv[1], port)) == NULL)
    {
      bunny_perror("connect");
      return (EXIT_FAILURE);
    }
  const t_bunny_communication *com;
  char			buffer[512];
  int			bufsize;
  int			red;
  bool			err;

  err = false;
  bufsize = 0;
  buffer[0] = '\0';
  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
  do
    {
      if ((red = read(0, &buffer[bufsize], 1)) == -1)
	{
	  if (errno != EAGAIN)
	    {
	      perror("read");
	      err = true;
	      break ;
	    }
	  red = 0;
	}
      else if ((bufsize += red) >= (int)sizeof(buffer))
	{
	  fprintf(stderr, "Input buffer is full. Exiting.\n");
	  err = true;
	  break ;
	}
      if (bufsize > 0 && buffer[bufsize - 1] == '\n')
	{
	  buffer[bufsize - 1] = '\0';
	  if (custom_command(client, &buffer[0]) == false)
	    bunny_client_write(client, &buffer[0], bufsize - 1);
	  bufsize = 0;
	}
      com = bunny_client_poll(client, 10);
      if (com->comtype == BCT_MESSAGE)
	{
	  char		tmp[com->message.size + 1];

	  memcpy(&tmp[0], com->message.buffer, com->message.size);
	  tmp[com->message.size] = '\0';
	  if (custom_reading(client, &buffer[0], &tmp[0]) == false)
	    {
	      write(1, com->message.buffer, com->message.size);
	      write(1, "\n", 1);
	    }
	}
    }
  while (com == NULL || com->comtype != BCT_NETDISCONNECTED);
  bunny_delete_client(client);
  if (err)
    return (EXIT_FAILURE);
  return (EXIT_SUCCESS);
}

