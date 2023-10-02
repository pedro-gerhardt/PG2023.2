
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
const GLuint WIDTH = 600, HEIGHT = 600;

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
	projection = glm::ortho(0.0f, 600.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	shader.Use();
	shader.setMat4("projection", glm::value_ptr(projection));

	glm::vec4 colors[] = {
		glm::vec4(0.0, 0.0, 1.0, 1.0),
		glm::vec4(0.0, 1.0, 1.0, 1.0),
		glm::vec4(0.0, 1.0, 0.0, 1.0),
		glm::vec4(1.0, 1.0, 0.0, 1.0),
		glm::vec4(1.0, 0.0, 0.0, 1.0),
		glm::vec4(1.0, 0.0, 1.0, 1.0),
	};


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

		glm::mat4 model = glm::mat4(1); //matriz identidade
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				glBindVertexArray(VAO); //Conectando ao buffer de geometria
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(100.0 * j, 100.0 * i, 0.0));
				model = glm::scale(model, glm::vec3(100.0, 100.0, 1.0));

				shader.setMat4("model", glm::value_ptr(model));
				int idxColor = j - i;
				if (idxColor < 0)
					idxColor = 6 + idxColor;
				shader.setVec4("inputColor", colors[idxColor].r, colors[idxColor].g, colors[idxColor].b, colors[idxColor].a);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0); //Desconectando o buffer de geometria
			}
		}

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
}

int setupGeometry()
{
	GLfloat vertices[] = {
		0.0, 0.0, 0.0, 
		1.0, 0.0, 0.0, 
		0.0, 1.0, 0.0, 

		0.0, 1.0, 0.0, 
		1.0, 0.0, 0.0, 
		1.0, 1.0, 0.0, 
	};

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Faz a conexão (vincula) do buffer como um buffer de array
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Envia os dados do array de floats para o buffer da OpenGl

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

