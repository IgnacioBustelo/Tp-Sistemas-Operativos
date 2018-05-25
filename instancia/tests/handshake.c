#include "../../libs/chunker.h"
#include "../../libs/messenger.h"
#include "../../libs/mocks/client_server.h"
#include "../../libs/conector.h"
#include "../coordinator_api.h"
#include "../globals.h"

int total_entries, entry_size;

void client_server_execute_server(int fd_client) {
	receive_handshake(fd_client);

	messenger_show("INFO", "[COORDINADOR]: Se recibio una solicitud de handshake de una instancia");

	messenger_show("INFO", "[COORDINADOR]: Se envio confirmacion de handshake");

	messenger_show("INFO", "[COORDINADOR]: Esperando recibir el nombre de la instancia");

	send_confirmation(fd_client, true);

	char* received_name;

	chunk_recv_variable(fd_client, (void**) &received_name);

	messenger_show("INFO", "[COORDINADOR]: Se recibio el nombre de la instancia, que se llama %s", received_name);

	free(received_name);

	chunk_t* chunk = chunk_create();

	storage_setup_t setup = {.total_entries = total_entries, .entry_size = entry_size};

	messenger_show("INFO", "[COORDINADOR]: Se va a enviar el tamano del storage de %d entradas de tamano %d", setup.total_entries, setup.entry_size);

	chunk_add(chunk, &setup.entry_size, sizeof(size_t));

	chunk_add(chunk, &setup.total_entries, sizeof(size_t));

	void* serialized_chunk = chunk_build(chunk);

	chunk_send(fd_client, serialized_chunk, chunk->current_size);

	chunk_destroy(chunk);

	free(serialized_chunk);
}

void client_server_execute_client(int fd_server) {
	fd_coordinador = fd_server;

	storage_setup_init(0, 0);

	coordinator_api_handshake(client_name, storage_setup);

	storage_setup_destroy();
}

int main(int argc, char* argv[]) {
	server_name = "Coordinador";

	total_entries = (argc < 2) ? 16 : atoi(argv[1]);
	entry_size = (argc < 3) ? 4 : atoi(argv[2]);
	client_name = (argc < 4) ? "Instancia 1" : argv[3];

	client_server_run();
}
