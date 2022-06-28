#include "window.h"
#include "image.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "element_buffer.h"
#include "texture.h"
#include "shader.h"
#include <iostream>

namespace Hub
{
	void test()
	{
		Hub::Window hWindow(800, 600);
		auto window = hWindow.getNativeHandle();

		// shader
		Shader ourShader("./shader/shader.vs", "./shader/shader.fs");

		// 三角形三个顶点的标准化设备坐标
		GLfloat vertices[] = {
			// 位置             // 颜色            // 纹理
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 左下
			-0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 左上
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.3f, 1.0f, 1.0f, // 右上
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f, // 顶部
		};
		GLuint indices[] = {
			0,1,2,
			2,3,0
		};

		auto VAO = VertexArray::create();
		auto VBO = VertexBuffer::create(vertices, sizeof(vertices), BufferUsage::StaticDraw);
		auto EBO = ElementBuffer::create(indices, sizeof(indices), BufferUsage::StaticDraw);
		VAO->bindElements(*EBO);
		VAO->bindAttribute(0, 3, *VBO, Type::Float, 8 * sizeof(float), 0 * sizeof(float));
		VAO->bindAttribute(1, 3, *VBO, Type::Float, 8 * sizeof(float), 3 * sizeof(float));
		VAO->bindAttribute(2, 2, *VBO, Type::Float, 8 * sizeof(float), 6 * sizeof(float));
		
		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// 解绑VAO
		glBindVertexArray(0);

		const char* filePath = "../Asset/container.jpg";
		auto texture1 = Texture::create(filePath);
		texture1->setWrapping(Wrapping::axis_t::S, Wrapping::wrapping_t::MirroredRepeat);
		texture1->setWrapping(Wrapping::axis_t::T, Wrapping::wrapping_t::MirroredRepeat);
		texture1->setFilter(Filter::operator_t::Mag, Filter::filter_t::Linear);
		texture1->setFilter(Filter::operator_t::Min, Filter::filter_t::Linear);
		
		filePath = "../Asset/awesomeface.png"; 
		auto texture2 = Texture::create(filePath);
		texture2->setWrapping(Wrapping::axis_t::S, Wrapping::wrapping_t::MirroredRepeat);
		texture2->setWrapping(Wrapping::axis_t::T, Wrapping::wrapping_t::MirroredRepeat);
		texture2->setFilter(Filter::operator_t::Mag, Filter::filter_t::Linear);
		texture2->setFilter(Filter::operator_t::Min, Filter::filter_t::Linear);
		
		
		ourShader.use(); // 设置shader属性前需要激活程序
		// 绑定纹理
		glActiveTexture(GL_TEXTURE0); // 使用一个纹理时默认激活
		glBindTexture(GL_TEXTURE_2D, *texture1);
		ourShader.setInt("ourTexture1", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *texture2);
		ourShader.setInt("ourTexture2", 1);

		while (!glfwWindowShouldClose(window)) // 使图像不立即关闭
		{
			glfwPollEvents(); // 检查事件触发：比如键盘输入

			// 渲染指令
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 清除颜色缓冲后需要填入的颜色，是一个状态设置函数
			glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲，是一个状态使用函数，使得应该清除为上面设置的颜色

			glm::mat4 trans(1.0f);
			auto r = static_cast<float>(glfwGetTime());
			std::cout << "r = " << r << std::endl;
			trans = glm::rotate(trans, r, glm::vec3(0.0f, 0.0f, 1.0f));
			ourShader.setMatirx4("transform", trans);
			// Draw
			//ourShader.use(); // 激活着色程序
			glBindVertexArray(*VAO);
			//glDrawArrays(GL_TRIANGLES, 0, 3); // 0: 顶点起始索引，3绘制个数
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 6个点，索引类型为unsigned int, offset = 0
			glBindVertexArray(0);

			// swap the screen buffers
			glfwSwapBuffers(window);

		}
		glfwTerminate();
	}
}
int main()
{
	Hub::test();
	return 0;
}



