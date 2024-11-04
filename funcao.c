void distribuirLivrosNasEstantes(Biblioteca* biblioteca, Livro* listaLivros) {
    Estante* estanteAtual = biblioteca->estantes;
    int prateleiraAtual = 0;

    if (estanteAtual == NULL) {
        adicionarEstante(biblioteca);  // Garante que exista ao menos uma estante
        estanteAtual = biblioteca->estantes;
    }

    int areaPrateleira = 32 * 96;
    int capacidadeRestante = areaPrateleira;
    int volumeMaximo = 100000;
    int volumeAtual = 0;
    Livro* livroAtual = listaLivros;
    
    while (livroAtual != NULL) {
        Prateleira* prateleira = &estanteAtual->prateleiras[prateleiraAtual];
        int areaLivro = livroAtual->largura * livroAtual->profundidade;

        // Verifica se o livro cabe na prateleira pelo espaço e pelo volume
        if (areaLivro <= capacidadeRestante && (volumeAtual + livroAtual->volume) <= volumeMaximo) {
            prateleira->livros = livroAtual;
            capacidadeRestante -= areaLivro;
            volumeAtual += livroAtual->volume;
            livroAtual = livroAtual->next;
        } else {
            // Vai para a próxima prateleira
            prateleiraAtual++;
            if (prateleiraAtual == 6) {
                adicionarEstante(biblioteca);
                estanteAtual = estanteAtual->next;
                prateleiraAtual = 0;
            }
            // Redefine a capacidade e o volume para a nova prateleira
            capacidadeRestante = areaPrateleira;
            volumeAtual = 0;
        }
    }
}
