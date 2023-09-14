# smart-supermarket

## Tabela de Conteúdo

- [Tabela de Conteúdo](#tabela-de-conte%C3%BAdo)
- [Sobre o Projeto](#sobre-o-projeto)

## Sobre o Projeto

O prjeto consite de um protótipo de caixa de supermercado inteligente usando RFID. O objetivo é melhorar a experiência do cliente, reduzir os tempos de espera e otimizar as operações do supermercado. O sistema deve ser implementado usando uma arquitetura de Nuvem IoT centralizada, contêineres Docker, API REST e Raspberry Pi. Além da interação com clientes através dos caixas, devem estar presentes também funções adiministrativas para gerenciamentos dos caixas, das compras e de estoque.


## Configuração do ambiente de desenvolvimento:

Ubuntu:
``` bash
sudo apt install build-essential gdb cmake
```

Compilando:
``` bash
git clone https://github.com/DanielSRS/smarket
cd smarket
mkdir build
cd build
cmake ..
cmake --build .
```

Executando servidor:
``` bash
cd smarket/build
./src/Apps/Api/Api
```

Executando leitor:
``` bash
cd smarket/build
./src/Apps/SensorManager/SensorManager
```

Executando Testes:
``` bash
cd smarket/build
ctest
```

## Executando sem a biblioteca MercuryApi

Altere o arquivo src/libs/CMakeLists.txt
```diff
- add_subdirectory(MercuryApi)
+
```

Altere o arquivo src/util/ReadTags/CMakeLists.txt
```diff
- target_link_libraries(ReadTags Cstrings Map ReadRFIDTags)
+ target_link_libraries(ReadTags Cstrings Map)
```

Altere o arquivo src/util/ReadTags/ReadTags.c
```diff
Map* readTagsWithArgs(char *tmr, char *antena, Map * result) {
  char *args[4];
  args[0] = "readRFID";
  args[1] = tmr;
  args[2] = "--ant";
  args[3] = antena;

- readRFIDTags(4, args, result);
+ mockedRead(4, args, result);
  return result;
}
```

## API

### Leitor
#### GET: localhost:3497/read
Faz a leitura das tags RFID

(sem corpo na requisição)

formato de resposta:
```JSON
{
	"success": "true",
	"data": {
		"0": "rfidValue0",
		"1": "rfidValue1",
		"2": "rfidValue2",
		"3": "rfidValue3",
		"4": "rfidValue4",
		"length": "5"
	},
	"message": "Tags read with success"
}
```

### Administrador - Caixas
#### GET: localhost:3490/cashiers
Lista todos os caixas

(sem corpo na requisição)

formato de resposta:
```JSON
{
	"sucess": "true",
	"data": {
		"Cashiers": {
			"length": "1",
			"0": {
				"CaixaID": "CAIXA01",
				"Nome": "Caixa 01",
				"Descricao": "Caixa da Maria"
			}
		}
	},
	"message": "Lista de caixas retornado com sucesso"
}
```

#### POST: localhost:3490/cashiers
Cria novo caixa

corpo da requisição:
```json
{
	"CaixaID": "CAIXA01",
	"Nome": "Caixa 01",
	"Descricao": "Caixa da Maria"
}
```

formato de resposta:
```JSON
{
	"sucess": "true",
	"data": {
		"Cashiers": {
			"length": "1",
			"0": {
				"CaixaID": "CAIXA01",
				"Nome": "Caixa 01",
				"Descricao": "Caixa da Maria"
			}
		}
	},
	"message": "Lista de caixas retornado com sucesso"
}
```

#### GET: localhost:3490/cashier/purchases
Lista as compras feitas no caixa

corpo da requisição:
```json
{
	"CaixaID": "CAIXA01"
}
```

formato de resposta:
```JSON
{
	"sucess": "true",
	"data": {
		"Purchases": {
			"0": {
				"CompraID": "0",
				"CaixaID": "CAIXA01",
				"DataHoraInicio": 1694713313.000000,
				"DataHoraFim": 0.000000,
				"TotalCompra": -1.000000
			},
			"length": "1"
		}
	},
	"message": "Lista de todas as compras retornadas com sucesso"
}
```

### Administrador - Compras

#### GET: localhost:3490/purchases
Lista todas as compras

(não tem corpo na requisição)

formato de resposta:
```JSON
{
	"sucess": "true",
	"data": {
		"Purchases": {
			"length": "1",
			"0": {
				"CompraID": "0",
				"CaixaID": "CAIXA01",
				"DataHoraInicio": 1694717366.000000,
				"DataHoraFim": 0.000000,
				"TotalCompra": -1.000000
			}
		}
	},
	"message": "Lista de todas as compras retornadas com sucesso"
}
```

### Administrador - Produtos

#### GET: localhost:3490/products
Lista todos os produtos

(não tem corpo na requisição)

formato de resposta:
```JSON
{
	"sucess": "true",
	"data": {
		"Produtos": {
			"length": "0",
			"0": {
				"ProdutoID": "produto1",
				"NomeProduto": "NomeProduto",
				"Descricao": "Descricao",
				"Preco": 72.790000,
				"QuantidadeEmEstoque": 5.000000
			}
		}
	},
	"message": "Produtos retornadas com sucesso"
}

```


#### POST: localhost:3490/products
Cria novo produto

corpo da requisição
```json
{
	"ProdutoID": "produto1",
	"NomeProduto": "NomeProduto",
	"Descricao": "Descricao",
	"Preco": "72.79",
	"QuantidadeEmEstoque": "5"
}
```

formato de resposta:
```JSON
{
	"sucess": "true",
	"data": {
		"product": {
			"ProdutoID": "produto1",
			"NomeProduto": "NomeProduto",
			"Descricao": "Descricao",
			"Preco": 72.790000,
			"QuantidadeEmEstoque": 5.000000
		}
	},
	"message": "Produto criado com sucesso"
}
```

### Cliente - Caixas

#### POST: localhost:3490/checkout
Inicia uma nova compra

corpo da requisição
```json
{
	"CaixaID": "CAIXA01"
}
```

formato de resposta:
```JSON
{
	"sucess": "true",
	"data": {
		"purchase": {
			"CompraID": "0",
			"CaixaID": "CAIXA01",
			"DataHoraInicio": 1694717366.000000,
			"DataHoraFim": 0.000000,
			"TotalCompra": -1.000000
		}
	},
	"message": "Compra iniciada com sucesso"
}
```

#### PUT: localhost:3490/checkout
Adiciona produtos a uma compra

corpo da requisição
```json
{
	"CompraID": "0",
	"itens": {
		"length": "2",
		"0": "produto1",
		"1": "produto3"
	}
}
```

formato de resposta:
```JSON
{
	"sucess": "true",
	"data": {
		"itens": {
			"0": {
				"ItemCompraID": "0produto1",
				"CompraID": "0",
				"ProdutoID": "produto1",
				"Quantidade": 1.000000,
				"PrecoUnitario": 72.790000,
				"Produto": {
					"ProdutoID": "produto1",
					"NomeProduto": "NomeProduto",
					"Descricao": "Descricao",
					"Preco": 72.790000,
					"QuantidadeEmEstoque": 5.000000
				}
			},
			"1": {
				"ItemCompraID": "0produto3",
				"CompraID": "0",
				"ProdutoID": "produto3",
				"Quantidade": 1.000000,
				"PrecoUnitario": 92.790000,
				"Produto": {
					"ProdutoID": "produto3",
					"NomeProduto": "NomeProduto",
					"Descricao": "Descricao",
					"Preco": 92.790000,
					"QuantidadeEmEstoque": 5.000000
				}
			}
		},
		"valorTotal": 165.580000
	},
	"message": "Compra atualizada com sucesso"
}
```

Conhecimento nescessario:
- C (linguagem de programação)
  - opaque pointers
  - alocação de memória
  - estruturas de dados.
    - Arvore
    - Lista encadeada
    - tabela hash
    - matrizes
- sockets (comunicação em rede)
- Protocolos de rede
  - Protocolo TCP/IP
  - Protocolo http
- Aplicaçãoes muitprocessos
  - comando fork no linux
- make (pra fazer o build do projeto)
- cmake (configuração de bulld do projeto)
