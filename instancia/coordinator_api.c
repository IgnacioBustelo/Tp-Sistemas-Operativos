#include <commons/string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../libs/chunker.h"
#include "../libs/conector.h"
#include "../libs/messenger.h"
#include "coordinator_api.h"
#include "globals.h"

void coordinator_api_connect(char* host, int port) {
	// TODO: Manejar error cuando no encuentra al Coordinador

	fd_coordinador = connect_to_server(host, port);
}

void coordinator_api_handshake(char* instance_name, storage_setup_t* setup){
	chunk_t* chunk;
	void* serialized_chunk;
	bool is_confirmed;

	messenger_show("INFO", "Enviada la solicitud de handshake con el Coordinador");

	messenger_show("INFO", "Esperando confirmacion de handshake");

	send_handshake(fd_coordinador, INSTANCE);

	receive_confirmation(fd_coordinador, &is_confirmed);

	messenger_show("INFO", "Recibida la confirmacion de handshake");

	if (is_confirmed){

		messenger_show("INFO", "El Coordinador acepto la solicitud de handshake");

		messenger_show("INFO", "Se prepara el envio del nombre de la instancia (%s)", instance_name);

		chunk = chunk_create();

		chunk_add_variable(chunk, instance_name, string_length(instance_name) + 1);

		serialized_chunk = chunk_build(chunk);

		chunk_send(fd_coordinador, serialized_chunk, chunk->current_size);

		chunk_recv(fd_coordinador, &setup->entry_size, sizeof(size_t));

		chunk_recv(fd_coordinador, &setup->total_entries, sizeof(size_t));

		messenger_show("INFO", "Se asigno una dimension de %d entradas de tamano %d para el Storage", setup->total_entries, setup->entry_size);

		chunk_destroy(chunk);

		free(serialized_chunk);
	}

	else {

		messenger_show("ERROR", "El Coordinador rechazo la solicitud de handshake");

		// TODO: Manejar error

	}

}

request_coordinador coordinator_api_receive_header() {
	request_coordinador header;

	chunk_recv(fd_coordinador, &header, sizeof(request_coordinador));

	return header;
}

key_value_t* coordinator_api_receive_set() {
	char *key, *value;

	chunk_recv_variable(fd_coordinador, (void**) &key);
	chunk_recv_variable(fd_coordinador, (void**) &value);

	key_value_t* key_value = key_value_create(key, value);

	free(key);
	free(value);

	return key_value;
}

void coordinator_api_notify_status(int status) {
	chunk_t* chunk = chunk_create();
	request_instancia header = PROTOCOL_IC_NOTIFY_STATUS;

	chunk_add(chunk, &header, sizeof(header));
	chunk_add(chunk, &status, sizeof(status));

	void* status_message = chunk_build(chunk);

	chunk_send(fd_coordinador, &status_message, chunk->current_size);

	free(status_message);
	chunk_destroy(chunk);
}
