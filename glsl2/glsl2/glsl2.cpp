#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using namespace std;
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); macϵͳ��Ҫ������һ��

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangle", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create a window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	// step 1 ��������������

	// ���ȶ���һ��������3D���꣬���귶Χ��-1~1.������������Խ���Ϊdepth
	float vertices[] = {
		// λ��              // ��ɫ
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // ����
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // ����
	};

	// step 2 ���������ݴ���GPU���ڴ���
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	// step 3 ���ö���ĺõĶ�ȡshader���ļ�
	Shader ourShader("shader_vs.txt", "shader_fs.txt");

	// step 4 ���Ӷ�������

	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// ���ö�������
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);




	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //͸����=1������ȫ��͸��
		glClear(GL_COLOR_BUFFER_BIT);
	

		ourShader.use();
		glBindVertexArray(VAO);
		// ���ǰ����ɫ������һ������������Ҫ����OpenGLͼԪ�����ͣ��ڶ��������ƶ��˶����������ʼ���������һ��ָ��������ƶ��ٸ�����
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents(); //����Ƿ����¼����� �Ӷ�����״̬

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}