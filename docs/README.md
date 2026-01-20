# A2L: Anonymous Atomic Locks Proof-of-Concept

## 1. Visão Geral e Motivação
Este projeto implementa uma Prova de Conceito (PoC) para o protocolo **A2L (Anonymous Atomic Locks)**. O objetivo é permitir pagamentos off-chain através de um Tumbler (Mixer) sem a necessidade de confiança e sem a possibilidade de correlação entre remetente e destinatário.

As duas propriedades fundamentais garantidas pelo protocolo são:
1.  [cite_start]**Atomicidade:** Ou todas as etapas ocorrem (pagamento e recebimento), ou nenhuma ocorre[cite: 424].
2.  [cite_start]**Irrestreabilidade:** O Tumbler não consegue ligar o Sender (Ps) ao Receiver (Pr), quebrando o grafo de transações[cite: 432].

---

## 2. Arquitetura do Protocolo

[cite_start]O protocolo envolve três atores principais[cite: 491, 492, 493]:
* **Ps (Sender):** Quem envia os fundos.
* **Pr (Receiver):** Quem recebe os fundos.
* **Pt (Tumbler):** O intermediário que facilita a troca sem saber quem paga quem.

### Fluxo de Execução
[cite_start]O ciclo de vida do protocolo segue estas fases[cite: 342]:

1.  [cite_start]**Registration & Credential:** Sender e Receiver se registram para garantir liquidez e evitar ataques de negação de serviço (DoS/Griefing)[cite: 360, 443].
2.  **Puzzle Promise:** O Tumbler emite um puzzle criptográfico.
3.  **Randomization (O "Pulo do Gato"):**
    * O Receiver randomiza o puzzle com um fator secreto.
    * [cite_start]O Sender randomiza novamente para garantir que o Tumbler não reconheça o puzzle original[cite: 388, 392].
4.  [cite_start]**Pay & Release:** Uso de *Adaptor Signatures* para destravar os fundos atomicamente[cite: 349, 350].

---

## 3. Primitivas Criptográficas (The Core)

[cite_start]A segurança do protocolo baseia-se em **Grupos de Classe de Corpos Quadráticos Imaginários** e no esquema de encriptação **Castagnos-Laguillaumie (CL)**[cite: 3, 334].

### 3.1 Formas Quadráticas Binárias
[cite_start]Os objetos matemáticos manipulados são formas quadráticas binárias[cite: 11]:
$$f(x,y) = ax^2 + bxy + cy^2$$
Onde a tripla $(a, b, c)$ representa um elemento do grupo. [cite_start]A operação de grupo é a composição de formas, seguida pela **Redução de Gauss**, necessária para manter os coeficientes pequenos e gerenciáveis computacionalmente[cite: 40, 46].

### 3.2 O Problema do Logaritmo Discreto (DLP)
[cite_start]O sistema utiliza dois tipos de geradores para construir a criptografia homomórfica[cite: 61]:

* **Gerador Difícil ($g_q$):** Onde o DLP é computacionalmente inviável. [cite_start]Usado para gerar máscaras e chaves públicas[cite: 124, 127].
* **Gerador Fácil ($f_m$):** Um subgrupo onde o DLP é trivial. [cite_start]Usado para codificar a mensagem (o segredo $L$) de forma que ela possa ser recuperada na decriptação[cite: 132, 136].

### 3.3 Propriedades Homomórficas
[cite_start]A propriedade crucial para o A2L é o homomorfismo linear, que permite operar sobre os valores encriptados (randomização) sem revelá-los[cite: 451, 452]:

$$C^\beta = Enc(\alpha \cdot \beta)$$

[cite_start]Isso permite que o Sender e o Receiver modifiquem o puzzle (multiplicando por fatores de cegueira) enquanto mantêm a validade matemática da solução[cite: 191, 202].

---

## 4. Integração com Bitcoin (Adaptor Signatures)

O desafio final é conectar a matemática dos Grupos de Classe (off-chain) com a curva elíptica do Bitcoin (Secp256k1).

* [cite_start]**O Segredo ($\alpha$):** Precisa ser compatível tanto com a ordem do grupo de classe quanto com a ordem da curva do Bitcoin[cite: 257, 264].
* **Adaptor Signatures:** Funcionam como uma "assinatura quebrada" ou pré-assinatura.
    * [cite_start]A assinatura válida $\sigma$ é ocultada por um valor $d$ (o segredo do puzzle)[cite: 274].
    * [cite_start]Ao publicar a assinatura válida na blockchain, o segredo $d$ é revelado, permitindo a resolução do puzzle off-chain[cite: 306, 307].

---

## 5. Implementação & Estrutura de Dados

A implementação foca na eficiência da aritmética de formas quadráticas.

### Estruturas Principais (Go/Rust Binding)
* **Discriminante ($\Delta$):** Define o campo quadrático. [cite_start]Deve ser fundamental e satisfazer certas congruências (ex: $\Delta \equiv 1 \pmod 4$)[cite: 33].
* **Forma Quadrática:** Estrutura contendo BigInts para $(a, b, c)$.
* [cite_start]**Gaussian Reduction:** Algoritmo crítico implementado para normalizar as formas após cada operação de composição[cite: 40].