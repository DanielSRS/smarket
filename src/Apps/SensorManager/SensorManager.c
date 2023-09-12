#include "../../util/Socket/socket.h" // connectTo, sendRequest
#include "../../util/ReadTags/ReadTags.h" //readtags
#include <unistd.h> // close
#include <stdio.h> // printf
#include <string.h> // strlen

void readerHandler(HTTPConnection *newConnection, void *context);

// #define ANSI_COLOR_RED     "\x1b[31m"
// #define ANSI_COLOR_GREEN   "\x1b[32m"
// #define ANSI_COLOR_YELLOW  "\x1b[33m"
// #define ANSI_COLOR_BLUE    "\x1b[34m"
// #define ANSI_COLOR_MAGENTA "\x1b[35m"
// #define ANSI_COLOR_CYAN    "\x1b[36m"
// #define ANSI_COLOR_RESET   "\x1b[0m"

int main() {

    // printf(ANSI_COLOR_RED     "This text is RED!"     ANSI_COLOR_RESET "\n");
    // printf(ANSI_COLOR_GREEN   "This text is GREEN!"   ANSI_COLOR_RESET "\n");
    // printf(ANSI_COLOR_YELLOW  "This text is YELLOW!"  ANSI_COLOR_RESET "\n");
    // printf(ANSI_COLOR_BLUE    "This text is BLUE!"    ANSI_COLOR_RESET "\n");
    // printf(ANSI_COLOR_MAGENTA "This text is MAGENTA!" ANSI_COLOR_RESET "\n");
    // printf(ANSI_COLOR_CYAN    "This text is CYAN!"    ANSI_COLOR_RESET "\n");

    HTTPServer *server = createHTTPServer();

    server
        ->setNewConnectionHanddler(server, readerHandler)
        ->setPort(server, 3497)
        ->serve(server);

    server->destroy(&server);
}


void readerHandler(HTTPConnection *newConnection, void *context) {
    Map *tagsList = readTags();
    alocatedCString tags = tagsList->toJsonString(tagsList);

    Map* data = newMap();
    data->setMap(data, "tags", tagsList);

    newConnection->response
        ->withJSON(newConnection->response)
        ->withStatusCode(200, newConnection->response)
        ->addStringToJson("success", "true", newConnection->response)
        ->addObjectToJson("data", data, newConnection->response)
        ->addStringToJson("message", "Tags read with success", newConnection->response);

    newConnection->sendResponse(newConnection);
    newConnection->close(newConnection);
    newConnection->destroy(&newConnection);
}
