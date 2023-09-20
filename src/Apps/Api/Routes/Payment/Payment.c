#include "Payment.h"
#include "../../../../util/Logger/Logger.h" // Logger
#include "../../../../Models/Models.h" // PURCHASE_TABLE_NAME PRODUCTS_TABLE_NAME ITENS_COMPRA_TABLE_NAME CASHIER_TABLE_NAME
#include <stdlib.h> // NULL

void getPaymentMethods(Request* req, Response* res, void* context) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Payment[getPaymentMethods]");

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

void setPaymentMethods(Request* req, Response* res, void* context) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Payment[setPaymentMethods]");

  Map* appState = (Map*) context;

  char* CompraID = (char*) req->data->get(req->data, "CompraID");
  char* metodoDePagementoID = (char*) req->data->get(req->data, "metodoDePagementoID");

  boolean isInvalid = CompraID == NULL || metodoDePagementoID == NULL;

  /** Se alguem for nulo */
  if (isInvalid) {
    console->warn(console, "Tentando atualizar uma compra com dados invalidos");
    Map* data = newMap();
    Map* errors = newMap();

    /** Lista os erros */
    if (CompraID == NULL)
      errors->setString(errors, "0", "CompraID é requerido e deve ser uma string");
    if (metodoDePagementoID == NULL)
      errors->setString(errors, "1", "metodoDePagementoID é requerido e deve ser uma string");

    data->setMap(data, "errors", errors);
    alocatedCString length = intToCString(errors->length);
    errors->setString(errors, "length", length);
    freeAlocatedCString(length);

    res
      ->withStatusCode(400, res)
      ->withStatusMessage("Bad Request", res)
      ->withJSON(res)
      ->addStringToJson("sucess", "false", res)
      ->addObjectToJson("data", data, res)
      ->addStringToJson("message", "Erro ao atualizar compra. Dados inválidos", res);
    
    console->destroy(&console);
    return;
  }

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* MetodosDePagamento = database->get(database, PAYMENT_METHOD_TABLE_NAME);
  Map* Compras = database->get(database, PURCHASE_TABLE_NAME);
  // Map* Produtos = database->get(database, PRODUCTS_TABLE_NAME);
  // Map* ItensCompra = database->get(database, ITENS_COMPRA_TABLE_NAME);
  // Map* Caixas = database->get(database, CASHIER_TABLE_NAME);

  PaymentMethod method = MetodosDePagamento->get(MetodosDePagamento, metodoDePagementoID);
  Purchase compra = Compras->get(Compras, CompraID);

  /** Se alguem for nulo */
  if (method == NULL || compra  == NULL) {
    console->warn(console, "Tentando atualizar uma compra com dados invalidos");
    Map* data = newMap();
    Map* errors = newMap();

    /** Lista os erros */
    if (compra == NULL)
      errors->setString(errors, "0", "Não existe compra com esse id");
    if (method == NULL)
      errors->setString(errors, "1", "Não existe metodo de pagamento com esse id");

    data->setMap(data, "errors", errors);
    alocatedCString length = intToCString(errors->length);
    errors->setString(errors, "length", length);
    freeAlocatedCString(length);

    res
      ->withStatusCode(400, res)
      ->withStatusMessage("Bad Request", res)
      ->withJSON(res)
      ->addStringToJson("sucess", "false", res)
      ->addObjectToJson("data", data, res)
      ->addStringToJson("message", "Erro ao atualizar compra. Dados inválidos", res);
    
    console->destroy(&console);
    return;
  }

  Map* responseData = newMap();

  compra->setString(compra, "MTPagamentoID", method->get(method, "metodoDePagementoID"));
  double endTime = currentTime(); // DataHoraFim
  double* DataHoraFim = (double*) compra->get(compra, "DataHoraFim");
  *DataHoraFim = endTime;

  Purchase resCompra = copyPurchase(compra);
  resCompra->setMap(resCompra, "MTPagamento", copyPeymentMethod(method));
  Map* responsePurchase = responseData->setMap(responseData, "purchase", resCompra);

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Metodos de pagamento retornadas com sucesso", res);

  console->destroy(&console);
}
