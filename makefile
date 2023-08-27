serve: server
	@echo "\n🏭 Executando servidor"
	@echo "\ec"
	./build/server.executable
	@echo "\n\n"

all: server map.o
	@echo "\nBuilding all"

server: server.o socket.o childProcess.o
	@echo "\n🏗️ Construindo servidor"
	gcc -o build/server.executable build/server.o build/socket.o build/childProcess.o

server.o: src/server.c | buildFolder
	@echo "\n🚧 Resolvendo dependencias do servidor"
	gcc -c src/server.c -o build/server.o -Wall

socket.o: src/socket.c | buildFolder
	gcc -c src/socket.c -o build/socket.o -Wall

childProcess.o: src/childProcess.c | buildFolder
	gcc -c src/childProcess.c -o build/childProcess.o -Wall

map.o: src/util/Map/Map.c | buildFolder
	gcc -c src/util/Map/Map.c -o build/map.o -Wall

buildFolder:
	@echo "📂 Criando diretório build"
	mkdir -p build

# Exclui todos os arquivos que foram gerados automaticamente durante
# o processo de build ou execução
clean:
	@echo "🧹 Limpando arquivos gerados automaticamente"
	rm -rf ./build
	@echo "\n"