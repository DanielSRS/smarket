#include "../util/Map/Map.h"

#ifndef PRODUCT_H
#define PRODUCT_H

#define PURCHASE_TABLE_NAME "Purchases"
#define CASHIER_TABLE_NAME "Cashier"
#define ITENS_COMPRA_TABLE_NAME "ItenCompra"
#define PRODUCTS_TABLE_NAME "Produtos"
#define PAYMENT_METHOD_TABLE_NAME "MetodosDePagamento"

typedef Map* Product;
typedef Map* Cashier;
typedef Map* Purchase;
typedef Map* ItenCompra;
typedef Map* PaymentMethod;

Product newProduct(char* ProdutoID, char* NomeProduto, char* Descricao, double Preco, int QuantidadeEmEstoque);
void initProductTable(Map* database);
Product copyProduct(Product toBeCopied);

/** Caixas */

void initCashierTable(Map* database);
Cashier copyCashier(Cashier toBeCopied);
Cashier newCashier(char* CaixaID, char* Nome, char* Descricao);

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

#endif // PRODUCT_H