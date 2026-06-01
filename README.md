# TrabalhoED
Este trabalho tem como objetivo implementar uma árvore B na linguagem C, abordando suas operações fundamentais e sua organização estrutural no contexto de manipulação de grandes volumes de dados armazenados em memória secundária.

Como Contribuir com o Projeto

Este projeto utiliza Git e GitHub para controle de versão. Para evitar conflitos e manter a organização do código, siga o fluxo de trabalho descrito abaixo.


1. Mantenha sua Branch Atualizada

Antes de começar qualquer atividade, certifique-se de que sua cópia local está atualizada:

git checkout main
git pull origin main

Isso garante que você estará trabalhando sobre a versão mais recente do projeto.

2. Crie uma Branch para sua Tarefa

Nunca desenvolva diretamente na branch main.

Crie uma branch específica para a funcionalidade ou correção que irá implementar:

git checkout -b feature/nome-da-funcionalidade

Exemplos:

git checkout -b feature/insercao-arvore-b
git checkout -b feature/remocao-arvore-b
git checkout -b fix/correcao-busca
Convenção de nomes

Utilize os seguintes prefixos:

Tipo	                 Prefixo	  Exemplo
Nova funcionalidade	   feature/	  feature/insercao
Correção de erro	     fix/	      fix/divisao-no
Documentação	         docs/  	  docs/readme
Refatoração	           refactor/  refactor/estrutura-no

3. Desenvolva e Teste sua Implementação

Implemente sua tarefa normalmente.

Antes de enviar suas alterações:

Verifique se o código compila.
Teste as funcionalidades alteradas.
Certifique-se de que não foram introduzidos erros em partes já implementadas.

4. Adicione os Arquivos Modificados

Após concluir a implementação:

git add .

Ou adicione apenas os arquivos específicos:

git add arquivo.c
5. Faça um Commit Descritivo

Utilize mensagens claras para facilitar o acompanhamento da evolução do projeto.

Exemplos:

git commit -m "Implementa inserção em árvore B"
git commit -m "Corrige divisão de nós durante inserção"
git commit -m "Atualiza documentação do projeto"

Evite mensagens genéricas como:

update
teste
mudanças
commit final
6. Envie sua Branch para o GitHub
git push origin nome-da-branch

Exemplo:

git push origin feature/insercao-arvore-b
7. Abra um Pull Request

Após enviar a branch:

Acesse o repositório no GitHub.
Clique em Compare & Pull Request.
Escreva uma descrição do que foi implementado.
Solicite revisão dos colegas, se necessário.
Crie o Pull Request.
Modelo de descrição
Descrição:
Implementação da operação de inserção na Árvore B.

Alterações realizadas:
- Criação da função de inserção;
- Tratamento de overflow;
- Divisão de nós.

Observações:
Necessário validar casos de teste com mais de 100 registros.
8. Revisão e Aprovação

Antes de realizar o merge:

Verifique se o código está funcionando.
Analise possíveis conflitos.
Confirme que a implementação segue o padrão definido pela equipe.

Somente após aprovação o Pull Request deverá ser integrado à branch main.

Fluxo Resumido
main
  ↓
git pull
  ↓
criar branch
  ↓
desenvolver
  ↓
git add
  ↓
git commit
  ↓
git push
  ↓
Pull Request
  ↓
Revisão
  ↓
Merge na main
Boas Práticas
Nunca trabalhe diretamente na main.
Faça commits pequenos e frequentes.
Utilize mensagens de commit descritivas.
Teste seu código antes de enviar.
Mantenha sua branch sincronizada com a main.
Documente alterações importantes.
Comunique à equipe qualquer mudança estrutural significativa
