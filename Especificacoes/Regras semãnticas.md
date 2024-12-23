
# Regras Semânticas  

## Declarações

- [x] Declarações de ID globais e locais
- [x] Protótipagem de função apenas antes de sua definição
- [x] Atribuição de constante deve ser o mesmo de sua declaração
- [x] Aceitar somente inteiros e IDs inteiros no tamanho de vetores
- [x] Quantidade de parametros em Declarações de função
- [x] Declaração de lista de variáveis antes de CMD
- [x] Init não deve ter protótipo
- [x] Funções podem ser chamadas somente mediante o uso de DO

## Consistência Tipos

- [x] Inteiros podem receber tanto inteiros quanto caracteres, o mesmo vale para caracter
- [ ] Boleanos recebem somente inteiros, recebendo 0(False) caso o inteiro tenha valor 0 e 1(True) caso tenha valor diferente
- [x] Valores de chamada de procedimento usando o Do devem ser compatíveis com os parâmetros de sua definição

## Declaração e Definição de procedimentos

- [x] Procedimentos não podem ocorrer dentro de expressões
- [x] Procedimentos podem ter variáveis locais
- [x] Procedimento não pode ser declarado dentro de outro
- [x] & Indica que o parâmetro é transferido via referência

## Expressões

- [x] Se expr é somente um identificador então o tipo de expr é desse identificador
- [x] Se expr é uma expressão então seu tipo é compatível com os tipos utilizados nela, seguindo a regra de que inteiro é compatível somente com caracter e boolean e vice versa, se tiver qualquer desses tipos e vier um real, um erro vai acontecer
- [x] Os argumentos passados em chamadas de procedimento devem ser compatíveis com os parâmetros em sua declaração ou prototipagem
- [x] Expressões utilizadas em índices devem ser de tipo inteiro // Aparentemente ta certo

## Comandos

- [x] Apenas os tipos básicos real, int, bool e char podem receber atribuições e o tipo associado deve ser compatível

## Dados

- [x] Estrutura máxima suportada é matriz
- [x] Constantes devem ser inicializadas em sua declaração
