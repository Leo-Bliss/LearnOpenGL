#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

int main()
{
	// 平移
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); // 齐次坐标： x ,y ,z , w 
	glm::mat4 trans(1.0f); // 变换矩阵, 1.0f指明线性变换矩阵为单位矩阵

	glm::vec3 offset(1.0f, 1.0f, 0.0f);
	trans = glm::translate(trans, offset); 

	auto showTransRes = [](auto& trans, auto& vec){
		vec = trans * vec;
		std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
	};
	showTransRes(trans, vec);

	// 旋转
	glm::mat4 trans2(1.0f);
	glm::vec3 axisZ(0.0f, 0.0f, 1.0f);
	trans2 = glm::rotate(trans2, glm::radians(90.0f), axisZ); // 这里第三个参数指明旋转轴
	showTransRes(trans2, vec);

	// 缩放
	glm::mat4 trans3(1.0f);
	glm::vec3 scaleVec(0.5f, 0.5f, 0.5f); // 每个轴缩放因子为0.5
	trans3 = glm::scale(trans3, scaleVec);
	showTransRes(trans3, vec);

	return 0;
}