serve: server
	@echo "\n🏭 Executando servidor"
	./build/server.executable
	@echo "\n\n"

server: server.o socket.o childProcess.o
	@echo "\n🏗️ Construindo servidor"
	gcc -o build/server.executable build/server.o build/socket.o build/childProcess.o

server.o: server.c | buildFolder
	@echo "\n🚧 Resolvendo dependencias do servidor"
	gcc -c server.c -o build/server.o -Wall

socket.o: socket.c | buildFolder
	gcc -c socket.c -o build/socket.o -Wall

childProcess.o: childProcess.c | buildFolder
	gcc -c childProcess.c -o build/childProcess.o -Wall

buildFolder:
	@echo "📂 Criando diretório build"
	mkdir -p build

# Exclui todos os arquivos que foram gerados automaticamente durante
# o processo de build ou execução
clean:
	@echo "🧹 Limpando arquivos gerados automaticamente"
	rm -rf ./build
	@echo "\n"