// BIBLIOTECAS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <locale.h>
#include <ctype.h>
#include <windows.h>

//VARIAVEIS GLOBAIS
//int cadastroDeuCerto = 0;
char usuarioAtual[1000];
char arquivoCarrinhoUsuario[1100];
char arquivoPedidosUsuario[1100];

//LOWER CASE 
void toLowerCase(char str[]) {
  int i = 0;
  while (str[i] != '\0') {
    str[i] = tolower(str[i]);
    i++;
  }
}

//FUNÇÕES LIMPAR TELA E PARA ESPERAR
void limparTela() {
  #ifdef _WIN32
  system("cls"); // Windows
  #endif
}

void esperar(float segundos) {
  #ifdef _WIN32
  Sleep((DWORD)(segundos * 1000)); // Windows usa milissegundos
  #else
  usleep((useconds_t)(segundos * 1000000)); // Linux / macOS usa microssegundos
  #endif
}

int gerarId() {
    FILE *arquivoId = fopen("ultimoId.txt", "r");
    int id = 1;

    if (arquivoId != NULL) {
        fscanf(arquivoId, "%d", &id);
        id++; // incrementa o último id
        fclose(arquivoId);
    }

    // Salva o novo último ID
    arquivoId = fopen("ultimoId.txt", "w");
    if (arquivoId != NULL) {
        fprintf(arquivoId, "%d", id);
        fclose(arquivoId);
    }

    return id;
}

//FUNÇÕES
void cadastro() {
	limparTela();
  // VARIAVEIS
  char novoUsuario[1000];
  char novaSenha[100];
  char linha[300];
  char usuarioDoArquivo[1000];
  int encontrou_usuario = 0;


 
  printf("=== CADASTRO ===\n");
  printf("Usuario: ");
  scanf("%s", novoUsuario);

  // VARIFICA SE USUARIO  JA EXISTE
  FILE * arquivo = fopen("usuarios.txt", "r");
  if (arquivo == NULL) {
    arquivo = fopen("usuarios.txt", "a+"); // SE NAO, CRIA ARQUIVO
  } else {
    while (fgets(linha, 300, arquivo) != NULL) {
      sscanf(linha, "%[^,]", usuarioDoArquivo); //SÓ LE O USUARIO

      if (strcmp(novoUsuario, usuarioDoArquivo) == 0) {
        printf("Usuário já existe!\n");
        esperar(3);
        encontrou_usuario = 1;
        break;
      }
    }
    fclose(arquivo);
  }
  // SE NAO EXISTIR CADASTRA NO FORMATO: usuario,senha
  if (encontrou_usuario == 0) {
    arquivo = fopen("usuarios.txt", "a");
    if (arquivo == NULL) {
      printf("ERRO!");
    } else {
      printf("Senha: ");
      scanf("%s", novaSenha);
      fprintf(arquivo, "%s,%s\n", novoUsuario, novaSenha);
      printf("Usuário e senha cadastrados com sucesso!\n");
      printf("Agora digite 2 no menu para fazer login e entrar no sistema!\n");
      //cadastroDeuCerto = 1;
      esperar(2.5);
      
      fclose(arquivo);
    }
  }
}

void loginUsuario() {
	limparTela();
  // VARIAVEIS
  char usuarioDigitado[1000];
  char senhaDigitada[300];
  char usuarioLogado[1000];
  char senhaLogada[300];
  int encontrouLogin = 0;
  char linhaLogin[300];

  
  printf("=== LOGIN ===\n");
  printf("Usuário: ");
  scanf("%s", usuarioDigitado);
  printf("Senha: ");
  scanf("%s", senhaDigitada);

  FILE * arquivo = fopen("usuarios.txt", "r"); // MESMA COISA CADASTRO
  if (arquivo == NULL) {
    printf("Nenhum usuário cadastrado!\n");
    esperar(1.5);
  } else {
    while (fgets(linhaLogin, 300, arquivo) != NULL) {
      sscanf(linhaLogin, "%[^,],%s", usuarioLogado, senhaLogada);

	  // COMPARA USUARIO E SENHA
      if (strcmp(usuarioDigitado, usuarioLogado) == 0 && strcmp(senhaDigitada, senhaLogada) == 0) {
        printf("Login Feito com sucesso!\n");
        esperar(2);
        
        strcpy(usuarioAtual, usuarioDigitado);
        
        // CRIA ARQUIVO ESPECIFICO PARA CADA USUARIO
        sprintf(arquivoCarrinhoUsuario, "carrinho_%s.txt", usuarioAtual);
        sprintf(arquivoPedidosUsuario, "pedidos_%s.txt", usuarioAtual);

        esperar(2);
        encontrouLogin = 1;
        menuPrincipal(); // VAI PARA MENU PRINCIPAL
        break;
      }
    }

    if (encontrouLogin == 0) {
      printf("Senha e/ou usuario incorretos, tente fazer login de novo ou realize o cadastro.\n");
      esperar(4);
      
    }
    fclose(arquivo);
  }
}

void menuPrincipal() {

  // VARIAVEL
  int opcaoMenuPrincipal;

  do {
    limparTela();
    printf("=== MENU PRINCIPAL FEIFOOD ===\n");
    printf("1.Buscar Alimento\n2.Fazer Pedido\n3.Avaliar Pedido\n4.Excluir Pedido\n5.Sair\n");
    printf("Opção: ");
    scanf("%d", & opcaoMenuPrincipal);
    getchar();

    switch (opcaoMenuPrincipal) {
    case 1:
      buscarAlimento(); // BUSCA ALIMENTOS NO CARDAPIO

      break;
    case 2:
      novoFazerPedido(); // FAZ UM NOVO PEDIDO
      break;
    case 3:
      avaliarPedido(); // AVALIA PEDIDOS JÁ FEITOS
      break;
    case 4: {
    	excluirPedido();
		break;
	}
    case 5:
      return; // SAI DO PROGRAMA
    }
  } while (opcaoMenuPrincipal != 5);
}

void buscarAlimento() {
	limparTela();
    // VARIAVEIS
    char restauranteQEscolheu[100];
    char categoriaQEscolheu[100];
    char linha[300];
    char restaurante[100];
    char categoria[100];
    int encontrourest = 0;
	int encontroucateg = 0; 

    printf("=== BUSCAR ALIMENTO/ADICIONAR ITEM ===\n");

    printf("Digite o nome do Restaurante (McDonalds, Subway, Habibs, Starbucks, etc...): ");
    scanf(" %[^\n]", restauranteQEscolheu); 
    toLowerCase(restauranteQEscolheu);
    getchar();

    // ABRE ARQUIVO
    FILE *arquivo = fopen("cardapio.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // PROCURA O RESTAURANTE
    while (fgets(linha, 300, arquivo) != NULL) {
        sscanf(linha, "%[^,],", restaurante); 
        toLowerCase(restaurante);

        if (strcmp(restaurante, restauranteQEscolheu) == 0) {
            encontrourest = 1;
            break; 
        }
    }
    
    while (fgets(linha, 300, arquivo) != NULL) {
        sscanf(linha, "%[^,],", restaurante); 
        toLowerCase(restaurante);

        if (strcmp(restaurante, restauranteQEscolheu) == 0) {
            encontrourest = 1;
            break; 
        }
    }

    fclose(arquivo);

    // NÃO ENCONTROU
    if (encontrourest == 0) {
        printf("Restaurante NÃO encontrado!\n");
        printf("PRESSIONE ENTER PARA VOLTAR...");
        getchar();
        return;
    } 
    mostrarCategoria(restauranteQEscolheu);
    printf("Digite a Categoria: ");
    scanf(" %[^\n]", categoriaQEscolheu); 
    getchar();
   	toLowerCase(categoriaQEscolheu);
    	
    	arquivo = fopen("cardapio.txt", "r");
    	if (arquivo == NULL) {
        	printf("Erro ao abrir o arquivo!\n");
        	return;
    	}
    	while (fgets(linha, 300, arquivo) != NULL) {
        	sscanf(linha, "%[^,],%[^,],", restaurante,categoria); 
        	toLowerCase(restaurante);
        	toLowerCase(categoria);

        	if (strcmp(restaurante, restauranteQEscolheu) == 0 && strcmp(categoria, categoriaQEscolheu) == 0) {
            	encontroucateg = 1;
            	break; 
        	}
    	}
		fclose(arquivo);
		
    // ENCONTROU
	if(encontroucateg == 0){
        printf("Categoria NÃO encontrada nesse restaurante!\n");
        printf("PRESSIONE ENTER PARA VOLTAR...");
        getchar();
        return;
	}

    // MOSTRA ITENS DA CATEGORIA E ADICIONA NO CARRINHO
    mostrarItens(restauranteQEscolheu, categoriaQEscolheu);
    adicionarCarrinho(restauranteQEscolheu, categoriaQEscolheu);
	}

int mostrarCategoria(char restauranteQEscolheu[]) {
  limparTela();
  //VARIAVEIS
  char linhaCardapio[300];
  char restauranteDoArquivo[100];
  char categoriaDoArquivo[100];
  char categoriasMostradas[50][100]; // ARMAZENA AS CATEGORIAS QUE JA FORAM MOSTRADAS
  int totalCategorias = 0;
  int jaMostrou;
  int encontrouCategoria = 0;
  int i;
  char restauranteEscritoArquivo[100];
  char comidaCardapio[100];
  char ingrediente[200];
  float preco;

  // ABRE ARQUIVO
  FILE * arquivo = fopen("cardapio.txt", "r");
  if (arquivo == NULL) {
    printf("Nenhum Cardápio encontrado!\n");
    return 0;
  }

  // LE A LINHA DO CARDAPIO
  printf("=== CATEGORIAS ===\n");

  encontrouCategoria = 0;
  toLowerCase(restauranteEscritoArquivo);
  while (fgets(linhaCardapio, 300, arquivo) != NULL) {
    sscanf(linhaCardapio, "%[^,],%[^,],%[^,],%f,%[^\n]", restauranteEscritoArquivo, categoriaDoArquivo, comidaCardapio, & preco, ingrediente);
	// VERIFICA SE É DO RESTAURANTE QUE O USUARIO DIGITOU
    if (strcmp(restauranteQEscolheu, restauranteEscritoArquivo) == 0) {
      jaMostrou = 0;
	
	// VE SE A CATEGORIA JA FOI MOSTRADA
      for (i = 0; i < totalCategorias; i++) {
        if (strcmp(categoriaDoArquivo, categoriasMostradas[i]) == 0) {
          jaMostrou = 1;
          break;
        }
      }
      // SE N MOSTROU AINDA, ELE IMPRIME E ARMAZENA 
      if (jaMostrou == 0) {
        strcpy(categoriasMostradas[totalCategorias], categoriaDoArquivo);
        totalCategorias++;
        printf("- %s\n", categoriaDoArquivo);
        encontrouCategoria = 1;
      }
    }
  }
  fclose(arquivo);
  if (encontrouCategoria == 0) {
    printf("Nenhuma categoria encontrada nesse restaurante!\n");
    printf("PRESSIONE ENTER PARA VOLTAR...");
    getchar();
  }

  return totalCategorias; // RETORNA O NUMERO DE CATEGORIAS QUE FORAM ENCONTRADAS (ASSIM NAO TEM REPETICAO)
}

void mostrarItens(char restauranteQEscolheu[], char categoriaQEscolheu[]) {
  //VARIAVEIS 
  limparTela();
  char restauranteEscritoArquivo[100];
  char categoriaDoArquivo[100];
  char comidaCardapio[100];
  char ingredientes[200];
  float preco;
  char linhaCardapio[300];
	
  FILE * arquivo = fopen("cardapio.txt", "r");
  if (arquivo == NULL) {
    printf("Nenhum cardáio encontrado!\n");
    return;
  }

  // MOSTRA ITENS
  printf("=== ITENS ===\n");

  while (fgets(linhaCardapio, 300, arquivo) != NULL) {
    sscanf(linhaCardapio, "%[^,],%[^,],%[^,],%f,%[^\n]", restauranteEscritoArquivo, categoriaDoArquivo, comidaCardapio, & preco, ingredientes);
    if (strcmp(restauranteQEscolheu, restauranteEscritoArquivo) == 0 && strcmp(categoriaQEscolheu, categoriaDoArquivo) == 0) {
      printf(" - %s \n", comidaCardapio);
    }
  }
  fclose(arquivo);
}

int itemjaExiste(char restauranteQEscolheu[], char comidaCardapio[]) {
  //VARIAVEIS
  char restauranteDoArquivo[100], categoriaDoArquivo[100];
  char comidaDoArquivo[100], ingredientes[1000];
  float preco;
  int qtd;
  int quantidadeExistente = 0;

  // VE SE O CARINHO EXISTE
  FILE * arquivo = fopen(arquivoCarrinhoUsuario, "r");
  if (arquivo == NULL) {
    return 0; // SE FOR 0, ENTÃO O CARRINHO ESTA VAZIO
  }

  // PROCURA ITEM NO CARRINHO
  while (fscanf(arquivo, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n",usuarioAtual, restauranteDoArquivo, categoriaDoArquivo,comidaDoArquivo, ingredientes, & qtd, & preco) == 7) {

    if (strcmp(restauranteQEscolheu, restauranteDoArquivo) == 0 && strcmp(comidaCardapio, comidaDoArquivo) == 0) {
      quantidadeExistente = qtd; // QUANTIDADE DE ITENS
      break;
    }
  }

  fclose(arquivo); // FECHA ARQUIVO
  return quantidadeExistente;
}

void adicionarCarrinho( char restauranteQEscolheu[],char categoriaQEscolheu[]) {
  //VARIAVEIS
  char comidaPesquisada[1000];
  char restauranteDoArquivo[100];
  char categoriaDoArquivo[100];
  char comidaCardapio[1000];
  char ingredientes[1000];
  float preco;
  int resposta, qtd = 0, encontrouItem;
  char linhaCardapio[300];
  int qtdJaExiste;

  //ABRE ARQUIVO
  FILE * arquivo = fopen("cardapio.txt", "r");
  if (arquivo == NULL) {
    printf("Nenhum cardápio encontrado!\n");
    return;
  }

  //PEDE NOME DO ITEM            
  printf("Digite o nome do item para ver os DETALHES: ");
  scanf("%[^\n]", comidaPesquisada);
  toLowerCase(comidaPesquisada);
  getchar();


  // PROCURA ITEM
  encontrouItem = 0;
  rewind(arquivo);
  while (fgets(linhaCardapio, 300, arquivo) != NULL) {
    linhaCardapio[strcspn(linhaCardapio, "\n")] = 0; // REMOVE \N

    // PROCESSA A LINHA DO CARDAPIO USANDO STRTOK
    char * token;
    char temporario[300];
    strcpy(temporario, linhaCardapio);

    token = strtok(temporario, ",");
    if (token) strcpy(restauranteDoArquivo, token);

    token = strtok(NULL, ",");
    if (token) strcpy(categoriaDoArquivo, token);

    token = strtok(NULL, ",");
    if (token) strcpy(comidaCardapio, token);

    token = strtok(NULL, ",");
    if (token) preco = atof(token);

    token = strtok(NULL, "\n"); // RESTO DA LINHA É INGREDIENTE
    if (token) strcpy(ingredientes, token);
    
    toLowerCase(restauranteDoArquivo);
    toLowerCase(categoriaDoArquivo);
    toLowerCase(comidaCardapio);


    
	// VE SE É O ITEM QUE O USUARIO DIGITOU
    if (strcmp(comidaPesquisada, comidaCardapio) == 0 && strcmp(categoriaQEscolheu, categoriaDoArquivo) == 0 && strcmp(restauranteQEscolheu, restauranteDoArquivo) == 0) {
      printf("\n=== DETALHES DO ITEM ===\n");
      printf("Nome: %s\n", comidaCardapio);
      printf("Preço: R$%.2f\n", preco);
      printf("Ingredientes: %s\n", ingredientes);
      encontrouItem = 1;
      break;
    }
  }
  fclose(arquivo);

  if (encontrouItem == 0) {
    printf("Item não encontrado!\n");
    printf("PRESSIONE ENTER PARA VOLTAR...");
    getchar();
    return;
  } else {
    printf("\nDeseja ADICIONAR ao carrinho? (1. SIM / 2. NÃO): ");
    scanf("%d", & resposta);
    getchar();
    esperar(1);
	limparTela();
    if (resposta == 1) {
      // VERIFICA SE ITEM JÁ EXISTE NO CARRINHO
      qtdJaExiste = itemjaExiste(restauranteQEscolheu, comidaCardapio);

      if (qtdJaExiste > 0) {
        
        printf("=== ADICIONANDO AO CARRINHO ===\n");
        esperar(1);
        printf("\nItem já existe no carrinho! Quantidade atual: %d\n", qtdJaExiste);
        printf("Digite quantas unidades deseja ADICIONAR: ");
        scanf("%d", & qtd);
        getchar();

        if (qtd > 0) {
          int novaQuantidade = qtdJaExiste + qtd; // ATUALIZA QTD DO CARRINHO

          attqtdCarrinho(restauranteQEscolheu, comidaCardapio, novaQuantidade);
          printf("Quantidade atualizada: %d unidades\n", novaQuantidade);
          esperar(1.5);

        } else {
          printf("Quantidade inválida! Item não foi alterado.\n");
        }
      } else {
        // ITEM NOVO 
        while (qtd <= 0) {
        	esperar(1);
          
          printf("===ADICIONANDO AO CARRINHO ===\n");
          printf("Digite a quantidade: ");
          scanf("%d", & qtd);
          getchar();
          if (qtd <= 0) printf("Quantidade deve ser maior que zero!\n");
        }

        //ABRE ARQUIVO PARA ADD AO CARRINHO
        FILE * arquivoCarrinho = fopen(arquivoCarrinhoUsuario, "a");
        if (arquivoCarrinho == NULL) {
          printf("Erro ao abrir carrinho do usuário!\n");
          return;
        }
		// SALVA 
        fprintf(arquivoCarrinho, "%s,%s,%s,%s,%s,%d,%.2f\n",usuarioAtual, restauranteQEscolheu, categoriaQEscolheu,comidaCardapio, ingredientes, qtd, preco);
        fclose(arquivoCarrinho);

        printf("Item adicionado com sucesso!\n");
      }

      printf("\nPRESSIONE ENTER PARA CONTINUAR...");
      getchar();
    } else if (resposta == 2){
    	printf("Item NÃO adicionado ao carrinho!\n");
    	printf("PRESSIONE ENTER PARA CONTINUAR...\n");
    	getchar();
	}
	else {
		printf("Opção INVÁLIDA!\n");
		printf("PRESIONE ENTER PARA CONTINUAR...\n");
		getchar();
	}
  }
}

void attqtdCarrinho(char restauranteQEscolheu[], char comidaCardapio[], int novaQuantidade) {
  // VARIAVEIS
  char restauranteDoArquivo[100];
  char categoriaDoArquivo[100];
  char comidaDoArquivo[100]; 
  char ingredientes[1000];
  char usuario[100];
  int qtd;
  float preco;

	// ABRE O CARRINHO ATUAL
  FILE * arquivo = fopen(arquivoCarrinhoUsuario, "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir carrinho!\n");
    printf("PRESSIONE ENTER PARA VOLTAR...");
    getchar();
    return;
  }
  
  
  // CRIA UM ARQUIVO TEMPORARIO
  FILE * temporario = fopen("temporario.txt", "w");
  if (temporario == NULL) {
    printf("Erro ao criar arquivo temporário!\n");
    fclose(arquivo);
    return;
  }

  while (fscanf(arquivo, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n",usuario, restauranteDoArquivo, categoriaDoArquivo,comidaDoArquivo, ingredientes, & qtd, & preco) == 7) {
	// VE SE É O ITEM QUE TEM Q SER ATUALIZADO
    if (strcmp(restauranteQEscolheu, restauranteDoArquivo) == 0 && strcmp(comidaCardapio, comidaDoArquivo) == 0) {
      // ATUALIZA QUANTIDADE
      fprintf(temporario, "%s,%s,%s,%s,%s,%d,%.2f\n",usuario, restauranteDoArquivo, categoriaDoArquivo,comidaDoArquivo, ingredientes, novaQuantidade, preco);
    } else {
      // MANTEM OUTROS ITENS
      fprintf(temporario, "%s,%s,%s,%s,%s,%d,%.2f\n",usuario, restauranteDoArquivo, categoriaDoArquivo,comidaDoArquivo, ingredientes, qtd, preco);
    }
  }

  fclose(arquivo);
  fclose(temporario);
 // SUBSTITUI O ARQUIVO ORIGINAL PELO TEMPORARIO
  remove(arquivoCarrinhoUsuario);
  rename("temporario.txt", arquivoCarrinhoUsuario);
}

void verCarrinho() {
	limparTela();
	// VARIAVEIS
	char linha[300];
  char restaurante[100];
  char categoria[100];
  char comida[1000];
  char usuario[100];
  char ingredientes[1000];
  int qtd;
  int temItem = 0;
  float preco, subtotal, total = 0;

  FILE * arquivoCarrinho = fopen(arquivoCarrinhoUsuario, "r");
  if (arquivoCarrinho == NULL) {
    printf("Carrinho vazio!\n");
    return;
  }
  
  	 printf("=== CARRINHO ===\n");

  // MOSTRA OS ITENS DO CARRINHO
  	 while (fscanf(arquivoCarrinho, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n",usuario, restaurante, categoria, comida, ingredientes, & qtd, & preco) == 7) {
    	subtotal = qtd * preco;
    	printf("%s - %s (%s)\n", restaurante, comida, categoria);
    	printf("   Ingredientes: %s\n", ingredientes);
    	printf("   %d x R$%.2f = R$%.2f\n", qtd, preco, subtotal);
    	printf("----------------------------------------\n");
    	total += subtotal;
		temItem = 1;
		}

  
  
  if(temItem == 0){
  	printf("Seu Carrinho está VAZIO!\n");
  } else {
  	printf("Total: R$ %.2f\n", total);
  }
  
 
  fclose(arquivoCarrinho);

  printf("PRESSIONE ENTER PARA CONTINUAR...");
  getchar();
}

void finalizarPedido() {
	limparTela();
    // VARIÁVEIS
    int metodoPagamento;
    char restaurante[100];
    char categoria[100];
    char comida[100];
    char usuario[100];
    char ingredientes[100];
    char linha[200];
    int qtd;
    float preco, subtotal = 0, total = 0;
    int idPedido = gerarId();

    
    printf("=== FINALIZAR PEDIDO ===\n");

    // VERIFICA SE CARRINHO EXISTE
    FILE *arquivoCarrinho = fopen(arquivoCarrinhoUsuario, "r");
    if (arquivoCarrinho == NULL) {
        printf("Seu carrinho está vazio!\n");
        printf("PRESSIONE ENTER PARA CONTINUAR...");
        getchar();
        return;
    }

    // VERIFICA SE O CARRINHO TEM CONTEÚDO
    if (fgets(linha, 200, arquivoCarrinho) == NULL) {
        fclose(arquivoCarrinho);
        printf("\nSeu carrinho está vazio!\n");
        printf("PRESSIONE ENTER PARA VOLTAR AO MENU...\n");
        getchar();
        return;
    }
    rewind(arquivoCarrinho);

    // ABRE ARQUIVO DE PEDIDOS PARA SALVAR
    FILE *arquivoPedidos = fopen(arquivoPedidosUsuario, "a");
    if (arquivoPedidos == NULL) {
        fclose(arquivoCarrinho);
        printf("Erro ao abrir arquivo de pedidos!\n");
        printf("PRESSIONE ENTER PARA CONTINUAR...");
        getchar();
        return;
    }

    // RESUMO DO PEDIDO
    while (fscanf(arquivoCarrinho, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n",usuario, restaurante, categoria, comida, ingredientes, &qtd, &preco) == 7) {

        subtotal = qtd * preco;
        total += subtotal;

        printf("%s - %s (%s)\n", restaurante, comida, categoria);
        printf("Qtd: %d | Preço: R$%.2f | Subtotal: R$%.2f\n", qtd, preco, subtotal);
        printf("----------------------------------------\n");

        // SALVA CADA ITEM NO ARQUIVO DE PEDIDOS
		fprintf(arquivoPedidos, "%d,%s,%s,%s,%s,%d,%.2f,0\n",idPedido, usuario, restaurante, categoria, comida, qtd, total);
    	
    }

    fclose(arquivoCarrinho);
    fclose(arquivoPedidos);

    printf("TOTAL: R$%.2f\n", total);
    printf("PRESSIONE ENTER PARA CONTINUAR...");
    getchar();

    // PAGAMENTO
    metodoPagamento = 0;
    
    while (metodoPagamento != 1 && metodoPagamento != 2 && metodoPagamento != 3) {
        printf("=== MÉTODO DE PAGAMENTO === \n");
        printf("1. Crédito\n2. Débito\n3. Pix\n");
        printf("Opção: ");
        scanf("%d", &metodoPagamento);
        getchar();

        if (metodoPagamento < 1 || metodoPagamento > 3) {
            printf("Opção inválida! Tente novamente.\n");
        }
    }

    printf("\nProcessando pagamento...\n");
    esperar(1.5);
    printf("Pagamento realizado com sucesso!\n");
    
    printf("Pedido finalizado!\n");
    printf("ID do Pedido: %d\n",idPedido);
    esperar(2);

    // LIMPA CARRINHO QUANDO FINALIZAR O PEDIDO
    arquivoCarrinho = fopen(arquivoCarrinhoUsuario, "w");
    if (arquivoCarrinho != NULL) {
        fclose(arquivoCarrinho);
    }

    printf("PRESSIONE ENTER PARA CONTINUAR...");
    getchar();
}

void novoFazerPedido() {
	limparTela();
	// VARIAVEIS
  int continuar = 1;
  char restauranteQEscolheu[100];
  char categoriaQEscolheu[100];
  int querContinuar;
  int opcaoPedido;

  do {
   	limparTela();
    printf("=== FAZER PEDIDO ===\n");
    printf("1. Adicionar Item\n");
    printf("2. Ver Carrinho\n");
    printf("3. Gerenciar Carrinho\n");
    printf("4. Finalizar Pedido\n");

    printf("5. Sair\n");
    printf("Opção: ");
    scanf("%d", & opcaoPedido);
    getchar();

    switch (opcaoPedido) {
    case 1: {
      buscarAlimento();

      break;
    } 

    case 2: {
      
      verCarrinho(); // MOSTRA O CARRINHO
      break;
    }

    case 4: {
      finalizarPedido(); // FINALIZA O PEDIDO
      break;
    }
    case 3: {
      removerCarrinho(); // GERENCIA O CARRINHO NESSE CASO REMOVE PEDIDO OU EDITA A QUANTIDADE
      break;
    }

    case 5: {
      
      printf("VOLTANDO AO MENU PRINCIPAL...\n");
      esperar(1);
      break;
    }
    default:
      printf("Opção inválida!");
      esperar(1);
    }
  } while (opcaoPedido != 5);
}

void removerCarrinho() {
	limparTela();
	// VARIAVEIS
  char comidaRemover[100];
  char usuario[100], restaurante[100], categoria[100], comida[100], ingredientes[100];
  int qtd;
  float preco;
  int opcao;
  int qtdNova;
  char restauranteRemover[100];
  int encontrouItem = 0;
  
  FILE *arquivoCarrinho = fopen(arquivoCarrinhoUsuario, "r");
    if (arquivoCarrinho == NULL) {
        printf("Carrinho vazio!\n");
        printf("Pressione ENTER para voltar...");
        getchar();
        return;
    }

    // Testa se há algum item no carrinho
    char testeLinha[500];
    if (fgets(testeLinha, sizeof(testeLinha), arquivoCarrinho) == NULL) {
    	printf("=== GERENCIAR CARRINHO ===\n");
        printf("Não é possível continuar — o carrinho está vazio!\n");
        fclose(arquivoCarrinho);
        printf("Pressione ENTER para voltar...");
        getchar();
        return;
    }

    // volta o ponteiro do arquivo pro início
    rewind(arquivoCarrinho);


  printf("=== GERENCIAR CARRINHO ===\n");
  printf("1. Remover item\n");
  printf("2. Editar quantidade\n");
  printf("Escolha uma opção: ");
  scanf("%d", & opcao);
  getchar();


  FILE * arquivoTemporario = fopen("temporario.txt", "w");
  if (arquivoTemporario == NULL) {
    printf("Erro ao criar arquivo temporário.\n");
    fclose(arquivoCarrinho);
    return;
  }

  if (opcao == 1) {
  	
  	
    verCarrinho();

    printf("\n=== REMOVER ITEM ===\n");
    printf("Digite o nome do restaurante: ");
    scanf(" %[^\n]", restauranteRemover);
    getchar();
    

    printf("Digite o nome da comida que deseja REMOVER: ");
    scanf(" %[^\n]", comidaRemover);
    getchar();


  } else if (opcao == 2) {
    printf("=== EDITAR QUANTIDADE ===\n");
    verCarrinho();

    printf("Digite o nome do restaurante: ");
    scanf(" %[^\n]", restauranteRemover);
    getchar();
    



    printf("Digite o nome da comida que deseja EDITAR: ");
    scanf(" %[^\n]", comidaRemover);
    getchar();



  } else {
    printf("Opção inválida!\n");
    printf("\nPRESSIONE ENTER PARA CONTINUAR...\n");
    getchar();
    fclose(arquivoCarrinho);
    fclose(arquivoTemporario);
    remove("temporario.txt");
    return;
  }
 toLowerCase(restauranteRemover);
 toLowerCase(comidaRemover);

  // MESMA IDEIA DE ARQ TEMPORARIO, AQUI ELE CRIA A VERSAO ATUALIZADA
  while (fscanf(arquivoCarrinho, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f\n",usuario, restaurante, categoria, comida, ingredientes, & qtd, & preco) == 7) {
	
	// VE SE É O ITEM CERTO PARA MODIFICAR
    if (strcmp(comida, comidaRemover) == 0 && strcmp(restaurante, restauranteRemover) == 0) {
      encontrouItem =1;
	  if (opcao == 1) {
        printf("Item removido: %s - %s\n", restaurante, comida);
        continue; // NAO ESCREVE NO ARQUIVO TEMPORARIO
      } else if (opcao == 2) {
        printf("Quantidade atual: %d\n", qtd);
        printf("Digite a nova quantidade: ");
        scanf("%d", & qtdNova);
        getchar();
        if (qtdNova <= 0) {
          printf("Quantidade inválida, item será removido.\n");
          continue; 
        }
        qtd = qtdNova; // ATUALIZA A QUANTIDAADE
        printf("Quantidade atualizada!\n");
      }
    }


    // ESCREVE OS ITENS NO ARQUIVO TEMPORARIO
    fprintf(arquivoTemporario, "%s,%s,%s,%s,%s,%d,%.2f\n",usuario, restaurante, categoria, comida, ingredientes, qtd, preco);
  }
  if (encontrouItem == 0) { 
    printf("Item/Restaurante NÃO encontrado!\n");
	}
	printf("PRESSIONE ENTER PARA CONTINUAR...");
  	getchar();

  fclose(arquivoCarrinho);
  fclose(arquivoTemporario);

 // SUBSTITUI O ORIGINAL PELO TEMPORARIO
  remove(arquivoCarrinhoUsuario);
  rename("temporario.txt", arquivoCarrinhoUsuario);


}

void avaliarPedido() {
	limparTela();
 // VARIAVEIS
	char linha[300];
	char pedidosParaMostrar[100][300];
	int totalParaMostrar = 0;
	int idPedido, qtd,nota;
	float total;
	char usuario[100], restaurante[100], categoria[100], comida[100];
	int notaPedidoAvaliar =0, i;

//ABRE ARQUIVO
	FILE *arquivoPedidos = fopen(arquivoPedidosUsuario, "r");
	if (arquivoPedidos == NULL) {
    	printf("Nenhum pedido encontrado!\n");
    	printf("PRESSIONE ENTER PARA VOLTAR...");
    	getchar();
    	return;
		}
// LE O ARQUIVO
	while (fscanf(arquivoPedidos, "%d,%[^,],%[^,],%[^,],%[^,],%d,%f,%d\n",&idPedido, usuario, restaurante, categoria, comida, &qtd, &total, &nota) == 8) {
    if (strcmp(usuario, usuarioAtual) == 0 && nota == 0) {
        sprintf(pedidosParaMostrar[totalParaMostrar], "%d - %s (%s) x%d", idPedido, comida, restaurante, qtd);
        totalParaMostrar++;
    	}
	}
	fclose(arquivoPedidos);

	if (totalParaMostrar == 0) {
		printf("=== AVALIAR PEDIDO ===\n");
    	printf("Nenhum pedido para avaliar!\n");
    	printf("PRESIONE ENTER PARA VOLTAR...");
    	getchar();
    	return;
		}
		
	// MOSTRAR PEDIDOS
	printf("=== PEDIDOS PARA AVALIAR ===\n");
	for (i = 0; i < totalParaMostrar; i++) {
    	printf("ID: %s\n", pedidosParaMostrar[i]);
	}
	// ID PEDIDO
	int numeroProcurado, encontrado = 0;
	printf("Digite o ID do pedido para AVALIAR: ");
	scanf("%d",&numeroProcurado);
	getchar();
	
	for (i = 0; i < totalParaMostrar; i++) {
    int idTemporario;
    sscanf(pedidosParaMostrar[i], "%d", &idTemporario);
    if (idTemporario == numeroProcurado) {
        encontrado = 1;
        break;
    	}
	}
	if(encontrado == 0){
		printf("ID inválido!\n");
		printf("PRESSIONE ENTER PARA VOLTAR...");
		getchar();
		return;
	}
	
	//NOTA
	while(notaPedidoAvaliar < 1 || notaPedidoAvaliar > 5){
		printf("Qual nota (1-5): ");
		scanf(" %d",&notaPedidoAvaliar);
		getchar();
		
		if(notaPedidoAvaliar < 1 || notaPedidoAvaliar > 5){
			printf("Nota inválida!\n");
		}
	} 
	
	//SALVAR NO ARQUIVO
	FILE *arquivoAntigo = fopen(arquivoPedidosUsuario, "r");
	FILE *arquivoTemporario = fopen("temporario.txt", "w");

	while (fscanf(arquivoAntigo, "%d,%[^,],%[^,],%[^,],%[^,],%d,%f,%d\n",&idPedido, usuario, restaurante, categoria, comida, &qtd, &total, &nota) == 8) {
    	if (idPedido == numeroProcurado && strcmp(usuario, usuarioAtual) == 0) {
        	nota = notaPedidoAvaliar;  // ATUALIZA A NOTA
    	}
    	fprintf(arquivoTemporario, "%d,%s,%s,%s,%s,%d,%.2f,%d\n",idPedido, usuario, restaurante, categoria, comida, qtd, total, nota);
	}

	fclose(arquivoAntigo);
	fclose(arquivoTemporario);

// SUBSTITUIR ARQUIVP
	remove(arquivoPedidosUsuario);
	rename("temporario.txt", arquivoPedidosUsuario);

	printf("Pedido avaliado com sucesso!\n");
	printf("PRESSIONE ENTER PARA VOLTAR...");
	getchar();

	


}

void excluirPedido(){
	limparTela();
    int idProcurado;
    int idPedido, qtd, nota;
    char usuario[100], restaurante[100], categoria[100], comida[100];
    float total;
    int encontrado = 0;
    int temPedidos = 0;
	
	
	printf("=== EXCLUIR PEDIDO ===\n");
	
	// ABRE ARQUIVO
    FILE *arquivoPedidos = fopen(arquivoPedidosUsuario, "r");
    if (arquivoPedidos == NULL) {
        printf("Nenhum pedido encontrado!\n");
        printf("PRESSIONE ENTER PARA VOLTAR...");
        getchar();
        return;
    }
    
    printf("=== PEDIDOS ===\n");
    while (fscanf(arquivoPedidos, "%d,%[^,],%[^,],%[^,],%[^,],%d,%f,%d\n",&idPedido, usuario, restaurante, categoria, comida, &qtd, &total, &nota) == 8) {
        if (strcmp(usuario, usuarioAtual) == 0) {
            printf("ID %d - %s (%s) | Qtd: %d | Preço Total: R$%.2f | Nota: %d\n",idPedido, comida, restaurante, qtd, total, nota);
      		temPedidos=1;
	    }
        
    }
    
        if (temPedidos == 0) {
        printf("\nNenhum pedido para excluir!\n");
        printf("PRESSIONE ENTER PARA VOLTAR...");
        getchar();
        fclose(arquivoPedidos);
        return;
    }
    
    rewind(arquivoPedidos);
    
    // QUAL ID EXCLUIR
    printf("\nDigite o ID do pedido que deseja excluir: ");
    scanf("%d", &idProcurado);
    getchar();
    
    
    
    FILE *temporario = fopen("temporario.txt", "w");
    if (temporario == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(arquivoPedidos);
        return;
    }
    
    while (fscanf(arquivoPedidos, "%d,%[^,],%[^,],%[^,],%[^,],%d,%f,%d\n",&idPedido, usuario, restaurante, categoria, comida, &qtd, &total, &nota) == 8) {
        if (idPedido != idProcurado || strcmp(usuario, usuarioAtual) != 0) {
            fprintf(temporario, "%d,%s,%s,%s,%s,%d,%.2f,%d\n",idPedido, usuario, restaurante, categoria, comida, qtd, total, nota);
        } else {
            encontrado = 1; // Marcou que achou e excluiu
            printf("Pedido EXCLUIDO\n");
        }
    }

    
    if(encontrado == 0){
    	printf("Pedido NÃO encontrado!\n");
	} 
	
	printf("PRESSIONE ENTER PARA VOLTAR...");
	getchar();
	
	fclose(arquivoPedidos);
    fclose(temporario);
    
    remove(arquivoPedidosUsuario);
    rename("temporario.txt", arquivoPedidosUsuario);

	
}

int main() {
  int opcao;
  setlocale(LC_ALL, "Portuguese"); // CONFIGURA PARA PORTUGUES

  do {
  	limparTela();
    printf("=== BEM VINDO AO FEIFOOD ===\n");
    printf("\n1.Cadastro\n2.Login\n3.Sair\n");
    printf("Opcao: ");
    scanf("%d", & opcao);
    getchar();

    switch (opcao) {
    case 1:
      cadastro(); // VAI PARA CADASTRO
      break;
    case 2:
      loginUsuario(); //VAI PAEA LOGIN
      break;
    case 3:
      return 0; // SAI DO PROGRAMA
    }
  } while (opcao != 3);

  return 0;
}


