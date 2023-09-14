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
  Map* Produtos = database->nest(database, "Produtos");
}

//-----

/** Cria novo caixa */
Cashier newCashier(char* CaixaID, char* Nome, char* Descricao) {
  Cashier cashier = newMap();

  cashier
    ->setString(cashier, "CaixaID", CaixaID)
    ->setString(cashier, "Nome", Nome)
    ->setString(cashier, "Descricao", Descricao);

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
  Map* Caixas = database->nest(database, "Cashier");
}

