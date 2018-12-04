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
	// step 1 定义所有点坐标和索引顺序

	// 首先定义所有点坐标，坐标范围在-1~1.第三个坐标可以解释为depth
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};
	// 定义绘制三角形时候的索引坐标
	unsigned int indices[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	// step 2 将顶点数据传到GPU的内存中
	// 定义一个顶点缓冲对象，管理内存; 索引缓冲对象管理索引顺序；顶点数组对象存储VBO
	unsigned int VBO, VAO, EBO;
	// 或者 GLuint VBO;
	glGenBuffers(1, &VBO);//第一个参数表现创建对象的个数
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VAO);
	// 绑定VAO
	glBindVertexArray(VAO);


	// 顶点对象的缓冲类型是GL_ARRAY_BUFFER 首先要将缓冲对象绑定目标，而Buffer对象的行为取决于绑定点
	// 在绑定之后 才可以使用VBO检索或存储数据 例如可以调用glBufferData 将顶点数据复制到VBO中
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData 把用户的数据复制到当前绑定到当前缓冲， GL_STATIC_DRAW是因为三角形数据基本不会改变
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// step 3 顶点着色器渲染
	// 使用GLSL语音编写顶点着色器, 指明了版本号，以及core模式， 在位置0出定义了position属性
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
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
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
	// 由于顶点着色器支持任何顶点属性形式的输入，因此在此我们需要指定输入数据哪一部分对应着色器哪一部分
	// 第一个对应顶点着色器中layout(location=0)， 第二个指定顶点属性的大小，顶点属性是一个vec3,故为3
	// 第三个指定数据类型， 第四个指定数据是否要被标准化，映射到0-1之间，第五个是步长，代表下个数组的位置，
	// 最后一个位置代表offset，由于位置数据位于数组开头，所以为0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// 启用顶点属性
	glEnableVertexAttribArray(0);

	// 0代表当前缓冲器类型没有绑定任何的缓冲器对象
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 解绑VAO对象,避免别的VAO对象无意间修改现场
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //透明度=1代表完全不透明
		glClear(GL_COLOR_BUFFER_BIT);

		// 绘制物体
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// 激活当前的着色器，第一个参数代表想要绘制OpenGL图元的类型，
		// 第二个参数制定了顶点数组的起始索引，最后一个指代打算绘制多少个顶点
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		// 指定如何绘制图元， GL_LINE是wireframe mode（线框模式, GL_FILL是默认填充模式
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// 由于需要按照索引顺序来绘图，所以使用了GL_ELEMENT_ARRAYBUFFER，参数分别图元类型，绘制顶点个数，索引类型，offset
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents(); //检测是否有事件触发 从而更新状态

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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