#include "../util/Map/Map.h"

#ifndef PRODUCT_H
#define PRODUCT_H

typedef Map* Product;
typedef Map* Cashier;
typedef Map* Purchase;

Product newProduct(char* ProdutoID, char* NomeProduto, char* Descricao, double Preco, int QuantidadeEmEstoque);
void initProductTable(Map* database);
Product copyProduct(Product toBeCopied);

/** Caixas */

void initCashierTable(Map* database);
Cashier copyCashier(Cashier toBeCopied);
Cashier newCashier(char* CaixaID, char* Nome, char* Descricao);

/** Compras */
Purchase newPurchase(char* CompraID, char* CaixaID, int DataHoraInicio, int DataHoraFim, double TotalCompra);
Purchase copyPurchase(Cashier toBeCopied);
void initPurchasesTable(Map* database);

#endif // PRODUCT_H