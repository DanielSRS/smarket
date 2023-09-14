#include "../util/Map/Map.h"

#ifndef PRODUCT_H
#define PRODUCT_H

typedef Map* Product;
typedef Map* Cashier;

Product newProduct(char* ProdutoID, char* NomeProduto, char* Descricao, double Preco, int QuantidadeEmEstoque);
void initProductTable(Map* database);
Product copyProduct(Product toBeCopied);

/** Caixas */

void initCashierTable(Map* database);
Cashier copyCashier(Cashier toBeCopied);
Cashier newCashier(char* CaixaID, char* Nome, char* Descricao);

#endif // PRODUCT_H