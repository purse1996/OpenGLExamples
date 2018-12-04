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
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); mac系统需要加上这一句

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
	// step 1 定义三角形坐标

	// 首先定义一个三角形3D坐标，坐标范围在-1~1.第三个坐标可以解释为depth
	float vertices[] = {
		// 位置              // 颜色
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 左下
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
	};

	// step 2 将顶点数据传到GPU的内存中
	// 定义一个顶点缓冲对象，管理内存
	unsigned int VBO;
	// 或者 GLuint VBO;
	glGenBuffers(1, &VBO);//第一个参数表现创建对象的个数
	// 顶点对象的缓冲类型是GL_ARRAY_BUFFER 首先要将缓冲对象绑定目标，而Buffer对象的行为取决于绑定点
	// 在绑定之后 才可以使用VBO检索或存储数据 例如可以调用glBufferData 将顶点数据复制到VBO中
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData 把用户的数据复制到当前绑定到当前缓冲， GL_STATIC_DRAW是因为三角形数据基本不会改变
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// step 3 顶点着色器渲染
	// 使用GLSL语音编写顶点着色器, 指明了版本号，以及core模式， 在位置0出定义了position属性
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"   ourColor = aColor;\n"
		"}\n\0";
	// 利用OpenGL使用着色器源码,首先创建一个顶点着色器对象
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// 传入顶点着色器的参数j
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//第二个参数指代需要传递的字符串数量
	glCompileShader(vertexShader);// 编译源码
	//检测是否编译成功了,如果编译失败了检测错误是什么
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR in VERTEX SHADER\n" << infoLog << endl;
	}

	// step 4 定义片段着色器
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

	// step 5 将做色漆对象链接成为一个着色器程序对象
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// 检测链接是否成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR in LINK STATUS\n" << infoLog << endl;
	}
	// 激活程序对象
	glUseProgram(shaderProgram);
	// 删除着色器对象此时已经没用了
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// step 6 链接顶点属性

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// 启用顶点属性
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);




	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //透明度=1代表完全不透明
		glClear(GL_COLOR_BUFFER_BIT);
		// 绘制物体或者说在更新着色器变量之前 首先要激活着色器程序！！！！
		glUseProgram(shaderProgram);

		// 该部分解释如何使用uniform变量
		//// 更新uniform定义的颜色
		//float timevalue = glfwGetTime();
		//float red = sin(timevalue) / 2.0f + 0.5f;
		//// 获取定义的uniform变量位置
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUniform4f(vertexColorLocation, red, 0.0f, 0.0f, 1.0f);

		// 激活当前的着色器，第一个参数代表想要绘制OpenGL图元的类型，第二个参数制定了顶点数组的起始索引，最后一个指代打算绘制多少个顶点
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents(); //检测是否有事件触发 从而更新状态

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