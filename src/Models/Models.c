#include "Models.h" // Product, Map
#include "../util/Cstrings/Cstrings.h"

/** Produtos */
Product newProduct(char* ProdutoID, char* NomeProduto, char* Descricao, double Preco, int QuantidadeEmEstoque);
Product copyProduct(Product toBeCopied);
void initProductTable(Map* database);

/** Caixas */
Cashier newCashier(char* CaixaID, char* Nome, char* Descricao);
Cashier copyCashier(Cashier toBeCopied);
void initCashierTable(Map* database);

/** Compras */
Purchase newPurchase(char* CompraID, char* CaixaID, int DataHoraInicio, int DataHoraFim, double TotalCompra);
Purchase copyPurchase(Purchase toBeCopied);
void initPurchasesTable(Map* database);

/** ItenCompra */
ItenCompra newItensCompra(char* ItemCompraID, char* CompraID, char* ProdutoID, int Quantidade, double PrecoUnitario);
ItenCompra copyItensCompra(ItenCompra toBeCopied);
void initItensCompraTable(Map* database);

/** PeymentMethod */
PaymentMethod newPeymentMethod(char* metodoDePagementoID, char* nome, char* descricao);
PaymentMethod copyPeymentMethod(PaymentMethod toBeCopied);
void initPeymentMethodTable(Map* database);


Product newProduct(char* ProdutoID, char* NomeProduto, char* Descricao, double Preco, int QuantidadeEmEstoque) {
  Product product = newMap();

  product
    ->setString(product, "ProdutoID", ProdutoID)
    ->setString(product, "NomeProduto", NomeProduto)
    ->setString(product, "Descricao", Descricao)
    ->setNumber(product, "Preco", Preco)
    ->setNumber(product, "QuantidadeEmEstoque", (double) QuantidadeEmEstoque);

  return product;
}

Product copyProduct(Product toBeCopied) {
  alocatedCString ProdutoID = (alocatedCString) toBeCopied->get(toBeCopied, "ProdutoID");
  alocatedCString NomeProduto = (alocatedCString) toBeCopied->get(toBeCopied, "NomeProduto");
  alocatedCString Descricao = (alocatedCString) toBeCopied->get(toBeCopied, "Descricao");
  double Preco = *((double*) toBeCopied->get(toBeCopied, "Preco"));
  double QuantidadeEmEstoque = *((double*) toBeCopied->get(toBeCopied, "QuantidadeEmEstoque"));

  Product product = newMap();

  product
    ->setString(product, "ProdutoID", ProdutoID)
    ->setString(product, "NomeProduto", NomeProduto)
    ->setString(product, "Descricao", Descricao)
    ->setNumber(product, "Preco", Preco)
    ->setNumber(product, "QuantidadeEmEstoque", (double) QuantidadeEmEstoque);

  return product;
}

void initProductTable(Map* database) {
  /** Produtos */
  Map* Produtos = database->nest(database, PRODUCTS_TABLE_NAME);
}

//-----

/** Cria nova compra */
Purchase newPurchase(char* CompraID, char* CaixaID, int DataHoraInicio, int DataHoraFim, double TotalCompra) {
  Purchase purchase = newMap();

  purchase
    ->setString(purchase, "CompraID", CompraID)
    ->setString(purchase, "CaixaID", CaixaID)
    ->setNumber(purchase, "DataHoraInicio", DataHoraInicio)
    ->setNumber(purchase, "DataHoraFim", DataHoraFim)
    ->setNumber(purchase, "TotalCompra", TotalCompra);

  return purchase;
}

/** Copia os dados de um caixa */
Purchase copyPurchase(Purchase toBeCopied) {
  alocatedCString CompraID = (alocatedCString) toBeCopied->get(toBeCopied, "CompraID");
  alocatedCString CaixaID = (alocatedCString) toBeCopied->get(toBeCopied, "CaixaID");
  double DataHoraInicio = *((double*) toBeCopied->get(toBeCopied, "DataHoraInicio"));
  double DataHoraFim = *((double*) toBeCopied->get(toBeCopied, "DataHoraFim"));
  double TotalCompra = *((double*) toBeCopied->get(toBeCopied, "TotalCompra"));

  return newPurchase(
    CompraID,
    CaixaID,
    DataHoraInicio,
    DataHoraFim,
    TotalCompra
  );
}

/** Inicia a tabela de caixa */
void initPurchasesTable(Map* database) {
  /** Produtos */
  Map* Caixas = database->nest(database, PURCHASE_TABLE_NAME);
}

//-----

/** Cria novo caixa */
Cashier newCashier(char* CaixaID, char* Nome, char* Descricao) {
  Cashier cashier = newMap();

  cashier
    ->setString(cashier, "CaixaID", CaixaID)
    ->setString(cashier, "Nome", Nome)
    ->setString(cashier, "Descricao", Descricao)
    ->setString(cashier, "CompraAtual", "undefined");

  return cashier;
}

/** Copia os dados de um caixa */
Cashier copyCashier(Cashier toBeCopied) {
  alocatedCString CaixaID = (alocatedCString) toBeCopied->get(toBeCopied, "CaixaID");
  alocatedCString Nome = (alocatedCString) toBeCopied->get(toBeCopied, "Nome");
  alocatedCString Descricao = (alocatedCString) toBeCopied->get(toBeCopied, "Descricao");

  Cashier cashier = newMap();

  cashier
    ->setString(cashier, "CaixaID", CaixaID)
    ->setString(cashier, "Nome", Nome)
    ->setString(cashier, "Descricao", Descricao);

  return cashier;
}

/** Inicia a tabela de caixa */
void initCashierTable(Map* database) {
  /** Produtos */
  Map* Caixas = database->nest(database, CASHIER_TABLE_NAME);
}

//-----

/** Cria novo ItenCompra */
ItenCompra newItensCompra(char* ItemCompraID, char* CompraID, char* ProdutoID, int Quantidade, double PrecoUnitario) {
  ItenCompra itensCompra = newMap();

  itensCompra
    ->setString(itensCompra, "ItemCompraID", ItemCompraID)
    ->setString(itensCompra, "CompraID", CompraID)
    ->setString(itensCompra, "ProdutoID", ProdutoID)
    ->setNumber(itensCompra, "Quantidade", (double) Quantidade)
    ->setNumber(itensCompra, "PrecoUnitario", PrecoUnitario);

  return itensCompra;
}

/** Copia os dados de um ItenCompra */
ItenCompra copyItensCompra(ItenCompra toBeCopied) {
  alocatedCString ItemCompraID = (alocatedCString) toBeCopied->get(toBeCopied, "ItemCompraID");
  alocatedCString CompraID = (alocatedCString) toBeCopied->get(toBeCopied, "CompraID");
  alocatedCString ProdutoID = (alocatedCString) toBeCopied->get(toBeCopied, "ProdutoID");
  double Quantidade = *((double*) toBeCopied->get(toBeCopied, "Quantidade"));
  double PrecoUnitario = *((double*) toBeCopied->get(toBeCopied, "PrecoUnitario"));

  ItenCompra itensCompra = newItensCompra(
    ItemCompraID,
    CompraID,
    ProdutoID,
    Quantidade,
    PrecoUnitario
  );

  return itensCompra;
}

/** Inicia a tabela de ItenCompra */
void initItensCompraTable(Map* database) {
  /** ItenCompra */
  Map* itensCompra = database->nest(database, ITENS_COMPRA_TABLE_NAME);
}

//-----

/** Cria novo Metodo de pagametno */
PaymentMethod newPeymentMethod(char* metodoDePagementoID, char* nome, char* descricao) {
  PaymentMethod metodoDePagemento = newMap();

  metodoDePagemento
    ->setString(metodoDePagemento, "metodoDePagementoID", metodoDePagementoID)
    ->setString(metodoDePagemento, "nome", nome)
    ->setString(metodoDePagemento, "descricao", descricao);

  return metodoDePagemento;
}

/** Copia os dados de um PaymentMethod */
PaymentMethod copyPeymentMethod(PaymentMethod toBeCopied) {
  alocatedCString metodoDePagementoID = (alocatedCString) toBeCopied->get(toBeCopied, "metodoDePagementoID");
  alocatedCString nome = (alocatedCString) toBeCopied->get(toBeCopied, "nome");
  alocatedCString descricao = (alocatedCString) toBeCopied->get(toBeCopied, "descricao");

  PaymentMethod metodoDePagemento = newPeymentMethod(
    metodoDePagementoID,
    nome,
    descricao
  );

  return metodoDePagemento;
}

/** Inicia a tabela de PaymentMethod */
void initPeymentMethodTable(Map* database) {
  /** PeymentMethod */
  Map* metodosDePagamento = database->nest(database, PAYMENT_METHOD_TABLE_NAME);
}

