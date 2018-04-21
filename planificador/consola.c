#include <stdio.h>

#include "../libs/comando.h"
#include "consola.h"

static void pause_scheduler(char **_);
static void lock_process(char **args);
static void unlock_process(char **args);
static void list_locked_process(char **args);
static void kill_process(char **args);
static void instance_status(char **args);
static void check_deadlock(char **_);

#define COMMANDS_SIZE (sizeof(commands) / sizeof(*commands))

static struct command_t commands[] = {
	DEF_COMMAND("pausar",      0, pause_scheduler),
	DEF_COMMAND("bloquear",    2, lock_process),
	DEF_COMMAND("desbloquear", 1, unlock_process),
	DEF_COMMAND("listar",      1, list_locked_process),
	DEF_COMMAND("kill",        1, kill_process),
	DEF_COMMAND("status",      1, instance_status),
	DEF_COMMAND("deadlock",    0, check_deadlock)
};

int execute_console_command(char *command_line)
{
	return execute_command_line(commands, COMMANDS_SIZE, command_line);
}

static void pause_scheduler(char **_)
{
	printf("Pausar planificacion\n");
}

static void lock_process(char **args)
{
	void _lock_process(char *key, char *pid) {
		printf("Bloquear proceso ESI (clave = %s, id = %s)\n", key, pid);
	}
	_lock_process(args[0], args[1]);
}

static void unlock_process(char **args)
{
	void _unlock_process(char *key) {
		printf("Desbloquear proceso ESI (clave = %s)\n", key);
	}

	_unlock_process(args[0]);
}

static void list_locked_process(char **args)
{
	void _list_locked_process(char *resource) {
		printf("Listar processos bloqueados por recurso %s\n", resource);
	}
	_list_locked_process(args[0]);
}

static void kill_process(char **args)
{
	void _kill_process(char *pid) {
		printf("Finalizar proceso %s\n", pid);
	}
	_kill_process(args[0]);
}

static void instance_status(char **args)
{
	void _instance_status(char *key) {
		printf("Informar estado de la instancia %s\n", key);
	}
	_instance_status(args[0]);
}

static void check_deadlock(char **_)
{
	printf("Detectar deadlock\n");
}