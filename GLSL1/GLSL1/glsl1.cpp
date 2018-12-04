#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

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
	// ����һ�����㻺����󣬹����ڴ�
	unsigned int VBO;
	// ���� GLuint VBO;
	glGenBuffers(1, &VBO);//��һ���������ִ�������ĸ���
	// �������Ļ���������GL_ARRAY_BUFFER ����Ҫ����������Ŀ�꣬��Buffer�������Ϊȡ���ڰ󶨵�
	// �ڰ�֮�� �ſ���ʹ��VBO������洢���� ������Ե���glBufferData ���������ݸ��Ƶ�VBO��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData ���û������ݸ��Ƶ���ǰ�󶨵���ǰ���壬 GL_STATIC_DRAW����Ϊ���������ݻ�������ı�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// step 3 ������ɫ����Ⱦ
	// ʹ��GLSL������д������ɫ��, ָ���˰汾�ţ��Լ�coreģʽ�� ��λ��0��������position����
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"   ourColor = aColor;\n"
		"}\n\0";
	// ����OpenGLʹ����ɫ��Դ��,���ȴ���һ��������ɫ������
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// ���붥����ɫ���Ĳ���j
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//�ڶ�������ָ����Ҫ���ݵ��ַ�������
	glCompileShader(vertexShader);// ����Դ��
	//����Ƿ����ɹ���,�������ʧ���˼�������ʲô
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR in VERTEX SHADER\n" << infoLog << endl;
	}

	// step 4 ����Ƭ����ɫ��
	//const char *fragmentShaderSource = "#version 330 core\n"
	//	"out vec4 FragColor;\n"
	//	"uniform vec4 ourColor;\n"
	//	"void main()\n"
	//	"{\n"
	//	"   FragColor = ourColor;\n"
	//	"}\n\0";
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(ourColor, 1.0f);\n"
		"}\n\0";


	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR in FRAGMENT SHADER\n" << infoLog << endl;
	}

	// step 5 ����ɫ��������ӳ�Ϊһ����ɫ���������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// ��������Ƿ�ɹ�
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR in LINK STATUS\n" << infoLog << endl;
	}
	// ����������
	glUseProgram(shaderProgram);
	// ɾ����ɫ�������ʱ�Ѿ�û����
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// step 6 ���Ӷ�������

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
		// �����������˵�ڸ�����ɫ������֮ǰ ����Ҫ������ɫ�����򣡣�����
		glUseProgram(shaderProgram);

		// �ò��ֽ������ʹ��uniform����
		//// ����uniform�������ɫ
		//float timevalue = glfwGetTime();
		//float red = sin(timevalue) / 2.0f + 0.5f;
		//// ��ȡ�����uniform����λ��
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUniform4f(vertexColorLocation, red, 0.0f, 0.0f, 1.0f);

		// ���ǰ����ɫ������һ������������Ҫ����OpenGLͼԪ�����ͣ��ڶ��������ƶ��˶����������ʼ���������һ��ָ��������ƶ��ٸ�����
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents(); //����Ƿ����¼����� �Ӷ�����״̬

	}

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