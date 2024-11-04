void distribuirLivrosNasEstantes(Biblioteca* biblioteca, Livro* listaLivros) {
    Estante* estanteAtual = biblioteca->estantes;
    int prateleiraAtual = 0;

    if (estanteAtual == NULL) {
        adicionarEstante(biblioteca);  // Garante que exista ao menos uma estante
        estanteAtual = biblioteca->estantes;
    }

    int areaPrateleira = 32 * 96; // Área total da prateleira
    int capacidadeRestante = areaPrateleira; // Capacidade disponível da prateleira
    int volumeMaximo = 100000; // Volume máximo da prateleira
    int volumeAtual = 0; // Volume atual da prateleira
    Livro* livroAtual = listaLivros;

    while (livroAtual != NULL) {
        Prateleira* prateleira = &estanteAtual->prateleiras[prateleiraAtual];
        int areaLivro = livroAtual->largura * livroAtual->profundidade; // Cálculo da área do livro

        // Verifica se o livro cabe na prateleira em termos de área e volume
        if (areaLivro <= capacidadeRestante && (volumeAtual + livroAtual->volume) <= volumeMaximo) {
            prateleira->livros = livroAtual; // Adiciona o livro à prateleira
            capacidadeRestante -= areaLivro; // Atualiza a capacidade restante
            volumeAtual += livroAtual->volume; // Atualiza o volume atual
            livroAtual = livroAtual->next; // Avança para o próximo livro
        } else {
            // Se não couber, vai para a próxima prateleira
            prateleiraAtual++;
            if (prateleiraAtual == 6) { // Se todas as prateleiras da estante estão cheias
                adicionarEstante(biblioteca); // Adiciona uma nova estante
                estanteAtual = estanteAtual->next; // Move para a próxima estante
                prateleiraAtual = 0; // Reinicia o índice da prateleira
            }
            // Reinicia a capacidade e o volume para a nova prateleira
            capacidadeRestante = areaPrateleira; 
            volumeAtual = 0; 
        }
    }
}
