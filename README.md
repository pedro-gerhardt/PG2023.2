# PG2023-2
## Repositório das Listas do Grau A da cadeira de Processamento Gráfico 23/2

### <b>Os exercícios estão divididos nas branches do repositório.</b>

Respostas às perguntas iniciais da Lista 1:

1) P: O que é a GLSL? Quais os dois tipos de shaders são obrigatórios no pipeline programável da versão atual que trabalhamos em aula e o que eles processam?
R: A GLSL (GL Shading Language) é uma linguagem de programação usada para o desenvolvimento de shaders dentro do pipeline do OpenGL. Os dois tipos de shaders mencionados são o Vertex Shader, responsável pelos atributos individuais dos vértices, e o Fragment Shader, responsável pelos fragmentos gerados a partir da rasteirização em um conjunto de cores e profundidade.

2) P: O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL?
R: Primitivas Gráficas são os elementos básicos que compõem um desenho, tais como pontos, segmentos de retas e círculos, a partir dos quais são construídos outros, mais complexos. Elas são manipuladas via comandos e funções que alteram os elementos gráficos de uma imagem. Os vértices são armazenados nos Vertex Buffer Objects (VBOs), e tem seus atributos presentes nos Vertex Array Objects (VAOs).

3) P: Explique o que é VBO, VAO e EBO, e como se relacionam (se achar mais fácil, pode fazer um gráfico representando a relação entre eles).
R: Um VBO é um recurso OpenGL que fornece métodos para carregar dados de vértice no dispositivo de vídeo para renderização em modo não imediato. Um VAO é um objeto OpenGL que armazena todo o estado necessário para fornecer dados de vértice, como posição e cor. O EBO é responsável por aplicar o conceito de índices referentes ao buffer de vértices, evitando a replicação desnecessária de informações.
