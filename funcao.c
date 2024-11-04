int excluirEstante(Biblioteca *biblioteca, Estante *estanteParaRemover) {
    if (biblioteca == NULL || estanteParaRemover == NULL) {
        printf("Biblioteca ou estante inválida.\n");
        return -1;
    }

    // Se a estante a ser removida for a primeira da lista
    if (biblioteca->estantes == estanteParaRemover) {
        biblioteca->estantes = estanteParaRemover->next;
        if (biblioteca->estantes != NULL) {
            biblioteca->estantes->prev = NULL;
        }
    } else {
        // Se a estante estiver no meio ou no final
        if (estanteParaRemover->prev != NULL) {
            estanteParaRemover->prev->next = estanteParaRemover->next;
        }
        if (estanteParaRemover->next != NULL) {
            estanteParaRemover->next->prev = estanteParaRemover->prev;
        }
    }

    // Liberar a memória da estante excluída e de suas prateleiras e livros
    for (int i = 0; i < 6; i++) {
        Livro *livroAtual = estanteParaRemover->prateleiras[i].livros;
        while (livroAtual != NULL) {
            Livro *proxLivro = livroAtual->next;
            free(livroAtual);
            livroAtual = proxLivro;
        }
    }
    free(estanteParaRemover);

    printf("Estante excluída com sucesso.\n");
    return 0;
}
