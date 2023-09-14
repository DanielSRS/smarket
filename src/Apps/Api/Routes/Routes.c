#include "Routes.h" // Request, Response
#include "../../../util/Map/Map.h" // Map
#include <stdlib.h> // NULL
#include "../../../Models/Models.h" // Product, copyProduct
#include "../../../util/Logger/Logger.h"
#include <time.h> // time_t, time

double currentTime();

/** Api */
void about(Request* req, Response* res, void* context);

/** Compras */
void getAllPurchases(Request* req, Response* res, void* context);

/** Caixas */
void listCashiers(Request* req, Response* res, void* context);
void createCashier(Request* req, Response* res, void* context);

/** Produtos */
void getAllProducts(Request* req, Response* res, void* context);
void createProduct(Request* req, Response* res, void* context);

/** Compras */
void createPurchase(Request* req, Response* res, void* context);



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
  Map* caixas = database->get(database, "Cashier");   // tabela de caixas
  int numberOfCashiers = caixas->length;              // quantidade de caixas na tabela
  char** keys = caixas->getKeys(caixas);

  /** Resposta para o cliente */
  Map* responseData = newMap();
  Map* responseCashiers = responseData->nest(responseData, "Cashiers");
  responseCashiers->setString(responseCashiers, "length", intToCString(caixas->length));

  for (int i = 0; i < numberOfCashiers; i++) {
    alocatedCString key = intToCString(i);
    Cashier caixa = (Cashier) caixas->get(caixas, keys[i]);
    responseCashiers->setMap(responseCashiers, key, copyCashier(caixa));
    freeAlocatedCString(key);
  }

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Lista de caixas retornado com sucesso", res);
}

/** Retorna a lista dos caixas registrados */
void createCashier(Request* req, Response* res, void* context) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Routes[createCashier]");

  console->debug(console, "Criando novo caixa...");

  /** Caixa a ser registrado */
  char* CaixaID = (char*) req->data->get(req->data, "CaixaID");
  char* Nome = (char*) req->data->get(req->data, "Nome");
  char* Descricao = (char*) req->data->get(req->data, "Descricao");

  boolean isInvalid = CaixaID == NULL
                    || Nome == NULL
                    || Descricao == NULL;

  /** Se alguem for nulo */
  if (isInvalid) {
    console->warn(console, "Tentando registrar caixa com dados invalidos");
    Map* data = newMap();
    List* errors = newList();

    /** Lista os erros */
    if (CaixaID == NULL)
      errors->pushString(errors, "CaixaID é requerido e deve ser uma string");
    if (Nome == NULL)
      errors->pushString(errors, "Nome é requerido e deve ser uma string");
    if (Descricao == NULL)
      errors->pushString(errors, "Descricao é requerido e deve ser uma string");

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
      ->addStringToJson("message", "Erro ao registrar caixa. Dados inválidos", res);
    
    console->destroy(&console);
    return;
  }

  console->debug(console, "Dados validados");

  /** Global state */
  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* Caixas = database->get(database, "Cashier");   // tabela de caixas

  if (Caixas == NULL)
    console->error(console, "Tabela de caixas não existe");

  Cashier caixa = (Cashier) Caixas->get(Caixas, CaixaID);

  /** Resposta para o cliente */
  Map* responseData = newMap();

  /** Se caixa ja existe */
  if (caixa != NULL) {
    console->warn(console, "Tentando registrar um caixa que já existe");
    /** Envia de volta */
    responseData->setMap(responseData, "cashier", (Map*) copyCashier(caixa));
    res
    ->withStatusCode(409, res)
    ->withStatusMessage("Confict", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "false", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Erro! Caixa já existe", res);

    console->destroy(&console);
    return;
  }
  
  Map* novoCaixa = newCashier(CaixaID, Nome, Descricao);
  Caixas->setMap(Caixas, CaixaID, novoCaixa);

  responseData->setMap(responseData, "cashier", copyCashier(novoCaixa));

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Caixa criado com sucesso", res);
}

/** retorna a lista de compras realizadas */
void getAllPurchases(Request* req, Response* res, void* context) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Routes[getAllPurchases]");

  /** Global state */
  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* Purchases = database->get(database, PURCHASE_TABLE_NAME);   // tabela de caixas
  
  if (Purchases == NULL)
    console->error(console, "Purchases table do not exist!!");
  
  char** keys = Purchases->getKeys(Purchases);
  int numberOfPurchases = Purchases->length;

  /** Para enviar ao cliente */
  Map* responseData = newMap();
  Map* responsePurchases = responseData->nest(responseData, "Purchases");
  responsePurchases->setString(responsePurchases, "length", intToCString(numberOfPurchases));

  for (int i = 0; i < numberOfPurchases; i++) {
    alocatedCString key = intToCString(i);
    Map* purchase = Purchases->get(Purchases, keys[i]);
    responsePurchases->setMap(responsePurchases, key, copyPurchase(purchase));
    freeAlocatedCString(key);
  }

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Lista de todas as compras retornadas com sucesso", res);

  console->info(console, "Listado todas as compras");
  console->destroy(&console);
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
  responseProdutos->setString(responseProdutos, "length", intToCString(responseProdutos->length));

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

  /** Produto a ser registrado */
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

/** Inicia uma nova compra */
void createPurchase(Request* req, Response* res, void* context) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Routes[createPurchase]");

  console->debug(console, "Iniciando nova compra...");

  /** Caixa onde a compra esta sendo feita */
  char* CaixaID = (char*) req->data->get(req->data, "CaixaID");

  boolean isInvalid = CaixaID == NULL;

  /** Se alguem for nulo */
  if (isInvalid) {
    console->warn(console, "Tentando registrar compra com dados invalidos");
    Map* data = newMap();
    List* errors = newList();

    /** Lista os erros */
    if (CaixaID == NULL)
      errors->pushString(errors, "CaixaID é requerido e deve ser uma string");

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
      ->addStringToJson("message", "Erro ao registrar nova compra. Dados inválidos", res);
    
    console->destroy(&console);
    return;
  }

  console->debug(console, "Dados validados");

  /** Global state */
  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* Compras = database->get(database, PURCHASE_TABLE_NAME);   // tabela de caixas
  Map* Caixas = database->get(database, "Cashier");   // tabela de caixas

  if (Compras == NULL)
    console->error(console, "Tabela de Compras não existe");
  if (Caixas == NULL)
    console->error(console, "Tabela de Caixas não existe");

  int ComprasCount = Compras->length;
  /** Busca o caixa onde esta sendo feita a compra */
  Cashier caixa = (Cashier) Caixas->get(Caixas, CaixaID);

  /** Se o caixa não existe */
  if (caixa == NULL) {
    console->error(console, "Caixa não existe!");
    Map* data = newMap();
    List* errors = newList();

    errors->pushString(errors, "CaixaID não pertence a nenhum caixa cadastrado!");

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
      ->addStringToJson("message", "Erro ao registrar nova compra. Dados inválidos", res);
    
    console->destroy(&console);
    return;
  }

  /** Resposta para o cliente */
  Map* responseData = newMap();

  /** Se caixa em compra/se caixa já logado? */

  alocatedCString CompraID = intToCString(ComprasCount);
  
  Purchase novaCompra = newPurchase(CompraID, CaixaID, currentTime(), 0, -1);
  Compras->setMap(Compras, CompraID, novaCompra);

  responseData->setMap(responseData, "purchase", copyPurchase(novaCompra));

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Compra iniciada com sucesso", res);
  
  alocatedCString logMessage = formatedCString("Compra iniciada no caixa: %s", CaixaID);
  console->info(console, logMessage);
  console->destroy(&console);
  freeAlocatedCString(logMessage);
}

/**
 * Obtem a hora atual
*/
double currentTime(){
  time_t rawtime;
  
  time(&rawtime);
  
  return (double) rawtime;
}
