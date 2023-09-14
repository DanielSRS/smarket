#include "Models.h" // Product, Map
#include "../util/Cstrings/Cstrings.h"

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
