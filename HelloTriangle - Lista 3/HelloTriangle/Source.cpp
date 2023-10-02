
#include <iostream>
#include <string>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
using namespace std;


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
//int setupShader();
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;
glm::mat4 model = glm::mat4(1); //matriz identidade
float velX = 0.0005, velY = 0.001;
// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! - Pedro Gerhardt", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;

	// Compilando e buildando o programa de shader
	//GLuint shaderID = setupShader();
	Shader shader("Shaders/helloTriangle.vs", "Shaders/helloTriangle.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();

	glm::mat4 projection = glm::mat4(1); //matriz identidade

	projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	shader.Use();
	shader.setMat4("projection", glm::value_ptr(projection));

	model = glm::translate(model, glm::vec3(400.0, 300.0, 0.0));
	model = glm::scale(model, glm::vec3(100.0, 75.0, 1.0));

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(5);
		glPointSize(10);

		glBindVertexArray(VAO); //Conectando ao buffer de geometria
		shader.setMat4("model", glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(velX, velY, 0.0));
		if (model[3].x < 100 || model[3].x > 700)
			velX = velX * -1;
		if (model[3].y > 525 || model[3].y < 75)
			velY = velY * -1;

		glDrawArrays(GL_TRIANGLES, 30, 15);
		glDrawArrays(GL_LINES, 0, 30);

		glBindVertexArray(0); //Desconectando o buffer de geometria

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS)
	{
		model = glm::translate(model, glm::vec3(-1.0, 0.0, 0.0));
		if (model[3].x < 100) 
			model = glm::translate(model, glm::vec3(1.0, 0.0, 0.0));
	}
	if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_PRESS) 
	{
		model = glm::translate(model, glm::vec3(1.0, 0.0, 0.0));
		if (model[3].x > 700)
			model = glm::translate(model, glm::vec3(-1.0, 0.0, 0.0));
	}
	if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_PRESS)
	{
		model = glm::translate(model, glm::vec3(0.0, 1.0, 0.0));
		if (model[3].y > 525)
			model = glm::translate(model, glm::vec3(0.0, -1.0, 0.0));
	}
	if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && action == GLFW_PRESS) 
	{
		model = glm::translate(model, glm::vec3(0.0, -1.0, 0.0));
		if (model[3].y < 75)
			model = glm::translate(model, glm::vec3(0.0, 1.0, 0.0));
	}
}

int setupGeometry()
{
	GLfloat vertices[] = {
		// chao laranja - 2 pontos
		-1.0, -1.0, 0.0, 1.0, 0.2734, 0.0,
		1.0, -1.0, 0.0, 1.0, 0.2734, 0.0,

		// estrutura casa linhas pretas - 10 pontos
		-0.75, -1.0, 0.0, 0.0, 0.0, 0.0,
		-0.75, 0.0, 0.0, 0.0, 0.0, 0.0,

		-0.75, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.75, 0.0, 0.0, 0.0, 0.0,

		0.0, 0.75, 0.0, 0.0, 0.0, 0.0,
		0.75, 0.0, 0.0, 0.0, 0.0, 0.0,

		-0.75, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.75, 0.0, 0.0, 0.0, 0.0, 0.0,

		0.75, -1.0, 0.0, 0.0, 0.0, 0.0,
		0.75, 0.0, 0.0, 0.0, 0.0, 0.0,

		// linhas porta - 6 pontos
		0.2, -1.0, 0.0, 0.0, 0.0, 0.0,
		0.2, -0.5, 0.0, 0.0, 0.0, 0.0,

		-0.2, -1.0, 0.0, 0.0, 0.0, 0.0,
		-0.2, -0.5, 0.0, 0.0, 0.0, 0.0,

		 0.2, -0.5, 0.0, 0.0, 0.0, 0.0,
		-0.2, -0.5, 0.0, 0.0, 0.0, 0.0,

		//linhas janela - 12 pontos
		-0.1, -0.35, 0.0, 0.0, 0.0, 0.0,
		-0.1, -0.15, 0.0, 0.0, 0.0, 0.0,

		-0.3, -0.35, 0.0, 0.0, 0.0, 0.0,
		-0.3, -0.15, 0.0, 0.0, 0.0, 0.0,

		-0.3, -0.35, 0.0, 0.0, 0.0, 0.0,
		-0.1, -0.35, 0.0, 0.0, 0.0, 0.0,

		-0.3, -0.15, 0.0, 0.0, 0.0, 0.0,
		-0.1, -0.15, 0.0, 0.0, 0.0, 0.0,

		-0.3, -0.25, 0.0, 0.0, 0.0, 0.0,
		-0.1, -0.25, 0.0, 0.0, 0.0, 0.0,

		-0.2, -0.35, 0.0, 0.0, 0.0, 0.0,
		-0.2, -0.15, 0.0, 0.0, 0.0, 0.0,

		// telhado vermelho - 3 pontos
		-0.75, 0.0, 0.0, 1.0, 0.0, 0.0,
		0.0, 0.75, 0.0, 1.0, 0.0, 0.0,
		0.75, 0.0, 0.0, 1.0, 0.0, 0.0,

		// janela amarela - 6 pontos
		-0.1, -0.35, 0.0, 1.0, 1.0, 0.0,
		-0.1, -0.15, 0.0, 1.0, 1.0, 0.0,
		-0.3, -0.35, 0.0, 1.0, 1.0, 0.0,

		-0.1, -0.15, 0.0, 1.0, 1.0, 0.0,
		-0.3, -0.35, 0.0, 1.0, 1.0, 0.0,
		-0.3, -0.15, 0.0, 1.0, 1.0, 0.0,

		// porta marrom - 6 pontos
		0.2, -1.0, 0.0, 0.55, 0.23, 0.0,
		0.2, -0.5, 0.0, 0.55, 0.23, 0.0,
		-0.2, -1.0, 0.0, 0.55, 0.23, 0.0,

		-0.2, -1.0, 0.0, 0.55, 0.23, 0.0,
		-0.2, -0.5, 0.0, 0.55, 0.23, 0.0,
		0.2, -0.5, 0.0, 0.55, 0.23, 0.0,
	};

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Faz a conexão (vincula) do buffer como um buffer de array
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Envia os dados do array de floats para o buffer da OpenGl

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

