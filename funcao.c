void distribuirLivrosNasEstantes(Biblioteca* biblioteca, Livro* listaLivros) {
    Estante* estanteAtual = biblioteca->estantes;
    int prateleiraAtual = 0;

    if (estanteAtual == NULL) {
        adicionarEstante(biblioteca);  // Garante que exista ao menos uma estante
        estanteAtual = biblioteca->estantes;
    }

    Livro* livroAtual = listaLivros;
    while (livroAtual != NULL) {
        Prateleira* prateleira = &estanteAtual->prateleiras[prateleiraAtual];
        int capacidadeRestanteLargura = prateleira->capacidadeLargura;
        int capacidadeRestanteProfundidade = prateleira->capacidadeProfundidade;

        // Encontra o melhor livro que caiba no espaço disponível de largura e profundidade da prateleira
        Livro* melhorLivro = NULL;
        Livro* anterior = NULL;
        Livro* atual = livroAtual;
        Livro* anteriorMelhor = NULL;
        int menorDiferencaLargura = capacidadeRestanteLargura + 1;
        int menorDiferencaProfundidade = capacidadeRestanteProfundidade + 1;

        while (atual != NULL) {
            int diferencaLargura = capacidadeRestanteLargura - atual->largura;
            int diferencaProfundidade = capacidadeRestanteProfundidade - atual->profundidade;
            if (diferencaLargura >= 0 && diferencaProfundidade >= 0 && diferencaLargura < menorDiferencaLargura) {
                melhorLivro = atual;
                anteriorMelhor = anterior;
                menorDiferencaLargura = diferencaLargura;
                menorDiferencaProfundidade = diferencaProfundidade;
                if (diferencaLargura == 0 && diferencaProfundidade == 0) break;
            }
            anterior = atual;
            atual = atual->next;
        }

        if (melhorLivro != NULL) {
            // Adiciona o melhor livro encontrado na prateleira
            Livro* novoLivro = (Livro*) malloc(sizeof(Livro));
            *novoLivro = *melhorLivro;
            novoLivro->next = prateleira->livros;
            if (prateleira->livros != NULL) {
                prateleira->livros->prev = novoLivro;
            }
            prateleira->livros = novoLivro;
            prateleira->capacidadeLargura -= melhorLivro->largura;
            prateleira->capacidadeProfundidade -= melhorLivro->profundidade;

            // Remove o livro da lista original
            if (anteriorMelhor != NULL) {
                anteriorMelhor->next = melhorLivro->next;
            } else {
                livroAtual = melhorLivro->next;
            }
            if (melhorLivro->next != NULL) {
                melhorLivro->next->prev = anteriorMelhor;
            }
            free(melhorLivro);
        } else {
            prateleiraAtual++; // Vai para a próxima prateleira
            if (prateleiraAtual == 6) {
                adicionarEstante(biblioteca);
                estanteAtual = estanteAtual->next;
                prateleiraAtual = 0;
            }
        }
    }
}
