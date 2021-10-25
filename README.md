# O que é?
	Arq é uma biblioteca com funções de alto nivel para operações em arquivos,
	na versão atual, as funções que se destacam são a de envio e download de 
	arquivos a partir de um socket.

# Documentação
	Breve explicação de como deve funcionar cada função

	# sendfile_arq(int sock, char *filename, int rbytes)

		Envia um arquivo para o socket passado no primeiro parâmetro.

		# Parâmetros:
			int sock: Deve ser o descritor de arquivo (socket, nesse caso)
			para onde será enviado o arquivo passado em char *filename.

			char *filename: Caminho até o arquivo que será enviado.

			int rbytes: Quantidade de bytes que deve ser lido por vez no arquivo.

		# Valor de retorno:
			Retorna -1/INVALID_SOCKET em caso do socket ser um descritor inválido, retorna o valor
			do ultimo erro em errno/GetLastError (Windows) que ocorreu dentro da função
			ou retorna 0 se o envio foi um sucesso.

		# OBS:
			É possível obter uma mensagem de erro com o erro retornado pela função,
			diretamente do sistema, usando as seguintes funções:
			
			strerror() - Linux
			FormatMessage() - Windows

	# downloadfile_arq(int sock, char *output, int rbytes)

		Faz o download de um arquivo enviado pelo socket no primeiro parâmetro.

		# Parâmetros:
			int sock: Deve ser o descritor de arquivo (socket, nesse caso) de onde
			deve ser feito o donwload do arquivo.

			char *output: Novo caminho para o arquivo que será feito o download,
			caso seja NULL, a função deve criar um arquivo com o nome original
			que foi enviado pelo socket.

			int rbytes: Quantidade de bytes que deve ser lido por vez.

		# Valor de retorno:
			Retorna -1/INVALID_SOCKET em caso do socket ser um descritor inválido, retorna o valor
			do ultimo erro em errno/GetLastError (Windows) que ocorreu dentro da função
			ou retorna 0 se o download foi um sucesso.

		# OBS:
			É possível obter uma mensagem de erro com o erro retornado pela função,
			diretamente do sistema, usando as seguintes funções:
			
			strerror() - Linux
			FormatMessage() - Windows

	# copyfile_arq(char *src , char *dest, int rbytes)

		Copia um arquivo de src para dest;

		# Parâmetros:
			char *src: Deve ser o caminho até o arquivo que será copiado.

			char *dest: Novo caminho para o arquivo copiado.

			int rbytes: Quantidade de bytes que deve ser lido/escrito por vez.

		# Valor de retorno:
			Retorna o valor do ultimo erro em errno/GetLastError (Windows) que ocorreu 
			dentro da função ou retorna 0 se o envio foi um sucesso.

		# OBS:
			É possível obter uma mensagem de erro com o erro retornado pela função,
			diretamente do sistema, usando as seguintes funções:
			
			strerror() - Linux
			FormatMessage() - Windows

	# getfilesize_arq(FILE *file)

		Retorna o tamanho total de um arquivo.

		# Parâmetros:
			FILE *file: Arquivo no qual deseja-se obter o tamanho total.

		# Valor de retorno:
			Retorna -1 caso o arquivo passado seja NULL ou retorna o tamanho total do arquivo.

	# getfilename_arq(char *path)

		Retorna apenas o nome de um arquivo dentro de o caminho completo passado em char *path.

		# Parâmetros:
			char *path: O caminho que deve ser buscado o nome do arquivo.

		# Valor de retorno:
			Nome do arquivo passado em char *path.