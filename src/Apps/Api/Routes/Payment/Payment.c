#include "Payment.h"
#include "../../../../util/Logger/Logger.h" // Logger
#include "../../../../Models/Models.h" // PURCHASE_TABLE_NAME PRODUCTS_TABLE_NAME ITENS_COMPRA_TABLE_NAME CASHIER_TABLE_NAME

void getPaymentMethods(Request* req, Response* res, void* context) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Seed");

  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* MetodosDePagamento = database->get(database, PAYMENT_METHOD_TABLE_NAME);
  // Map* Produtos = database->get(database, PRODUCTS_TABLE_NAME);
  // Map* ItensCompra = database->get(database, ITENS_COMPRA_TABLE_NAME);
  // Map* Caixas = database->get(database, CASHIER_TABLE_NAME);

  char** keys = MetodosDePagamento->getKeys(MetodosDePagamento);
  int numberOfMetodosDePagamento = MetodosDePagamento->length;
  alocatedCString length = intToCString(numberOfMetodosDePagamento);

  Map* responseData = newMap();
  Map* responseMethods = responseData->nest(responseData, "metodos");
  responseMethods->setString(responseMethods, "length", length);
  freeAlocatedCString(length);

  for (int i = 0; i < numberOfMetodosDePagamento; i++) {
    alocatedCString key = intToCString(i);
    Map* method = MetodosDePagamento->get(MetodosDePagamento, keys[i]);
    responseMethods->setMap(responseMethods, key, copyPeymentMethod((PaymentMethod) method));
    freeAlocatedCString(key);
  }

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Metodos de pagamento retornadas com sucesso", res);

  console->destroy(&console);
}