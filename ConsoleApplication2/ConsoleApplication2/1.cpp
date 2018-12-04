#include <glad/glad.h>
// OpenGLֻ��һ���淶��׼��ʵ��������������ʵ�ֵģ������������λ����Ҫ�����н��в�ѯ��ȡ�ú���λ�õĹ��̼��������˸ÿ���
// ���ڸÿ��������OpenGL�ļ���������Ҫ������ǰ��
#include <GLFW/glfw3.h>
// OpenGl�����ĺʹ��ڲ��������������ϵͳ��أ����ò��ֳ����ȥ�õ���һЩ���еĿ�GLUTA SDL SFML GLFW�ȣ�
// ���Զ��崰�ڲ�����OpenGL�����ģ��Լ������û�����
#include<iostream>

// ����һ���ص��������Ӷ�ÿ�δ��ڴ�С�ı䶼������øú���
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// ����û��Ƿ�����Esc
void processInput(GLFWwindow* window);
int main()
{
	glfwInit();//��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//GLFW�汾��
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_OPENGL_CORE_PROFILE);//GLFW����ģʽ

	//GLFW��������
	GLFWwindow* window = glfwCreateWindow(800, 600, "FIRST_TRY", NULL, NULL);// �� ��
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//ע�ắ�� framebuffer_size_callback�� ÿ�ε������ڴ�С������øú���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ͨ��GLAD����ָ�� glfwGetProcAddress����ָ���ַ
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initalize GLAD" << std::endl;
		return -1;
	}

	// ��Ⱦ���ڵĳߴ� (Viewport)
	glViewport(0, 0, 800, 600);//ǰ���������½����꣬�������ǿ�Ⱥ͸߶ȣ����أ���  



	// ��Ⱦѭ���������������رճ���ǰ���Բ��ϻ���ͼ�񲢽����û�����
	while (!glfwWindowShouldClose(window))
	{
		//ÿ����Ⱦǰ�����Ļ��ɫ����״̬���ã�Ȼ��״̬ʹ��

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //��졢�̡�����ռ�ķ�������Χ��0.0f~1.0f,���һ��������͸����Alphaֵ,��ΧҲ��0.0f~1.0f
		glClear(GL_COLOR_BUFFER_BIT); //����һ������λ(Buffer_Bit)ָ����Ҫ��յĻ��壬����ֻ�������ɫ���壻

		processInput(window);

		glfwSwapBuffers(window); //������ɫ����,ʹ�õ���Double Bufferʹ�û���ͼ��������һ������ʱ��ͼ����˸����
		glfwPollEvents(); //����Ƿ����¼��������Ӷ����´���״̬�������ö�Ӧ�Ļص����������ֶ�����
	}

	glfwTerminate(); //�ͷ���Դ
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);// ������glfwWindowShouldCloseΪtrue �Ӷ���whileѭ�����Ƴ�
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
