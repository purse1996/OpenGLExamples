#include <glad/glad.h>
// OpenGL只是一个规范标准，实现是由驱动厂商实现的，因而它函数的位置需要在运行进行查询，取得函数位置的过程即定义在了该库中
// 由于该库包含的是OpenGL文件，所以需要放在最前面
#include <GLFW/glfw3.h>
// OpenGl上下文和窗口参数的设置与操作系统相关，将该部分抽象出去得到了一些流行的库GLUTA SDL SFML GLFW等，
// 可以定义窗口参数，OpenGL上下文，以及处理用户输入
#include<iostream>

// 定义一个回调函数，从而每次窗口大小改变都将会调用该函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 检查用户是否按下了Esc
void processInput(GLFWwindow* window);
int main()
{
	glfwInit();//初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//GLFW版本号
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_OPENGL_CORE_PROFILE);//GLFW核心模式

	//GLFW窗口设置
	GLFWwindow* window = glfwCreateWindow(800, 600, "FIRST_TRY", NULL, NULL);// 宽 高
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//注册函数 framebuffer_size_callback， 每次调整窗口大小都会调用该函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 通过GLAD加载指针 glfwGetProcAddress函数指针地址
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initalize GLAD" << std::endl;
		return -1;
	}

	// 渲染窗口的尺寸 (Viewport)
	glViewport(0, 0, 800, 600);//前两个是左下角坐标，后两个是宽度和高度（像素）；  



	// 渲染循环，在我们主动关闭程序前可以不断绘制图像并接受用户输入
	while (!glfwWindowShouldClose(window))
	{
		//每次渲染前清空屏幕颜色；先状态设置，然后状态使用

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //表红、绿、蓝所占的分量，范围从0.0f~1.0f,最后一个参数是透明度Alpha值,范围也是0.0f~1.0f
		glClear(GL_COLOR_BUFFER_BIT); //接受一个缓冲位(Buffer_Bit)指定将要清空的缓冲，这里只清除了颜色缓冲；

		processInput(window);

		glfwSwapBuffers(window); //交换颜色缓冲,使用的是Double Buffer使得绘制图像避免出现一点点绘制时的图像闪烁问题
		glfwPollEvents(); //检查是否有事件触发，从而更新窗口状态，并调用对应的回调函数，可手动设置
	}

	glfwTerminate(); //释放资源
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);// 设置了glfwWindowShouldClose为true 从而在while循环中推出
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
