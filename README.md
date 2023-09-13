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

Compilando servidor:
``` bash
make server
```

Executando servidor:
``` bash
make serve
```

Conhecimento nescessario:
- C (linguagem de programação)
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
