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

        if (areaLivro <= capacidadeRestante && volumeAtual <= volumeMaximo) {
            estanteAtual->prateleiras[prateleiraAtual].livros = livroAtual;
            capacidadeRestante -= areaLivro;
            volumeAtual += livroAtual->volume;
            livroAtual = livroAtual->next;
        } 
        else {
            prateleiraAtual++; //vai para a proxima prateleira
            if (prateleiraAtual == 6) {
                adicionarEstante(biblioteca);
                estanteAtual = estanteAtual->next;
                prateleiraAtual = 0;
            }
            areaPrateleira = 32 * 96;
            capacidadeRestante = areaPrateleira;
            volumeAtual = 0;
        }
    }
}
