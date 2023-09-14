#include "../util/Map/Map.h"

#ifndef PRODUCT_H
#define PRODUCT_H

typedef Map* Product;

Product newProduct(char* ProdutoID, char* NomeProduto, char* Descricao, double Preco, int QuantidadeEmEstoque);
void initProductTable(Map* database);
Product copyProduct(Product toBeCopied);

#endif // PRODUCT_H