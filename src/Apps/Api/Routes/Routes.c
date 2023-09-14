#include "Routes.h" // Request, Response
#include "../../../util/Map/Map.h" // Map
#include <stdlib.h> // NULL
#include "../../../Models/Models.h" // Product, copyProduct
#include "../../../util/Logger/Logger.h"

/** Retorna informações sobre a API */
void about(Request* req, Response* res, void* context) {
  Map* appState = (Map*) context;
  alocatedCString appVersion = appState->get(appState, "version");

  Map* data = newMap();
  data->setString(data, "version", appVersion ? appVersion : "unknown");

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", data, res)
    ->addStringToJson("message", "Informações sobre a api retornadas com sucesso", res);
}

/** Retorna a lista dos caixas registrados */
void listCashiers(Request* req, Response* res, void* context) {
  /** Global state */
  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* caixas = database->get(database, "cashier");   // tabela de caixas
  int numberOfCashiers = caixas->length;              // quantidade de caixas na tabela
  char** keys = caixas->getKeys(caixas);

  List* listaDeCaixas = newList();
  for (int i = 0; i < numberOfCashiers; i++) {
    listaDeCaixas->pushMap(listaDeCaixas, (Map*) caixas->get(caixas, keys[i]));
  }

  Map* data = newMap();
  data->setList(data, "cashiers", listaDeCaixas);

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", data, res)
    ->addStringToJson("message", "Lista de caixas retornado com sucesso", res);
}

/** Retorna a lista dos caixas registrados */
void registryCashier(Request* req, Response* res, void* context) {
  /** Caixa a ser registrado */
  char* id = (char*) req->data->get(req->data, "id");

  /** Se id for nulo */
  if (id == NULL) {
    Map* data = newMap();
    List* errors = newList();
    errors->pushString(errors, "id é requerido");
    errors->pushString(errors, "id deve ser uma string");
    data->setList(data, "errors", errors);
    res
      ->withStatusCode(400, res)
      ->withStatusMessage("Bad Request", res)
      ->withJSON(res)
      ->addStringToJson("sucess", "false", res)
      ->addObjectToJson("data", data, res)
      ->addStringToJson("message", "Erro ao registrar caixa", res);
    
    return;
  }

  /** Global state */
  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* caixas = database->get(database, "cashier");   // tabela de caixas

  Map* responseData = newMap();
  Map* caixa = caixas->get(caixas, id);

  /** Se caixa ja existe */
  if (caixa != NULL) {
    responseData->setString(responseData, "id", (char*) caixa->get(caixa, "id"));
  } else {
    Map* novoCaixa = newMap();
    novoCaixa->setString(novoCaixa, "id", id);
    caixas->setMap(caixas, id, novoCaixa);
    responseData->setString(responseData, "id", id);
  }

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Caixa registrado com sucesso", res);
}

/** retorna a lista de compras realizadas */
void getAllPurchases(Request* req, Response* res, void* context) {
  /** Global state */
  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* purchases = database->get(database, "purchases");   // tabela de caixas

  Map* responseData = newMap();
  char** keys = purchases->getKeys(purchases);
  int numberOfPurchases = purchases->length;

  Map* responsePurchases = responseData->nest(responseData, "purchases");

  for (int i = 0; i < numberOfPurchases; i++) {
    alocatedCString key = intToCString(i);
    Map* purchase = purchases->get(purchases, keys[i]);
    responsePurchases->setMap(responsePurchases, key, purchase);
    freeAlocatedCString(key);
  }

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Lista de todas as compras retornadas com sucesso", res);
}

/** retorna a lista de compras realizadas */
void getAllProducts(Request* req, Response* res, void* context) {
  /** Global state */
  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* Produtos = database->get(database, "Produtos");   // tabela de caixas
  char** keys = Produtos->getKeys(Produtos);
  int numberOfProdutos = Produtos->length;

  /** vAI PRO CLIENTE */
  Map* responseData = newMap();
  Map* responseProdutos = responseData->nest(responseData, "Produtos");

  for (int i = 0; i < numberOfProdutos; i++) {
    alocatedCString key = intToCString(i);
    Map* product = Produtos->get(Produtos, keys[i]);
    responseProdutos->setMap(responseProdutos, key, copyProduct((Product) product));
    freeAlocatedCString(key);
  }

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Produtos retornadas com sucesso", res);
}

/** retorna a lista de compras realizadas */
void createProduct(Request* req, Response* res, void* context) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Routes[createProduct]");

  /** Caixa a ser registrado */
  char* ProdutoID = (char*) req->data->get(req->data, "ProdutoID");
  char* NomeProduto = (char*) req->data->get(req->data, "NomeProduto");
  char* Descricao = (char*) req->data->get(req->data, "Descricao");
  char* Preco = (char*) req->data->get(req->data, "Preco");
  char* QuantidadeEmEstoque = (char*) req->data->get(req->data, "QuantidadeEmEstoque");

  boolean isInvalid = ProdutoID == NULL
                    || NomeProduto == NULL
                    || Descricao == NULL
                    || Preco == NULL
                    || QuantidadeEmEstoque == NULL;

  /** Se alguem for nulo */
  if (isInvalid) {
    console->warn(console, "Tentando registrar produtos com dados invalidos");
    Map* data = newMap();
    List* errors = newList();

    /** Lista os erros */
    if (ProdutoID == NULL)
      errors->pushString(errors, "ProdutoID é requerido e deve ser uma string");
    if (NomeProduto == NULL)
      errors->pushString(errors, "NomeProduto é requerido e deve ser uma string");
    if (Descricao == NULL)
      errors->pushString(errors, "Descricao é requerido e deve ser uma string");
    if (Preco == NULL)
      errors->pushString(errors, "Preco é requerido e deve ser uma string");
    if (QuantidadeEmEstoque == NULL)
      errors->pushString(errors, "QuantidadeEmEstoque é requerido e deve ser uma string");

    data->setMap(data, "errors", (Map*) errors->_map);
    errors->_map->setString((Map*) errors->_map, "length", intToCString(errors->length(errors)));
    errors->_map = newMap();
    errors->destroy(&errors);
    res
      ->withStatusCode(400, res)
      ->withStatusMessage("Bad Request", res)
      ->withJSON(res)
      ->addStringToJson("sucess", "false", res)
      ->addObjectToJson("data", data, res)
      ->addStringToJson("message", "Erro ao registrar produto. Dados inválidos", res);
    
    console->destroy(&console);
    return;
  }

  /** Global state */
  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* Produtos = database->get(database, "Produtos");   // tabela de Produtos
  int ProdutosCount = Produtos->length;
  Product product = (Product) Produtos->get(Produtos, ProdutoID);

  /** Resposta para o cliente */
  Map* responseData = newMap();

  /** Se produto ja existe */
  if (product != NULL) {
    console->warn(console, "Tentando registrar um produto que já existe");
    /** Envia de volta */
    responseData->setMap(responseData, "product", (Map*) copyProduct(product));
    res
    ->withStatusCode(409, res)
    ->withStatusMessage("Confict", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "false", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Erro! Produto já existe", res);

    console->destroy(&console);
    return;
  }
  
  Product novoProduto = newProduct(ProdutoID, NomeProduto, Descricao, atof(Preco), atoi(QuantidadeEmEstoque));
  // alocatedCString key = intToCString(ProdutosCount);
  Produtos->setMap(Produtos, ProdutoID, novoProduto);

  responseData->setMap(responseData, "product", (Map*) copyProduct(novoProduto));
  // freeAlocatedCString(key);

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Produto criado com sucesso", res);
  
  console->info(console, "Novo produto registrado");
  console->destroy(&console);

  return;
}
