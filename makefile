serve: server
	@echo "\n🏭 Executando servidor"
	./build/server.executable
	@echo "\n\n"

server: server.o
	@echo "\n🏗️ Construindo servidor"
	gcc -o build/server.executable build/server.o

server.o: server.c | buildFolder
	@echo "\n🚧 Resolvendo dependencias do servidor"
	gcc -c server.c -o build/server.o -Wall

buildFolder:
	@echo "📂 Criando diretório build"
	mkdir -p build

# Exclui todos os arquivos que foram gerados automaticamente durante
# o processo de build ou execução
clean:
	@echo "🧹 Limpando arquivos gerados automaticamente"
	rm -rf ./build
	@echo "\n"