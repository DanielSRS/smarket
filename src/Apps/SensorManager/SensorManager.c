#include "../../util/Socket/socket.h" // connectTo, sendRequest
#include "../../util/ReadTags/ReadTags.h" //readtags
#include <unistd.h> // close
#include <stdio.h> // printf
#include <string.h> // strlen

void readerHandler(HTTPConnection *newConnection, void *context);


int main() {

    HTTPServer *server = createHTTPServer();

    server
        ->setNewConnectionHanddler(server, readerHandler)
        ->setPort(server, 3497)
        ->serve(server);

    server->destroy(&server);
}


void readerHandler(HTTPConnection *newConnection, void *context) {
    /** Tags lidas */
    Map *tagsList = readTags();
    alocatedCString tags = tagsList->toJsonString(tagsList);

    Map* data = newMap();
    data->setMap(data, "itens", tagsList);
    int numberOfItens = tagsList->length;
    alocatedCString length = intToCString(numberOfItens);
    char** keys = tagsList->getKeys(tagsList);

    Map* responseData = newMap();
    for (int i = 0; i < numberOfItens; i++) {
        alocatedCString key = intToCString(i);
        responseData->setString(responseData, key, (char*) tagsList->get(tagsList, keys[i]));
        freeAlocatedCString(key);
    }

    responseData->setString(responseData, "length", length);
    freeAlocatedCString(length);


    newConnection->response
        ->withJSON(newConnection->response)
        ->withStatusCode(200, newConnection->response)
        ->addStringToJson("success", "true", newConnection->response)
        ->addObjectToJson("data", responseData, newConnection->response)
        ->addStringToJson("message", "Tags read with success", newConnection->response);

    newConnection->sendResponse(newConnection);
    newConnection->close(newConnection);
    newConnection->destroy(&newConnection);
}
