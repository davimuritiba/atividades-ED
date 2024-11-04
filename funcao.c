int excluirEstantePorIndice(Biblioteca *biblioteca, int indice) {
    if (biblioteca == NULL || indice < 0) {
        printf("Biblioteca inválida ou índice negativo.\n");
        return -1;
    }

    Estante *estanteAtual = biblioteca->estantes;
    int contador = 0;

    // Percorrer a lista até encontrar a estante no índice especificado
    while (estanteAtual != NULL && contador < indice) {
        estanteAtual = estanteAtual->next;
        contador++;
    }

    // Se não encontrou a estante, retorna um erro
    if (estanteAtual == NULL) {
        printf("Estante com índice %d não encontrada.\n", indice);
        return -1;
    }

    // Ajuste dos ponteiros para remover a estante da lista
    if (estanteAtual->prev != NULL) {
        estanteAtual->prev->next = estanteAtual->next;
    } else {
        // Caso seja a primeira estante
        biblioteca->estantes = estanteAtual->next;
    }

    if (estanteAtual->next != NULL) {
        estanteAtual->next->prev = estanteAtual->prev;
    }

    // Liberar a memória da estante e dos livros nela contidos
    for (int i = 0; i < 6; i++) {
        Livro *livroAtual = estanteAtual->prateleiras[i].livros;
        while (livroAtual != NULL) {
            Livro *proxLivro = livroAtual->next;
            free(livroAtual);
            livroAtual = proxLivro;
        }
    }
    free(estanteAtual);

    printf("Estante de índice %d excluída com sucesso.\n", indice);
    return 0;
}
