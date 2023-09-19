#include "Seed.h"
#include "../../../../util/Logger/Logger.h" // Logger
#include "../../../../Models/Models.h" // PURCHASE_TABLE_NAME PRODUCTS_TABLE_NAME ITENS_COMPRA_TABLE_NAME CASHIER_TABLE_NAME

/**
 * Popula o banco de dados com valores iniciais
*/
void seed(Request* req, Response* res, void* context) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Seed");

  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* Compras = database->get(database, PURCHASE_TABLE_NAME);
  Map* Produtos = database->get(database, PRODUCTS_TABLE_NAME);
  Map* ItensCompra = database->get(database, ITENS_COMPRA_TABLE_NAME);
  Map* Caixas = database->get(database, CASHIER_TABLE_NAME);

  /** Adicionando produtos */
  Product p1 = newProduct("E20000172211010118905454", "Vêrrato", "O melhor veneno de rato! É tiro e queda", 5.25, 500);
  Produtos->setMap(Produtos, "E20000172211010118905454", p1);
  Product p2 = newProduct("E2000017221100961890544A", "Caixa de bombom", "Tem gosto de gordura com açucar", 72.47, 304);
  Produtos->setMap(Produtos, "E2000017221100961890544A", p2);
  Product p3 = newProduct("E20000172211009418905449", "Pneu careca", "Derrapa que é uma beleza!", 22.90, 24);
  Produtos->setMap(Produtos, "E20000172211009418905449", p3);
  Product p4 = newProduct("E20000172211011718905474", "IPhone 15", "100\% original. Confia", 100, 10);
  Produtos->setMap(Produtos, "E20000172211011718905474", p4);
  Product p5 = newProduct("E20000172211011118905471", "Filme da barbie pirateado", "Qualidade 10/10", 2.20, 45);
  Produtos->setMap(Produtos, "E20000172211011118905471", p5);
  Product p6 = newProduct("E2000017221101321890548C", "CD as melhores do Pablo 2008", "As melhores do arrocha", 10.15, 5);
  Produtos->setMap(Produtos, "E2000017221101321890548C", p6);
  Product p7 = newProduct("E20000172211010218905459", "Cream soda", "I need all ya cream soda", 7.90, 9);
  Produtos->setMap(Produtos, "E20000172211010218905459", p7);
  Product p8 = newProduct("E20000172211012518905484", "Fone de ouvido manasses", "Dura pra sempre. Confia", 3, 20);
  Produtos->setMap(Produtos, "E20000172211012518905484", p8);
  Product p9 = newProduct("E2000017221101241890547C", "The bat", "Taco de baseball", 43, 20);
  Produtos->setMap(Produtos, "E2000017221101241890547C", p9);

  /** Adicionando caixas */
  Cashier c1 = newCashier("CAIXA01", "01", "Caixa preferêncial");
  Caixas->setMap(Caixas, "CAIXA01", c1);
  Cashier c2 = newCashier("CAIXA02", "02", "Caixa eletrodomesticos");
  Caixas->setMap(Caixas, "CAIXA02", c2);
  Cashier c3 = newCashier("CAIXA03", "03", "Caixa garal");
  Caixas->setMap(Caixas, "CAIXA03", c3);
  Cashier c4 = newCashier("CAIXA04", "04", "Caixa garal");
  Caixas->setMap(Caixas, "CAIXA04", c4);
  Cashier c5 = newCashier("CAIXA05", "05", "Caixa papelaria");
  Caixas->setMap(Caixas, "CAIXA05", c5);

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    // ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Banco populado com sucesso!", res);

  console->destroy(&console);
}